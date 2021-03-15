/*
显示模块3级火箭：驱动drive/应用application/接口interface
1 lcd_drv 驱动芯片 ST7735&ILI9163 的代码，在此提供初始化芯片、读写数据的函数
2 lcd_app display 提供点、线、形及汉字的函数（保证此部分对所有机型通用）
3 lcd_api 列表函数，（此为对外的唯一接口，外部不可调用display及lcd_drv）
*/

#define LCD_API
#include "lcd_api.h"
#include "lcd_app.h"
#include "language.h"
#include "new_ui_data.h"
#include <string.h>
#include "poc.h"
#include "ui.h"
#include "telephone.h"

#define ITEM_DISP_NUM_MAX 4             //显示的最大项目数
#define ITEM_START_POINT_X 20 + 8 + 3   //项目起始x点
#define ITEM_START_POINT_Y 34           //项目起始y点
#define ITEM_TRUE_ICO_START_POINT_X 140 //对号的起始点
#define ITEM_SPACE_Y 20                 //每个项目格的宽度

#define GROUP_ITEM_DISP_NUM_MAX 3                //显示的最大项目数
#define GROUP_ITEM_START_POINT_X 20 + 20 + 8 + 3 //项目起始x点
#define GROUP_ITEM_SPACE_X 120                   //每个项目格的宽度
#define GROUP_ITEM_SPACE_Y 26                    //每个项目格的宽度

static void item_num_display(u16 num, u8 dis_num);
static void item_num_display_for_3max(u16 num, u8 dis_num);

void lcd_api_init(void)
{
    lcd_app_display_init();
#if 0 //ui-test
    ui_clear(0, 0, COLOR_YELLOW, 160, 128);
    ui_text_gbk_display(size_12, display_left, 0, 0, (u8 *)c_No_this_feature, COLOR_WIGHT, COLOR_BLACK);
    ui_text_gbk_display(size_12, display_middle, 0, 20, (u8 *)c_No_this_feature, COLOR_WIGHT, COLOR_BLACK);
    ui_text_gbk_display(size_16, display_left, 0, 40, (u8 *)c_No_this_feature, COLOR_WIGHT, COLOR_BLACK);
    ui_text_gbk_display(size_16, display_middle, 0, 60, (u8 *)c_No_this_feature, COLOR_WIGHT, COLOR_BLACK);
    ui_bmp_with_one_color(120, 80, gImage_record, 12, 12, COLOR_RED, COLOR_BLUE);
    ui_bmp_with_one_color(140, 80, gImage_record, 12, 12, COLOR_RED, COLOR_BLUE);
    ui_bmp(0, 0, (u16 *)gImage_group_20, 20, 20);
    ui_bmp_with_background(0, 20, (u16 *)gImage_group_20, 20, 20, COLOR_RED);
    ui_x_line(5, 5, 100, 5, COLOR_BLACK);
    ui_y_line(5, 5, 50, 5, COLOR_BLACK);
    ui_bmp(0, 0, (u16 *)start_logo, 160, 128);
#endif
}

/* 
通用选项显示
current_item:当前光标所在的选项
item_name:项目名（二维数组）
items_num_max:最大项目数
actual_item:实际选中的选项（勾选，无需勾选输入0xFFFF）
*/
void lcd_api_normal_item_disp(int current_item, char *item_name[], u8 items_num_max, u16 actual_item, DISPLAY_REFLASH_TYPE refresh_type)
{
    u8 display_first_item = 0;     //当前显示项目的第一个项目
    u8 last_page_item_num_sum = 0; //最后一页的总项目数
    u8 last_page_first_item = 0;   //最后一页的第一个项目

    display_first_item = (current_item / ITEM_DISP_NUM_MAX) * ITEM_DISP_NUM_MAX;    //显示的第一个选项
    last_page_first_item = (items_num_max / ITEM_DISP_NUM_MAX) * ITEM_DISP_NUM_MAX; //最后一页的第一个项目
    last_page_item_num_sum = items_num_max - last_page_first_item;                  //最后一页的总项目数

    //比如总项目7,0-3 第一页 4-6 第二页
    if (current_item >= last_page_first_item) //最后一页的项目数可能<ITEM_DISP_NUM_MAX，所以要特别处理
    {

        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + last_page_item_num_sum - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, X_MAX, ITEM_SPACE_Y * ITEM_DISP_NUM_MAX - 1);
            for (u8 i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                item_num_display(i + 1, last_page_item_num_sum);
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }

                lcd_api_text_gbk_display(size_12, display_left, ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * ITEM_SPACE_Y, (u8 *)item_name[i], COLOR_FONT, COLOR_BACKGROUND);

                //是否显示对号√
                if (actual_item != 0xFF) //0xFF则不显示对号
                {
                    if (actual_item == i)
                    {
                        lcd_api_bmp_with_one_color(ITEM_TRUE_ICO_START_POINT_X, ITEM_START_POINT_Y + (i - display_first_item) * ITEM_SPACE_Y, gImage_true, 16, 16, COLOR_RED, COLOR_BACKGROUND);
                    }
                }
            }
        }
        else
        {
            for (u8 i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
    else
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + ITEM_DISP_NUM_MAX - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, X_MAX, ITEM_SPACE_Y * ITEM_DISP_NUM_MAX - 1);
            for (u8 i = display_first_item; i < display_first_item + ITEM_DISP_NUM_MAX; i++)
            {
                item_num_display(i + 1, ITEM_DISP_NUM_MAX);
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
                lcd_api_text_gbk_display(size_12, display_left, ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * 20, (u8 *)item_name[i], COLOR_FONT, COLOR_BACKGROUND);

                //是否显示对号√
                if (actual_item != 0xFFFF) //0xFF则不显示对号
                {
                    if (actual_item == i)
                    {
                        lcd_api_bmp_with_one_color(ITEM_TRUE_ICO_START_POINT_X, ITEM_START_POINT_Y + (i - display_first_item) * ITEM_SPACE_Y, gImage_true, 16, 16, COLOR_RED, COLOR_BACKGROUND);
                    }
                }
            }
        }
        else
        {
            for (u8 i = display_first_item; i < display_first_item + ITEM_DISP_NUM_MAX; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20, ITEM_START_POINT_Y + ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
}

/*
通用选项显示（群组专用显示函数）
项目名在函数内部从flash中获取，无需输入
*/
void lcd_api_normal_item_disp_with_group_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type)
{
    u8 display_first_item = 0;     //当前显示项目的第一个项目
    u8 last_page_item_num_sum = 0; //最后一页的总项目数
    u8 last_page_first_item = 0;   //最后一页的第一个项目
    u8 i = 0;

    display_first_item = (current_item / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX;    //显示的第一个选项
    last_page_first_item = (items_num_max / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX; //最后一页的第一个项目
    last_page_item_num_sum = items_num_max - last_page_first_item;                              //最后一页的总项目数

    //比如总项目7,0-3 第一页 4-6 第二页
    if (current_item >= last_page_first_item) //最后一页的项目数可能<GROUP_ITEM_DISP_NUM_MAX，所以要特别处理
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + last_page_item_num_sum - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, last_page_item_num_sum);
                //显示群组图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_group_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_GROUP, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
                lcd_api_text_unicode_display(size_12, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)poc.group_list_name[i], POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_GROUP, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
    else
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + GROUP_ITEM_DISP_NUM_MAX - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, GROUP_ITEM_DISP_NUM_MAX);

                //显示群组图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_group_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_GROUP, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
                lcd_api_text_unicode_display(size_12, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)poc.group_list_name[i], POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_GROUP, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
}

/*
通用选项显示（成员-好友专用显示函数）
项目名在函数内部从flash中获取，无需输入
*/
void lcd_api_normal_item_disp_with_userfriend_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type)
{
    u8 display_first_item = 0;     //当前显示项目的第一个项目
    u8 last_page_item_num_sum = 0; //最后一页的总项目数
    u8 last_page_first_item = 0;   //最后一页的第一个项目

    u8 i = 0;
    display_first_item = (current_item / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX;    //显示的第一个选项
    last_page_first_item = (items_num_max / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX; //最后一页的第一个项目
    last_page_item_num_sum = items_num_max - last_page_first_item;                              //最后一页的总项目数

    //比如总项目7,0-3 第一页 4-6 第二页
    if (current_item >= last_page_first_item) //最后一页的项目数可能<GROUP_ITEM_DISP_NUM_MAX，所以要特别处理
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + last_page_item_num_sum - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);

            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, last_page_item_num_sum);

                //显示成员图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_member_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_USER, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
                lcd_api_text_unicode_display(size_12, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)poc.user_list_name[i], POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_USER, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
    else
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + GROUP_ITEM_DISP_NUM_MAX - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);

            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, GROUP_ITEM_DISP_NUM_MAX);

                //显示成员图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_member_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_USER, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
                lcd_api_text_unicode_display(size_12, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)poc.user_list_name[i], POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    play_tts_group_and_user(TTS_USER, i);
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
}

/*
通用选项显示（电话联系人显示函数）
项目名在函数内部从flash中获取，无需输入
*/
void lcd_api_normal_item_disp_with_contacts_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type)
{
    u8 display_first_item = 0;     //当前显示项目的第一个项目
    u8 last_page_item_num_sum = 0; //最后一页的总项目数
    u8 last_page_first_item = 0;   //最后一页的第一个项目
    u8 i = 0;

    display_first_item = (current_item / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX;    //显示的第一个选项
    last_page_first_item = (items_num_max / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX; //最后一页的第一个项目
    last_page_item_num_sum = items_num_max - last_page_first_item;                              //最后一页的总项目数

    //比如总项目7,0-3 第一页 4-6 第二页
    if (current_item >= last_page_first_item) //最后一页的项目数可能<GROUP_ITEM_DISP_NUM_MAX，所以要特别处理
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + last_page_item_num_sum - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, last_page_item_num_sum);
                //显示群组图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_group_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }

                if (call_record.current_contact[i].miss_call_flag == 1) //如果是未接来电则显示为红色
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_contacts.current_contact[i].name, COLOR_RED, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_contacts.current_contact[i].name, COLOR_FONT, COLOR_BACKGROUND);
                }
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
    else
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + GROUP_ITEM_DISP_NUM_MAX - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, GROUP_ITEM_DISP_NUM_MAX);

                //显示群组图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_group_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }

                if (call_record.current_contact[i].miss_call_flag == 1) //如果是未接来电则显示为红色
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_contacts.current_contact[i].name, COLOR_RED, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_contacts.current_contact[i].name, COLOR_FONT, COLOR_BACKGROUND);
                }
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
}

/*
通用选项显示（电话记录显示函数）
项目名在函数内部从flash中获取，无需输入
*/
void lcd_api_normal_item_disp_with_record_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type)
{
    u8 display_first_item = 0;     //当前显示项目的第一个项目
    u8 last_page_item_num_sum = 0; //最后一页的总项目数
    u8 last_page_first_item = 0;   //最后一页的第一个项目
    u8 i = 0;
    u8 display_item[50];

    last_page_first_item = (items_num_max / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX; //最后一页的第一个项目
    last_page_item_num_sum = items_num_max - last_page_first_item;                              //最后一页的总项目数

    display_first_item = (current_item / GROUP_ITEM_DISP_NUM_MAX) * GROUP_ITEM_DISP_NUM_MAX; //显示的第一个选项

    //比如总项目7,0-3 第一页 4-6 第二页
    if (current_item >= last_page_first_item) //最后一页的项目数可能<GROUP_ITEM_DISP_NUM_MAX，所以要特别处理
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + last_page_item_num_sum - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, last_page_item_num_sum);
                //显示群组图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_group_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }

                if (strlen((char *)call_record.current_contact[i].name) == 0)
                {
                    memset(display_item, 0, sizeof(display_item));
                    memcpy(display_item, call_record.current_contact[i].phone, 12); //由于显示时间所有，只去12位
                }
                else
                {
                    memset(display_item, 0, sizeof(display_item));
                    memcpy(display_item, call_record.current_contact[i].name, 12);
                }

                if (call_record.current_contact[i].miss_call_flag == 1) //如果是未接来电则显示为红色
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)display_item, COLOR_RED, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * 5 - 5, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_record.current_contact[i].time, COLOR_RED, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)display_item, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * 5 - 5, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_record.current_contact[i].time, COLOR_FONT, COLOR_BACKGROUND);
                }
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + last_page_item_num_sum; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
    else
    {
        if ((refresh_type == REFLASH_KEY_DOWN && current_item == display_first_item) ||
            (refresh_type == REFLASH_KEY_UP && current_item == display_first_item + GROUP_ITEM_DISP_NUM_MAX - 1) ||
            (refresh_type == REFLASH_ALL))
        {
            lcd_api_clear(0, ITEM_START_POINT_Y, COLOR_BACKGROUND, 160, GROUP_ITEM_SPACE_Y * GROUP_ITEM_DISP_NUM_MAX - 1);
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                //显示序号
                item_num_display_for_3max(i + 1, GROUP_ITEM_DISP_NUM_MAX);

                //显示群组图标
                lcd_api_bmp(20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item), gImage_group_20x20, 20, 20);

                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }

                if (strlen((char *)call_record.current_contact[i].name) == 0)
                {
                    memset(display_item, 0, sizeof(display_item));
                    memcpy(display_item, call_record.current_contact[i].phone, 12); //由于显示时间所有，只去12位
                }
                else
                {
                    memset(display_item, 0, sizeof(display_item));
                    memcpy(display_item, call_record.current_contact[i].name, 12);
                }

                if (call_record.current_contact[i].miss_call_flag == 1) //如果是未接来电则显示为红色
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)display_item, COLOR_RED, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * 5 - 5, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_record.current_contact[i].time, COLOR_RED, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_left, GROUP_ITEM_START_POINT_X, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)display_item, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * 5 - 5, 3 + ITEM_START_POINT_Y + (i - display_first_item) * GROUP_ITEM_SPACE_Y, (u8 *)call_record.current_contact[i].time, COLOR_FONT, COLOR_BACKGROUND);
                }
            }
        }
        else
        {
            for (i = display_first_item; i < display_first_item + GROUP_ITEM_DISP_NUM_MAX; i++)
            {
                if (i == current_item) //如果选中则显示翻转色
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else //未被选中则正常显示
                {
                    lcd_api_bmp_with_one_color(20 + 20, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * (i - display_first_item) + 2, gImage_triangle_8x16, 8, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }
        }
    }
}

//项目序号显示 dis_num显示的总项目数：一般为4，可能是1-3
static void item_num_display(u16 num, u8 dis_num)
{
    u8 num_1 = 0, num_2 = 0, num_3 = 0; //个十百位
    u8 display_num = 0;                 //计算数字位置

    num_1 = (num / 1) % 10;
    num_2 = (num / 10) % 10;
    num_3 = (num / 100) % 10;

    display_num = (num - 1) % ITEM_DISP_NUM_MAX;

    //lcd_api_display_range(0 + 2, ITEM_START_POINT_Y + ITEM_SPACE_Y * i + 1, 20 - 2 - 2, 20 - 2 - 2, COLOR_FONT);

    lcd_api_bmp_with_one_color(0, ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, gImage_backgrund, 20, 20, COLOR_FONT, COLOR_BACKGROUND);
    if (num <= 9)
    {
        lcd_api_bmp_with_one_color(6, 4 + ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, bits[num_1 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
    }
    else if (num <= 99)
    {
        lcd_api_bmp_with_one_color(3, 4 + ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, bits[num_2 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
        lcd_api_bmp_with_one_color(3 + 6, 4 + ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, bits[num_1 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
    }
    else if (num <= 999)
    {
        lcd_api_bmp_with_one_color(0, 4 + ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, bits[num_3 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
        lcd_api_bmp_with_one_color(0 + 6, 4 + ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, bits[num_2 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
        lcd_api_bmp_with_one_color(0 + 12, 4 + ITEM_START_POINT_Y + ITEM_SPACE_Y * display_num, bits[num_1 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
    }
    else
    {
    }
}

//项目序号显示 dis_num显示的总项目数：一般为4，可能是1-3
static void item_num_display_for_3max(u16 num, u8 dis_num)
{
    u8 num_1 = 0, num_2 = 0, num_3 = 0; //个十百位
    u8 display_num = 0;                 //计算数字位置

    num_1 = (num / 1) % 10;
    num_2 = (num / 10) % 10;
    num_3 = (num / 100) % 10;

    display_num = (num - 1) % GROUP_ITEM_DISP_NUM_MAX;

    lcd_api_bmp_with_one_color(0, ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, gImage_backgrund, 20, 20, COLOR_FONT, COLOR_BACKGROUND);
    if (num <= 9)
    {
        lcd_api_bmp_with_one_color(6, 4 + ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, bits[num_1 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
    }
    else if (num <= 99)
    {
        lcd_api_bmp_with_one_color(3, 4 + ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, bits[num_2 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
        lcd_api_bmp_with_one_color(3 + 6, 4 + ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, bits[num_1 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
    }
    else if (num <= 999)
    {
        lcd_api_bmp_with_one_color(0, 4 + ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, bits[num_3 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
        lcd_api_bmp_with_one_color(0 + 6, 4 + ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, bits[num_2 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
        lcd_api_bmp_with_one_color(0 + 12, 4 + ITEM_START_POINT_Y + GROUP_ITEM_SPACE_Y * display_num, bits[num_1 % 10], 6, 12, COLOR_WIGHT, COLOR_FONT);
    }
    else
    {
    }
}

/*
文本滚动显示-实时刷新
max_len:显示的最大字数，超过max_len即滚动 比如“123”即3位、“高子晨123”即6位
refresh_type：false首次传入显示数组 true实时刷新
text_buf：传入的数组
*/
void lcd_api_text_scroll_display(u8 x, u8 y, u8 max_len, bool refresh_type, char *input_text_buf)
{
    static char display_buf[100];
    static u32 time_count = 0;
    static char pointer = 0;
    static u8 pointer_max = 0;

    if (ui.page == UI_PAGE_INVITE)
    {
        if (refresh_type == false) //首次传入显示数组
        {
            //            memset(display_buf, 0, sizeof(display_buf));
            //            Ucs2HexData_R((u8 *)display_buf, input_text_buf, &pointer_max); //len is output
            pointer_max = cala_unicode_name_len((u8 *)input_text_buf);
            pointer = 0;
            time_count = SysTick_Cnt;
            if (pointer_max <= max_len * 2) //当字数小于8位显示实际的位数
            {
                lcd_api_text_unicode_display(size_12, 42 - 6 * pointer_max / 2, y, (u8 *)input_text_buf, pointer_max * 2, COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, x, y, (u8 *)input_text_buf, max_len * 2, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        else //实时刷新
        {
            if (pointer_max > max_len * 2) //当字数大于8位时实时刷新（数组>16）
            {
                if (SysTick_Cnt - time_count >= 1000) //一秒刷新一次
                {
                    time_count = SysTick_Cnt;
                    pointer += 2;
                    if (pointer > abs(pointer_max - max_len * 2))
                    {
                        pointer = 0;
                    }
                    //解决用户名："004版本测试4号"显示异常的问题
                    lcd_api_text_unicode_display(size_12, x, y, (u8 *)"            ", max_len * 2, COLOR_BACKGROUND, COLOR_BACKGROUND);
                    lcd_api_text_unicode_display(size_12, x, y, (u8 *)input_text_buf + pointer, max_len * 2, COLOR_FONT, COLOR_BACKGROUND);
                }
            }
        }
    }
}

/*
size_16最多显示19个英文或9个中文
 */
void lcd_api_text_gbk_display(SIZE_TYPE size, DISPLAY_TYPE display_type, u16 LocX, u16 LocY, u8 *Gb2312Data, u16 uiFontColor, u16 uiBackColor)
{
    u8 single_data_size = 0;
    u16 x = 0;
    u8 data_len = 0;

    data_len = strlen((char *)Gb2312Data);
    if (size == size_12)
    {
        single_data_size = 6;
        if (data_len > 26)
        {
            data_len = 26; //size_12最多显示26个英文或13个中文
        }
    }
    else if (size == size_16)
    {
        single_data_size = 8;
        if (data_len > 19)
        {
            data_len = 19; //size_16最多显示19个英文或9个中文
        }
    }
    else
    {
    }

    if (display_type == display_middle) //居中
    {
        x = (160 - LocX - single_data_size * data_len) / 2;
    }
    else //居右和自定义
    {
        x = LocX;
    }

    if (size == size_12)
    {
        lcd_app_display_chinese_11x12(x, LocY, (char *)Gb2312Data, uiBackColor, uiFontColor);
    }
    else if (size == size_16)
    {
        lcd_app_display_chinese_15x16(x, LocY, (char *)Gb2312Data, uiBackColor, uiFontColor);
    }
    else
    {
        /* code */
    }
}

//unicode转为gbk后在显示gbk
void lcd_api_text_unicode_display(SIZE_TYPE size, u16 LocX, u16 LocY, u8 *unicode_data, u8 data_len, u16 uiFontColor, u16 uiBackColor)
{
    u8 gbk_data[POC_NAME_LENTH_MAX + 2];

    memset(gbk_data, 0, sizeof(gbk_data));

    lcd_app_string_uincode_to_gb2312(unicode_data, data_len, gbk_data);

    lcd_api_text_gbk_display(size, display_left, LocX, LocY, gbk_data, uiFontColor, uiBackColor);
}

//清除某一行
void lcd_api_clear_a_line(SIZE_TYPE size, u8 y)
{
    if (size == size_12) //size_12最多显示26个英文或13个中文
    {
        lcd_api_text_gbk_display(size, display_middle, 0, y, (u8 *)"                          ", COLOR_BACKGROUND, COLOR_BACKGROUND); //44 24
    }
    else //size_16最多显示19个英文或9个中文
    {
        lcd_api_text_gbk_display(size, display_middle, 0, y, (u8 *)"                   ", COLOR_BACKGROUND, COLOR_BACKGROUND); //44 24
    }
}

void lcd_api_clear(u8 x, u8 y, u16 colorData, u8 width, u8 height)
{
    lcd_app_display_clear(x, y, colorData, width, height);
}
void lcd_api_bmp(u8 x, u8 y, const u8 *bmp, u8 width, u8 height)
{
    lcd_app_display_bmp(x, y, bmp, width, height);
}

void lcd_api_bmp_with_background(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 back_color)
{
    lcd_app_display_bmp_with_background(x, y, bmp, width, height, back_color);
}

void lcd_api_bmp_with_one_color(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 colorData, u16 back_color)
{
    lcd_app_display_bmp_with_one_color(x, y, bmp, width, height, colorData, back_color);
}

void lcd_api_x_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color)
{
    lcd_app_display_x_line(StartX, StartY, Length, Width, Color);
}

void lcd_api_y_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color)
{
    lcd_app_display_y_line(StartX, StartY, Length, Width, Color);
}

void lcd_api_tan(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 X2, u16 Y2, u16 LineWidth, u16 Color)
{
    lcd_app_display_tan(X0, Y0, X1, Y1, X2, Y2, LineWidth, Color);
}

void lcd_api_display_range(u16 x, u16 y, u16 width, u16 height, u16 line_color)
{
    lcd_app_display_x_line(x, y, width, 1, line_color);
    lcd_app_display_x_line(x, y + height, width, 1, line_color);
    lcd_app_display_y_line(x, y, width, 1, line_color);
    lcd_app_display_y_line(x + width, y, width + 1, 1, line_color);
}
