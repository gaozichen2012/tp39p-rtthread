#define PA_LED_VOLUME
#include "pa_led_volume.h"
#include "tom_interface.h"
#include "poc.h"
#include "ui.h"
#include "lcd_api.h"
#include "new_ui_data.h"

static u32 s_pa_on_timer = 0;
static u8 s_red_second = 0;   //红灯闪烁时间（秒）
static u8 s_green_second = 0; //绿灯闪烁时间（秒）
static u8 volume_level_old = 0xff;

void volume_init(void)
{
    volume_level_old = 0xff;
}

void pa_timer_process(void)
{
    static u32 tmp_pa_count = 0;

    if (s_pa_on_timer > 0)
    {
        if (tmp_pa_count == 0)
        {
            tmp_pa_count = SysTick_Cnt;
        }

        if (abs(SysTick_Cnt - tmp_pa_count) > s_pa_on_timer) //当PA定时已到且没人说话时关闭喇叭
        {
            s_pa_on_timer = 0;
            tmp_pa_count = 0;
            set_gpio_state(GPIO_PA_EN, off);
            SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        }
    }
}

void pa_on_timer(u32 timer)
{
    set_gpio_state(GPIO_PA_EN, on); //开启喇叭
    SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
    s_pa_on_timer = timer;
}

//设置指示灯状态
void led_state_set(LED_STATES led_state)
{
    switch (led_state)
    {
    case NOT_LOGGED: //未登录 绿灯1秒闪1次
        set_gpio_state(GPIO_RED_LED, off);
        set_gpio_state(GPIO_GREEN_LED, off);
        s_red_second = 0xFF;
        s_green_second = 1;

        break;
    case STANDBY: //已登录或模拟待机状态 绿灯8秒闪1次
        set_gpio_state(GPIO_RED_LED, off);
        set_gpio_state(GPIO_GREEN_LED, off);

        s_red_second = 0xFF;
        s_green_second = 3;
        break;
    case RX:
        set_gpio_state(GPIO_GREEN_LED, on);
        s_green_second = 0xFE;
        set_gpio_state(GPIO_RED_LED, off);
        s_red_second = 0xFF;
        break;
    case LOW_BATTERY: //低电 红灯1秒闪1次
        s_red_second = 1;
        break;
    case TX:
        set_gpio_state(GPIO_RED_LED, on);
        s_red_second = 0;
        set_gpio_state(GPIO_GREEN_LED, off);
        s_green_second = 0xFF;
        break;
    case RED_OFF:
        set_gpio_state(GPIO_RED_LED, off);
        s_red_second = 0xFF;
        break;
    case GREEN_OFF:
        set_gpio_state(GPIO_GREEN_LED, off);
        s_green_second = 0xFF;
        break;
    default:
        break;
    }
}

void led_bling_timer(void)
{
    static u32 s_green_led_count = 0;
    static u32 s_red_led_count = 0;
    static u8 s_green_second_old = 0;
    static u8 s_red_second_old = 0;

    if (s_green_second_old != s_green_second || s_red_second_old != s_red_second)
    {
        s_green_second_old = s_green_second;
        s_red_second_old = s_red_second;
        s_green_led_count = SysTick_Cnt;
        s_red_led_count = SysTick_Cnt;
    }

    /* 绿灯 */
    if (s_green_second == 0xFF)
    {
        set_gpio_state(GPIO_GREEN_LED, off);
    }
    else if (s_green_second == 0xFE)
    {
        set_gpio_state(GPIO_GREEN_LED, on);
    }
    else
    {
        if (SysTick_Cnt - s_green_led_count == s_green_second * 1000)
        {
            set_gpio_state(GPIO_GREEN_LED, on); //绿灯亮
        }
        else if (SysTick_Cnt - s_green_led_count >= s_green_second * 1000 + 50)
        {
            set_gpio_state(GPIO_GREEN_LED, off); //绿灯灭
            s_green_led_count = SysTick_Cnt;
        }
        else
        {
            //none
        }
    }

    /* 红灯 */
    if (s_red_second == 0xFF)
    {
        set_gpio_state(GPIO_RED_LED, off); //红灯灭
    }
    else
    {
        if (SysTick_Cnt - s_red_led_count == s_red_second * 1000)
        {
            set_gpio_state(GPIO_RED_LED, on); //红灯亮
        }
        else if (SysTick_Cnt - s_red_led_count >= s_red_second * 1000 + 20)
        {
            set_gpio_state(GPIO_RED_LED, off); //红灯灭
            s_red_led_count = SysTick_Cnt;
        }
        else
        {
            //none
        }
    }
}

#if 1 //解决音量设置区间不规范的问题
#define VOLUME_VALUE_SPASING 500
#define VOLUME_VALUE_LEVEL_1 98
#define VOLUME_VALUE_LEVEL_2 256
#define VOLUME_VALUE_LEVEL_3 425
#define VOLUME_VALUE_LEVEL_4 600
#define VOLUME_VALUE_LEVEL_5 1000
#define VOLUME_VALUE_LEVEL_6 1548
#define VOLUME_VALUE_LEVEL_7 2706
#define VOLUME_VALUE_LEVEL_8 3683
//volume adc 0-4038
void volume_timer_process(void)
{
    if (all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_1)
    {
        poc.volume_level = 0;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_1 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_2)
    {
        poc.volume_level = 1;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_2 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_3)
    {
        poc.volume_level = 2;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_3 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_4)
    {
        poc.volume_level = 3;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_4 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_5)
    {
        poc.volume_level = 4;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_5 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_6)
    {
        poc.volume_level = 5;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_6 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_7)
    {
        poc.volume_level = 6;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_7 && all_adc.vol_adc_value < VOLUME_VALUE_LEVEL_8)
    {
        poc.volume_level = 7;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_LEVEL_8)
    {
        poc.volume_level = 8;
    }
    else
    {
        /* code */
    }
}
#else
#define VOLUME_VALUE_SPASING 400
//volume adc 0-4038
void volume_timer_process(void)
{
    if (all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 1)
    {
        poc.volume_level = 0;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 1 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 2)
    {
        poc.volume_level = 1;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 2 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 3)
    {
        poc.volume_level = 2;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 3 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 4)
    {
        poc.volume_level = 3;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 4 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 5)
    {
        poc.volume_level = 4;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 5 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 6)
    {
        poc.volume_level = 5;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 6 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 7)
    {
        poc.volume_level = 6;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 7 && all_adc.vol_adc_value < VOLUME_VALUE_SPASING * 8)
    {
        poc.volume_level = 7;
    }
    else if (all_adc.vol_adc_value > VOLUME_VALUE_SPASING * 8)
    {
        poc.volume_level = 8;
    }
    else
    {
        /* code */
    }
}
#endif
void volume_level_control(void)
{

    if (ui.page != UI_PAGE_BOOT_SCREEN)
    {
        if (poc.module_init_flag == true)
        {
            if (volume_level_old != poc.volume_level)
            {
                volume_level_old = poc.volume_level;
                set_volume_level(poc.volume_level);
                dispaly_volume_ico(poc.volume_level);
            }
        }
    }
}

//按老39p的增益调
#if 1
void set_volume_level(u8 value)
{
#define VOL_LEVEL_MAX 9

#if THIS_VERSION == TYT_VERSION
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0x1f1f", "0x2222", "0x2525", "0xffff"}; //喇叭
#elif (THIS_VERSION == LENOVO_VERSION || THIS_VERSION == LENOVO_ZZD_VERSION)
    //联想版本最大增益为0x1313
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x1313", "0x1515", "0x1717", "0x1919", "0x1b1b", "0x1e1e", "0x2222", "0x2525", "0xffff"}; //喇叭
#elif (THIS_VERSION == EUX_VERSION)
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x0f0f", "0x1111", "0x1313", "0x1515", "0x1717", "0x1919", "0x1b1b", "0x1d1d", "0xffff"}; //喇叭
#else
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0x1f1f", "0x2222", "0x2525", "0xffff"}; //喇叭
#endif

    const char earphone_level[VOL_LEVEL_MAX][6] = {"0x0202", "0x0505", "0x0a0a", "0x0e0e", "0x1010", "0x1313", "0x1616", "0x1919", "0xffff"}; //耳机

    const char speaker_level_for_call[VOL_LEVEL_MAX][6] = {"0x1616", "0x1616", "0x1616", "0x1919", "0x1c1c", "0x1f1f", "0x2222", "0x2525", "0xffff"}; //最大0x1919不会导致卡顿
    const char earphone_for_call[VOL_LEVEL_MAX][6] = {"0x0e0e", "0x0e0e", "0x0e0e", "0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0xffff"};
    char cmd_earphone[32]; //耳机
    char cmd_speaker[32];  //PA

    memcpy(cmd_earphone, AT_SET_EARPHONE_VOL_GAIN, strlen(AT_SET_EARPHONE_VOL_GAIN));
    memcpy(cmd_speaker, AT_SET_SPRAKER_VOL_GAIN, strlen(AT_SET_SPRAKER_VOL_GAIN));

    if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL) //解决问题：当处于通话界面时，限制最大音量，电话时增益过大会导致，语音卡顿（截断导致）
    {
        memcpy(cmd_earphone + 22, earphone_for_call[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
        memcpy(cmd_speaker + 22, speaker_level_for_call[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
    }
    else
    {
        memcpy(cmd_earphone + 22, earphone_level[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
        memcpy(cmd_speaker + 22, speaker_level[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
    }

    module_at_tx("\r\n");
    delay_ms(30);
    module_at_tx(cmd_earphone);
    delay_ms(100);
    module_at_tx(cmd_speaker);
    delay_ms(100);
}
#else
void set_volume_level(u8 value)
{
#define VOL_LEVEL_MAX 9
#if 1
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x0a8a", "0x0c0c", "0x0e0e", "0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0xffff"};  //按林振奋要求修改增益只取4-5-6-7-8
#else
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x0a0a", "0x0c0c", "0x0e0e", "0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0xffff"}; //old
    const char speaker_level[VOL_LEVEL_MAX][6] = {"0x0808", "0x0a0a", "0x0c0c", "0x0e0e", "0x1010", "0x1212", "0x1414", "0x1616", "0xffff"}; //old-old
#endif
    const char earphone_level[VOL_LEVEL_MAX][6] = {"0x0000", "0x0202", "0x0606", "0x0f0f", "0x1616", "0x1919", "0x1a1a", "0x1d1d", "0xffff"}; //耳机

    const char speaker_level_for_call[VOL_LEVEL_MAX][6] = {"0x1010", "0x1010", "0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0x1f1f", "0xffff"}; //最大0x1919不会导致卡顿
    const char earphone_for_call[VOL_LEVEL_MAX][6] = {"0x0e0e", "0x0e0e", "0x0e0e", "0x1010", "0x1313", "0x1616", "0x1919", "0x1c1c", "0xffff"};
    char cmd_earphone[32]; //耳机
    char cmd_speaker[32];  //PA

    memcpy(cmd_earphone, AT_SET_EARPHONE_VOL_GAIN, strlen(AT_SET_EARPHONE_VOL_GAIN));
    memcpy(cmd_speaker, AT_SET_SPRAKER_VOL_GAIN, strlen(AT_SET_SPRAKER_VOL_GAIN));

    if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL) //解决问题：当处于通话界面时，限制最大音量，电话时增益过大会导致，语音卡顿（截断导致）
    {
        memcpy(cmd_earphone + 22, earphone_for_call[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
        memcpy(cmd_speaker + 22, speaker_level_for_call[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
    }
    else
    {
        memcpy(cmd_earphone + 22, earphone_level[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
        memcpy(cmd_speaker + 22, speaker_level[(VOL_LEVEL_MAX - value - 1) % VOL_LEVEL_MAX], 6);
    }

    module_at_tx("\r\n");
    delay_ms(30);
    module_at_tx(cmd_earphone);
    delay_ms(100);
    module_at_tx(cmd_speaker);
    delay_ms(100);
}
#endif

void dispaly_volume_ico(u8 level)
{
    switch (level)
    {
    case 0:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_0, 16, 16, COLOR_RED, COLOR_ICO_BACKGROUND);
        break;
    case 1:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_1, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 2:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_2, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 3:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_3, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 4:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_4, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 5:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_5, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 6:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_6, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 7:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_7, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 8:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_8, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 9:
        lcd_api_bmp_with_one_color(ICO_X_START_VOLUME_LEVEL, 0, gImage_volume_9, 16, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    default:
        break;
    }
}
