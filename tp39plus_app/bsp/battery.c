#define BATTERY
#include "battery.h"
#include "tom_interface.h"
#include "lcd_api.h"
#include "new_ui_data.h"
#include "pa_led_volume.h"
#include "poc.h"
#include "language.h"

static int8_t s_battery_level = 0;

//battery level ico
void dispaly_battery_ico(int8_t level)
{
    switch (level)
    {
    case -1:
        play_tts(2, TTS_Low_power);
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_0, 20, 16, COLOR_RED, COLOR_ICO_BACKGROUND);
        break;
    case 0:
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_0, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 1:
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_1, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 2:
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_2, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 3:
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_3, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 4:
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_4, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 5:
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_5, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 6: //电压过低 关机
        play_tts(2, TTS_Low_power);
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_charge, 20, 16, COLOR_RED, COLOR_ICO_BACKGROUND);
        break;
    case 7: //正在充电
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_charge, 20, 16, COLOR_RED, COLOR_ICO_BACKGROUND);
        break;
    case 8: //电压过高
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_5, 20, 16, COLOR_RED, COLOR_ICO_BACKGROUND);
        break;
    default: //charge
        lcd_api_bmp_with_one_color(ICO_X_START_BATTARY, 0, gImage_battery_charge, 20, 16, COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    }
}

/*
新39电量显示标准

5格：3.90V-4.20V
4格：3.80V-3.90V
3格：3.70V-3.80V
2格：3.60V-3.70V
1格：3.50V-3.60V
0格：3.45V-3.50V（白色）
0格：3.40V-3.45V（红色，播低电）

3.40V以下关机

和林振奋确认过了
*/

//100ms update once
void battery_process(ui_ico_change_type type)
{
#define BATTERY_ADC_VALUE_LEVEL_SHUTDOWN 2320 //3.40V
#define BATTERY_ADC_VALUE_LEVEL_0 2353        //3.45V(33)
#define BATTERY_ADC_VALUE_LEVEL_1 2389        //3.50V(36)
#define BATTERY_ADC_VALUE_LEVEL_2 2456        //3.60V(67)
#define BATTERY_ADC_VALUE_LEVEL_3 2525        //3.70V(69)
#define BATTERY_ADC_VALUE_LEVEL_4 2590        //3.80V(70)
#define BATTERY_ADC_VALUE_LEVEL_5 2660        //3.90V
#define BATTERY_ADC_VALUE_LEVEL_ERROR 3070    //4.50v power high

#define BATTERY_ADC_D_VALUE 5

    static u8 battery_level_old = 0xff;
    static u16 reflash_flag = 0;
    static u16 shut_down_count = 0;
    u16 battery_adc_value = 0;

    battery_adc_value = all_adc.bat_adc_value;

    if (battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_SHUTDOWN)
    {
        //电压过低，关机
        s_battery_level = 6;
        led_state_set(LOW_BATTERY);

        shut_down_count++;

        if (battery_adc_value < 10) //旋钮转到0，电压可能监测到2 3点
        {
            shut_down_count = 0;
            ui_shutdown(0);
        }
        else
        {
            if (shut_down_count >= 500)
            {
                shut_down_count = 500;
                ui_shutdown(1);
            }
        }
    }
    else
    {
        shut_down_count = 0;

        if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_SHUTDOWN && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_0 - BATTERY_ADC_D_VALUE)
        {
            s_battery_level = -1;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_0 + BATTERY_ADC_D_VALUE && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_1 - BATTERY_ADC_D_VALUE)
        {
            s_battery_level = 0;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_1 + BATTERY_ADC_D_VALUE && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_2 - BATTERY_ADC_D_VALUE)
        {
            s_battery_level = 1;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_2 + BATTERY_ADC_D_VALUE && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_3 - BATTERY_ADC_D_VALUE)
        {
            s_battery_level = 2;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_3 + BATTERY_ADC_D_VALUE && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_4 - BATTERY_ADC_D_VALUE)
        {
            s_battery_level = 3;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_4 + BATTERY_ADC_D_VALUE && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_5 - BATTERY_ADC_D_VALUE)
        {
            s_battery_level = 4;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_5 + BATTERY_ADC_D_VALUE && battery_adc_value <= BATTERY_ADC_VALUE_LEVEL_ERROR)
        {
            s_battery_level = 5;
        }
        else if (battery_adc_value > BATTERY_ADC_VALUE_LEVEL_ERROR)
        {
            //电压过高
            s_battery_level = 8;
        }
        else
        {
            /* code */
        }
    }

    if (type == ICO_REFRESH_ON_CHANGE)
    {
        if (battery_level_old != s_battery_level)
        {
            reflash_flag = 0;
            battery_level_old = s_battery_level;
        }
        else
        {
            reflash_flag++;

            if (reflash_flag == 80) //30s
            {
                if (ui.page != UI_PAGE_BOOT_SCREEN)
                {
                    dispaly_battery_ico(s_battery_level);
                }
            }
            else if (reflash_flag > 80)
            {
                reflash_flag = 0;
            }
            else
            {
                /* code */
            }
        }
    }
    else if (type == ICO_REFRESH_ALL)
    {
        if (ui.page != UI_PAGE_BOOT_SCREEN)
        {
            dispaly_battery_ico(s_battery_level);
        }
    }
    else
    {
        /* code */
    }
}

int8_t get_battery_level(void)
{
    return s_battery_level;
}
