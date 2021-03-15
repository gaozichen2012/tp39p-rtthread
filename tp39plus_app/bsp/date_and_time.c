#define DATE_AND_TIME
#include "date_and_time.h"
#include "ui.h"
#include "lcd_api.h"
#include "language.h"
#include "poc.h"
#include <stdio.h>

static void process_yymmdd_cnt(void);

//时间定时（RTC由于使用LSI精度太差，一分钟差几秒）
void custom_date_time_timer(void)
{
    if (((datetime.hour != 0) || (datetime.minite != 0) || (datetime.second != 0))) ///00:00:00进不来
    {
        u64 Tcnt = (datetime.hour * 3600 + datetime.minite * 60 + datetime.second + 1) % 86400;
        datetime.hour = (u8)(Tcnt / 3600);
        datetime.minite = (u8)((Tcnt % 3600) / 60);
        datetime.second = (u8)(Tcnt % 60);
        if ((datetime.hour == 0) && (datetime.minite == 0) && (datetime.second == 0))
        {
            process_yymmdd_cnt();
        }
    }
    else
    {
        datetime.hour = 0;
        datetime.minite = 0;
        datetime.second = 1;
    }
}

static void process_yymmdd_cnt(void)
{
    u8 days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (((datetime.year % 4 == 0) && (datetime.year % 100 != 0)) || (datetime.year % 400 == 0))
    {
        days[1] = 29;
    }

    datetime.day++;
    if (datetime.day > days[datetime.month - 1])
    {
        datetime.day = 1;
        datetime.month++;
        if (datetime.month > 12)
        {
            datetime.month = 1;
            datetime.year++;
        }
    }
}

#ifdef CUSTOM_TIME
void display_time_icon(u8 refush_flag)
{
    static u8 cur_h[3] = {0}, bef_h[3] = {0xff, 0xfe, 0xff};
    static u8 cur_m[3] = {0}, bef_m[3] = {0xff, 0xfe, 0xff};
    static u8 cur_s[3] = {0}, bef_s[3] = {0};

    if (poc.got_time_flag == 1)
    {
        cur_s[0] = datetime.second / 10 + 0x30;
        cur_s[1] = datetime.second % 10 + 0x30;
        cur_m[0] = datetime.minite / 10 + 0x30;
        cur_m[1] = datetime.minite % 10 + 0x30;
        cur_h[0] = datetime.hour / 10 + 0x30;
        cur_h[1] = datetime.hour % 10 + 0x30;

        if (cur_h[0] != bef_h[0] || cur_h[1] != bef_h[1] || refush_flag)
        {
            memcpy(bef_h, cur_h, 2);

            lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_TIME, 3, (u8 *)cur_h, COLOR_BACKGROUND, COLOR_FONT);

            lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_TIME + 2 * 6, 3, (u8 *)":", COLOR_BACKGROUND, COLOR_FONT);
        }
        if (cur_m[0] != bef_m[0] || cur_m[1] != bef_m[1] || refush_flag)
        {
            memcpy(bef_m, cur_m, 2);
            lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_TIME + 2 * 6 + 6, 3, (u8 *)cur_m, COLOR_BACKGROUND, COLOR_FONT);
        }
    }
}
#else
void display_time_icon(u8 refush_flag)
{
    static u8 cur_h[3] = {0}, bef_h[3] = {0xff, 0xfe, 0xff};
    static u8 cur_m[3] = {0}, bef_m[3] = {0xff, 0xfe, 0xff};
    static u8 cur_s[3] = {0}, bef_s[3] = {0};

    if (poc.got_time_flag == 1)
    {
        HAL_RTC_GetTime(&hrtc, &read_time, RTC_FORMAT_BCD);
        HAL_RTC_GetDate(&hrtc, &read_data, RTC_FORMAT_BCD);

        cur_s[0] = (read_time.Seconds >> 4) + 0x30;
        cur_s[1] = (read_time.Seconds & 0x0f) + 0x30;
        cur_m[0] = (read_time.Minutes >> 4) + 0x30;
        cur_m[1] = (read_time.Minutes & 0x0f) + 0x30;
        cur_h[0] = (read_time.Hours >> 4) + 0x30;
        cur_h[1] = (read_time.Hours & 0x0f) + 0x30;

        if (cur_h[0] != bef_h[0] || cur_h[1] != bef_h[1] || refush_flag)
        {
            memcpy(bef_h, cur_h, 2);

            lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_TIME, 3, (u8 *)cur_h, COLOR_BACKGROUND, COLOR_FONT);

            lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_TIME + 2 * 6, 3, (u8 *)":", COLOR_BACKGROUND, COLOR_FONT);
        }
        if (cur_m[0] != bef_m[0] || cur_m[1] != bef_m[1] || refush_flag)
        {
            memcpy(bef_m, cur_m, 2);
            lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_TIME + 2 * 6 + 6, 3, (u8 *)cur_m, COLOR_BACKGROUND, COLOR_FONT);
        }
    }
}
#endif

#ifdef CUSTOM_TIME
void display_date_and_week(void)
{
    u8 display_data_buf[20];
    char *week[7] = {c_week_7,
                     c_week_1,
                     c_week_2,
                     c_week_3,
                     c_week_4,
                     c_week_5,
                     c_week_6};

    if (poc.got_time_flag == 1 && poc.lock_flag == false)
    {
        memset(display_data_buf, 0, sizeof(display_data_buf));
        sprintf((char *)display_data_buf, "%d-%d-%d", datetime.year, datetime.month, datetime.day);

#ifdef LANGUAGE_ENGLISH
        lcd_api_text_gbk_display(size_12, display_middle, 0, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)week[datetime.week], COLOR_BACKGROUND, COLOR_FONT);
        lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * strlen((char *)display_data_buf) - 5, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)display_data_buf, COLOR_BACKGROUND, COLOR_FONT);
#else
        lcd_api_text_gbk_display(size_12, display_middle, 0, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)display_data_buf, COLOR_BACKGROUND, COLOR_FONT);
        lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * strlen((char *)week[datetime.week]) - 5, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)week[datetime.week], COLOR_BACKGROUND, COLOR_FONT);
#endif
    }
}
#else
void display_date_and_week(void)
{
    u8 display_data_buf[20];
    u8 week_index = 0;
    char *week[7] = {c_week_1,
                     c_week_2,
                     c_week_3,
                     c_week_4,
                     c_week_5,
                     c_week_6,
                     c_week_7};

    if (poc.got_time_flag == 1 && poc.lock_flag == false)
    {
        HAL_RTC_GetTime(&hrtc, &read_time, RTC_FORMAT_BCD);
        HAL_RTC_GetDate(&hrtc, &read_data, RTC_FORMAT_BIN);

        week_index = (read_data.Date + 2 * read_data.Month + 3 * (read_data.Month + 1) / 5 + read_data.Year + read_data.Year / 4 - read_data.Year / 100 + read_data.Year / 400) % 7;
        memset(display_data_buf, 0, sizeof(display_data_buf));
        sprintf((char *)display_data_buf, "%d-%d-%d", read_data.Year, read_data.Month, read_data.Date);

        //lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * strlen(week[week_index]) - 6 * strlen((char *)display_data_buf) - 5 - 5, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)week[week_index], COLOR_BACKGROUND, COLOR_FONT);
        lcd_api_text_gbk_display(size_12, display_middle, 0, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)week[week_index], COLOR_BACKGROUND, COLOR_FONT);
        lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * strlen((char *)display_data_buf) - 5, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)display_data_buf, COLOR_BACKGROUND, COLOR_FONT);
    }
}
#endif
