/*
poc处理所有收到的at指令，不能在此文件中包含UI显示相关操作
*/
#define POC_DEFINE
#include "poc.h"
#include "ec20_at_cmd.h"
#include "ui.h"
#include "lcd_api.h"
#include "stdlib.h"
#include "tom_flash.h"
#include "pa_led_volume.h"
#include <stdio.h>
#include "timed.h"
#include "language.h"
#include "battery.h"
#include "new_ui_data.h"
#include "date_and_time.h"
#include "gps.h"
//extern RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef read_time;
RTC_DateTypeDef read_data;

//u8 rx_buf_old_test[UART_BUF_LEN];
bool g_bug_fix_first_tts_play_flag = false; //true表示要播报在线人数

//static u8 is_bcd(u8 y, u8 m, u8 d);
//static int get_weekday(int y, int m, int d);
//static void RTC_Date_add(RTC_DateTypeDef *Date, u8 y, u8 m, u8 d, u8 w);
//static u8 days_in_month(u8 month, u8 year);
//static u8 is_leap_year(unsigned int year);
static FlagStatus para_is_reas(const u8 *pval);
static void bug_fix_not_logged(void);

//flg=0 开机第一次 flg=1 恢复出厂或卓智达写码
void main_init(u8 flg)
{

    gps_init();

    if (flg == 1) //解决恢复出厂设置白屏的问题
    {
        uart2_buf_clear(); //恢复出厂设置时需要清除残留数组
        volume_init();     //解决恢复出厂设置后不显示电量的问题
    }
    else
    {
        tom_interface_init();
    }

    tim_it_init();
    uart_it_init();

    hc595_init();
    lcd_api_init();

    set_gpio_state(GPIO_MODULE_PWR_EN, on);
    set_gpio_state(GPIO_PWR_CTL, on);
    set_gpio_state(GPIO_MODULE_PWRKEY, on);
    adc_start_init();

    poc_init();
    telephone_init();
    ui_init();
    set_gpio_state(GPIO_LCD_BL, on);
}

void poc_init(void)
{
#ifdef POC_ZZD
    //卓智达获取成员需要组序号，必须获取一次群组才能得到，所有默认index为0000
    memset(poc.my_group_index, 0, sizeof(poc.my_group_index));
    strcpy((char *)poc.my_group_index, "0000");
#endif
    poc.got_time_flag = 0;

    //登录中的标志位
    poc.module_ready_flag = false;
    poc.module_init_flag = false;
    poc.sim_card_flag = FIRST_CHECK_SIM_CARD;
    poc.get_meid_flag = 0;
    poc.get_imsi_sim_card_type = 0;
    poc.get_module_ver_flag = false;
    poc.get_poc_ver_flag = false;
    poc.get_account_flag = false;
    poc.csq_value = 0;
    poc.csq_level = 0;
    poc.network_type = 0;
    poc.has_logged_flag = false;
    poc.logged_before_flag = false;
    poc.offline_test_flag = 0;
    poc.tts_state = false;
    poc.can_i_speak_flag = true;
    poc.headphone_state = off;

    poc.system_clock_flag = CLOCK_48M;

    poc.getting_users_flag = false;
    poc.getting_group_flag = false;

    led_state_set(NOT_LOGGED);

    read_flash_poc_data();

    if (poc.flash_data.ptt_tone_toggle == 0)
    {
    }
    else
    {
        poc.flash_data.ptt_tone_toggle = 1;
    }

    if (poc.flash_data.encode_8k_toggle == 0)
    {
    }
    else
    {
        poc.flash_data.encode_8k_toggle = 1;
    }

    if (poc.flash_data.sim_card_toggle == 0)
    {
        //SIM1
        set_gpio_state(GPIO_SIM_TOGGLE, off);
    }
    else
    {
        poc.flash_data.sim_card_toggle = 1;
        //SIM2
        set_gpio_state(GPIO_SIM_TOGGLE, on);
    }

    if (poc.flash_data.gps_enable_flag == 0)
    {
        set_gpio_state(GPIO_GPS_EN, off);
    }
    else
    {
        poc.flash_data.gps_enable_flag = 1;
        set_gpio_state(GPIO_GPS_EN, on);
    }

    poc.usb_toggle = 0;
    set_gpio_state(GPIO_USB_EN, off); //关闭adb

    if (poc.flash_data.key_lock_toggle == 0)
    {
    }
    else
    {
        poc.flash_data.key_lock_toggle = 1;
    }

    switch (poc.flash_data.backlight_flag)
    {
    case 0:
        poc.backlight_flash_time = 10;
        break;
    case 1:
        poc.backlight_flash_time = 20;
        break;
    case 2:
        poc.backlight_flash_time = 30;
        break;
    case 3:
        poc.backlight_flash_time = 0xff; //常亮
        break;

    default:
        poc.flash_data.backlight_flag = 2;
        poc.backlight_flash_time = 30;
        break;
    }

    if (poc.flash_data.network_flag > 4)
    {
        poc.flash_data.network_flag = 1;
    }
    poc_set_net_mode(poc.flash_data.network_flag);

    if (poc.flash_data.time_zone_flag > 25)
    {
#ifdef LANGUAGE_ENGLISH
        poc.flash_data.time_zone_flag = 17; //华盛顿-5
#else
        poc.flash_data.time_zone_flag = 8;
#endif
    }

    if (poc.volume_level > 8)
    {
        poc.volume_level = 8;
    }

    if (poc.flash_data.tx_time_limit == 0)
    {
    }
    else
    {
        poc.flash_data.tx_time_limit = 1;
    }

    save_flash_poc_data();
    delay_ms(100);
    read_flash_poc_data();

    read_flash_hotkey_data();
    //读取hotkey
    for (u8 i = 0; i < 10; i++)
    {
        if (poc.hotkey_info[i].hotkey_type > 3)
        {
            poc.hotkey_info[i].hotkey_type = 0;
        }
    }
    save_flash_hotkey_data();
}

void poc_first_deal(void)
{
    static u8 buf_temp_deal[BUF_TEMP_SIZE] = {0};
    static u16 cmp_cnt = 0;
    while (poc.module_data.read_p != poc.module_data.write_p) //有数据未处理
    {
        if (poc.module_data.recv_buf[(poc.module_data.read_p + cmp_cnt) % UART_RXBUF_LEN_MAX] == '\r' ||
            poc.module_data.recv_buf[(poc.module_data.read_p + cmp_cnt) % UART_RXBUF_LEN_MAX] == '\n') //检测到未处理数据中有 \r\n结尾的数据
        {
            if (cmp_cnt >= 2) //只处理长度大于2的指令
            {
                memset(buf_temp_deal, 0, BUF_TEMP_SIZE);
                if (poc.module_data.read_p + cmp_cnt < UART_RXBUF_LEN_MAX) //如果不是循环存储
                    memcpy((void *)buf_temp_deal, (const void *)&poc.module_data.recv_buf[poc.module_data.read_p], cmp_cnt % BUF_TEMP_SIZE);
                else
                {
                    memcpy((void *)buf_temp_deal,
                           (const void *)&poc.module_data.recv_buf[poc.module_data.read_p],
                           (UART_RXBUF_LEN_MAX - poc.module_data.read_p) % BUF_TEMP_SIZE);
                    memcpy((void *)&buf_temp_deal[UART_RXBUF_LEN_MAX - poc.module_data.read_p], (const void *)poc.module_data.recv_buf,
                           (poc.module_data.read_p + cmp_cnt) % UART_RXBUF_LEN_MAX % (BUF_TEMP_SIZE - ((UART_RXBUF_LEN_MAX - poc.module_data.read_p) % BUF_TEMP_SIZE)));
                }
                poc_process(buf_temp_deal);
                SEGGER_RTT_printf(0, "cmd->%s\r\n", buf_temp_deal);
            }
            poc.module_data.read_p = (poc.module_data.read_p + (cmp_cnt + 1)) % UART_RXBUF_LEN_MAX; //处理过了
            cmp_cnt = 0;
        }
        else
        {
            if (((poc.module_data.read_p < poc.module_data.write_p) && (cmp_cnt + 1 < poc.module_data.write_p - poc.module_data.read_p)) ||
                ((poc.module_data.read_p > poc.module_data.write_p) && (cmp_cnt + 1 < (UART_RXBUF_LEN_MAX - poc.module_data.read_p + poc.module_data.write_p))))
            {
                if (cmp_cnt < 200)
                    cmp_cnt++;
                else
                {
                    poc.module_data.write_p = 0;
                    poc.module_data.read_p = 0;
                    cmp_cnt = 0;
                    break;
                }
            }
            else
            {
                if (cmp_cnt >= 200)
                {
                    poc.module_data.write_p = 0;
                    poc.module_data.read_p = 0;
                    cmp_cnt = 0;
                }
                break;
            }
        }
    }
}

#if 1
void poc_process(u8 *ptr)
{
    u8 name_len = 0;

    if (memcmp(ptr, "\r\n", strlen("\r\n")) == 0 ||
        memcmp(ptr, "\r", strlen("\r")) == 0 ||
        memcmp(ptr, "\n", strlen("\n")) == 0)
    {
        ptr[0] = ptr[0];
        //识别空指令\r\n，不处理
    }
    else if (memcmp(ptr, "AT+", strlen("AT+")) == 0)
    {
        ptr[0] = ptr[0];
    }
    else
    {
        // SEGGER_RTT_printf(0, "%s-%d:cmd--%s\r\n", __FILE__, __LINE__, ptr);
        if (memcmp(ptr, AT_RX_OK, strlen(AT_RX_OK)) == 0) //收到ok
        {
            if (poc.module_ready_flag == false)
            {
                poc.module_ready_flag = true;
            }
        }
        else if (memcmp(ptr, AT_RX_CPIN_READY, strlen(AT_RX_CPIN_READY)) == 0) //SIM  READY
        {
            if (poc.sim_card_flag == FIRST_CHECK_SIM_CARD)
            {
                if (poc.flash_data.sim_card_toggle == 0)
                {
                    poc.sim_card_flag = HAVE_SIM_CARD_1;
                    lcd_api_bmp_with_one_color(ICO_X_START_SIM_CARD_TYPE, 0, gImage_sim_card1_16x16, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
                }
                else
                {
                    poc.sim_card_flag = HAVE_SIM_CARD_2;
                    lcd_api_bmp_with_one_color(ICO_X_START_SIM_CARD_TYPE, 0, gImage_sim_card2_16x16, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
                }
            }
            else if (poc.sim_card_flag == SECOND_CHECK_SIM_CARD)
            {
                if (poc.flash_data.sim_card_toggle == 0)
                {
                    poc.sim_card_flag = HAVE_SIM_CARD_2;
                    lcd_api_bmp_with_one_color(ICO_X_START_SIM_CARD_TYPE, 0, gImage_sim_card2_16x16, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
                }
                else
                {
                    poc.sim_card_flag = HAVE_SIM_CARD_1;
                    lcd_api_bmp_with_one_color(ICO_X_START_SIM_CARD_TYPE, 0, gImage_sim_card1_16x16, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
                }
            }

            if (ui.page == UI_PAGE_BOOT)
            {
                ui_logging(UI_OPT_PART_REFRESH);
            }
        }
        else if (memcmp(ptr, AT_RX_CME_ERROR, strlen(AT_RX_CME_ERROR)) == 0) //SIM  ERROR
        {
            if (poc.sim_card_flag == FIRST_CHECK_SIM_CARD)
            {
                if (poc.flash_data.sim_card_toggle == 0)
                {
                    set_gpio_state(GPIO_SIM_TOGGLE, on);
                }
                else
                {
                    set_gpio_state(GPIO_SIM_TOGGLE, off);
                }

                module_at_tx(AT_TX_POWEROFF);
                delay_ms(500);
                module_at_tx(AT_TX_POWERON);
                delay_ms(500);

                poc.sim_card_flag = SECOND_CHECK_SIM_CARD;
            }
            else if (poc.sim_card_flag == SECOND_CHECK_SIM_CARD)
            {
                poc.sim_card_flag = SECOND_NO_SIM_CARD;

                if (ui.page == UI_PAGE_BOOT)
                {
                    ui_logging(UI_OPT_PART_REFRESH);
                }
            }
            else
            {
                poc.sim_card_flag = poc.sim_card_flag;
            }
        }
        else if (memcmp(ptr, AT_RX_MEID_1, strlen(AT_RX_MEID_1)) == 0) //MEID:0x00A10000689B7210
        {
            poc.get_meid_flag = 2;
            memset(poc.meid_number, 0, strlen((char *)poc.meid_number));
            memcpy(poc.meid_number, ptr + 11, 7);
            //memcpy(poc.meid_number, ptr, strlen((char *)ptr) % 32);
        }
        else if (memcmp(ptr, AT_RX_MEID_2, strlen(AT_RX_MEID_2)) == 0) //MEID:A100005DBFCC45
        {
            poc.get_meid_flag = 2;
            memset(poc.meid_number, 0, strlen((char *)poc.meid_number));
            memcpy(poc.meid_number, ptr + 7, 7);
            //memcpy(poc.meid_number, ptr, strlen((char *)ptr) % 32);
        }
        else if (memcmp(ptr, AT_RX_MEID_3, strlen(AT_RX_MEID_3)) == 0) //MEID:0x80F365F7
        {
            poc.get_meid_flag = 2;
            memset(poc.meid_number, 0, strlen((char *)poc.meid_number));
            memcpy(poc.meid_number, ptr + 3, 7);
            //memcpy(poc.meid_number, ptr, strlen((char *)ptr) % 32);
        }
        else if (memcmp(ptr, AT_RX_IMSI, strlen(AT_RX_IMSI)) == 0) //
        {
            memcpy(poc.imsi_number, ptr, strlen((char *)ptr) % 32);
            if (memcmp(ptr, "46000", 5) == 0 ||
                memcmp(ptr, "46002", 5) == 0 ||
                memcmp(ptr, "46004", 5) == 0 ||
                memcmp(ptr, "46007", 5) == 0)
            {
                poc.get_imsi_sim_card_type = 1;
            }
            else if (memcmp(ptr, "46001", 5) == 0 ||
                     memcmp(ptr, "46006", 5) == 0 ||
                     memcmp(ptr, "46009", 5) == 0)
            {
                poc.get_imsi_sim_card_type = 2;
            }
            else if (memcmp(ptr, "46003", 5) == 0 ||
                     memcmp(ptr, "46011", 5) == 0)
            {
                poc.get_imsi_sim_card_type = 3;
            }
            else
            {
                poc.get_imsi_sim_card_type = 3;
            }
        }
        else if (memcmp(ptr, AT_RX_MODULE_VER, strlen(AT_RX_MODULE_VER)) == 0) //module ver
        {
            memcpy(poc.module_ver, ptr + 10, (strlen((char *)ptr) - 10) % 40);
            poc.get_module_ver_flag = true;
        }

        else if (memcmp(ptr, AT_RX_CSQ, strlen(AT_RX_CSQ)) == 0) //CSQ
        {
            poc.csq_value = atoi((char *)(ptr + 6));

            if (poc.csq_value > 3 && poc.csq_value != 99)
            {
                if (poc.has_logged_flag == false)
                {
                    if (ui.page == UI_PAGE_BOOT)
                    {
                        ui_ico_csq(ICO_REFRESH_ALL);
                        ui_logging(UI_OPT_PART_REFRESH);
                    }
                }
            }
        }
        else if (memcmp(ptr, AT_RX_QCSQ_NOSERVICE, strlen(AT_RX_QCSQ_NOSERVICE)) == 0) //CSQ
        {
            poc.csq_value = 0;
        }
        else if (memcmp(ptr, AT_RX_QCSQ_LTE, strlen(AT_RX_QCSQ_LTE)) == 0) //CSQ
        {
            u8 i, cnt;
            int csq;
            for (i = 0, cnt = 0; i < 10; i++)
            {
                if (ptr[i + 12] == ',')
                {
                    if (++cnt == 2)
                    {
                        csq = atoi((const char *)ptr + i + 13);

                        if (csq >= -128 && csq <= -50)
                        {
                            if (csq >= -78)
                                poc.csq_value = 31;
                            else
                                poc.csq_value = 31 * (csq + 128) / 50;
                        }
                        else
                        {
                            poc.csq_value = 0;
                        }
                    }
                }
            }
        }
        else if (memcmp(ptr, AT_RX_HDRCSQ, strlen(AT_RX_HDRCSQ)) == 0) //CSQ
        {
            if (memcmp(ptr + 9, "20", 2) == 0)
                poc.csq_value = 9;
            else if (memcmp(ptr + 9, "40", 2) == 0)
                poc.csq_value = 17;
            else if (memcmp(ptr + 9, "60", 2) == 0)
                poc.csq_value = 23;
            else if (memcmp(ptr + 9, "80", 2) == 0)
                poc.csq_value = 31;
            else if (memcmp(ptr + 9, "99", 2) == 0)
                poc.csq_value = 0;
            else
                poc.csq_value = 0;
        }
        else if (memcmp(ptr, AT_RX_QNWINFO, strlen(AT_RX_QNWINFO)) == 0)
        {
            if (memcmp(ptr + 11, "TDD LTE", 7) == 0 ||
                memcmp(ptr + 11, "FDD LTE", 7) == 0)
            {
                poc.network_type = 4; //4G
            }
            else if (memcmp(ptr + 11, "HSUPA", 5) == 0 ||
                     memcmp(ptr + 11, "HSDPA", 5) == 0 ||
                     memcmp(ptr + 11, "WCDMA", 5) == 0 ||
                     memcmp(ptr + 11, "HDR", 3) == 0 ||
                     memcmp(ptr + 11, "TDSCDMA", 7) == 0 ||
                     memcmp(ptr + 11, "CDMA2000", 8) == 0 ||
                     memcmp(ptr + 11, "EVDO", 4) == 0 ||
                     memcmp(ptr + 11, "EHRPD", 5) == 0 ||
                     memcmp(ptr + 11, "HSPA+", 5) == 0)
            {
                poc.network_type = 3; //3G
            }
            else if (memcmp(ptr + 11, "GSM", 3) == 0 ||
                     memcmp(ptr + 11, "GPRS", 4) == 0 ||
                     memcmp(ptr + 11, "CDMA", 4) == 0 ||
                     memcmp(ptr + 11, "EDGE", 4) == 0)
            {
#ifdef LENOVO
                //规避仅限4G下显示2G的问题
                if (poc.flash_data.network_flag == 1)
                {
                    poc.network_type = 4;
                }
                else
                {
                    poc.network_type = 2; //2G
                }
#else
                poc.network_type = 2; //2G
#endif
            }
            else if (memcmp(ptr + 11, "No Service", 9) == 0)
            {
                poc.network_type = 0; //No Service
                poc.csq_value = 0;
            }
            else
            {
                /* code */
            }
        }
        else if (memcmp(ptr, AT_RX_QTTS0, strlen(AT_RX_QTTS0)) == 0) //CSQ
        {
            if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL)
            {
                //电话状态播报incoming call 不关喇叭 有嘟嘟音
            }
            else
            {
                if (poc.receive_voice == false) //解决进组后听不到说话人的声音
                {
                    set_gpio_state(GPIO_PA_EN, off);
                    SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                }
            }
        }
        else if (memcmp(ptr, AT_RX_POC_02, strlen(AT_RX_POC_02)) == 0)
        {
            //+POC:0200000069703d3131342e3134312e3133322e35353b69643d687561323330313b00
            //+POC:0200000069703d302e302e302e303b69643d54333330303030303031383b00
            //+POC:0200000069703d302e302e302e303b69643d54333330303030303031383b6770733d313b696e763d313b00
            u8 temp[128], i;
            char *str1 = NULL, *str2 = NULL, *str3 = NULL, *str4 = NULL;
            u8 len;
            u8 nct_3b;

            len = strlen((const char *)ptr);
            SEGGER_RTT_printf(0, "%s!!! INTO 02-1 !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);
            if ((ptr[len - 4] == '3') && (ptr[len - 3] == 'b') && (ptr[len - 2] == '0') && (ptr[len - 1] == '0'))
            {
                SEGGER_RTT_printf(0, "%s!!! INTO 02-2 !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);
                str_to_hex(ptr + 13, temp, (strlen((char *)ptr) - 13) % 256);
                str1 = strstr((const char *)temp, (const char *)"ip=");
                str2 = strstr((const char *)temp, (const char *)"id=");
                str3 = strstr((const char *)temp, (const char *)"gps=");
                str4 = strstr((const char *)temp, (const char *)"inv=");

                nct_3b = 0x00;
                for (i = 0; i < 127; i++)
                {
                    if (temp[i] == ';')
                    {
                        temp[i] = 0;
                        nct_3b++;
                    }
                }

#ifdef POC_ZZD
                if (str1 != NULL)
                {
                    memset(poc.flash_data.server_ip, 0, sizeof(poc.flash_data.server_ip));
                    memcpy(poc.flash_data.server_ip, str1 + 3, (strlen((char *)str1) - 3) % 32);
                }

                if (str2 != NULL)
                {
                    memset(poc.flash_data.user_id, 0, sizeof(poc.flash_data.user_id));
                    memcpy(poc.flash_data.user_id, str2 + 3, strlen((char *)str2) - 3);
                }

                //poc.flash_data.invite_enable_toggle = 1;
                //卓智达的定位通过poc:89判断

#else
                if ((str1 != NULL) && (nct_3b == 4))
                {
                    SEGGER_RTT_printf(0, "%s!!! INTO 02-3 !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);

                    memset(poc.flash_data.server_ip, 0, sizeof(poc.flash_data.server_ip));
                    memcpy(poc.flash_data.server_ip, str1 + 3, (strlen((char *)str1) - 3) % 32);
                }

                if ((str2 != NULL) && (nct_3b == 4))
                {
                    SEGGER_RTT_printf(0, "%s!!! INTO 02-4 !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);
                    memset(poc.flash_data.user_id, 0, sizeof(poc.flash_data.user_id));
                    memcpy(poc.flash_data.user_id, str2 + 3, strlen((char *)str2) - 3);
                }

                if ((str3 != NULL) &&
                    (nct_3b == 4) &&
                    ((*(str3 + 4)) == 0x30 || (*(str3 + 4)) == 0x31)) //(app->status == APP_STATUS_WRITE_CODE_AIR || app->status == APP_STATUS_WRITE_CODE_AIR_END)
                {
                    SEGGER_RTT_printf(0, "%s!!! INTO 02-5 !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);

                    if (*(str3 + 4) == 0x31)
                    {
                        poc.flash_data.gps_enable_flag = 1;
                    }
                    else
                    {
                        poc.flash_data.gps_enable_flag = 0;
                    }
                }

                if ((str4 != NULL) && (nct_3b == 4) &&
                    ((*(str4 + 4)) == 0x30 || (*(str4 + 4)) == 0x31)) //(app->status == APP_STATUS_WRITE_CODE_AIR || app->status == APP_STATUS_WRITE_CODE_AIR_END)
                {
                    u8 tmp = 0;

                    tmp = *(str4 + 4);

                    if (tmp == '0')
                    {
                        poc.flash_data.invite_enable_toggle = 0;
                    }
                    else if (tmp == '1')
                    {
                        poc.flash_data.invite_enable_toggle = 1;
                    }
                    else
                    {
                        poc.flash_data.invite_enable_toggle = 0;
                    }

                    //poc.flash_data.invite_enable_toggle = *(str4 + 4);
                }
#endif
                if (RESET == para_is_reas((const u8 *)&(poc.flash_data.server_ip)))
                {
                    nct_3b = 0x00;
                }

                if (RESET == para_is_reas((const u8 *)&(poc.flash_data.user_id)))
                {
                    nct_3b = 0x00;
                }
            }
            poc.get_account_flag = true;

            g_air_code_account_update_flag = true;

            save_flash_poc_data();
        }
        else if (memcmp(ptr, AT_RX_POC_0D, strlen(AT_RX_POC_0D)) == 0)
        {
            //+POC:0d0000000007   群组列表信息， 群组个数
            bug_fix_not_logged();
            poc.getting_group_flag = false;

            poc.total_number_of_groups = str2int_hex((char *)(ptr + 13));
#ifdef POC_ZZD //卓智达的群组数需要在总人数上-1
            if (poc.total_number_of_groups >= 1)
            {
                poc.total_number_of_groups--;
            }
#endif
            if (poc.total_number_of_groups != poc.group_index)
            {
                //如果这两个值不相等，则表示有群组丢失,在获取一次，直到获取成功
                poc.group_index = 0;
                module_at_tx(AT_TX_GROUP_REFRESH);
            }
            else
            {
                if (poc.total_number_of_groups >= GROUP_NUM_MAX)
                {
                    poc.total_number_of_groups = GROUP_NUM_MAX;
                }

                if (ui.page == UI_PAGE_GROUP)
                {
                    ui_group(UI_OPT_PART_REFRESH);
                }
            }
        }
        else if (memcmp(ptr, AT_RX_POC_0E, strlen(AT_RX_POC_0E)) == 0)
        {

            //+POC:0e0000000002
            bug_fix_not_logged();
            g_bug_fix_first_tts_play_flag = true;
            poc.getting_users_flag = false;
#ifdef POC_ZZD
            //卓智达0e包含离线成员，TYT平台只包含在线成员
            poc.total_number_of_users = poc.user_index;
#else
            if (ui.page == UI_PAGE_FRIEND)
            {
                //好友的0e不减
                poc.total_number_of_users = str2int_hex((char *)(ptr + 13));
            }
            else
            {
                //成员的0e减去自己
                poc.total_number_of_users = str2int_hex((char *)(ptr + 13)) - 1;
            }
#endif

            if (ui.page != UI_PAGE_PHONE_DIAL && poc.total_number_of_users != 0)
            {
#ifdef LANGUAGE_ENGLISH
                char tts_dat[32];
                //数字31转十进制字符串“31”
                memset(tts_dat, 0, sizeof(tts_dat));
                int2str_dec(tts_dat, poc.total_number_of_users);
                memcpy(&tts_dat[strlen(tts_dat)], " users online", 18);
                play_tts(2, tts_dat);
#else
                u8 i = 0, j = 0;
                char ZTTSDataBuf[64];
                char DataBuf[6] = {0, 0, 0, 0, 0, 0};
                memset(ZTTSDataBuf, 0, sizeof(ZTTSDataBuf));
                itoa((int)(poc.total_number_of_users), DataBuf, 10);
                for (i = 0; i < strlen(DataBuf); i++)
                {
                    ZTTSDataBuf[j++] = '0';
                    ZTTSDataBuf[j++] = '0';
                    ZTTSDataBuf[j++] = '3';
                    ZTTSDataBuf[j++] = DataBuf[i];
                }
                play_tts_with_2Param(2, ZTTSDataBuf, "4EBA57287EBF");
#endif
            }

            if (poc.total_number_of_users >= USER_NUM_MAX)
            {
                poc.total_number_of_users = USER_NUM_MAX;
            }

            if (ui.page == UI_PAGE_USER)
            {
                if (poc.total_number_of_users == 0)
                {
                    ui_user(UI_OPT_OTHER_REFRESH);
                }
                else
                {
                    ui_user(UI_OPT_PART_REFRESH);
                }
            }

            if (ui.page == UI_PAGE_FRIEND)
            {
                if (poc.total_number_of_users == 0)
                {
                    ui_friend(UI_OPT_OTHER_REFRESH);
                }
                else
                {
                    ui_friend(UI_OPT_PART_REFRESH);
                }
            }
        }
        else if (memcmp(ptr, AT_RX_POC_41, strlen(AT_RX_POC_41)) == 0)
        {
            //+POC:41020064
            poc.poc_upgrade_state = ptr[8] - 0x30;
            if (memcmp(ptr, "+POC:410200", strlen("+POC:410200")) == 0)
            {
                str_to_hex(ptr + 11, (u8 *)&poc.poc_upgrade_percent, 2);
            }
            else
            {
                poc.poc_upgrade_state = poc.poc_upgrade_state;
            }
        }
#ifdef POC_ZZD
        else if (memcmp(ptr, AT_RX_POC_26, strlen(AT_RX_POC_26)) == 0)
        {
            poc.poc_upgrade_percent = 0;
            poc.poc_upgrade_state = 0;
        }
        else if (memcmp(ptr, AT_RX_POC_FE0, strlen(AT_RX_POC_FE0)) == 0)
        {
            poc.poc_upgrade_percent = 0;
            poc.poc_upgrade_state = 3;
        }
        else if (memcmp(ptr, AT_RX_POC_FE1, strlen(AT_RX_POC_FE1)) == 0)
        {
            poc.poc_upgrade_percent = 100;
            poc.poc_upgrade_state = 1;
        }
#endif
        else if (memcmp(ptr, AT_RX_POC_45, strlen(AT_RX_POC_45)) == 0)
        {
            poc.enter_air_code_flag = true;
        }
#ifdef POC_ZZD
        else if (memcmp(ptr, AT_RX_POC_7d00, strlen(AT_RX_POC_7d00)) == 0)
        {
            poc.get_poc_ver_flag = true;
            str_to_hex(ptr + 13, poc.poc_ver, strlen((char *)ptr) % 64);
        }
#else
        else if (memcmp(ptr, AT_RX_POC_46, strlen(AT_RX_POC_46)) == 0)
        {
            //kirisunPocv1.71 [2020.11.27]
            poc.get_poc_ver_flag = true;
            memcpy(poc.poc_ver, ptr + 14, (strlen((char *)ptr) - 14) % 32);
        }
#endif
        else if (memcmp(ptr, AT_RX_POC_80, strlen(AT_RX_POC_80)) == 0)
        {
            //+POC:80 00 0000 0001 00002d3a 000e 9b513a534b6dd58bc47e0000//+POC:80 00 0000 0001 00000287 00d9797...
#ifdef POC_ZZD
            if (memcmp(ptr + 17, "ffffffff", strlen("ffffffff")) == 0)
            {
                //+POC:80 00 0000 0000 ffffffff
                //卓智达群组列表第一个显示：好友列表
            }
            else
            {
                memcpy(poc.group_list_index[poc.group_index % GROUP_NUM_MAX], ptr + 13, 4);
                memcpy(poc.group_list_id[poc.group_index % GROUP_NUM_MAX], ptr + 17, 8);
                memset(poc.group_list_name[poc.group_index % GROUP_NUM_MAX], 0, POC_NAME_LENTH_MAX);

                name_len = strlen((char *)(ptr + 29));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;

                str_to_hex(ptr + 29, poc.group_list_name[poc.group_index % GROUP_NUM_MAX], name_len);
                poc.group_index++; //start from 0

                if (memcmp(ptr + 17, poc.my_group_id, 8) == 0)
                {
                    memcpy(poc.my_group_index, ptr + 13, 4);
                }
            }
#else
            u8 group_index = 0;
            char temp_group_list_index[4 + 1];

            //解决有的群组刷不出来的问题（有的80数据丢失）
            memset(temp_group_list_index, 0, sizeof(temp_group_list_index));
            memcpy(temp_group_list_index, ptr + 13, 4);
            group_index = str2int_hex(temp_group_list_index) - 1; //start from 0

            memcpy(poc.group_list_index[group_index % GROUP_NUM_MAX], ptr + 13, 4);
            memcpy(poc.group_list_id[group_index % GROUP_NUM_MAX], ptr + 17, 8);
            memset(poc.group_list_name[group_index % GROUP_NUM_MAX], 0, POC_NAME_LENTH_MAX);

            name_len = strlen((char *)(ptr + 29));
            if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                name_len = ORIGINAL_NAME_LENTH_MAX;

            str_to_hex(ptr + 29, poc.group_list_name[group_index % GROUP_NUM_MAX], name_len);
            poc.group_index++;                       //start from 0
#endif
        }
        else if (memcmp(ptr, AT_RX_POC_81, strlen(AT_RX_POC_81)) == 0)
        {
            //+POC:81 03 0000 0001 000120e2 4b6dd58b310033000000
            //+POC:81 03 0000 0014 000451f1 e55d5c4f1f9658543f96035e6567d0630000

#ifdef POC_ZZD
            if (ptr[8] == '2' || ptr[8] == '3')
            {
                //1离线，2在线（不在此群组中），3在此群组中（在线）。

                memcpy(poc.user_list_index[poc.user_index % USER_NUM_MAX], ptr + 13, 4);
                memcpy(poc.user_list_id[poc.user_index % USER_NUM_MAX], ptr + 17, 8);
                memset(poc.user_list_name[poc.user_index % USER_NUM_MAX], 0, POC_NAME_LENTH_MAX);

                name_len = strlen((char *)(ptr + 25));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;

                str_to_hex(ptr + 25, poc.user_list_name[poc.user_index % USER_NUM_MAX], name_len);
                poc.user_index++;
            }

#else
            if (memcmp(ptr + 17, poc.my_id, 8) != 0) //为了去除自己，使用累加计数器获取列表
            {
                memcpy(poc.user_list_index[poc.user_index % USER_NUM_MAX], ptr + 13, 4);
                memcpy(poc.user_list_id[poc.user_index % USER_NUM_MAX], ptr + 17, 8);
                memset(poc.user_list_name[poc.user_index % USER_NUM_MAX], 0, POC_NAME_LENTH_MAX);

                name_len = strlen((char *)(ptr + 25));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;

                str_to_hex(ptr + 25, poc.user_list_name[poc.user_index % USER_NUM_MAX], name_len);
                poc.user_index++;
            }
            else
            {
                //+POC:8103 0000 0001 0003f99d 74
                //+POC:8202 0003f99d 74
                poc.my_group_id[0] = poc.my_group_id[0];
            }
#endif
        }
        else if (memcmp(ptr, AT_RX_POC_82, strlen(AT_RX_POC_82)) == 0) //login state
        {
            //+POC:82 02 000120e2 4b6dd58b310033000000
            //+POC:82020003f99d74
            if (memcmp(ptr + 7, "02", 2) == 0) //登录成功
            {
                led_state_set(STANDBY);
                memset(poc.my_name, 0, sizeof(poc.my_name));
                memcpy(poc.my_id, ptr + 9, 8);

                name_len = strlen((char *)(ptr + 17));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;
                str_to_hex(ptr + 17, poc.my_name, name_len); //将unicode转换成hex码

                poc.has_logged_flag = true;
                poc.logged_before_flag = true;
                if (ui.page == UI_PAGE_BOOT)
                {
                    ui_logging(UI_OPT_PART_REFRESH);
                }
            }
            else if (memcmp(ptr + 7, "01", 2) == 0) //登录中
            {
                if (poc.has_logged_flag == true)
                {
                    poc.offline_test_flag = 1;
                }

                poc.has_logged_flag = false;
            }
            else if (memcmp(ptr + 7, "00", 2) == 0) //掉线
            {
                if (poc.has_logged_flag == true)
                {
                    poc.offline_test_flag = 3;
                }

                led_state_set(NOT_LOGGED);
                poc.has_logged_flag = false;
                backlight_idle_clear();
                if (ui.page == UI_PAGE_DESKTOP) //如果掉线则显示掉线
                {
                    back_to_desktop();
                }
            }
            else
            {
                if (poc.has_logged_flag == true)
                {
                    poc.offline_test_flag = 3;
                }
                poc.has_logged_flag = false;
            }
        }
        else if (memcmp(ptr, AT_RX_POC_83, strlen(AT_RX_POC_83)) == 0)
        {
            backlight_idle_clear();
            //+POC:8300000120e44b6dd58b310035000000+POC:8b0001
            //+POC:8301ffffffff0000
            if (memcmp(ptr + 7, "01ffffffff", 10) == 0) //对方结束讲话
            {
                if (ui.page != UI_PAGE_INVITE) //解决A在呼叫时，B按发射键，B在单呼A时，会显示不出对方用户名（和优先级也有关）
                {
                    memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name));
                }

                poc.receive_voice = false;
                poc.can_i_speak_flag = true;
                if (poc.im_talking_flag == false) //A级别高，B级别低，B在对讲时，A抢呼B,A发射灯不亮
                {
                    led_state_set(STANDBY);
                }
            }
            else //对方开始讲话
            {
#ifdef POC_ZZD
                if (memcmp(ptr + 17, "2c673a672857b28bdd8b2e002e002e000000", 20) == 0) //本机正在说话
                {
                    //解决A级别高，B级别低，A在对讲结束后，B接收完马上按发射，有显示发射状态，没发射出去
                    //卓智达修改逻辑：发射时收到83才显示发射
                    poc.receive_voice = false;
                    poc.im_talking_flag = true;
                    led_state_set(TX);
                }

#endif

                if (memcmp(ptr + 17, "2c673a672857b28bdd8b2e002e002e000000", 20) != 0 && //本机正在说话
                    memcmp(ptr + 17, "dd8b43677a7af295", 16) != 0)                       // 话权空闲
                {
                    poc.receive_voice = true;
#ifdef POC_ZZD
                    if (poc.tts_state == true)
                    {
                        module_at_tx(AT_TX_QTTS0);
                        delay_ms(100);
                    }
#endif
                    if (*(ptr + 8) == '1')
                    {
                        poc.can_i_speak_flag = true;
                    }
                    else if (*(ptr + 8) == '0')
                    {
                        poc.can_i_speak_flag = false;
                    }

                    memcpy(poc.talk_user_id, ptr + 9, 8);
                    memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name));

                    name_len = strlen((char *)(ptr + 17));
                    if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                        name_len = ORIGINAL_NAME_LENTH_MAX;

                    str_to_hex(ptr + 17, poc.talk_user_name, name_len);

                    //if (poc.flash_data.ptt_tone_toggle == 0)
                    {
                        set_gpio_state(GPIO_PA_EN, on); //2
                        SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                    }
                    led_state_set(RX);
                }
            }
        }
        else if (memcmp(ptr, AT_RX_POC_84, strlen(AT_RX_POC_84)) == 0)
        {
            backlight_idle_clear();

            if (memcmp(ptr + 9, "7c54eb5310629f52", 16) == 0) //呼叫成功
            {
                //+POC:84007c54eb5310629f520000

                play_tts(2, TTS_Play_calling_success);
                ui_invite(UI_OPT_FIRST_REFRESH);
            }
            else if (memcmp(ptr + 9, "0090fa51344ef6657c54eb53", 20) == 0) //退出单呼 ok
            {
                //+POC:84000090fa51344ef6657c54eb530000

                play_tts(2, TTS_Exit_Single_call);
                delay_ms(100);
                ui_invite(UI_OPT_PART_REFRESH);
            }
#ifdef POC_ZZD
            else if (memcmp(ptr + 9, "f95bb965e065945e547b21", strlen("f95bb965e065945e547b21")) == 0 || //对方无应答（请求单呼）
                     memcmp(ptr + 9, "f95bb965d95f", strlen("f95bb965d95f")) == 0)                       //对方忙
            {
                //+POC:8400f95bb965e065945e547b21000000
                //+POC:8400f95bb965d95f21000000
                if (ui.page == UI_PAGE_INVITE)
                {
                    ui_invite(UI_OPT_PART_REFRESH);
                    play_tts(2, TTS_Call_failed); //播报”呼叫失败“
                }
            }
#else
            else if (memcmp(ptr + 9, "7c54eb533159258d", 16) == 0) //呼叫失败（请求单呼）
            {
                //+POC:84007c54eb533159258d0000
                if (ui.page == UI_PAGE_INVITE)
                {
                    ui_invite(UI_OPT_PART_REFRESH);
                    play_tts(2, TTS_Call_failed); //播报”呼叫失败“
                }
            }
#endif

            else if (memcmp(ptr + 9, "7c54fa513159258d", 16) == 0) //呼出失败（群组及单呼内的强呼失败）
            {
                //+POC:84007c54fa513159258d0000（包括组呼里的呼叫失败，同时按PTT就会有84007c54fa513159258d0000）
            }
            else if (memcmp(ptr + 9, "344ef6657c54eb53", 16) == 0) //被单呼 ok
            {
                //+POC:8400344ef6657c54eb53 4b6dd58b310035000000

                memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name));

                name_len = strlen((char *)(ptr + 25));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;

                str_to_hex(ptr + 25, poc.talk_user_name, name_len); //转成hex码
                ui_invite(UI_OPT_FIRST_REFRESH);

                play_unicode_tts(poc.talk_user_name);
            }
            else
            {
                //poc_audio_tts_paly((char *)ptr + 9, strlen((char *)ptr) - 9);
            }
        }
#ifdef POC_ZZD
        else if (memcmp(ptr, AT_RX_POC_8600, strlen(AT_RX_POC_8600)) == 0)
        {
            //+POC:860000002d3a9b513a534b6dd58bc47e0000
            //set_gpio_state(GPIO_PA_EN, on); //开启喇叭
            SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);

            if (memcmp(ptr + 9, "ffffffff", 8) != 0)
            {
                memset(poc.my_group_id, 0, sizeof(poc.my_group_id));
                memcpy(poc.my_group_id, ptr + 9, 8);
                memset(poc.my_group_name, 0, sizeof(poc.my_group_name));

                name_len = strlen((char *)(ptr + 17));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;
                str_to_hex(ptr + 17, poc.my_group_name, name_len); //将unicode转换成hex码

                poc.has_logged_flag = true;
                poc.logged_before_flag = true;
                led_state_set(STANDBY);

                if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL) //解决刷新
                {
                    //挂电话主动设置一次音量增益
                    ui.page = UI_PAGE_DESKTOP;
                    set_volume_level(poc.volume_level);
                    delay_ms(100);
                }

                if (ui.page == UI_PAGE_INVITE)
                {
                    poc.can_i_speak_flag = true; //解决被呼时优先级低的机器主动退出单呼，再按PTT无显示发射的问题
                    poc.im_talking_flag = false; //解决A单呼B,A在对讲，B退出单呼时，A发射没松开，还一直显示在发射状态
                }

                back_to_desktop();
            }
        }
        else if (memcmp(ptr, AT_RX_POC_8601, strlen(AT_RX_POC_8601)) == 0)
        {
            //+POC:860000002d3a9b513a534b6dd58bc47e0000
            set_gpio_state(GPIO_PA_EN, on); //开启喇叭
            SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);

            if (ui.page != UI_PAGE_INVITE) //解决本机主动进入单呼，显示对方为本机用户的问题
            {
                memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name));

                name_len = strlen((char *)(ptr + 17));
                if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                    name_len = ORIGINAL_NAME_LENTH_MAX;

                str_to_hex(ptr + 17, poc.talk_user_name, name_len); //转成hex码
                ui_invite(UI_OPT_FIRST_REFRESH);

                led_state_set(STANDBY);
            }
        }
#else
        else if (memcmp(ptr, AT_RX_POC_86, strlen(AT_RX_POC_86)) == 0)
        {
            //+POC:860000002d3a9b513a534b6dd58bc47e0000
            //set_gpio_state(GPIO_PA_EN, on); //开启喇叭
            SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);

            memset(poc.my_group_id, 0, sizeof(poc.my_group_id));
            memcpy(poc.my_group_id, ptr + 9, 8);
            memset(poc.my_group_name, 0, sizeof(poc.my_group_name));

            name_len = strlen((char *)(ptr + 17));
            if (name_len >= ORIGINAL_NAME_LENTH_MAX)
                name_len = ORIGINAL_NAME_LENTH_MAX;

            str_to_hex(ptr + 17, poc.my_group_name, name_len); //将unicode转换成hex码
            poc.has_logged_flag = true;
            poc.logged_before_flag = true;
#if 1 //解决A级别高，B级别低。B在1群组发射，A从1群组切换到2群组时，一直显示接收状态（切组时未收到8301ffffffff0000）
            poc.receive_voice = false;
            poc.can_i_speak_flag = true;
#endif
            led_state_set(STANDBY);

            if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL) //解决刷新
            {
                //挂电话主动设置一次音量增益
                ui.page = UI_PAGE_DESKTOP;
                set_volume_level(poc.volume_level);
                delay_ms(100);
            }

            back_to_desktop();
        }
#endif

        else if (memcmp(ptr, AT_RX_POC_89, strlen(AT_RX_POC_89)) == 0)
        {
            u32 privilege = 0;
            //+POC:8900ffff0000 关闭所有功能
            //+POC:8900ffff0001 好友 0000000000000001
            //+POC:8900ffff0002 单呼 0000000000000010
            //+POC:8900ffff1000 查位 0001000000000000
            //+POC:8900ffff0010 记录 0000000000010000
            //+POC:8900ffff0040 监听 0000000001000000
            //+POC:8900ffff0200 管控 0000001000000000
            //+POC:8900ffff0800 定位 0000100000000000
            //+POC:8900ffff3adf 全选 0011101011011111
            privilege = str2int_hex((char *)ptr + 13);
            if ((privilege | 0x800) == privilege) //与0x800表示开启了GPS定位
            {
                poc.flash_data.gps_enable_flag = 1;
            }
            else
            {
                poc.flash_data.gps_enable_flag = 0;
            }

            if ((privilege | 0x2) == privilege) //与0x800表示开启了GPS定位
            {
                poc.flash_data.invite_enable_toggle = 1;
            }
            else
            {
                poc.flash_data.invite_enable_toggle = 0;
            }

            save_flash_poc_data();
        }
        else if (memcmp(ptr, AT_RX_POC_8b, strlen(AT_RX_POC_8b)) == 0)
        {
            switch (ptr[10])
            {
            case '0': //播放结束
                poc.tts_state = false;
                if (poc.im_talking_flag == false)
                {
#ifdef POC_ZZD
                    if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL)
                    {
                        //电话状态播报incoming call 不关喇叭
                        //拨打电话时，卓智达播报无数据网络信号时，结束无声
                    }
                    else
                    {
                        set_gpio_state(GPIO_PA_EN, off); //83会打开嘟嘟音提示，但在此处会关闭
                        SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                    }

#endif
                    led_state_set(STANDBY);
                    poc.receive_voice = false;
                }

#ifndef POC_ZZD //TYT平台接收结束无tone音去除pa_on_timer(200)（尝试解决接收有时无声的问题）
                set_gpio_state(GPIO_PA_EN, off);
                SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
#endif

                break;
            case '1': //播放开始对方语音
                backlight_idle_clear();
                led_state_set(RX);
                set_gpio_state(GPIO_PA_EN, on); //3
                SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                poc.im_talking_flag = false;
                break;
            case '2': //开始播放TTS
                poc.tts_state = true;
                set_gpio_state(GPIO_PA_EN, on);
                SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                break;
            default:
                break;
            }
        }

        else if (memcmp(ptr, AT_RX_POC_96, strlen(AT_RX_POC_96)) == 0)
        {
#ifdef POC_ZZD
            //卓智达群组内呼叫失败，会下发+POC:960002，不会下发83

            if (ptr[10] == '2')
            {
                //call failed
            }
#endif
        }
        else if (memcmp(ptr, AT_RX_POC_0B, strlen(AT_RX_POC_0B)) == 0)
        {
#ifdef POC_ZZD
            // if (poc.can_i_speak_flag == true)
            // {
            //     poc.receive_voice = false;
            //     poc.im_talking_flag = true;
            //     led_state_set(TX);
            // }
#else
            //if (poc.can_i_speak_flag == true)//暂时去除，解决有时发射无显示的问题（实际发射出去了）
            {
                poc.receive_voice = false; //A优先级低B优先级高，解决A组呼时，B快速收发一次后一直异常显示“本机正在说话...”
                poc.im_talking_flag = true;
                led_state_set(TX);
            }
            bug_fix_not_logged();
#endif
        }
        else if (memcmp(ptr, AT_RX_POC_0C, strlen(AT_RX_POC_0C)) == 0)
        {
#ifdef POC_ZZD
            if (poc.can_i_speak_flag == true) //暂时去除，解决有时发射无显示的问题（实际发射出去了）
            {
                poc.im_talking_flag = false;
                led_state_set(STANDBY);
            }

            if (poc.receive_voice == false) //解决被呼时，按PTT抢呼失败，松开PTT接收无声的问题
            {
                if (poc.flash_data.ptt_tone_toggle == 0)
                {
                    pa_on_timer(100); //-2
                    SEGGER_RTT_printf(0, "%s %s-%d:pa_on_timer(100)%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                }
                else
                {
                    set_gpio_state(GPIO_PA_EN, off);
                    SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                }
            }
#else
            //if (poc.can_i_speak_flag == true)//暂时去除，解决有时发射无显示的问题（实际发射出去了）
            {
                poc.im_talking_flag = false;
                led_state_set(STANDBY);
            }

            if (poc.receive_voice == false) //解决被呼时，按PTT抢呼失败，松开PTT接收无声的问题
            {
                if (poc.flash_data.ptt_tone_toggle == 0)
                {
                    pa_on_timer(100); //-2
                    SEGGER_RTT_printf(0, "%s %s-%d:pa_on_timer(100)%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                }
                else
                {
                    set_gpio_state(GPIO_PA_EN, off);
                    SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                }
            }
#endif
        }

#ifdef CUSTOM_TIME
        else if (memcmp(ptr, AT_RX_POC_28, strlen(AT_RX_POC_28)) == 0) //
        {
            //+POC:2800000014061211060404
            //+POC:28000000a50719060b0c05
            //+POC:2800000007e40c1809262c04 科立讯
            char buffer[10];

            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 13), 2);
            datetime.year = str2int_hex(buffer);

            if ((datetime.year <= 47) && (datetime.year >= 17)) ///年份合法性判断17年生产，往后推30年
            {
                poc.got_time_flag = 1;
                datetime.year += 2000;
            }
            else
            {
                poc.got_time_flag = 0;
            }

            //月 "3137"转为17
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 15), 2);
            datetime.month = str2int_hex(buffer);

            //日
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 17), 2);
            datetime.day = str2int_hex(buffer);

            //时
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 19), 2);
            datetime.hour = str2int_hex(buffer);

            //分
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 21), 2);
            datetime.minite = str2int_hex(buffer);
            //秒
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 23), 2);
            datetime.second = str2int_hex(buffer);

            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, (u8 *)(ptr + 25), 2);
            datetime.week = str2int_hex(buffer);

            if (poc.got_time_flag == 1)
            {
                display_time_icon(1);
                if (ui.page == UI_PAGE_DESKTOP)
                {
                    display_date_and_week();
                }
            }
        }
#else
        else if (memcmp(ptr, AT_RX_POC_28, strlen(AT_RX_POC_28)) == 0) //
        {
            //+POC:280000001203020b082305
            poc.got_time_flag = 1;
            str_to_hex(ptr + 13, (u8 *)&poc.data.Year, 2);
            str_to_hex(ptr + 15, (u8 *)&poc.data.Month, 2);
            str_to_hex(ptr + 17, (u8 *)&poc.data.Date, 2);
            poc.data.WeekDay = get_weekday(poc.data.Year, poc.data.Month, poc.data.Date);

            str_to_hex(ptr + 19, (u8 *)&poc.cur_time.Hours, 2);
            str_to_hex(ptr + 21, (u8 *)&poc.cur_time.Minutes, 2);
            str_to_hex(ptr + 23, (u8 *)&poc.cur_time.Seconds, 2);

            HAL_RTC_SetDate(&hrtc, &poc.data, RTC_FORMAT_BIN);
            HAL_RTC_SetTime(&hrtc, &poc.cur_time, RTC_FORMAT_BIN);
            if (ui.page == UI_PAGE_DESKTOP && poc.lock_flag == false)
            {
                ui_show_bottom_button(BUTTOM_MENU);
            }
        }
#endif
#if 0
            else if (memcmp(ptr, AT_RX_POC_8c, strlen(AT_RX_POC_8c)) == 0) //TYT平台登录成功后下发一次时间
            {
                u8 buf[64] = {0};
                str_to_hex(ptr + 7, buf, 44);
                if (((buf[0] - '0') << 4) + (buf[1] - '0') >= 0x10 &&
                    ((buf[3] - '0') << 4) + (buf[4] - '0') <= 0x12 &&
                    ((buf[6] - '0') << 4) + (buf[7] - '0') <= 0x31)
                {
                    poc.data.Year = ((buf[0] - '0') << 4) + (buf[1] - '0');
                    poc.data.Month = ((buf[3] - '0') << 4) + (buf[4] - '0');
                    poc.data.Date = ((buf[6] - '0') << 4) + (buf[7] - '0');
                    poc.data.WeekDay = get_weekday(((buf[0] - '0') * 10) + (buf[1] - '0') + 2000, poc.data.Month, poc.data.Date);
                    if (is_bcd(poc.data.Year, poc.data.Month, poc.data.Date))
                    {
                        if (HAL_RTC_SetDate(&hrtc, &poc.data, RTC_FORMAT_BCD) != HAL_OK)
                        {
                            poc.data.Year = poc.data.Year;
                        }
                    }
                }
                if (((buf[9] - '0') << 4) + (buf[10] - '0') <= 0x24 &&
                    ((buf[12] - '0') << 4) + (buf[13] - '0') <= 0x60 &&
                    ((buf[15] - '0') << 4) + (buf[16] - '0') <= 0x60)
                {
                    poc.cur_time.Hours = ((buf[9] - '0') << 4) + (buf[10] - '0');
                    poc.cur_time.Minutes = ((buf[12] - '0') << 4) + (buf[13] - '0');
                    poc.cur_time.Seconds = ((buf[15] - '0') << 4) + (buf[16] - '0');
                    if (is_bcd(poc.cur_time.Hours, poc.cur_time.Minutes, poc.cur_time.Seconds))
                    {
                        if (HAL_RTC_SetTime(&hrtc, &poc.cur_time, RTC_FORMAT_BCD) != HAL_OK)
                        {
                            poc.data.Year = poc.data.Year;
                        }
                    }
                }
                poc.got_time_flag = 1;
            }
            else if (memcmp(ptr, "+CCLK: \"", 7) == 0) //cclk获取的是世界时间
            {
                poc.got_time_flag = 1;
                HAL_RTC_GetDate(&hrtc, &read_data, RTC_FORMAT_BCD);
                HAL_RTC_GetTime(&hrtc, &read_time, RTC_FORMAT_BCD);
                poc.data.Year = ((*(ptr + 8) - '0') << 4) + (*(ptr + 9) - '0');
                poc.data.Month = ((*(ptr + 11) - '0') << 4) + (*(ptr + 12) - '0');
                poc.data.Date = ((*(ptr + 14) - '0') << 4) + (*(ptr + 15) - '0');
                poc.data.WeekDay = get_weekday(poc.data.Year, poc.data.Month, poc.data.Date);

                poc.cur_time.Hours = ((*(ptr + 17) - '0') << 4) + (*(ptr + 18) - '0');
                poc.cur_time.Minutes = ((*(ptr + 20) - '0') << 4) + (*(ptr + 21) - '0');
                poc.cur_time.Seconds = ((*(ptr + 23) - '0') << 4) + (*(ptr + 24) - '0');
                if (poc.cur_time.Hours >= 0x16)
                {
                    RTC_Date_add(&poc.data, poc.data.Year, poc.data.Month, poc.data.Date, poc.data.WeekDay);
                    if (poc.cur_time.Hours <= 0x19)
                        poc.cur_time.Hours = (poc.cur_time.Hours - 0x16);
                    else
                        poc.cur_time.Hours = (poc.cur_time.Hours - 0x20) + 4;
                }
                else
                {
                    poc.cur_time.Hours = ((*(ptr + 18) - '0') + 8) % 10 + ((((*(ptr + 17) - '0' + *(ptr + 18) - '0') + 8) / 10) << 4);
                }

                if (is_bcd(poc.data.Year, poc.data.Month, poc.data.Date))
                {
                    if (HAL_RTC_SetDate(&hrtc, &poc.data, RTC_FORMAT_BCD) != HAL_OK)
                    {
                        poc.data.Year = poc.data.Year;
                    }
                }
                if (is_bcd(poc.cur_time.Hours, poc.cur_time.Minutes, poc.cur_time.Seconds))
                {
                    if (HAL_RTC_SetTime(&hrtc, &poc.cur_time, RTC_FORMAT_BCD) != HAL_OK)
                    {
                        poc.data.Year = poc.data.Year;
                    }
                }
            }
#endif

#if 1 //电话
        else if (memcmp(ptr, AT_RX_NO_CARRIER, strlen(AT_RX_NO_CARRIER)) == 0 ||
                 memcmp(ptr, AT_RX_NO_ANSWER, strlen(AT_RX_NO_ANSWER)) == 0)
        {
            backlight_idle_clear();

            if (poc.miss_call_state == 1)
            {
                //显示未接来电
                poc.miss_call_state = 2;
            }
            set_gpio_state(GPIO_PA_EN, off);
            SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);

            g_call_state = false;
            display_phone_time_count_down(g_call_state);
            //存入呼叫记录
            deposit_call_record();

            back_to_desktop();

            //挂电话主动设置一次音量增益
            set_volume_level(poc.volume_level);
            delay_ms(100);
        }
        else if (memcmp(ptr, AT_RX_CLCC, strlen(AT_RX_CLCC)) == 0)
        {
            backlight_idle_clear();
            //+CLCC: 2,0,3,0,0,"18588224620",128 主呼
            //+CLCC: 2,1,4,0,0,"18588224620",161 被呼

            u8 len = 0;
            u8 frame_count = 0; //, 号切割	数据帧，计数帧个数
            u8 p_start = 0;
            u8 p_end = 0;

            if (ui.page == UI_PAGE_INCOMING_CALL)
            {
                //+CLCC: 2,1,4,0,0,"18588224620",161 被呼
                if (ptr[9] == '1' && ptr[11] == '4')
                {
                    len = strlen((char *)ptr);

                    for (u8 i = 0; i < len; i++)
                    {
                        if (ptr[i] == 0x22) //0x22是'"'
                        {
                            if (frame_count == 0)
                            {
                                p_start = i + 1;
                            }
                            else if (frame_count == 1)
                            {
                                p_end = i;
                                memset(poc.dial.phone, 0, sizeof(poc.dial.phone));
                                memcpy(poc.dial.phone, ptr + p_start, p_end - p_start);

                                memset(poc.dial.name, 0, sizeof(poc.dial.name));
                                //根据呼入的号码查找联系人的名字
                                read_flash_call_contacts_data();
                                for (u8 i = 0; i < call_contacts.count; i++)
                                {
                                    if (strcmp((char *)call_contacts.current_contact[i].phone, (char *)poc.dial.phone) == 0)
                                    {
                                        memcpy(poc.dial.name, call_contacts.current_contact[i].name, sizeof(poc.dial.name));
                                        break;
                                    }
                                }

                                ui_incoming_call(UI_OPT_PART_REFRESH);
                            }
                            else
                            {
                                /* code */
                            }
                            frame_count++;
                        }
                    }
                }
            }
        }
        else if (memcmp(ptr, AT_RX_RING, strlen(AT_RX_RING)) == 0)
        {
            poc.lock_flag = false;
            backlight_idle_clear();

            if (ui.page != UI_PAGE_INCOMING_CALL)
            {
                ui_incoming_call(UI_OPT_FIRST_REFRESH);
                //设置音量增益
                module_at_tx("\r\n");
                delay_ms(30);
                set_volume_level(poc.volume_level); //解决问题：当处于通话界面时，限制最大音量，电话时增益过大会导致，语音卡顿（截断导致）
                delay_ms(1000);
            }

            delay_ms(100);
            play_tts(2, TTS_Incoming_call);
            delay_ms(100);
            module_at_tx(AT_TX_CALL_CLCC);

            poc.miss_call_state = 1;
        }
#endif
#ifdef POC_ZZD
        else if (memcmp(ptr, AT_RX_NORMAL_POWER_DOWN, strlen(AT_RX_NORMAL_POWER_DOWN)) == 0)
        {
            //39p adb 写频完会自动reboot

            //收到NORMAL POWER DOWN need POC init
            select_system_clock_48M_or_8M(CLOCK_48M);
            set_gpio_state(GPIO_PA_EN, off);
            delay_ms(5000);
            poc_init();
            ui_init();
            volume_init();
        }
        else if (memcmp(ptr, "+POC:ff00", strlen("+POC:ff00")) == 0)
        {
            if (poc.has_logged_flag == true)
            {
                poc.offline_test_flag = 2;
            }
            poc.has_logged_flag = false;
        }
#endif
        else
        {
#if (defined EC25_EUX) || (defined EC25_USA) //EC25使用GSN替代MEID

            if (poc.get_meid_flag == 1 && strlen((char *)ptr) > 10) //正在获取GSN且长度大于10
            {
                //864977041641493
                //860588047331296
                poc.get_meid_flag = 2;
                memset(poc.meid_number, 0, strlen((char *)poc.meid_number));
                memcpy(poc.meid_number, &ptr[strlen((char *)ptr) - 7], 7);
            }
#endif
        }
    }
}
#endif

#if 1
// "AT+POC=01030069703d 3131342e3134312e3133322e3535 3b69643d 4a51544553543134 3b7077643d 7931643277336c34 3b00\r\n"
//读取flash中的账号信息，并发送给poc
void poc_set_account(u8 *ip, u8 *name, u8 *pwd, u8 gps_flag, u8 inv_flag)
{
    u8 cmd[256] = {0};
    u8 len;

    if (strlen((char *)name) == 0 || strlen((char *)name) > 32)
        return;
    memcpy(cmd, "AT+POC=01000069703d", 19); //ip=
    len = 19;
    hex_to_str(&cmd[len], ip, strlen((char *)ip) % 32);
    len += (strlen((char *)ip) % 32) * 2;
    memcpy(&cmd[len], "3b69643d", 8); //;id=
    len += 8;
    hex_to_str(&cmd[len], name, strlen((char *)name) % 32);
    len += (strlen((char *)name) % 32) * 2;
    memcpy(&cmd[len], "3b7077643d", 10); //;pwd=
    len += 10;
    hex_to_str(&cmd[len], pwd, strlen((char *)pwd) % 32);
    len += (strlen((char *)pwd) % 32) * 2;
    memcpy(&cmd[len], "3b6770733d", 10); //;gps=
    len += 10;

    if (gps_flag == 0x01 || gps_flag == 0x31)
    {
        gps_flag = 0x31;
    }
    else
    {
        gps_flag = 0x30;
    }
    hex_to_str(&cmd[len], &gps_flag, 1);
    len += 2;

    memcpy(&cmd[len], "3b696e763d", 10); //;inv=
    len += 10;

    if (inv_flag == 0x01 || inv_flag == 0x31)
    {
        inv_flag = 0x31;
    }
    else
    {
        inv_flag = 0x30;
    }
    hex_to_str(&cmd[len], &inv_flag, 1);
    len += 2;

    memcpy(&cmd[len], "3b\r\n", 6);

    module_at_tx((char *)cmd);
}
#else
// "AT+POC=01030069703d 3131342e3134312e3133322e3535 3b69643d 4a51544553543134 3b7077643d 7931643277336c34 3b00\r\n"
//读取flash中的账号信息，并发送给poc
void poc_set_account(u8 *ip, u8 *name, u8 *pwd, u8 gps_flag, u8 inv_flag)
{
    u8 cmd[256] = {0};
    u8 len;

    if (strlen((char *)name) == 0 || strlen((char *)name) > 32)
        return;
    memcpy(cmd, "AT+POC=01000069703d", 19); //ip=
    len = 19;
    hex_to_str(&cmd[len], ip, strlen((char *)ip) % 32);
    len += (strlen((char *)ip) % 32) * 2;
    memcpy(&cmd[len], "3b69643d", 8); //;id=
    len += 8;
    hex_to_str(&cmd[len], name, strlen((char *)name) % 32);
    len += (strlen((char *)name) % 32) * 2;
    memcpy(&cmd[len], "3b7077643d", 10); //;pwd=
    len += 10;
    hex_to_str(&cmd[len], pwd, strlen((char *)pwd) % 32);
    len += (strlen((char *)pwd) % 32) * 2;
    memcpy(&cmd[len], "3b6770733d", 10); //;gps=
    len += 10;

    if (gps_flag == 0x01 || gps_flag == 0x31)
    {
        gps_flag = 0x31;
    }
    else
    {
        gps_flag = 0x30;
    }
    hex_to_str(&cmd[len], &gps_flag, 1);
    len += 2;

    memcpy(&cmd[len], "3b696e763d", 10); //;inv=
    len += 10;

    if (inv_flag == 0x01 || inv_flag == 0x31)
    {
        inv_flag = 0x31;
    }
    else
    {
        inv_flag = 0x30;
    }
    hex_to_str(&cmd[len], &inv_flag, 1);
    len += 2;

    memcpy(&cmd[len], "3b00\r\n", 6);

    module_at_tx((char *)cmd);
}
#endif
void poc_set_net_mode(u8 mode)
{
    module_at_tx("\r\n");
    delay_ms(30);
    for (u8 i = 0; i < 3; i++)
    {
        switch (mode)
        {
        case 0: //自动（先设置4G再设置为自动）
            module_at_tx(AT_TX_SET_4G_ONLY);
            delay_ms(500);
            module_at_tx(AT_TX_SET_4G_AUTO);
            break;
        case 1: //4G
            module_at_tx(AT_TX_SET_4G_ONLY);
            break;
        case 2: //3G
            if (poc.get_imsi_sim_card_type == 3)
            {
                //电信3G HDR ONLY
                module_at_tx(AT_TX_SET_3G_CT);
            }
            else if (poc.get_imsi_sim_card_type == 2)
            {
                //联通3G WCDMA
                module_at_tx(AT_TX_SET_3G_CU);
            }
            else if (poc.get_imsi_sim_card_type == 1)
            {
                //移动3G TD-SCDMA
                module_at_tx(AT_TX_SET_3G_CM);
            }
            else
            {
                /* code */
            }
            break;
        case 3: //2G
            if (poc.get_imsi_sim_card_type == 3)
            {
                //电信2G CDMA
                module_at_tx(AT_TX_SET_2G_CT);
            }
            else
            {
                //GSM
                module_at_tx(AT_TX_SET_2G_GSM);
            }
            break;
        case 4: // 2/3G AUTO
            if (poc.get_imsi_sim_card_type == 3)
            {
                //电信2/3G  混合模式
                module_at_tx(AT_TX_SET_3G_2G_AUTO_CT);
            }
            break;
        default:
            module_at_tx(AT_TX_SET_4G_ONLY);
            break;
        }
        delay_ms(100);
    }
}

//static u8 is_bcd(u8 y, u8 m, u8 d)
//{
//    if ((y & 0x0f) > 9 || ((y >> 4) & 0x0f) > 9)
//        return 0;
//    if ((m & 0x0f) > 9 || ((m >> 4) & 0x0f) > 9)
//        return 0;
//    if ((d & 0x0f) > 9 || ((d >> 4) & 0x0f) > 9)
//        return 0;
//    return 1;
//}

//static int get_weekday(int y, int m, int d)
//{
//    int iWeek;
//    if (m == 1 || m == 2)
//    {
//        m += 12;
//        y--;
//    }
//    iWeek = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
//    return iWeek;
//}

//static void RTC_Date_add(RTC_DateTypeDef *Date, u8 y, u8 m, u8 d, u8 w)
//{
//    if (d + 1 > days_in_month(m % 12, y))
//    {
//        Date->RTC_Date = 1;
//        if (m >= 11)
//        {
//            Date->RTC_Month = 1;
//            Date->RTC_Year++;
//        }
//        else
//        {
//            Date->RTC_Month = (((m + 1) / 10) << 4) + (m + 1) % 10;
//        }
//    }
//    else
//        Date->RTC_Date = (((d + 1) / 10) << 4) + (d + 1) % 10;
//    Date->RTC_WeekDay = (w + 1) % 7;
//}

//static u8 days_in_month(u8 month, u8 year)
//{
//    const static u8 dim[14] = {
//        31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28};
//    u8 ret = dim[month - 1];
//    if (ret == 0)
//        ret = is_leap_year(year) ? 29 : 28;
//    return ret;
//}
//static u8 is_leap_year(unsigned int year)
//{
//    return (!(year % 4) && (year % 100)) || !(year % 400);
//}

/*
TTS播报函数（中英文公用）
中文版本：tts_type失效，AT+QTTS=1
英文版本：默认tts_type=2，在播报群组用户名时tts_type=1
 */
void play_tts(u8 tts_type, char *tts_data)
{
    char sprintf_str[400];

    memset(sprintf_str, 0, sizeof(sprintf_str));
    if (poc.receive_voice == false)
    {
        module_at_tx("\r\n");
        delay_ms(30);
#ifdef LANGUAGE_ENGLISH

        if (tts_type == 2)
        {
            sprintf(sprintf_str, AT_TX_TTS_ENGLISH, (char *)tts_data);
        }
        else
        {
            sprintf(sprintf_str, AT_TX_TTS_CHINESE, (char *)tts_data);
        }

#else
        sprintf(sprintf_str, AT_TX_TTS_CHINESE, (char *)tts_data);
#endif
        module_at_tx(sprintf_str);
    }
    set_gpio_state(GPIO_PA_EN, on); //5
    SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
}

void play_tts_with_2Param(u8 tts_type, char *tts_data1, char *tts_data2)
{
    char sprintf_str[255];
    memset(sprintf_str, 0, sizeof(sprintf_str));
    if (poc.receive_voice == false)
    {
#ifdef LANGUAGE_ENGLISH

        if (tts_type == 2)
        {
            sprintf(sprintf_str, AT_TX_TTS_ENGLISH_WITH_2PARAM, (char *)tts_data1, (char *)tts_data2);
        }
        else
        {
            sprintf(sprintf_str, AT_TX_TTS_CHINESE_WITH_2PARAM, (char *)tts_data1, (char *)tts_data2);
        }

#else
        sprintf(sprintf_str, AT_TX_TTS_CHINESE_WITH_2PARAM, (char *)tts_data1, (char *)tts_data2);
#endif
        module_at_tx(sprintf_str);
    }
    set_gpio_state(GPIO_PA_EN, on); //6
    SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
}

//tts播报group和user
//电信 0x75354FE1转为"3735 3335 3446 4531"再转为"3537 3533 4634 3145"
void play_tts_group_and_user(tts_group_or_user_t type, u16 group_index)
{
    char temp_name_1[POC_NAME_LENTH_MAX * 2 + 1];
    char temp_name_2[POC_NAME_LENTH_MAX * 2 + 1];

    if (g_bug_fix_first_tts_play_flag == true)
    {
        //第一次只播报在线人数，不播报用户名
        g_bug_fix_first_tts_play_flag = false;
    }
    else
    {
        memset(temp_name_1, 0, sizeof(temp_name_1));
        memset(temp_name_2, 0, sizeof(temp_name_2));
        if (type == TTS_GROUP)
        {
            hex_to_str((u8 *)temp_name_1, poc.group_list_name[group_index], POC_NAME_LENTH_MAX);
        }
        else
        {
            hex_to_str((u8 *)temp_name_1, poc.user_list_name[group_index], POC_NAME_LENTH_MAX);
        }

        for (u8 i = 0; i < POC_NAME_LENTH_MAX * 2;)
        {
            if (temp_name_1[i + 1] == 0 && temp_name_1[i] <= 0x7f)
            {
                if (i + 2 > POC_NAME_LENTH_MAX * 2)
                    break;
                temp_name_2[i] = temp_name_1[i];
                temp_name_2[i + 1] = temp_name_1[i + 1];
                i += 2;
            }
            else
            {
                if (i + 4 > POC_NAME_LENTH_MAX * 2)
                    break;
                temp_name_2[i] = temp_name_1[i + 2];
                temp_name_2[i + 1] = temp_name_1[i + 3];
                temp_name_2[i + 2] = temp_name_1[i];
                temp_name_2[i + 3] = temp_name_1[i + 1];
                i += 4;
            }
        }
        // SEGGER_RTT_printf(0, "%s-%d:TTTEST1--%s\r\n", __FILE__, __LINE__, temp_name_2);
        play_tts(1, temp_name_2);
    }
}

static FlagStatus para_is_reas(const u8 *pval)
{
    u8 len, i;

    len = strlen((const char *)pval);

    if (len == 0)
    {
        return RESET;
    }

    for (i = 0; i < len; i++)
    {
        if (pval[i] == 0xff || pval[i] == ' ')
        {
            return RESET;
        }
    }

    return SET;
}

void poc_factory_reset(void)
{
    //恢复默认poc flash
    memset(poc.flash_data.server_ip, 0, sizeof(poc.flash_data.server_ip));
    memset(poc.flash_data.user_id, 0, sizeof(poc.flash_data.user_id));
    memset(poc.flash_data.user_pwd, 0, sizeof(poc.flash_data.user_pwd));

    memcpy(poc.flash_data.server_ip, USER_IP, sizeof(USER_IP));

    memcpy(poc.flash_data.user_id, poc.meid_number, 7); //A100005DB70E6A

    memcpy(poc.flash_data.user_pwd, USER_PASSWORD, strlen(USER_PASSWORD));

    poc.flash_data.gps_enable_flag = 0;
    poc.flash_data.invite_enable_toggle = 1;
    poc.flash_data.ptt_tone_toggle = 0; //0:on 1:off
    poc.flash_data.encode_8k_toggle = 0;
    poc.flash_data.sim_card_toggle = 0;
    poc.flash_data.key_lock_toggle = 0;
    poc.flash_data.backlight_flag = 2;
    poc.flash_data.network_flag = 1;
    poc_set_net_mode(poc.flash_data.network_flag);
    delay_ms(3000);
#ifdef LANGUAGE_ENGLISH
    poc.flash_data.time_zone_flag = 17; //华盛顿-5
#else
    poc.flash_data.time_zone_flag = 8;
#endif
    poc_set_time_zone(poc.flash_data.time_zone_flag);

    poc.volume_level = 5;

    save_flash_poc_data();

    delay_ms(200);
    module_at_tx(AT_SET_POC_OPEN_WITH_NO_TTS);
    delay_ms(1500);
    //AT设置默认账号
    poc_set_account(USER_IP,
                    poc.flash_data.user_id,
                    USER_PASSWORD,
                    poc.flash_data.gps_enable_flag,
                    poc.flash_data.invite_enable_toggle);
    delay_ms(5000);
    set_gpio_state(GPIO_MODULE_PWRKEY, off);
    delay_ms(200);
    module_at_tx(AT_TX_MODULE_POWER_OFF);
    delay_ms(100);
    module_at_tx(AT_TX_MODULE_POWER_OFF);
    delay_ms(100);
    module_at_tx(AT_TX_MODULE_POWER_OFF);
    SEGGER_RTT_printf(0, "%s %s-%dAT_TX_MODULE_POWER_OFF-delay_ms(22500)%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
    delay_ms(22500);
    set_gpio_state(GPIO_MODULE_PWRKEY, on);
    SEGGER_RTT_printf(0, "%s %s-%dGPIO_MODULE_PWRKEY-ON-delay_ms(2000)%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
    delay_ms(2000);

    //清除快捷键
    memset(poc.hotkey_info, 0, 10 * sizeof(one_hotkey_flash_t));
    save_flash_hotkey_data();

    //清除通话记录
    memset(&call_record, 0, sizeof(call_record_t));
    save_flash_call_record_data();

    //清除联系人
    memset(&call_contacts, 0, sizeof(contacts_t));
    save_flash_call_contacts_data();

    backlight_idle_clear();

    //清除串口数组里的数据，方式开机检测有残留数据
    main_init(1);
    // volume_init();
}

//返回UNICODE格式百分比电量字符串（0-100）
char *get_battary_level(void)
{
    static char percent_bat_level[20];
    int8_t tmp_battery_level = 0;

    tmp_battery_level = get_battery_level();

    memset(percent_bat_level, 0, sizeof(percent_bat_level));
    switch (tmp_battery_level)
    {
    case -1:
    case 0:
    case 6: //电压过低 关机
        memcpy(percent_bat_level, "0035", strlen("0035"));
        break;
    case 1:
        memcpy(percent_bat_level, "00320030", strlen("00320030"));
        break;
    case 2:
        memcpy(percent_bat_level, "00340030", strlen("00340030"));
        break;
    case 3:
        memcpy(percent_bat_level, "00360030", strlen("00360030"));
        break;
    case 4:
        memcpy(percent_bat_level, "00380030", strlen("00380030"));
        break;
    case 5:
        memcpy(percent_bat_level, "003100300030", strlen("003100300030"));
        break;
    default: //charge
        break;
    }

    return percent_bat_level;
}

//比如将[0]=0xD8 [1]=0x9A 变为 [0]=0x9A [1]=0xD8（对调UNICODE大小头）
//返回对调后的数组
void unicode_convert(u8 *input_buf, u8 *output_buf)
{
    char temp_name_1[POC_NAME_LENTH_MAX * 2 + 1];

    memset(temp_name_1, 0, sizeof(temp_name_1));

    hex_to_str((u8 *)temp_name_1, input_buf, POC_NAME_LENTH_MAX);

    for (u8 i = 0; i < POC_NAME_LENTH_MAX * 2;)
    {
        if (temp_name_1[i + 1] == 0 && temp_name_1[i] <= 0x7f)
        {
            if (i + 2 > POC_NAME_LENTH_MAX * 2)
                break;
            output_buf[i] = temp_name_1[i];
            output_buf[i + 1] = temp_name_1[i + 1];
            i += 2;
        }
        else
        {
            if (i + 4 > POC_NAME_LENTH_MAX * 2)
                break;
            output_buf[i] = temp_name_1[i + 2];
            output_buf[i + 1] = temp_name_1[i + 3];
            output_buf[i + 2] = temp_name_1[i];
            output_buf[i + 3] = temp_name_1[i + 1];
            i += 4;
        }
    }
}

void play_local_info_tts(void)
{
    char cInfo[400];
    char tmp_my_name[POC_NAME_LENTH_MAX * 2 + 1];
    char tmp_my_group_name[POC_NAME_LENTH_MAX * 2 + 1];

    memset(cInfo, 0, sizeof(cInfo));
    memset(tmp_my_name, 0, sizeof(tmp_my_name));
    memset(tmp_my_group_name, 0, sizeof(tmp_my_group_name));

    unicode_convert(poc.my_name, (u8 *)tmp_my_name);
    unicode_convert(poc.my_group_name, (u8 *)tmp_my_group_name);

#ifdef LANGUAGE_ENGLISH
    sprintf(cInfo, "%s%s%s%s%s%s%s%s%s",
            TTS_UNICODE_Battery, get_battary_level(), TTS_UNICODE_percent, "002c",
            TTS_UNICODE_Current_Group, tmp_my_group_name, "002c",
            TTS_UNICODE_Current_User, tmp_my_name);
#else
    sprintf(cInfo, "%s%s%s%s%s%s%s%s%s",
            TTS_UNICODE_Battery, TTS_UNICODE_percent, get_battary_level(), "002c",
            TTS_UNICODE_Current_Group, tmp_my_group_name, "002c",
            TTS_UNICODE_Current_User, tmp_my_name);
#endif

    if (ui.page < UI_PAGE_SPECIAL_START && poc.has_logged_flag == true) //特殊界面不进（侧键&快捷方式）
    {
        play_tts(1, cInfo);
    }
}

void play_unicode_tts(u8 *input_buf)
{
    char temp_name_1[POC_NAME_LENTH_MAX * 2 + 1];
    char temp_name_2[POC_NAME_LENTH_MAX * 2 + 1];

    memset(temp_name_1, 0, sizeof(temp_name_1));
    memset(temp_name_2, 0, sizeof(temp_name_2));

    hex_to_str((u8 *)temp_name_1, input_buf, POC_NAME_LENTH_MAX);

    for (u8 i = 0; i < POC_NAME_LENTH_MAX * 2;)
    {
        if (temp_name_1[i + 1] == 0 && temp_name_1[i] <= 0x7f)
        {
            if (i + 2 > POC_NAME_LENTH_MAX * 2)
                break;
            temp_name_2[i] = temp_name_1[i];
            temp_name_2[i + 1] = temp_name_1[i + 1];
            i += 2;
        }
        else
        {
            if (i + 4 > POC_NAME_LENTH_MAX * 2)
                break;
            temp_name_2[i] = temp_name_1[i + 2];
            temp_name_2[i + 1] = temp_name_1[i + 3];
            temp_name_2[i + 2] = temp_name_1[i];
            temp_name_2[i + 3] = temp_name_1[i + 1];
            i += 4;
        }
    }
    play_tts(1, temp_name_2);
}

void poc_set_time_zone(u8 num)
{
    /*
  	发送的指令：AT+POC=6000002b303800

  	 0	 1	 2	 3	 4	 5	 6	 7	 8	 9	 10	 11	 12	13 14 15 16 17 18 19 20 21  22	23	24
  	+00 +01 +02 +03 +04 +05 +06 +07 +08 +09 +10 +11 +12 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10 -11 -12
   */
    char tmp_buf[9];

    if (num <= 12) //东区-使用‘+’
    {
        //+ 2b 3262
        tmp_buf[0] = 0x32; //2
        tmp_buf[1] = 0x62; //b
        if (num <= 9)
        {
            tmp_buf[2] = 0x33; //3
            tmp_buf[3] = 0x30; //0
            tmp_buf[4] = 0x33; //3
            tmp_buf[5] = 0x30 + num;
        }
        else
        {
            tmp_buf[2] = 0x33; //3
            tmp_buf[3] = 0x31; //1
            tmp_buf[4] = 0x33; //3
            tmp_buf[5] = 0x30 + num % 10;
        }
    }
    else //西区-使用‘-’
    {
        //- 2d 3264
        tmp_buf[0] = 0x32; //2
        tmp_buf[1] = 0x64; //b
        if (num <= 21)
        {
            tmp_buf[2] = 0x33;              //3
            tmp_buf[3] = 0x30;              //0
            tmp_buf[4] = 0x33;              //3
            tmp_buf[5] = 0x30 + (num - 12); //第num项对应时区为num-11
        }
        else
        {
            tmp_buf[2] = 0x33; //3
            tmp_buf[3] = 0x31; //1
            tmp_buf[4] = 0x33; //3
            tmp_buf[5] = 0x30 + (num - 12) % 10;
        }
    }
    tmp_buf[6] = 0x30;
    tmp_buf[7] = 0x30;
    tmp_buf[8] = 0;

    char sprintf_str[255];
    memset(sprintf_str, 0, sizeof(sprintf_str));
    sprintf(sprintf_str, "%s%s\r\n", (char *)AT_TX_SET_TIME_ZONE, (char *)tmp_buf);
    module_at_tx("\r\n");
    delay_ms(30);
    module_at_tx(sprintf_str);
    poc.got_time_flag = 0;
}

//耳机控制（插拔耳机时的耳机控制）
void headphone_control_process(void)
{
    static onoff_t headphone_state_old = off;
    static u8 tmp_count = 0;
    static onoff_t tmp_state = off;

#if 1 //解决对讲时声音过大会导致喇叭关闭一下（声音过大会导致GPIO_HPH_DET电平跳动）
    if (tmp_state != get_gpio_state(GPIO_HPH_DET))
    {
        tmp_state = get_gpio_state(GPIO_HPH_DET);
        tmp_count = 0;
    }
    else
    {
        tmp_count++;
    }

    if (tmp_count > 10)
    {
        tmp_count = 0;
        poc.headphone_state = tmp_state;
    }
#endif

    if (headphone_state_old != poc.headphone_state)
    {
        headphone_state_old = poc.headphone_state;
        if (poc.headphone_state == on)
        {
            set_gpio_state(GPIO_PA_EN, off);
            SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        }
        else
        {
            if (poc.receive_voice == true || ui.page == UI_PAGE_ON_CALL)
            {
                set_gpio_state(GPIO_PA_EN, on); //1
                SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
            }
            else
            {
                set_gpio_state(GPIO_PA_EN, off);
                SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
            }
        }
    }
}

//PTT按键松开如果还在讲话就连续发送结束指令（有时候收不到+POC:0C0000）
void bug_fix_ptt_end_with_0c00(void)
{
    if (poc.bug_fix_send_at_0c_flag == true)
    {
        //第一次不发AT_TX_0C0000
        poc.bug_fix_send_at_0c_flag = false;
    }
    else
    {
        if (poc.im_talking_flag == true && get_gpio_state(GPIO_KEY_PTT) == off)
        {
            backlight_idle_clear();
            //module_at_tx("\r\n");
            delay_ms(30);
            module_at_tx(AT_TX_0C0000);
            SEGGER_RTT_printf(0, "%s %s-%d:AT_TX_0C0000%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        }
    }
}

//解决实际登录，但显示未登录的问题
static void bug_fix_not_logged(void)
{
    if (poc.has_logged_flag == false)
    {
        poc.has_logged_flag = true;
        if (ui.page == UI_PAGE_BOOT)
        {
            ui_desktop(UI_OPT_FIRST_REFRESH);
        }
    }
}

void poc_set_ptt_tone(void)
{
#ifdef POC_ZZD
    if (poc.flash_data.ptt_tone_toggle == 0)
    {
        module_at_tx(AT_TX_SET_ZZD_TONE_OPEN);
    }
    else
    {
        module_at_tx(AT_TX_SET_ZZD_TONE_CLOSE);
    }

    delay_ms(50);
#if (THIS_VERSION == KER_VERSION)
    //客户要求PTT音增加5倍
    module_at_tx(AT_TX_SET_ZZD_TONE_LEVEL_100);
#else
    module_at_tx(AT_TX_SET_ZZD_TONE_LEVEL_DEFAULT);
#endif
    delay_ms(50);
#else
    module_at_tx(AT_TX_SET_TONE);

    delay_ms(50);
#endif
}

//8k编码声音过大，设置后需要重设一下音量增益
void zzd_poc_set_8k_encode(void)
{
#ifdef POC_ZZD
    if (poc.flash_data.encode_8k_toggle == 0)
    {
        module_at_tx(AT_TX_SET_ZZD_4K_CODING);
        // delay_ms(50);
        // module_at_tx(AT_SET_ALL_VOL_GAIN_DEFAULT);
    }
    else
    {
        module_at_tx(AT_TX_SET_ZZD_8K_CODING);
        // delay_ms(50);
        // module_at_tx(AT_SET_ALL_VOL_GAIN_2);
    }
#endif
}
