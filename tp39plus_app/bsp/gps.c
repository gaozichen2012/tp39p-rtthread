#define GPS
#include "gps.h"
#include "poc.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "lcd_api.h"
#include "ui.h"
#include "new_ui_data.h"
#define GPS_NODATA_TIME 12000

uint32_t gps_nodata_offtime;
FlagStatus gps_init_flag;

static void gps_enable_flag_handle(void);
static void gps_poc_report(u8 *ns, u8 *ew, itude_t tns, itude_t tew);
static int ddmm2dd(const char *ddmm, char *dd);
static void dispaly_gps_ico(u8 state);

void gps_init(void)
{

    gps_nodata_offtime = SysTick_Cnt;
    memset((u8 *)&gps_device, 0, sizeof(gps_device_t));
}

void gps_task(void)
{
#define GPS_BUF_SIZE 256
    uint8_t i;
    static uint8_t buf_temp_deal[GPS_BUF_SIZE] = {0};
    static uint16_t cmp_cnt = 0;

    for (i = 0; i < 32; i++) //卫星信号强度更新时间，如果10秒钟都没有更新的值给清0
    {
        if (SysTick_Cnt - gps_device.snr.time[i] > 10000)
        {
            gps_device.snr.time[i] = SysTick_Cnt;
        }
    }

    /***     gps  uart msg deal      ***/
    while (gps_device.gps_data.read_p != gps_device.gps_data.write_p) //有数据未处理
    {
        if (gps_device.gps_data.recv_buf[(gps_device.gps_data.read_p + cmp_cnt) % UART_RXBUF_LEN_MAX] == '\r' ||
            gps_device.gps_data.recv_buf[(gps_device.gps_data.read_p + cmp_cnt) % UART_RXBUF_LEN_MAX] == '\n') //检测到未处理数据中有 \r\n结尾的数据
        {
            if (cmp_cnt >= 2) //只处理长度大于2的指令
            {
                memset(buf_temp_deal, 0, sizeof(buf_temp_deal));
                if (gps_device.gps_data.read_p + cmp_cnt < UART_RXBUF_LEN_MAX) //如果不是循环存储
                {
                    memcpy(buf_temp_deal,
                           &gps_device.gps_data.recv_buf[gps_device.gps_data.read_p % UART_RXBUF_LEN_MAX],
                           (cmp_cnt % GPS_BUF_SIZE));
                }
                else
                {
                    memcpy(buf_temp_deal,
                           &gps_device.gps_data.recv_buf[gps_device.gps_data.read_p % UART_RXBUF_LEN_MAX],
                           (UART_RXBUF_LEN_MAX - gps_device.gps_data.read_p) % GPS_BUF_SIZE);
                    memcpy(&buf_temp_deal[(UART_RXBUF_LEN_MAX - gps_device.gps_data.read_p) % GPS_BUF_SIZE],
                           gps_device.gps_data.recv_buf,
                           (gps_device.gps_data.read_p + cmp_cnt) % UART_RXBUF_LEN_MAX % (GPS_BUF_SIZE - ((UART_RXBUF_LEN_MAX - gps_device.gps_data.read_p) % GPS_BUF_SIZE)));
                }
                gps_process(buf_temp_deal);
            }
            gps_device.gps_data.read_p = (gps_device.gps_data.read_p + (cmp_cnt + 1)) % UART_RXBUF_LEN_MAX; //处理过了
            cmp_cnt = 0;
        }
        else
        {
            if (((gps_device.gps_data.read_p < gps_device.gps_data.write_p) && (cmp_cnt + 1 < gps_device.gps_data.write_p - gps_device.gps_data.read_p)) ||
                ((gps_device.gps_data.read_p > gps_device.gps_data.write_p) && (cmp_cnt + 1 < (UART_RXBUF_LEN_MAX - gps_device.gps_data.read_p + gps_device.gps_data.write_p))))
                cmp_cnt++;
            else
            {
                if (cmp_cnt >= 200)
                {
                    gps_device.gps_data.write_p = 0;
                    gps_device.gps_data.read_p = 0;
                    cmp_cnt = 0;
                }
                break;
            }
        }
    }

    if (abs(SysTick_Cnt - gps_nodata_offtime) >= GPS_NODATA_TIME)
    {
        if (gps_init_flag == SET)
        {
            USART_DeInit(USART1);
            gps_init_flag = RESET;
        }

        gps_nodata_offtime = SysTick_Cnt;
    }
}

void gps_process(u8 *ptr)
{
    if (memcmp(ptr, "$GPGGA", 6) == 0 || memcmp(ptr, "$GNGGA", 6) == 0) //GPS定位的主要数据
    {
        u8 i, j, len = strlen((char *)ptr); //最大帧255byte
        u8 frame_count = 0;                 //, 号切割	数据帧，计数帧个数
        gps_device.status = GPS_DEVICE_NORMAL;

        for (i = 0, j = 0; i < len; i++)
        {
            //消息头，UTC时间，纬度，N/S指示，经度，E/W指示，定位状态（0 未定位，1实时有效，2差分有效），可用卫星数目，水平精度，天线高程，单位，海拔。。。。
            if (ptr[i] == ',')
            {
                if (frame_count == 2) //纬度  将 ddmm.mmmm  转换成dd.ddddd
                {
                    ddmm2dd((const char *)ptr + j + 1, (char *)gps_device.NS_value);
                }
                else if (frame_count == 3) //纬度类型 N/S
                    gps_device.ns = (itude_t) * (ptr + i - 1);
                else if (frame_count == 4) //经度  将 dddmm.mmmm  转换成ddd.ddddd
                {
                    gps_device.EW_value[0] = *(ptr + j + 1);
                    ddmm2dd((const char *)ptr + j + 2, (char *)&gps_device.EW_value[1]);
                }
                else if (frame_count == 5) //纬度类型 W/E
                    gps_device.ew = (itude_t) * (ptr + i - 1);
                else if (frame_count == 6) //定位结果
                {
                    if (*(ptr + i - 1) == 0x30 || *(ptr + i - 1) == 0x31) //定位成功后有时候此标志位有时=0，怀疑是收到异常数据gaozichen added
                    {
                        gps_device.position_state = *(ptr + i - 1);
                    }
                }

                else if (frame_count == 7) //可用卫星数量
                {
                    memset(gps_device.satellites_sum, 0, 4);
                    memcpy(gps_device.satellites_sum, ptr + j + 1, i - j - 1);
                }
                j = i;
                frame_count++;
            }
        }
    }
    else if ((memcmp(ptr, "$GPGSV", 6) == 0) ||
             (memcmp(ptr, "$BDGSV", 6) == 0)) //可视卫星状态输出
    {
        u8 i, j, len = strlen((char *)ptr); //最大帧255byte
        u8 frame_count;                     //, 号切割	数据帧，计数帧个数
        frame_count = 0;

        for (i = 0, j = 0; i < len; i++)
        {
            static u8 id;
            //消息头，消息数，消息编号，卫星总数，卫星id
            //仰角，方位角，信噪比 ......校验和\r\n
            if (ptr[i] == ',')
            {
                if (frame_count == 1) //消息数目
                {
                    //gsv_cnt =  (ptr[i-1]- '0');
                }
                else if (frame_count == 4) //卫星id
                {
                    id = (ptr[i - 1] - '0') + (ptr[i - 2] - '0') * 10;
                }
                else if (frame_count == 7) //信噪比
                {
                    memset(gps_device.snr.SNR[id % 32], 0, 4);
                    if (i - j == 3)
                        memcpy(gps_device.snr.SNR[id % 32], &ptr[i - 2], 2);
                    gps_device.snr.time[id % 32] = SysTick_Cnt;
                }
                else if (frame_count == 16) //卫星id
                {
                    id = (ptr[i - 1] - '0') + (ptr[i - 2] - '0') * 10;
                }
                else if (frame_count == 19) //信噪比
                {
                    memset(gps_device.snr.SNR[id % 32], 0, 4);
                    if (i - j == 3)
                        memcpy(gps_device.snr.SNR[id % 32], &ptr[i - 2], 2);
                    gps_device.snr.time[id % 32] = SysTick_Cnt;
                }

                j = i;
                frame_count++;
            }
        }
    }
}

//gps_enable_flag改变时更新GPS状态
static void gps_enable_flag_handle(void)
{
    static u8 flag_old = 0xff;

    if (poc.flash_data.gps_enable_flag != flag_old)
    {
			flag_old=poc.flash_data.gps_enable_flag;
        if (poc.flash_data.gps_enable_flag == 0)
        {
            set_gpio_state(GPIO_GPS_EN, off);
        }
        else
        {
            set_gpio_state(GPIO_GPS_EN, on);
        }
    }
}

void gps_poc_report_timer_process(void)
{
    static u32 gps_at_report_count = 0;
    static u8 gps_state = 0;
    static u8 gps_state_old = 0;

    //gps_enable_flag控制gps开关
    gps_enable_flag_handle();

    //gps report
    if (abs(SysTick_Cnt - gps_at_report_count) >= 5 * 1000)
    {
        gps_at_report_count = SysTick_Cnt;

        if (poc.has_logged_flag == true && poc.flash_data.gps_enable_flag != 0)
        {
            if (gps_device.position_state == '1' || gps_device.position_state == '2')
            {
                gps_poc_report(gps_device.NS_value, gps_device.EW_value, gps_device.ns, gps_device.ew);
            }
        }
    }

    //gps ico
    if (poc.flash_data.gps_enable_flag == 0)
    {
        gps_state = 0; //未开启定位
    }
    else
    {
        if (gps_device.position_state == 0 || gps_device.position_state == '0')
        {
            gps_state = 1; //开启定位-未定位成功
        }
        else
        {
            gps_state = 2; //定位成功
        }
    }

    if (gps_state_old != gps_state)
    {
        gps_state_old = gps_state;
        dispaly_gps_ico(gps_state);
    }
}

static void gps_poc_report(u8 *ns, u8 *ew, itude_t tns, itude_t tew)
{
    char send_value_double1[32] = {0};
    char send_value_double2[32] = {0};
    char send_temp[128] = {0};
    double ew_value, ns_value;

    ns_value = atof((char *)ns);
    ew_value = atof((char *)ew);

    if (tns == 'S')
        ns_value = -ns_value;
    if (tew == 'W')
        ew_value = -ew_value;
    sprintf(send_temp, "%s", "AT+POC=110000");
    sprintf(send_value_double1, "%f", ns_value);
    hex_to_str((u8 *)&send_temp[strlen(send_temp)], (u8 *)send_value_double1, strlen(send_value_double1) % 32);
    sprintf(&send_temp[strlen(send_temp)], "%s", "2C");
    sprintf(send_value_double2, "%f", ew_value);
    hex_to_str((u8 *)&send_temp[strlen(send_temp)], (u8 *)send_value_double2, strlen(send_value_double2) % 32);
    sprintf(&send_temp[strlen(send_temp)], "%s", "\r\n");
    module_at_tx(send_temp);
}

static int ddmm2dd(const char *ddmm, char *dd)
{
    if (NULL == ddmm || NULL == dd)
    {
        return -1;
    }
    int lenSrc = strlen(ddmm) + 1;
    int lenMm = 0;
    int flag = 1;

    memcpy(dd, ddmm, lenSrc);

    char *pcMm;
    double dMm;
    int iMm;

    /* 把pcMm定位到小数点位置 */
    pcMm = strstr(dd, ".");

    if (pcMm == NULL) /* 不含小数点的情况 */
    {
        pcMm = dd + strlen(dd) - 2;
        iMm = atoi(pcMm);
        dMm = iMm / 60.0;
    }
    else /* 含有小数点的情况 */
    {
        /* 有度 */
        if (pcMm - dd > 2)
        {
            pcMm = pcMm - 2;
        }
        else /* 没有度,只有分 */
        {
            pcMm = dd;
            flag = 0;
        }
        /* 将字符串转换为浮点数 */
        dMm = atof(pcMm);
        /* 将分转换为度 */
        dMm /= 60.0;
    }
    /* 把转换后的浮点数转换为字符串 */
    sprintf(pcMm, "%lf", dMm);
    if (flag)
    {
        /* 去掉小数点前面的0 */
        strcpy(pcMm, pcMm + 1);
    }
    /* 保留小数点后6位 */
    pcMm = strstr(dd, ".");
    lenMm = strlen(pcMm);
    if (lenMm > (6 + 2))
    {
        memset(pcMm + 6 + 2, 0, lenMm - 6 - 2);
    }

    return 1;
}

static void dispaly_gps_ico(u8 state)
{
    switch (state)
    {
    case 0: //未开启定位
        lcd_api_bmp_with_one_color(ICO_X_START_GPS, ICO_GPS_Y_START, gImage_gps_12x12, 12, 12, COLOR_ICO_BACKGROUND, COLOR_ICO_BACKGROUND);
        break;
    case 1: //开启定位-未定位成功
        lcd_api_bmp_with_one_color(ICO_X_START_GPS, ICO_GPS_Y_START, gImage_gps_12x12, 12, 12, COLOR_RED, COLOR_ICO_BACKGROUND);
        break;
    case 2: //定位成功
        lcd_api_bmp_with_one_color(ICO_X_START_GPS, ICO_GPS_Y_START, gImage_gps_12x12, 12, 12, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    default:
        break;
    }
}
