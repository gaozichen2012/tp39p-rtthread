
#define UI
#include "ui.h"
#include "lcd_api.h"
#include "language.h"
#include "new_ui_data.h"
#include "string.h"
#include "poc.h"
#include <stdio.h>
#include "tom_flash.h"
#include "version.h"
#include "pa_led_volume.h"
#include "gps.h"
#include "battery.h"
#include "timed.h"
#include "main_process.h"
#include "telephone.h"
#include "typewriting.h"
#include "date_and_time.h"
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
//extern RTC_TimeTypeDef read_time;
//extern RTC_DateTypeDef read_data;
//extern RTC_HandleTypeDef hrtc;

bool g_air_code_account_update_flag = false;
bool g_call_state = false;                 //0：不在通话状态 1：正在通话
bool g_desktop_or_phone_dial_page = false; //默认false：phone_dial      true:主界面进

static bool csq_basic_flag = false;

static void dispaly_csq_ico(u8 level);
static void dispaly_network_type(u8 type);
static void dispaly_red_vertical_line(u16 x, u16 y);
static void mcu_update_reay(void);
static void son_menu(item_opt_t opt, u8 index);

static u8 s_contacts_or_record_flag = 0; //0拨号界面 1联系人 2呼叫记录 3来电
static int s_phone_contact_current_item = 0;
static int s_call_log_current_item = 0;

void ui_init(void)
{
    csq_basic_flag = false;
    ui_boot_screen(UI_OPT_FIRST_REFRESH);
}

void ui_clear(void)
{
    lcd_api_clear(0, 16 + 16, COLOR_BACKGROUND, X_MAX, Y_MAX - 16 - 16 - 16 + 2);
    //lcd_api_bmp(0, 16, gImage_background, X_MAX, 112);
}

void ui_clear_all(void)
{
    lcd_api_clear(0, 0, COLOR_BACKGROUND, X_MAX, Y_MAX);
}

void ui_show_title(u8 *title_name)
{
    lcd_api_clear(0, 16, COLOR_BACKGROUND, X_MAX, 16);
    lcd_api_text_gbk_display(size_12, display_middle, 0, 16 + 2, title_name, COLOR_FONT, COLOR_WIGHT);
}

void ui_show_bottom_button(bottom_type buttom)
{
    lcd_api_clear(0, Y_MAX - Y_BOTTOM_SPACE, COLOR_ICO_BACKGROUND, X_MAX, Y_BOTTOM_SPACE);
    switch (buttom)
    {
    case BUTTOM_OK_BACK:
        lcd_api_text_gbk_display(size_12, display_left, X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Okey, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, X_MAX - 6 * strlen(c_Back) - X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Back, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        break;
    case BUTTOM_BACK:
        lcd_api_text_gbk_display(size_12, display_left, X_MAX - 6 * strlen(c_Back) - X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Back, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        break;
    case BUTTOM_MENU:
        lcd_api_text_gbk_display(size_12, display_left, X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Menu, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        display_date_and_week();
        break;
    case BUTTOM_SWITCH_BACK:
        lcd_api_text_gbk_display(size_12, display_left, X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Switch, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, X_MAX - 6 * strlen(c_Back) - X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Back, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        break;
    case BUTTOM_LOCKED:
        lcd_api_text_gbk_display(size_12, display_middle, 0, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Locked, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        break;
#if 0
    case BUTTOM_MUTE_BACK: //静音-返回
        lcd_api_text_gbk_display(size_12, display_left, X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Mute, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, X_MAX - 6 * strlen(c_Back) - X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Back, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        break;
    case BUTTOM_RED_MUTE_BACK: //静音（红）-返回
        lcd_api_text_gbk_display(size_12, display_left, X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Mute, COLOR_RED, COLOR_ICO_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, X_MAX - 6 * strlen(c_Back) - X_PADDING, Y_MAX - Y_BOTTOM_SPACE + 1, (u8 *)c_Back, COLOR_WIGHT, COLOR_ICO_BACKGROUND);
        break;
#endif
    default:
        break;
    }
}

void back_to_desktop(void)
{
    if (poc.has_logged_flag == false)
    {
        ui_logging(UI_OPT_FIRST_REFRESH);
    }
    else
    {
        ui_desktop(UI_OPT_FIRST_REFRESH);
    }
}

void ui_boot_screen(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        ui.page = UI_PAGE_BOOT_SCREEN;
        lcd_api_clear(0, 0, COLOR_LOG_BACKCOLOR, X_MAX, Y_MAX);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 12 + 12 + 12 * 5, (u8 *)"   ", COLOR_LOG_BACKCOLOR, COLOR_LOG_BACKCOLOR);

#ifndef POC_ZZD
        if (get_gpio_state(GPIO_KEY_PTT) == on)
        {
            //进写码模式，不显示开机界面
            ui_setting_air_code_writing(UI_OPT_FIRST_REFRESH);
        }
        else
#endif
        {
//显示开机图
#if (THIS_VERSION == TYT_VERSION || THIS_VERSION == KER_VERSION)

            lcd_api_clear(0, 0, COLOR_TYT_LOG, 160, 128);
            lcd_api_bmp(80 - 73, 64 - 33, gImage_HYSY4GDJJ_146x66, 146, 66);

#elif (THIS_VERSION == LENOVO_VERSION || THIS_VERSION == LENOVO_ZZD_VERSION)
            lcd_api_clear(0, 0, LENOVO_RED, 160, 128);
            lcd_api_bmp(80 - 51, 64 - 16, gImage_red2_lenovo_102x32, 102, 32);

#elif (THIS_VERSION == ENG_VERSION || THIS_VERSION == EUX_VERSION || THIS_VERSION == USA_VERSION || THIS_VERSION == ZZD_ENG_VERSION)
            lcd_api_bmp(20, 25, gImage_Tytpilot_96x26, 96, 26);
            lcd_api_bmp(62, 80, gImage_LTE_4G_88x28, 88, 28);
            lcd_api_clear(10, 60, COLOR_FONT, 140, 2);
#elif (THIS_VERSION == HWL_VERSION)
            lcd_api_bmp(0, 0, gImage_HWL_160x128, 160, 128);
#elif (THIS_VERSION == ZZD_VERSION)
            lcd_api_bmp(10, 25, gImage_smartptt_142x21, 142, 21);
            lcd_api_bmp(62, 80, gImage_LTE_4G_88x28, 88, 28);
            lcd_api_clear(10, 60, COLOR_FONT, 140, 2);
#endif
        }
        break;
    case UI_OPT_PART_REFRESH: //开启写码模式
        poc.usb_toggle = 1;
        set_gpio_state(GPIO_USB_EN, on); //打开adb

#if (THIS_VERSION == TYT_VERSION || THIS_VERSION == KER_VERSION)
        lcd_api_text_gbk_display(size_16, display_middle, 0, 128 - 8 - 16, (u8 *)c_Open_USB, COLOR_FONT, COLOR_LOG_BACKCOLOR);

#elif (THIS_VERSION == LENOVO_VERSION || THIS_VERSION == LENOVO_ZZD_VERSION)
        lcd_api_text_gbk_display(size_16, display_middle, 0, 128 - 8 - 16, (u8 *)c_Open_USB, COLOR_WIGHT, LENOVO_RED);
#elif (THIS_VERSION == HWL_VERSION)
        lcd_api_text_gbk_display(size_16, display_middle, 0, 128 - 8 - 16, (u8 *)c_Open_USB, COLOR_FONT, COLOR_BACKGROUND);

#else
        lcd_api_clear(10, 60, COLOR_LOG_BACKCOLOR, 140, 2);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 60 - 8, (u8 *)c_Open_USB, COLOR_FONT, COLOR_LOG_BACKCOLOR);
#endif
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        break;
    default:
        break;
    }
}

void ui_logging(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        if (ui.page == UI_PAGE_BOOT_SCREEN)
        {
            lcd_api_clear(0, 0, COLOR_ICO_BACKGROUND, X_MAX, Y_MAX);
        }

        ui.page = UI_PAGE_BOOT;
        battery_process(ICO_REFRESH_ALL);

        ui_clear();
        ui_show_title((u8 *)c_Offline);
        ui_show_bottom_button(BUTTOM_MENU);

        keyboard_number_clear();
    case UI_OPT_PART_REFRESH:
        lcd_api_clear_a_line(size_12, 64);
        if (poc.module_init_flag == false)
        {
            lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Checking_module, COLOR_FONT, COLOR_BACKGROUND);
        }
        else
        {
            if (poc.sim_card_flag == FIRST_CHECK_SIM_CARD)
            {
                lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Checking_SIM_card, COLOR_FONT, COLOR_BACKGROUND);
            }
            else if (poc.sim_card_flag == FIRST_NO_SIM_CARD)
            {
                lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Checking_SIM_card, COLOR_FONT, COLOR_BACKGROUND);
            }
            else if (poc.sim_card_flag == SECOND_CHECK_SIM_CARD)
            {
                lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Checking_SIM_card, COLOR_FONT, COLOR_BACKGROUND);
            }
            else if (poc.sim_card_flag == SECOND_NO_SIM_CARD)
            {
                lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_No_SIM_card, COLOR_FONT, COLOR_BACKGROUND);
            }
            else if (poc.sim_card_flag == HAVE_SIM_CARD_1 || poc.sim_card_flag == HAVE_SIM_CARD_2)
            {
                if (poc.csq_level == 0)
                {
                    if (poc.has_logged_flag == false)
                    {
                        lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Network_searching, COLOR_FONT, COLOR_BACKGROUND);
                    }
                    else
                    {
                        lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Has_logged, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
                else
                {
                    if (poc.has_logged_flag == false)
                    {
                        lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Logging_in, COLOR_FONT, COLOR_BACKGROUND);
                    }
                    else
                    {
                        lcd_api_text_gbk_display(size_12, display_middle, 0, 64, (u8 *)c_Has_logged, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
            }
            else
            {
                /* code */
            }
        }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        ui_menu(UI_OPT_FIRST_REFRESH);
        break;
    default:
        break;
    }
}

void ui_desktop(ui_change_type opt)
{
    static bool receive_voice_old = false;
    static bool im_talking_flag_old = false;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        if (ui.page != UI_PAGE_AIR_CODE_WRITING)
        {
            ui.page = UI_PAGE_DESKTOP;
            ui_clear();
            lcd_api_clear(0, 16, COLOR_BACKGROUND, X_MAX, 16);

            if (poc.lock_flag == true)
            {
                ui_show_bottom_button(BUTTOM_LOCKED);
            }
            else
            {
                ui_show_bottom_button(BUTTOM_MENU);
            }

            keyboard_number_clear();

            if (poc.receive_voice == true)
            {
                lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_FONT, COLOR_BACKGROUND);
                lcd_api_text_unicode_display(size_12, 16 + 16 + X_PADDING, 25, poc.talk_user_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_FONT, COLOR_BACKGROUND);
                lcd_api_text_gbk_display(size_12, display_left, 16 + 16 + X_PADDING, 25, (u8 *)c_Idle, COLOR_FONT, COLOR_BACKGROUND);
            }

            lcd_api_bmp(10, 45, gImage_member_28x28, 28, 28);
            lcd_api_bmp(10, 82, gImage_group_28x28, 28, 28);
            lcd_api_text_unicode_display(size_12, 10 + 28 + 5, 45 + 8, poc.my_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_unicode_display(size_12, 10 + 28 + 5, 82 + 8, poc.my_group_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    case UI_OPT_PART_REFRESH: //实时刷新
        if (ui.page == UI_PAGE_DESKTOP)
        {
            if (receive_voice_old != poc.receive_voice)
            {
                receive_voice_old = poc.receive_voice;

                if (poc.receive_voice == true)
                {
                    lcd_api_clear(0, 25, COLOR_BACKGROUND, X_MAX, 16);
                    lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_unicode_display(size_12, 16 + 16 + X_PADDING, 25, poc.talk_user_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
                }
                else
                {
                    if (poc.im_talking_flag == false)
                    {
                        lcd_api_clear(0, 25, COLOR_BACKGROUND, X_MAX, 16);
                        lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_FONT, COLOR_BACKGROUND);
                        lcd_api_text_gbk_display(size_12, display_left, 16 + 16 + X_PADDING, 25, (u8 *)c_Idle, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
            }

            if (im_talking_flag_old != poc.im_talking_flag)
            {
                im_talking_flag_old = poc.im_talking_flag;

                if (poc.im_talking_flag == true)
                {
                    lcd_api_clear(0, 25, COLOR_BACKGROUND, X_MAX, 16);
                    lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_RED, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 16 + 16 + X_PADDING, 25, (u8 *)c_I_am_talking, COLOR_RED, COLOR_BACKGROUND);
                }
                else
                {
                    if (poc.receive_voice == false)
                    {
                        lcd_api_clear(0, 25, COLOR_BACKGROUND, X_MAX, 16);
                        lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_FONT, COLOR_BACKGROUND);
                        lcd_api_text_gbk_display(size_12, display_left, 16 + 16 + X_PADDING, 25, (u8 *)c_Idle, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
            }

            display_talking_count_down(poc.im_talking_flag);
        }
        break;
    case UI_OPT_OTHER_REFRESH: //星键解锁上锁
        if (poc.lock_flag == true)
        {
            poc.lock_flag = false;
            ui_show_bottom_button(BUTTOM_MENU);
            play_tts(2, TTS_UNLOCKED);
        }
        else
        {
            poc.lock_flag = true;
            ui_show_bottom_button(BUTTOM_LOCKED);
            play_tts(2, TTS_LOCKED);
            set_gpio_state(GPIO_LCD_BL, off);
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        if (poc.lock_flag == false && poc.im_talking_flag == false)
        {
            ui_menu(UI_OPT_FIRST_REFRESH);
        }
        break;
    default:
        break;
    }
}

#ifdef LENOVO
void ui_menu(ui_change_type opt)
{
#define MENU_ITEM_NUM_MAX 6
    static int menu_current_item = 0;

    const uint16_t menu_frame[MENU_ITEM_NUM_MAX][2] = {{0 + 1, 16 + 1}, {53 + 1, 16 + 1}, {106 + 1, 16 + 1}, {0 + 1, 64 + 1}, {53 + 1, 64 + 1}, {106 + 1, 64 + 1}};
    const unsigned char *menu_bmp[MENU_ITEM_NUM_MAX] = {gImage_group_53x48,
                                                        gImage_member_53x48,
                                                        gImage_friend_53x48,
                                                        gImage_phone_53x48,
                                                        gImage_gps_53x48,
                                                        gImage_settings_53x48};
    const unsigned char *menu_dark_bmp[MENU_ITEM_NUM_MAX] = {gImage_group_dark_53x48,
                                                             gImage_member_dark_53x48,
                                                             gImage_friend_dark_53x48,
                                                             gImage_phone_dark_53x48,
                                                             gImage_gps_dark_53x48,
                                                             gImage_settings_dark_53x48};

    char *menu_tts[MENU_ITEM_NUM_MAX] = {TTS_Group_selection, TTS_Member_selection, TTS_Friend_selection, TTS_phone, TTS_gps, TTS_setting};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        menu_current_item = 0;
        lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);                   //解决振奋说的刷屏慢问题
        lcd_api_text_gbk_display(size_12, display_left, 16 + 16 + X_PADDING, 25, (u8 *)c_Idle, COLOR_BACKGROUND, COLOR_BACKGROUND); //解决振奋说的刷屏慢问题
    case UI_OPT_PART_REFRESH:
        ui.page = UI_PAGE_MENU;
        ui_clear();
        ui_show_bottom_button(BUTTOM_OK_BACK);

        lcd_api_bmp(menu_frame[0][0], menu_frame[0][1], gImage_group_53x48, 53, 48);
        lcd_api_bmp(menu_frame[1][0], menu_frame[1][1], gImage_member_53x48, 53, 48);
        lcd_api_bmp(menu_frame[2][0], menu_frame[2][1], gImage_friend_53x48, 53, 48);
        lcd_api_bmp(menu_frame[3][0], menu_frame[3][1], gImage_phone_53x48, 53, 48);
        lcd_api_bmp(menu_frame[4][0], menu_frame[4][1], gImage_gps_53x48, 53, 48);
        lcd_api_bmp(menu_frame[5][0], menu_frame[5][1], gImage_settings_53x48, 53, 48);
        break;
    case UI_OPT_KEY_UP: //上切换项目
        menu_current_item--;
        if (menu_current_item < 0)
        {
            menu_current_item = MENU_ITEM_NUM_MAX - 1;
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        menu_current_item++;
        if (menu_current_item > MENU_ITEM_NUM_MAX - 1)
        {
            menu_current_item = 0;
        }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        switch (menu_current_item)
        {
        case 0:
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                ui_group(UI_OPT_FIRST_REFRESH);
            }
            break;
        case 1:
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                ui_user(UI_OPT_FIRST_REFRESH);
            }

            break;
        case 2:
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                ui_friend(UI_OPT_FIRST_REFRESH);
            }

            break;
        case 3:
            ui_phone(UI_OPT_FIRST_REFRESH);
            break;
        case 4:
            ui_gps(UI_OPT_FIRST_REFRESH);
            break;
        case 5:
            ui_setting(UI_OPT_FIRST_REFRESH);
            break;
        default:
            break;
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        back_to_desktop();
        break;
    default:
        break;
    }

    if (opt != UI_OPT_KEY_ENTER && opt != UI_OPT_KEY_BACK)
    {
        if (menu_current_item == 0)
        {
            lcd_api_bmp(menu_frame[5][0], menu_frame[5][1], menu_bmp[5], 53, 48);
            lcd_api_bmp(menu_frame[1][0], menu_frame[1][1], menu_bmp[1], 53, 48);
        }
        else
        {
            lcd_api_bmp(menu_frame[menu_current_item - 1][0], menu_frame[menu_current_item - 1][1], menu_bmp[menu_current_item - 1], 53, 48);
            lcd_api_bmp(menu_frame[(menu_current_item + 1) % MENU_ITEM_NUM_MAX][0], menu_frame[(menu_current_item + 1) % MENU_ITEM_NUM_MAX][1], menu_bmp[(menu_current_item + 1) % MENU_ITEM_NUM_MAX], 53, 48);
        }

        lcd_api_bmp(menu_frame[menu_current_item][0], menu_frame[menu_current_item][1], menu_dark_bmp[menu_current_item], 53, 48);

        play_tts(2, menu_tts[menu_current_item % MENU_ITEM_NUM_MAX]);
    }
}

#else
void ui_menu(ui_change_type opt)
{
#define MENU_ITEM_NUM_MAX 6
    static int menu_current_item = 0;

    const uint16_t menu_frame[MENU_ITEM_NUM_MAX][2] = {{8, 40}, {65, 40}, {117, 40}, {8, 77}, {65, 77}, {117, 77}};
    char *menu_title[MENU_ITEM_NUM_MAX] = {c_GROUP, c_MEMBER, c_FRIEND, c_PHONE, c_GPS, c_Setting};

    char *menu_tts[MENU_ITEM_NUM_MAX] = {TTS_Group_selection, TTS_Member_selection, TTS_Friend_selection, TTS_phone, TTS_gps, TTS_setting};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        menu_current_item = 0;
        lcd_api_clear_a_line(size_12, 25);
        // lcd_api_bmp_with_one_color(16, 25 - 2, gImage_speaker_16x16, 16, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);                   //解决振奋说的刷屏慢问题
        // lcd_api_text_gbk_display(size_12, display_left, 16 + 16 + X_PADDING, 25, (u8 *)c_Idle, COLOR_BACKGROUND, COLOR_BACKGROUND); //解决振奋说的刷屏慢问题
    case UI_OPT_PART_REFRESH:
        ui.page = UI_PAGE_MENU;
        ui_clear();
        ui_show_bottom_button(BUTTOM_OK_BACK);

        lcd_api_bmp(menu_frame[0][0] + 2, menu_frame[0][1] + 2, gImage_group, 28, 28);
        lcd_api_bmp(menu_frame[1][0] + 2, menu_frame[1][1] + 2, gImage_member, 28, 28);
        lcd_api_bmp(menu_frame[2][0] + 2, menu_frame[2][1] + 2, gImage_friend, 28, 28);
        lcd_api_bmp(menu_frame[3][0] + 2, menu_frame[3][1] + 2, gImage_phone, 28, 28);
        lcd_api_bmp(menu_frame[4][0] + 2, menu_frame[4][1] + 2, gImage_gps, 28, 28);
        lcd_api_bmp(menu_frame[5][0] + 2, menu_frame[5][1] + 2, gImage_settings, 28, 28);

        break;
    case UI_OPT_KEY_UP: //上切换项目
        menu_current_item--;
        if (menu_current_item < 0)
        {
            menu_current_item = MENU_ITEM_NUM_MAX - 1;
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        menu_current_item++;
        if (menu_current_item > MENU_ITEM_NUM_MAX - 1)
        {
            menu_current_item = 0;
        }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        switch (menu_current_item)
        {
        case 0:
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                ui_group(UI_OPT_FIRST_REFRESH);
            }
            break;
        case 1:
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                ui_user(UI_OPT_FIRST_REFRESH);
            }
            break;
        case 2:
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                ui_friend(UI_OPT_FIRST_REFRESH);
            }
            break;
        case 3:
            ui_phone(UI_OPT_FIRST_REFRESH);
            break;
        case 4:
            ui_gps(UI_OPT_FIRST_REFRESH);
            break;
        case 5:
            ui_setting(UI_OPT_FIRST_REFRESH);
            break;
        default:
            break;
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        back_to_desktop();
        break;
    default:
        break;
    }

    if (opt != UI_OPT_KEY_ENTER && opt != UI_OPT_KEY_BACK)
    {
        //display menu title
        ui_show_title((u8 *)menu_title[menu_current_item % MENU_ITEM_NUM_MAX]);

        if (menu_current_item == 0)
        {

            lcd_api_display_range(menu_frame[5][0], menu_frame[5][1], 32, 32, COLOR_BACKGROUND);
            lcd_api_display_range(menu_frame[1][0], menu_frame[1][1], 32, 32, COLOR_BACKGROUND);
        }
        else
        {
            lcd_api_display_range(menu_frame[menu_current_item - 1][0], menu_frame[menu_current_item - 1][1], 32, 32, COLOR_BACKGROUND);
            lcd_api_display_range(menu_frame[(menu_current_item + 1) % MENU_ITEM_NUM_MAX][0], menu_frame[(menu_current_item + 1) % MENU_ITEM_NUM_MAX][1], 32, 32, COLOR_BACKGROUND);
        }
        lcd_api_display_range(menu_frame[menu_current_item][0], menu_frame[menu_current_item][1], 32, 32, COLOR_FONT);

        play_tts(2, menu_tts[menu_current_item % MENU_ITEM_NUM_MAX]);
    }
}
#endif

void ui_group(ui_change_type opt)
{
    static u16 group_item_num_max = 0;
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //首次刷新，显示正在查询
        if (poc.lock_flag == false)
        {
            if (ui.page < UI_PAGE_SPECIAL_START) //特殊界面不进（侧键&快捷方式）
            {
                ui.page = UI_PAGE_GROUP;
                ui_clear();
                ui_show_title((u8 *)c_GROUP);
                ui_show_bottom_button(BUTTOM_BACK);
                lcd_api_text_gbk_display(size_12, display_middle, 0, 60, (u8 *)c_Querying1, COLOR_FONT, COLOR_BACKGROUND);
                lcd_api_text_gbk_display(size_12, display_middle, 0, 80, (u8 *)c_Please_wait, COLOR_FONT, COLOR_BACKGROUND);
                module_at_tx("\r\n");
                delay_ms(30);
                play_tts(2, TTS_Group_selection);
                delay_ms(1200);

                group_item_num_max = 0;
                poc.group_index = 0;
                memset(poc.group_list_id, 0, sizeof(poc.group_list_id));
                memset(poc.group_list_name, 0, sizeof(poc.group_list_name));

                module_at_tx(AT_TX_GROUP_REFRESH);
                poc.getting_group_flag = true;
            }
        }
        break;
    case UI_OPT_PART_REFRESH: //搜到列表后，显示列表
        poc.group_current_item = 0;
        ui_clear();
        ui_show_title((u8 *)c_GROUP);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        group_item_num_max = poc.total_number_of_groups; //0d获取到群组数

        if (group_item_num_max != 0)
        {
            lcd_api_normal_item_disp_with_group_list(poc.group_current_item, group_item_num_max, REFLASH_ALL);
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        poc.group_current_item--;
        if (poc.group_current_item < 0)
        {
            poc.group_current_item = group_item_num_max - 1;
        }

        if (group_item_num_max != 0)
        {
            lcd_api_normal_item_disp_with_group_list(poc.group_current_item, group_item_num_max, REFLASH_KEY_UP);
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        poc.group_current_item++;
        if (poc.group_current_item > group_item_num_max - 1)
        {
            poc.group_current_item = 0;
        }

        if (group_item_num_max != 0)
        {
            lcd_api_normal_item_disp_with_group_list(poc.group_current_item, group_item_num_max, REFLASH_KEY_DOWN);
        }

        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        if (poc.group_current_item != 0xFFFF)
        {
            char sprintf_str[100];
            memset(sprintf_str, 0, sizeof(sprintf_str));
            sprintf(sprintf_str, AT_TX_ENTERY_GROUP, (char *)poc.group_list_id[poc.group_current_item]);
            module_at_tx("\r\n");
            delay_ms(30);
            module_at_tx(sprintf_str);

#ifdef POC_ZZD
            //换组后跟新下我的组索引，卓智达获取群组成员，需要组索引，不是组ID
            char tmp_buf[4];
            memset(tmp_buf, 0, sizeof(tmp_buf));
            memset(poc.my_group_index, 0, sizeof(poc.my_group_index));
            //卓智达获取成员的序号是16进制的（比如index=10，则是000a而不是0010）
            //int2str_dec(tmp_buf, poc.group_current_item + 1);
            int2str_hex(tmp_buf, poc.group_current_item + 1);
            memset(poc.my_group_index, '0', 4 - strlen(tmp_buf));
            memcpy(poc.my_group_index + 4 - strlen(tmp_buf), tmp_buf, strlen(tmp_buf));
#endif
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_user(ui_change_type opt)
{
    static u16 user_item_num_max = 0;
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        if (poc.lock_flag == false)
        {
            if (ui.page < UI_PAGE_SPECIAL_START) //特殊界面不进（侧键&快捷方式）
            {
                ui.page = UI_PAGE_USER;
                ui_clear();
                ui_show_title((u8 *)c_MEMBER);
                ui_show_bottom_button(BUTTOM_BACK);
                poc.user_current_item = 0xFFFF;
                if (poc.flash_data.invite_enable_toggle == 0) //单呼功能关闭
                {
                    play_tts(2, TTS_Single_call_function_is_not_enabled);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 60, (u8 *)c_Single_call_is_disable, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 80, (u8 *)c_Please_return, COLOR_FONT, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 60, (u8 *)c_Querying2, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 80, (u8 *)c_Please_wait, COLOR_FONT, COLOR_BACKGROUND);
                    poc.total_number_of_users = 0;
                    memset(poc.user_list_index, 0, sizeof(poc.user_list_index));
                    memset(poc.user_list_id, 0, sizeof(poc.user_list_id));
                    memset(poc.user_list_name, 0, sizeof(poc.user_list_name));

                    char sprintf_str[100];
                    memset(sprintf_str, 0, sizeof(sprintf_str));
#ifdef POC_ZZD
                    sprintf(sprintf_str, AT_TX_GET_ONLINE_USERS2, "0000", (char *)poc.my_group_index);
#else
                    sprintf(sprintf_str, AT_TX_GET_ONLINE_USERS, (char *)poc.my_group_id);
#endif
                    module_at_tx("\r\n");
                    delay_ms(30);
                    play_tts(2, TTS_Member_selection);
                    delay_ms(1200);
                    module_at_tx(sprintf_str);
                    poc.user_index = 0;
                    poc.getting_users_flag = true;
                }
            }
        }
        break;
    case UI_OPT_PART_REFRESH:                         //搜到列表后，显示列表
        if (poc.flash_data.invite_enable_toggle != 0) //单呼功能开启
        {
            poc.user_current_item = 0;
            ui.page = UI_PAGE_USER;
            ui_clear();
            ui_show_title((u8 *)c_MEMBER);
            ui_show_bottom_button(BUTTOM_OK_BACK);
            user_item_num_max = poc.total_number_of_users;

            if (poc.total_number_of_users != 0)
            {
                lcd_api_normal_item_disp_with_userfriend_list(poc.user_current_item, user_item_num_max, REFLASH_ALL);
            }
        }
        break;
    case UI_OPT_OTHER_REFRESH: //无成员在线
        poc.user_current_item = 0xFFFF;
        if (poc.flash_data.invite_enable_toggle != 0) //单呼功能开启
        {
            ui.page = UI_PAGE_USER;
            ui_clear();
            ui_show_title((u8 *)c_MEMBER);
            ui_show_bottom_button(BUTTOM_BACK);
            lcd_api_text_gbk_display(size_12, display_middle, 0, 70, (u8 *)c_No_one_online, COLOR_FONT, COLOR_BACKGROUND);
            play_tts(2, TTS_No_one_online);
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        if (poc.user_current_item != 0xFFFF)
        {
            poc.user_current_item--;
            if (poc.user_current_item < 0)
            {
                poc.user_current_item = user_item_num_max - 1;
            }

            if (poc.flash_data.invite_enable_toggle != 0 && poc.total_number_of_users != 0)
            {
                lcd_api_normal_item_disp_with_userfriend_list(poc.user_current_item, user_item_num_max, REFLASH_KEY_UP);
            }
        }

        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        if (poc.user_current_item != 0xFFFF)
        {
            poc.user_current_item++;
            if (poc.user_current_item > user_item_num_max - 1)
            {
                poc.user_current_item = 0;
            }

            if (poc.flash_data.invite_enable_toggle != 0 && poc.total_number_of_users != 0)
            {
                lcd_api_normal_item_disp_with_userfriend_list(poc.user_current_item, user_item_num_max, REFLASH_KEY_DOWN);
            }
        }

        break;
    case UI_OPT_KEY_ENTER:                                                               //确认保存数据
        if (poc.flash_data.invite_enable_toggle != 0 && poc.user_current_item != 0xFFFF) //单呼功能开启
        {
            //send at
            char sprintf_str_1[100];
            memset(sprintf_str_1, 0, sizeof(sprintf_str_1));
            sprintf(sprintf_str_1, AT_TX_CALL_USER, (char *)poc.user_list_id[poc.user_current_item]);
            module_at_tx(sprintf_str_1);

            //cpy invite user name
            memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name)); //5
            memcpy(poc.talk_user_name, poc.user_list_name[poc.user_current_item], sizeof(poc.talk_user_name));
            ui_invite(UI_OPT_FIRST_REFRESH);
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_friend(ui_change_type opt)
{
    static u16 firend_item_num_max = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        if (poc.lock_flag == false)
        {
            if (ui.page < UI_PAGE_SPECIAL_START) //特殊界面不进（侧键&快捷方式）
            {
                ui.page = UI_PAGE_FRIEND;
                ui_clear();
                ui_show_title((u8 *)c_FRIEND);
                ui_show_bottom_button(BUTTOM_BACK);
                poc.user_current_item = 0xFFFF;
                if (poc.flash_data.invite_enable_toggle == 0) //单呼功能关闭
                {
                    play_tts(2, TTS_Single_call_function_is_not_enabled);

                    lcd_api_text_gbk_display(size_12, display_middle, 0, 60, (u8 *)c_Single_call_is_disable, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 80, (u8 *)c_Please_return, COLOR_FONT, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 60, (u8 *)c_Querying2, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 80, (u8 *)c_Please_wait, COLOR_FONT, COLOR_BACKGROUND);
                    poc.total_number_of_users = 0;

                    char sprintf_str[100];
                    memset(sprintf_str, 0, sizeof(sprintf_str));
                    sprintf(sprintf_str, AT_TX_GET_ONLINE_USERS, "00000000");
                    module_at_tx("\r\n");
                    delay_ms(30);
                    play_tts(2, TTS_Friend_selection);
                    delay_ms(1200);
                    module_at_tx(sprintf_str);
                    poc.user_index = 0;
                    poc.getting_users_flag = true;
                }
            }
        }
        break;
    case UI_OPT_PART_REFRESH:                         //搜到列表后，显示列表
        if (poc.flash_data.invite_enable_toggle != 0) //单呼功能开启
        {
            poc.user_current_item = 0;
            ui.page = UI_PAGE_FRIEND;
            ui_clear();
            ui_show_title((u8 *)c_FRIEND);
            ui_show_bottom_button(BUTTOM_OK_BACK);
            firend_item_num_max = poc.total_number_of_users;

            if (poc.total_number_of_users != 0)
            {
                lcd_api_normal_item_disp_with_userfriend_list(poc.user_current_item, firend_item_num_max, REFLASH_ALL);
            }
        }
        break;
    case UI_OPT_OTHER_REFRESH: //无成员在线
        poc.user_current_item = 0xFFFF;
        if (poc.flash_data.invite_enable_toggle != 0) //单呼功能开启
        {
            ui.page = UI_PAGE_FRIEND;
            ui_clear();
            ui_show_title((u8 *)c_FRIEND);
            ui_show_bottom_button(BUTTOM_BACK);
            lcd_api_text_gbk_display(size_12, display_middle, 0, 70, (u8 *)c_No_one_online, COLOR_FONT, COLOR_BACKGROUND);
            play_tts(2, TTS_No_one_online);
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        if (poc.user_current_item != 0xFFFF)
        {
            poc.user_current_item--;
            if (poc.user_current_item < 0)
            {
                poc.user_current_item = firend_item_num_max - 1;
            }

            if (poc.flash_data.invite_enable_toggle != 0 && poc.total_number_of_users != 0)
            {
                lcd_api_normal_item_disp_with_userfriend_list(poc.user_current_item, firend_item_num_max, REFLASH_KEY_UP);
            }
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        if (poc.user_current_item != 0xFFFF)
        {
            poc.user_current_item++;
            if (poc.user_current_item > firend_item_num_max - 1)
            {
                poc.user_current_item = 0;
            }

            if (poc.flash_data.invite_enable_toggle != 0 && poc.total_number_of_users != 0)
            {
                lcd_api_normal_item_disp_with_userfriend_list(poc.user_current_item, firend_item_num_max, REFLASH_KEY_DOWN);
            }
        }

        break;
    case UI_OPT_KEY_ENTER:                                                               //确认保存数据
        if (poc.flash_data.invite_enable_toggle != 0 && poc.user_current_item != 0xFFFF) //单呼功能开启
        {
            //send at
            char sprintf_str_1[100];
            memset(sprintf_str_1, 0, sizeof(sprintf_str_1));
            sprintf(sprintf_str_1, AT_TX_CALL_USER, (char *)poc.user_list_id[poc.user_current_item]);
            module_at_tx("\r\n");
            delay_ms(30);
            module_at_tx(sprintf_str_1);

            //cpy invite user name
            memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name)); //5
            memcpy(poc.talk_user_name, poc.user_list_name[poc.user_current_item], sizeof(poc.talk_user_name));

            ui_invite(UI_OPT_FIRST_REFRESH);
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_gps(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        ui.page = UI_PAGE_GPS;
        ui_clear();
        ui_show_title((u8 *)c_GPS);
        ui_show_bottom_button(BUTTOM_BACK);

        lcd_api_text_gbk_display(size_12, display_left, 5, 40, (u8 *)c_latitude, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 60, (u8 *)c_longitude, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 80, (u8 *)c_satellite, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)c_gps_switch, COLOR_FONT, COLOR_BACKGROUND);

    case UI_OPT_PART_REFRESH: //部分刷新
        if (ui.page == UI_PAGE_GPS)
        {
            if (poc.flash_data.gps_enable_flag == 1)
            {
                if (gps_device.position_state == 0 || gps_device.position_state == '0')
                {
                    lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_latitude), 40, (u8 *)"              ", COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_longitude), 60, (u8 *)"              ", COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_satellite), 80, (u8 *)"              ", COLOR_FONT, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_latitude), 40, (u8 *)gps_device.NS_value, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_longitude), 60, (u8 *)gps_device.EW_value, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_satellite), 80, (u8 *)gps_device.satellites_sum, COLOR_FONT, COLOR_BACKGROUND);
                }
                lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_GPS_switch), 100, (u8 *)c_Open, COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_satellite), 80, (u8 *)"              ", COLOR_FONT, COLOR_BACKGROUND);
                lcd_api_text_gbk_display(size_12, display_left, 10 + 6 * strlen(c_gps_switch), 100, (u8 *)c_Close, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_phone(ui_change_type opt)
{
#define PHONE_ITEM_NUM_MAX 4
    static int phone_current_item = 0;
    char *item[PHONE_ITEM_NUM_MAX] = {c_Contact, c_New_contact, c_Phone_dial, c_Call_log};
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        phone_current_item = 0;
    case UI_OPT_PART_REFRESH:
        ui.page = UI_PAGE_PHONE;
        ui_clear();
        ui_show_title((u8 *)c_PHONE);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_normal_item_disp(phone_current_item, item, PHONE_ITEM_NUM_MAX, 0xFFFF, REFLASH_ALL);
        break;
    case UI_OPT_KEY_UP: //上切换项目
        phone_current_item--;
        if (phone_current_item < 0)
        {
            phone_current_item = PHONE_ITEM_NUM_MAX - 1;
        }
        lcd_api_normal_item_disp(phone_current_item, item, PHONE_ITEM_NUM_MAX, 0xFFFF, REFLASH_KEY_UP);
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        phone_current_item++;
        if (phone_current_item > PHONE_ITEM_NUM_MAX - 1)
        {
            phone_current_item = 0;
        }
        lcd_api_normal_item_disp(phone_current_item, item, PHONE_ITEM_NUM_MAX, 0xFFFF, REFLASH_KEY_DOWN);
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        switch (phone_current_item)
        {
        case 0:
            ui_phone_contact(UI_OPT_FIRST_REFRESH);
            break;
        case 1:
            ui_phone_new_contact(UI_OPT_FIRST_REFRESH);
            break;
        case 2:
            ui_phone_dial(UI_OPT_FIRST_REFRESH);
            break;
        case 3:
            ui_phone_call_log(UI_OPT_FIRST_REFRESH);
            break;
        default:
            break;
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_phone_contact(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        s_phone_contact_current_item = 0;
    case UI_OPT_PART_REFRESH:
        s_contacts_or_record_flag = 1;
        ui.page = UI_PAGE_PHONE_CONTACT;
        ui_clear();
        ui_show_title((u8 *)c_Contact);
        ui_show_bottom_button(BUTTOM_OK_BACK);

        read_flash_call_contacts_data();

        if (call_contacts.count != 0 && call_contacts.count != 0xFF)
        {
            lcd_api_normal_item_disp_with_contacts_list(s_phone_contact_current_item, call_contacts.count, REFLASH_ALL);
        }
        else
        {
            //无呼叫记录
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, (u8 *)c_no_records, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        s_phone_contact_current_item--;
        if (s_phone_contact_current_item < 0)
        {
            s_phone_contact_current_item = call_contacts.count - 1;
        }

        if (call_contacts.count != 0 && call_contacts.count != 0xFF)
        {
            lcd_api_normal_item_disp_with_contacts_list(s_phone_contact_current_item, call_contacts.count, REFLASH_KEY_UP);
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        s_phone_contact_current_item++;
        if (s_phone_contact_current_item > call_contacts.count - 1)
        {
            s_phone_contact_current_item = 0;
        }

        if (call_contacts.count != 0 && call_contacts.count != 0xFF)
        {
            lcd_api_normal_item_disp_with_contacts_list(s_phone_contact_current_item, call_contacts.count, REFLASH_KEY_DOWN);
        }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        if (call_contacts.count != 0 && call_contacts.count != 0xFF)
        {
            ui_son_menu(UI_OPT_FIRST_REFRESH, s_phone_contact_current_item % 3);
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_phone(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_phone_dial(ui_change_type opt)
{
    static u8 num_buf_len = 0;
    static char current_data_old = 0;
    char current_data = 0;

#define PHONE_ITEM_NUM_MAX 4
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        s_contacts_or_record_flag = 0;
        ui.page = UI_PAGE_PHONE_DIAL;
        g_desktop_or_phone_dial_page = false;
        ui_clear();
        ui_show_title((u8 *)c_Phone_dial);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 94, (u8 *)c_Enter_number_and_press_OK, COLOR_RED, COLOR_BACKGROUND);

        //显示一个初始数据或者0
        memset(poc.dial.phone, 0, sizeof(poc.dial.phone));
        num_buf_len = strlen((char *)poc.dial.phone);
        lcd_api_clear_a_line(size_16, 70);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, poc.dial.phone, COLOR_FONT, COLOR_BACKGROUND);
        break;
    case UI_OPT_OTHER_REFRESH: //主界面按某键进入电话界面(实时刷新)
        if (ui.page == UI_PAGE_DESKTOP)
        {
            if (poc.lock_flag == true)
            {
                if (get_keyboard_number() != 0)
                {
                    keyboard_number_clear();
                }
            }
            else
            {
                if (get_keyboard_number() != 0)
                {
                    current_data = get_keyboard_number();
                    keyboard_number_clear();
                }

                if (current_data == 0)
                {
                    current_data_old = 0;
                }
                else
                {
                    if (current_data_old != current_data)
                    {
                        current_data_old = current_data;

                        s_contacts_or_record_flag = 0;
                        ui.page = UI_PAGE_PHONE_DIAL;
                        g_desktop_or_phone_dial_page = true;

                        ui_clear();
                        ui_show_title((u8 *)c_Phone_dial);
                        ui_show_bottom_button(BUTTOM_OK_BACK);
                        lcd_api_text_gbk_display(size_12, display_middle, 0, 94, (u8 *)c_Enter_number_and_press_OK, COLOR_RED, COLOR_BACKGROUND);

                        //显示一个初始数据或者0
                        memset(poc.dial.phone, 0, sizeof(poc.dial.phone));
                        num_buf_len = 0;
                        poc.dial.phone[num_buf_len] = current_data;
                        num_buf_len++;
                        lcd_api_clear_a_line(size_16, 70);
                        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, poc.dial.phone, COLOR_FONT, COLOR_BACKGROUND);

                        char sprintf_str[100];
                        memset(sprintf_str, 0, sizeof(sprintf_str));

#ifdef POC_ZZD
                        sprintf(sprintf_str, AT_TX_GET_ONLINE_USERS2, "0000", (char *)poc.my_group_index);
#else
                        sprintf(sprintf_str, AT_TX_GET_ONLINE_USERS, (char *)poc.my_group_id);
#endif
                        module_at_tx("\r\n");
                        delay_ms(30);
                        module_at_tx(sprintf_str);
                        poc.user_index = 0;
                        poc.getting_users_flag = true;
                    }
                }
            }
        }
        break;
    case UI_OPT_KEY_KEYBOARD: //键盘输入，实时刷新
        if (ui.page == UI_PAGE_PHONE_DIAL)
        {
            if (num_buf_len < 15) //密码最大8位
            {
                if (get_keyboard_number() != 0)
                {
                    current_data = get_keyboard_number();
                    keyboard_number_clear();
                }

                if (current_data == 0)
                {
                    current_data_old = 0;
                }
                else
                {
                    if (current_data_old != current_data)
                    {
                        current_data_old = current_data;
                        poc.dial.phone[num_buf_len] = current_data;
                        num_buf_len++;
                        lcd_api_clear_a_line(size_16, 70);
                        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, poc.dial.phone, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
                dispaly_red_vertical_line(80 + 8 * num_buf_len / 2, 70 - 1);
            }
        }
        break;
    case UI_OPT_KEY_ENTER:
#ifdef LANGUAGE_ENGLISH
        if (num_buf_len == 3 &&
            memcmp(poc.dial.phone, "911", 3) != 0 &&
            memcmp(poc.dial.phone, "999", 3) != 0 &&
            memcmp(poc.dial.phone, "112", 3) != 0)
#else
        if (num_buf_len == 3 &&
            memcmp(poc.dial.phone, "110", 3) != 0 &&
            memcmp(poc.dial.phone, "120", 3) != 0 &&
            memcmp(poc.dial.phone, "119", 3) != 0)
#endif
        {
            //快捷呼叫
            if (poc.flash_data.invite_enable_toggle == 0)
            {
                play_tts(2, TTS_Single_call_function_is_not_enabled);
            }
            else
            {
                for (u8 i = 0; i < poc.total_number_of_users; i++)
                {
                    u8 buf1[10];

                    buf1[0] = poc.user_list_name[i][0];
                    buf1[1] = poc.user_list_name[i][2];
                    buf1[2] = poc.user_list_name[i][4];
                    buf1[3] = 0;

                    if (memcmp(poc.dial.phone, buf1, 3) == 0)
                    {
                        char sprintf_str_1[100];
                        memset(sprintf_str_1, 0, sizeof(sprintf_str_1));
                        sprintf(sprintf_str_1, AT_TX_CALL_USER, (char *)poc.user_list_id[i]);
                        module_at_tx("\r\n");
                        delay_ms(30);
                        module_at_tx(sprintf_str_1);

                        //cpy invite user name
                        memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name)); //4
                        memcpy(poc.talk_user_name, poc.user_list_name[i], sizeof(poc.talk_user_name));
                        ui_invite(UI_OPT_FIRST_REFRESH);
                        return;
                    }
                }
                play_tts(2, TT_No_related_numbers_found_please_re_edit);
            }
        }
        else
        {
            //电话
            if (poc.flash_data.network_flag == 1)
            {
                play_tts(2, TTS_No_phone_function);
                memset(poc.dial.phone, 0, sizeof(poc.dial.phone));
                num_buf_len = 0;
                lcd_api_clear_a_line(size_16, 70);
            }
            else
            {
                if (num_buf_len != 0)
                {
                    ui_on_call(UI_OPT_FIRST_REFRESH);
                }
            }
        }
        break;
    case UI_OPT_KEY_BACK:
        if (num_buf_len == 0) //如果数据为空，则返回
        {
            if (g_desktop_or_phone_dial_page == false)
            {
                ui_phone(UI_OPT_PART_REFRESH);
            }
            else
            {
                g_desktop_or_phone_dial_page = false;
                back_to_desktop();
            }
        }
        else //数据不为空，删除
        {
            poc.dial.phone[num_buf_len - 1] = 0;
            num_buf_len--;
            lcd_api_clear_a_line(size_16, 70);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, poc.dial.phone, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    default:
        break;
    }
}

void ui_phone_call_log(ui_change_type opt)
{

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        s_call_log_current_item = 0;
    case UI_OPT_PART_REFRESH:
        s_contacts_or_record_flag = 2;
        ui.page = UI_PAGE_PHONE_RECORDS;
        ui_clear();
        ui_show_title((u8 *)c_Call_log);
        ui_show_bottom_button(BUTTOM_OK_BACK);

        poc.miss_call_state = 0; //当进入通话记录界面时 去除未接电话图标

        if (call_record.count != 0 && call_record.count != 0xff)
        {
            if (call_record.had_call_record_num_max == 1)
            {
                lcd_api_normal_item_disp_with_record_list(s_call_log_current_item, CALL_RECORD_NUM_MAX, REFLASH_ALL);
            }
            else
            {
                lcd_api_normal_item_disp_with_record_list(s_call_log_current_item, call_record.count, REFLASH_ALL);
            }
        }
        else
        {
            //无呼叫记录
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, (u8 *)c_no_records, COLOR_FONT, COLOR_BACKGROUND);
        }

        break;
    case UI_OPT_KEY_UP: //上切换项目
        s_call_log_current_item--;
        if (s_call_log_current_item < 0)
        {
            if (call_record.had_call_record_num_max == 1)
            {
                s_call_log_current_item = CALL_RECORD_NUM_MAX - 1;
            }
            else
            {
                s_call_log_current_item = call_record.count - 1;
            }
        }

        if (call_record.count != 0 && call_record.count != 0xff)
        {
            if (call_record.had_call_record_num_max == 1)
            {
                lcd_api_normal_item_disp_with_record_list(s_call_log_current_item, CALL_RECORD_NUM_MAX, REFLASH_KEY_UP);
            }
            else
            {
                lcd_api_normal_item_disp_with_record_list(s_call_log_current_item, call_record.count, REFLASH_KEY_UP);
            }
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        s_call_log_current_item++;
        if (call_record.had_call_record_num_max == 1)
        {
            if (s_call_log_current_item > CALL_RECORD_NUM_MAX - 1)
            {
                s_call_log_current_item = 0;
            }
        }
        else
        {
            if (s_call_log_current_item > call_record.count - 1)
            {
                s_call_log_current_item = 0;
            }
        }

        if (call_record.count != 0 && call_record.count != 0xff)
        {
            if (call_record.had_call_record_num_max == 1)
            {
                lcd_api_normal_item_disp_with_record_list(s_call_log_current_item, CALL_RECORD_NUM_MAX, REFLASH_KEY_DOWN);
            }
            else
            {
                lcd_api_normal_item_disp_with_record_list(s_call_log_current_item, call_record.count, REFLASH_KEY_DOWN);
            }
        }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        if (call_record.count != 0 && call_record.count != 0xff)
        {
            ui_son_menu(UI_OPT_FIRST_REFRESH, s_call_log_current_item % 3);
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_phone(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting(ui_change_type opt)
{
    char *item[] = {
        c_PTT_tone_setting,
        c_SIM_card_toggle,
        c_USB_switch,
        c_Key_lock_mode,
        c_Local_information,
        c_Backlight_setting,
        c_Network_setting,
        c_Factory,
        c_Module_upgrade,
        c_MCU_upgrade,
        c_hot_key,
#ifdef LANGUAGE_ENGLISH
        c_Time_zone_setting,
#endif

#ifdef POC_ZZD
        c_8k_encode,
#endif

#if defined(LANGUAGE_ENGLISH) && defined(EC25_USA)
        c_tx_time_limit,
#endif
    };

    static int setting_current_item = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        setting_current_item = 0;
    case UI_OPT_PART_REFRESH:
        ui.page = UI_PAGE_SETTING;
        ui_clear();
        ui_show_title((u8 *)c_Setting);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        //        Poc_SendCmdPara0("AT+POC=020000\r\n");
        //        Delay_ms(10);
        //        Poc_SendCmdPara0(AT_READ_MODULE_VER);
        //        Delay_ms(10);

        //只有退出设置菜单，数据才会保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        lcd_api_normal_item_disp(setting_current_item, item, sizeof(item) / sizeof(item[0]), 0xFFFF, REFLASH_ALL);
        break;
    case UI_OPT_KEY_UP: //上切换项目
        setting_current_item--;
        if (setting_current_item < 0)
        {
            setting_current_item = sizeof(item) / sizeof(item[0]) - 1;
        }
        lcd_api_normal_item_disp(setting_current_item, item, sizeof(item) / sizeof(item[0]), 0xFFFF, REFLASH_KEY_UP);

        if (poc.get_poc_ver_flag == false)
        {
            module_at_tx(AT_TX_READ_POC_VER);
        }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        setting_current_item++;
        if (setting_current_item > sizeof(item) / sizeof(item[0]) - 1)
        {
            setting_current_item = 0;
        }
        lcd_api_normal_item_disp(setting_current_item, item, sizeof(item) / sizeof(item[0]), 0xFFFF, REFLASH_KEY_DOWN);

        if (poc.get_poc_ver_flag == false)
        {
            module_at_tx(AT_TX_READ_POC_VER);
        }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        switch ((setting_type)setting_current_item)
        {
        case SETTING_PTTTONE:
            ui_setting_ptttone(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_SIM_CARD:
            ui_setting_sim_card_toggle(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_USB:
            ui_setting_usb_switch(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_LOCK_MODE:
            ui_setting_lock_mode(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_ABOUT:
            ui_setting_local_info(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_BACKLIGHT:
            ui_setting_backlight(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_NETWORK:
            ui_setting_network(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_FACTORY:
            ui_setting_factory(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_MODULE_UPGRADE:
            ui_setting_module_upgrade(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_MCU_UPGRADE:
            ui_setting_mcu_upgrade(UI_OPT_FIRST_REFRESH);
            break;
        case SETTING_HOTKEY:
            ui_setting_hotkey(UI_OPT_FIRST_REFRESH);
            break;
#ifdef LANGUAGE_ENGLISH
        case SETTING_TIMEZONE:
            ui_setting_timezone(UI_OPT_FIRST_REFRESH);
            break;
#endif
#ifdef POC_ZZD

        case SETTING_8KCODE:
            ui_setting_8k_encode(UI_OPT_FIRST_REFRESH);
            break;
#endif

#if defined(LANGUAGE_ENGLISH) && defined(EC25_USA)

        case SETTING_TOT:
            ui_setting_tx_time_limit(UI_OPT_FIRST_REFRESH);
            break;
#endif
        default:
            break;
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_ptttone(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_PTTTONE;
        ui_clear();
        ui_show_title((u8 *)c_PTT_tone_setting);
        ui_show_bottom_button(BUTTOM_SWITCH_BACK);
        lcd_api_text_gbk_display(size_12, display_left, 80 - 14 - 6 * strlen(c_Close) - 12, 60 + 3, (u8 *)c_Close, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 80 + 14 + 12, 60 + 3, (u8 *)c_Open, COLOR_FONT, COLOR_BACKGROUND);
        if (poc.flash_data.ptt_tone_toggle == 1)
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        else
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (poc.flash_data.ptt_tone_toggle == 1)
        {
            poc.flash_data.ptt_tone_toggle = 0;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        else
        {
            poc.flash_data.ptt_tone_toggle = 1;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        poc_set_ptt_tone();
        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        break;
    case UI_OPT_KEY_BACK:
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_sim_card_toggle(ui_change_type opt)
{
#define SIM_CARD_TOGGLE_ITEM_NUM_MAX 2
    static int sim_card_current_item = 0;
    char *item[SIM_CARD_TOGGLE_ITEM_NUM_MAX] = {c_SIM_1, c_SIM_2};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_SIM_TOGGLE;
        ui_clear();
        ui_show_title((u8 *)c_SIM_card_toggle);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_normal_item_disp(sim_card_current_item, item, SIM_CARD_TOGGLE_ITEM_NUM_MAX, poc.flash_data.sim_card_toggle, REFLASH_ALL);

        break;
    case UI_OPT_KEY_UP:
        sim_card_current_item--;
        if (sim_card_current_item < 0)
        {
            sim_card_current_item = SIM_CARD_TOGGLE_ITEM_NUM_MAX - 1;
        }
        lcd_api_normal_item_disp(sim_card_current_item, item, SIM_CARD_TOGGLE_ITEM_NUM_MAX, poc.flash_data.sim_card_toggle, REFLASH_KEY_UP);

        break;
    case UI_OPT_KEY_DOWN:
        sim_card_current_item++;
        if (sim_card_current_item > SIM_CARD_TOGGLE_ITEM_NUM_MAX - 1)
        {
            sim_card_current_item = 0;
        }
        lcd_api_normal_item_disp(sim_card_current_item, item, SIM_CARD_TOGGLE_ITEM_NUM_MAX, poc.flash_data.sim_card_toggle, REFLASH_KEY_DOWN);

        break;
    case UI_OPT_KEY_ENTER:
        poc.flash_data.sim_card_toggle = sim_card_current_item;

        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        lcd_api_normal_item_disp(sim_card_current_item, item, SIM_CARD_TOGGLE_ITEM_NUM_MAX, poc.flash_data.sim_card_toggle, REFLASH_ALL);

        lcd_api_text_gbk_display(size_12, display_middle, 0, 120 - 20, (u8 *)c_Please_restart, COLOR_RED, COLOR_BACKGROUND);

        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_usb_switch(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_USB_SWITCH;
        ui_clear();
        ui_show_title((u8 *)c_USB_switch);
        ui_show_bottom_button(BUTTOM_SWITCH_BACK);
        lcd_api_text_gbk_display(size_12, display_left, 80 - 14 - 6 * strlen(c_Close) - 12, 60 + 3, (u8 *)c_Close, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 80 + 14 + 12, 60 + 3, (u8 *)c_Open, COLOR_FONT, COLOR_BACKGROUND);
        if (poc.usb_toggle == 0)
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        else
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (poc.usb_toggle == 0)
        {
            poc.usb_toggle = 1;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
            set_gpio_state(GPIO_USB_EN, on); //打开adb
        }
        else
        {
            poc.usb_toggle = 0;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
            set_gpio_state(GPIO_USB_EN, off); //关闭adb
        }
        //Poc_SaveConfig();
        break;
    case UI_OPT_KEY_BACK:
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_lock_mode(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_LOCK_MODE;
        ui_clear();
        ui_show_title((u8 *)c_Key_lock_mode);
        ui_show_bottom_button(BUTTOM_SWITCH_BACK);
        lcd_api_text_gbk_display(size_12, display_left, 80 - 14 - 6 * strlen(c_Manual) - 12, 60 + 3, (u8 *)c_Manual, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 80 + 14 + 12, 60 + 3, (u8 *)c_Automatic, COLOR_FONT, COLOR_BACKGROUND);
        if (poc.flash_data.key_lock_toggle == 0)
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        else
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (poc.flash_data.key_lock_toggle == 0)
        {
            poc.flash_data.key_lock_toggle = 1;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        else
        {
            poc.flash_data.key_lock_toggle = 0;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        //Poc_SaveConfig();
        break;
    case UI_OPT_KEY_BACK:
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_local_info(ui_change_type opt)
{
    static int local_info_current_item = 0;
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一页

        ui.page = UI_PAGE_SETTING_DEVICE;
        ui_clear();
        ui_show_title((u8 *)c_Local_information);
        ui_show_bottom_button(BUTTOM_BACK);
        local_info_current_item = 0;

        lcd_api_text_gbk_display(size_12, display_left, 5, 40, (u8 *)c_Account, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 60, (u8 *)c_Name, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 80, (u8 *)c_Group, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)c_Ver, COLOR_FONT, COLOR_BACKGROUND);

        lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)c_Account), 40, (u8 *)poc.flash_data.user_id, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_unicode_display(size_12, 5 + 6 * strlen((char *)c_Name), 60, poc.my_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_unicode_display(size_12, 5 + 6 * strlen((char *)c_Group), 80, poc.my_group_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen(c_Ver), 100, (u8 *)VER_SOFT, COLOR_FONT, COLOR_BACKGROUND);

        if (poc.get_poc_ver_flag == false)
        {
            module_at_tx(AT_TX_READ_POC_VER);
        }
        break;
    case UI_OPT_PART_REFRESH: //第二页
#if 0
        MX_DMA_Init();        //测试假死机问题；必须去掉
        MX_USART2_UART_Init();
        uart_it_init();
#endif
        ui_clear();
        ui_show_title((u8 *)c_Local_information);
        ui_show_bottom_button(BUTTOM_BACK);
        lcd_api_text_gbk_display(size_12, display_left, 5, 40, (u8 *)c_Module, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 80, (u8 *)"POC : ", COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)"IMSI: ", COLOR_FONT, COLOR_BACKGROUND);

        if (strlen((char *)poc.module_ver) <= 17)
        {
            lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)c_Module), 40, (u8 *)poc.module_ver, COLOR_FONT, COLOR_BACKGROUND);
        }
        else
        {
            u8 temp_buf[20];

            memset(temp_buf, 0, sizeof(temp_buf));
            memcpy(temp_buf, poc.module_ver, 17);
            lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)c_Module), 40, (u8 *)temp_buf, COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 5, 60, (u8 *)poc.module_ver + 17, COLOR_FONT, COLOR_BACKGROUND);
        }

        lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen("POC : "), 80, (u8 *)poc.poc_ver, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen("IMSI: "), 100, (u8 *)poc.imsi_number, COLOR_FONT, COLOR_BACKGROUND);
        break;
    case UI_OPT_OTHER_REFRESH: //第三页
        ui_clear();
        ui_show_title((u8 *)c_Local_information);
        ui_show_bottom_button(BUTTOM_BACK);
        break;
    case UI_OPT_KEY_UP: //上切换项目
        local_info_current_item--;
        if (local_info_current_item < 0)
        {
            local_info_current_item = 1;
        }

        if (local_info_current_item == 0)
        {
            ui_setting_local_info(UI_OPT_FIRST_REFRESH); //第一页
        }
        else
        {
            ui_setting_local_info(UI_OPT_PART_REFRESH); //第二页
        }

        // if (local_info_current_item == 0)
        // {
        //     ui_setting_local_info(UI_OPT_FIRST_REFRESH); //第一页
        // }
        // else if (local_info_current_item == 1)
        // {
        //     ui_setting_local_info(UI_OPT_PART_REFRESH); //第二页
        // }
        // else if (local_info_current_item == 2)
        // {
        //     ui_setting_local_info(UI_OPT_OTHER_REFRESH); //第三页
        // }
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        local_info_current_item++;
        if (local_info_current_item > 1)
        {
            local_info_current_item = 0;
        }

        if (local_info_current_item == 0)
        {
            ui_setting_local_info(UI_OPT_FIRST_REFRESH); //第一页
        }
        else
        {
            ui_setting_local_info(UI_OPT_PART_REFRESH); //第二页
        }

        // if (local_info_current_item == 0)
        // {
        //     ui_setting_local_info(UI_OPT_FIRST_REFRESH); //第一页
        // }
        // else if (local_info_current_item == 1)
        // {
        //     ui_setting_local_info(UI_OPT_PART_REFRESH); //第二页
        // }
        // else if (local_info_current_item == 2)
        // {
        //     ui_setting_local_info(UI_OPT_OTHER_REFRESH); //第二页//第三页
        // }
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_backlight(ui_change_type opt)
{
#define BACKLIGHT_ITEM_NUM_MAX 4
    static int backlight_current_item = 0;
    char *item[BACKLIGHT_ITEM_NUM_MAX] = {c_sec_10, c_sec_20, c_sec_30, c_Always_open};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_BL;
        ui_clear();
        ui_show_title((u8 *)c_Backlight_setting);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_normal_item_disp(backlight_current_item, item, BACKLIGHT_ITEM_NUM_MAX, poc.flash_data.backlight_flag, REFLASH_ALL);

        break;
    case UI_OPT_KEY_UP:
        backlight_current_item--;
        if (backlight_current_item < 0)
        {
            backlight_current_item = BACKLIGHT_ITEM_NUM_MAX - 1;
        }
        lcd_api_normal_item_disp(backlight_current_item, item, BACKLIGHT_ITEM_NUM_MAX, poc.flash_data.backlight_flag, REFLASH_KEY_UP);

        break;
    case UI_OPT_KEY_DOWN:
        backlight_current_item++;
        if (backlight_current_item > BACKLIGHT_ITEM_NUM_MAX - 1)
        {
            backlight_current_item = 0;
        }
        lcd_api_normal_item_disp(backlight_current_item, item, BACKLIGHT_ITEM_NUM_MAX, poc.flash_data.backlight_flag, REFLASH_KEY_DOWN);

        break;
    case UI_OPT_KEY_ENTER:
        poc.flash_data.backlight_flag = backlight_current_item;

        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        if (poc.flash_data.backlight_flag == 0) //10s
        {
            poc.backlight_flash_time = 10;
        }
        else if (poc.flash_data.backlight_flag == 1) //20s
        {
            poc.backlight_flash_time = 20;
        }
        else if (poc.flash_data.backlight_flag == 2) //30s
        {
            poc.backlight_flash_time = 30;
        }
        else if (poc.flash_data.backlight_flag == 3)
        {
            poc.backlight_flash_time = 0xff; //常亮
        }
        else
        {
            poc.backlight_flash_time = 0xff; //常亮
        }
        lcd_api_normal_item_disp(backlight_current_item, item, BACKLIGHT_ITEM_NUM_MAX, poc.flash_data.backlight_flag, REFLASH_ALL);

        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_network(ui_change_type opt)
{
#define NETWORK_ITEM_NUM_MAX 5
    static int network_current_item = 0;
    char *item[NETWORK_ITEM_NUM_MAX] = {c_Network_Automatic, c_only_4G, c_only_3G, c_only_2G, c_only_23G};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_NET;
        ui_clear();
        ui_show_title((u8 *)c_Network_setting);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_normal_item_disp(network_current_item, item, NETWORK_ITEM_NUM_MAX, poc.flash_data.network_flag, REFLASH_ALL);

        break;
    case UI_OPT_KEY_UP:
        network_current_item--;
        if (network_current_item < 0)
        {
            network_current_item = NETWORK_ITEM_NUM_MAX - 1;
        }
        lcd_api_normal_item_disp(network_current_item, item, NETWORK_ITEM_NUM_MAX, poc.flash_data.network_flag, REFLASH_KEY_UP);

        break;
    case UI_OPT_KEY_DOWN:
        network_current_item++;
        if (network_current_item > NETWORK_ITEM_NUM_MAX - 1)
        {
            network_current_item = 0;
        }
        lcd_api_normal_item_disp(network_current_item, item, NETWORK_ITEM_NUM_MAX, poc.flash_data.network_flag, REFLASH_KEY_DOWN);

        break;
    case UI_OPT_KEY_ENTER:
        poc.flash_data.network_flag = network_current_item;
        poc_set_net_mode(poc.flash_data.network_flag);
        poc.network_type = 0;
        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        lcd_api_normal_item_disp(network_current_item, item, NETWORK_ITEM_NUM_MAX, poc.flash_data.network_flag, REFLASH_ALL);

        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_timezone(ui_change_type opt)
{
#if 1 //#ifdef LANGUAGE_ENGLISH
#define TIMEZONE_ITEM_NUM_MAX 25
    static int timezone_current_item = 0;
    char *item[TIMEZONE_ITEM_NUM_MAX] = {
        "UTC+0", "UTC+1", "UTC+2", "UTC+3", "UTC+4",
        "UTC+5", "UTC+6", "UTC+7", "UTC+8", "UTC+9",
        "UTC+10", "UTC+11", "UTC+12", "UTC-1", "UTC-2", "UTC-3",
        "UTC-4", "UTC-5", "UTC-6", "UTC-7", "UTC-8",
        "UTC-9", "UTC-10", "UTC-11", "UTC-12"};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:

        ui.page = UI_PAGE_SETTING_TIME_ZONE;
        ui_clear();
        ui_show_title((u8 *)c_Time_zone_setting);
#ifdef POC_ZZD //卓智达不支持
        ui_show_bottom_button(BUTTOM_BACK);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 58, (u8 *)c_No_this_feature, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 81, (u8 *)c_Please_press_the_return_key, COLOR_FONT, COLOR_BACKGROUND);
#else
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_normal_item_disp(timezone_current_item, item, TIMEZONE_ITEM_NUM_MAX, poc.flash_data.time_zone_flag, REFLASH_ALL);
#endif
        break;
    case UI_OPT_KEY_UP:
#ifndef POC_ZZD //卓智达不支持
        timezone_current_item--;
        if (timezone_current_item < 0)
        {
            timezone_current_item = TIMEZONE_ITEM_NUM_MAX - 1;
        }
        lcd_api_normal_item_disp(timezone_current_item, item, TIMEZONE_ITEM_NUM_MAX, poc.flash_data.time_zone_flag, REFLASH_KEY_UP);
#endif
        break;
    case UI_OPT_KEY_DOWN:
#ifndef POC_ZZD //卓智达不支持
        timezone_current_item++;
        if (timezone_current_item > TIMEZONE_ITEM_NUM_MAX - 1)
        {
            timezone_current_item = 0;
        }
        lcd_api_normal_item_disp(timezone_current_item, item, TIMEZONE_ITEM_NUM_MAX, poc.flash_data.time_zone_flag, REFLASH_KEY_DOWN);
#endif
        break;
    case UI_OPT_KEY_ENTER:
#ifndef POC_ZZD //卓智达不支持
        poc.flash_data.time_zone_flag = timezone_current_item;
        poc_set_time_zone(poc.flash_data.time_zone_flag);

        lcd_api_normal_item_disp(timezone_current_item, item, TIMEZONE_ITEM_NUM_MAX, poc.flash_data.time_zone_flag, REFLASH_ALL);
        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();
        delay_ms(100);
        play_tts(2, TTS_Set_up_successfully_please_restart);
#endif
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }

#else
    UI.Page = UI_PAGE_SETTING_TIME_ZONE;
    UI_CleanMenuSelWindow();
    UI_ShowTitleBar1((uint8_t *)c_Time_zone_setting, FONT_GB2312);
    ui_text_gbk_display(size_12, display_middle, 0, 58, (u8 *)c_No_this_feature, COLOR_WIGHT, COLOR_MAIN_BACKCOLOR);
    ui_text_gbk_display(size_12, display_middle, 0, 81, (u8 *)c_Please_press_the_return_key, COLOR_WIGHT, COLOR_MAIN_BACKCOLOR);
    UI_ShowBottomOpt("    ", c_Back);
#endif
}

//输入窗口显示小红杠
static void dispaly_red_vertical_line(u16 x, u16 y)
{
    static u64 count = 0;

    count++;

    if (count == 1000 * 5)
    {
        lcd_api_text_gbk_display(size_16, display_left, x, y, (u8 *)"|", COLOR_RED, COLOR_BACKGROUND);
    }
    else if (count == 2000 * 5)
    {
        count = 0;
        lcd_api_text_gbk_display(size_16, display_left, x, y, (u8 *)"|", COLOR_BACKGROUND, COLOR_BACKGROUND);
    }
    else
    {
    }
}

void ui_setting_factory(ui_change_type opt)
{
    static u8 num_buf[50];
    static u8 num_buf_len = 0;
    static char current_data_old = 0;
    char current_data = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入

        ui.page = UI_PAGE_SETTING_FACTORY;
        ui_clear();
        ui_show_title((u8 *)c_Factory);
        ui_show_bottom_button(BUTTOM_OK_BACK);

        lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Enter_password, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 94, (u8 *)c_Enter_number_and_press_OK, COLOR_RED, COLOR_BACKGROUND);

        //显示一个初始数据或者0
        memset(num_buf, 0, sizeof(num_buf));
        num_buf_len = strlen((char *)num_buf);
        lcd_api_clear_a_line(size_16, 70);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        break;
    case UI_OPT_KEY_KEYBOARD: //键盘输入，实时刷新
        if (ui.page == UI_PAGE_SETTING_FACTORY)
        {
            if (num_buf_len < 8) //密码最大8位
            {
                if (get_keyboard_number() != 0)
                {
                    current_data = get_keyboard_number();
                    keyboard_number_clear();
                }

                if (current_data == 0)
                {
                    current_data_old = 0;
                }
                else
                {
                    if (current_data_old != current_data)
                    {
                        current_data_old = current_data;
                        num_buf[num_buf_len] = current_data;
                        num_buf_len++;
                        lcd_api_clear_a_line(size_16, 70);
                        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
                dispaly_red_vertical_line(80 + 8 * num_buf_len / 2, 70 - 1);
            }
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (strcmp((char *)num_buf, "27770875") == 0) //如果密码正确则恢复出厂
        {
            //跳转带升级界面
            memset(num_buf, 0, sizeof(num_buf));
            ui.page = UI_PAGE_SETTING_MODULE_UPDATE_STEP2;

            ui_clear();
            ui_show_title((u8 *)c_Factory);
            lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Restoring, COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_middle, 0, 70, (u8 *)c_Please_wait, COLOR_FONT, COLOR_BACKGROUND);
            poc_factory_reset();
            led_state_set(NOT_LOGGED);
            // ui_setting(UI_OPT_FIRST_REFRESH); //恢复出厂设置后返回设置界面
        }
        else
        {
            play_tts(2, TTS_Wrong_password_please_re_enter); //播报密码错误
            memset(num_buf, 0, sizeof(num_buf));
            num_buf_len = 0;
            lcd_api_clear_a_line(size_16, 70);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    case UI_OPT_KEY_BACK:                 //返回上一级
        if (strlen((char *)num_buf) == 0) //如果数据为空，则返回
        {
            ui_setting(UI_OPT_PART_REFRESH);
        }
        else //数据不为空，删除
        {
            num_buf[num_buf_len - 1] = 0;
            num_buf_len--;
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, (u8 *)"        ", COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    default:
        break;
    }
}

void ui_setting_module_upgrade(ui_change_type opt)
{
    static u8 num_buf[50];
    static u8 num_buf_len = 0;
    static char current_data_old = 0;
    char current_data = 0;

    static uint8_t download_percent_old = 0;
    static uint8_t download_state_old = 0xff;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入

        ui.page = UI_PAGE_SETTING_MODULE_UPDATE_STEP1;
        ui_clear();
        ui_show_title((u8 *)c_Module_upgrade);
        ui_show_bottom_button(BUTTOM_OK_BACK);

        lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Enter_password, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 94, (u8 *)c_Enter_number_and_press_OK, COLOR_RED, COLOR_BACKGROUND);

        //显示一个初始数据或者0
        memset(num_buf, 0, sizeof(num_buf));
        num_buf_len = strlen((char *)num_buf);
        lcd_api_clear_a_line(size_16, 70);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);

        break;

    case UI_OPT_KEY_KEYBOARD:                               //键盘输入，实时刷新
        if (ui.page == UI_PAGE_SETTING_MODULE_UPDATE_STEP1) //step1
        {
            if (num_buf_len < 8) //密码最大8位
            {
                if (get_keyboard_number() != 0)
                {
                    current_data = get_keyboard_number();
                    keyboard_number_clear();
                }

                if (current_data == 0)
                {
                    current_data_old = 0;
                }
                else
                {
                    if (current_data_old != current_data)
                    {
                        current_data_old = current_data;
                        num_buf[num_buf_len] = current_data;
                        num_buf_len++;
                        lcd_api_clear_a_line(size_16, 70);
                        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
                dispaly_red_vertical_line(80 + 8 * num_buf_len / 2, 70 - 1);
            }
        }

        if (ui.page == UI_PAGE_SETTING_MODULE_UPDATE_STEP2) //step2
        {
            backlight_idle_clear();

            if (download_percent_old != poc.poc_upgrade_percent || download_state_old != poc.poc_upgrade_state)
            {
                download_state_old = poc.poc_upgrade_state;
                if (poc.poc_upgrade_state == 0)
                {
                    lcd_api_clear_a_line(size_12, 97);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 97, (u8 *)c_Downloading, COLOR_FONT, COLOR_BACKGROUND);
                    lcd_api_text_gbk_display(size_12, display_left, 127, 97, (u8 *)"000%", COLOR_FONT, COLOR_BACKGROUND);
                }
                else if (poc.poc_upgrade_state == 1)
                {
                    lcd_api_clear_a_line(size_12, 97);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 97, (u8 *)c_Update_successed, COLOR_FONT, COLOR_BACKGROUND);
                    delay_ms(1500);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Please_restart, COLOR_RED, COLOR_BACKGROUND);
                }
                else if (poc.poc_upgrade_state == 2)
                {
                    u8 ucString[5] = "000%";

                    if (download_percent_old != poc.poc_upgrade_percent)
                    {
                        download_percent_old = poc.poc_upgrade_percent;

                        if (poc.poc_upgrade_percent == 100)
                        {
                            poc.poc_upgrade_state = 1;
                        }
                        else
                        {
                            ucString[0] = poc.poc_upgrade_percent / 100 + '0';
                            ucString[1] = (poc.poc_upgrade_percent % 100) / 10 + '0';
                            ucString[2] = poc.poc_upgrade_percent % 10 + '0';
                            ucString[3] = '%';
                            ucString[4] = 0x00;
                            lcd_api_text_gbk_display(size_12, display_left, 127, 97, (u8 *)ucString, COLOR_FONT, COLOR_BACKGROUND);
                        }
                    }
                }
                else if (poc.poc_upgrade_state == 3)
                {
                    lcd_api_clear_a_line(size_12, 97);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 97, (u8 *)c_Is_the_latest_version, COLOR_RED, COLOR_BACKGROUND);
                    delay_ms(1500);
                }
                else if (poc.poc_upgrade_state == 4)
                {
                    lcd_api_clear_a_line(size_12, 97);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 97, (u8 *)c_Restart_and_try_again, COLOR_FONT, COLOR_BACKGROUND);
                    delay_ms(1500);
                    lcd_api_clear_a_line(size_12, 60);
                    lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Please_restart, COLOR_RED, COLOR_BACKGROUND);
                }
                else if (poc.poc_upgrade_state == 5)
                {
                    // +POC:41000000
                    // +POC:41020064
                    // +POC:41050000
                    download_percent_old = poc.poc_upgrade_percent;
                    if (poc.poc_upgrade_percent == 100)
                    {
                        lcd_api_clear_a_line(size_12, 97);
                        lcd_api_text_gbk_display(size_12, display_middle, 0, 97, (u8 *)c_Is_the_latest_version, COLOR_RED, COLOR_BACKGROUND);
                    }
                    // else
                    // {
                    //     lcd_api_clear_a_line(size_12, 97);
                    //     lcd_api_text_gbk_display(size_12, display_middle, 0, 97, (u8 *)c_download_failed, COLOR_FONT, COLOR_BACKGROUND);
                    //     delay_ms(1500);
                    //     lcd_api_clear_a_line(size_12, 60);
                    //     lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Please_restart, COLOR_RED, COLOR_BACKGROUND);
                    // }
                }
                else
                {
                    //none
                }
            }
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (strcmp((char *)num_buf, "27770875") == 0) //如果密码正确则升级
        {
            //跳转带升级界面
            memset(num_buf, 0, sizeof(num_buf));
            ui.page = UI_PAGE_SETTING_MODULE_UPDATE_STEP2;

            ui_clear();
            ui_show_title((u8 *)c_Module_upgrade);
            // ui_show_bottom_button(BUTTOM_OK_BACK);
            lcd_api_text_gbk_display(size_12, display_left, 5, 81, (u8 *)c_Status, COLOR_FONT, COLOR_BACKGROUND);
            module_at_tx("\r\n");
            delay_ms(30);
            module_at_tx(AT_TX_POC_UPGRADE);
            poc.poc_upgrade_percent = 0;
            poc.poc_upgrade_state = 0;
            download_percent_old = 0;
            download_state_old = 0xff;
        }
        else
        {
            play_tts(2, TTS_Wrong_password_please_re_enter); //播报密码错误
            memset(num_buf, 0, sizeof(num_buf));
            num_buf_len = 0;
            lcd_api_clear_a_line(size_16, 70);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    case UI_OPT_KEY_BACK:                 //返回上一级
        if (strlen((char *)num_buf) == 0) //如果数据为空，则返回
        {
            ui_setting(UI_OPT_PART_REFRESH);
        }
        else //数据不为空，删除
        {
            num_buf[num_buf_len - 1] = 0;
            num_buf_len--;
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, (u8 *)"        ", COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    default:
        break;
    }
}

void ui_setting_mcu_upgrade(ui_change_type opt)
{
    static u8 num_buf[50];
    static u8 num_buf_len = 0;
    static char current_data_old = 0;
    char current_data = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入

        ui.page = UI_PAGE_SETTING_MCU_UPDATE_STEP1;
        ui_clear();
        ui_show_title((u8 *)c_MCU_upgrade);

#ifdef POC_ZZD //卓智达不支持MCU远程升级
        ui_show_bottom_button(BUTTOM_BACK);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 58, (u8 *)c_No_this_feature, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 81, (u8 *)c_Please_press_the_return_key, COLOR_FONT, COLOR_BACKGROUND);
#else
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 45, (u8 *)c_Enter_password, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_middle, 0, 94, (u8 *)c_Enter_number_and_press_OK, COLOR_RED, COLOR_BACKGROUND);

        //显示一个初始数据或者0
        memset(num_buf, 0, sizeof(num_buf));
        num_buf_len = strlen((char *)num_buf);
        lcd_api_clear_a_line(size_16, 70);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
#endif
        break;
    case UI_OPT_KEY_KEYBOARD: //键盘输入，实时刷新
#ifndef POC_ZZD               //卓智达不支持MCU远程升级
        if (ui.page == UI_PAGE_SETTING_MCU_UPDATE_STEP1)
        {
            if (num_buf_len < 8) //密码最大8位
            {
                if (get_keyboard_number() != 0)
                {
                    current_data = get_keyboard_number();
                    keyboard_number_clear();
                }

                if (current_data == 0)
                {
                    current_data_old = 0;
                }
                else
                {
                    if (current_data_old != current_data)
                    {
                        current_data_old = current_data;
                        num_buf[num_buf_len] = current_data;
                        num_buf_len++;
                        lcd_api_clear_a_line(size_16, 70);
                        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
                    }
                }
                dispaly_red_vertical_line(80 + 8 * num_buf_len / 2, 70 - 1);
            }
        }
#endif
        break;
    case UI_OPT_KEY_ENTER:
#ifndef POC_ZZD                                       //卓智达不支持MCU远程升级
        if (strcmp((char *)num_buf, "27770875") == 0) //如果密码正确则升级
        {
            mcu_update_reay(); //跳转带升级界面
        }
        else
        {
            play_tts(2, TTS_Wrong_password_please_re_enter); //播报密码错误
            memset(num_buf, 0, sizeof(num_buf));
            num_buf_len = 0;
            lcd_api_clear_a_line(size_16, 70);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        }
#endif
        break;
    case UI_OPT_KEY_BACK:
#ifndef POC_ZZD                           //卓智达不支持MCU远程升级
        if (strlen((char *)num_buf) == 0) //如果数据为空，则返回
        {
            ui_setting(UI_OPT_PART_REFRESH);
        }
        else //数据不为空，删除
        {
            num_buf[num_buf_len - 1] = 0;
            num_buf_len--;
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, (u8 *)"        ", COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_16, display_middle, 0, 70, num_buf, COLOR_FONT, COLOR_BACKGROUND);
        }
#else
        ui_setting(UI_OPT_PART_REFRESH);
#endif
        break;
    default:
        break;
    }
}

//mcu升级准备
static void mcu_update_reay(void)
{
    //重置boot标志位，让boot程序识别，进入update模式
    poc.boot_info.boot_flag = 0x88888888;

    //写入当前选中SIM卡槽
    if (poc.sim_card_flag == HAVE_SIM_CARD_1)
    {
        poc.boot_info.sim_card_select = 0;
    }
    else
    {
        poc.boot_info.sim_card_select = 1;
    }

    //将机器的版本和初始化标志位写入flash，为了在boot中成功读取到本机版本
    memset(poc.boot_info.ver, 0, sizeof(poc.boot_info.ver));
    memcpy(poc.boot_info.ver, VER_SOFT, sizeof(VER_SOFT));
    save_flash_bootflag_and_ver();
    read_flash_bootflag_and_ver();
    set_gpio_state(GPIO_MODULE_PWRKEY, off);
    delay_ms(100);

    module_at_tx(AT_TX_MODULE_POWER_OFF);
    delay_ms(2000);

    ui.page = UI_PAGE_SETTING_MCU_UPDATE_STEP2;

    delay_ms(100);
    NVIC_SystemReset();

    // __disable_fault_irq();
    // __disable_irq();
    // GenerateSystemReset();
}

void ui_setting_hotkey(ui_change_type opt)
{
    const char *hotkey_name[] = {"K0 ", "K1 ", "K2 ", "K3 ", "K4 ", "K5 ", "K6 ", "K7 ", "K8 ", "K9 "};
    const char *display_hotkey_type[] = {c_undefined, c_group_switching, c_member_call, c_friend_call};
    static int hotkey_current_item = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //首次进入
        hotkey_current_item = 0;
    case UI_OPT_PART_REFRESH: //上下键切换
        ui.page = UI_PAGE_SETTING_HOTKEY;
        ui_clear();
        ui_show_title((u8 *)c_hot_key);
        ui_show_bottom_button(BUTTOM_BACK);
        if (hotkey_current_item == 0 || hotkey_current_item == 1) //display 0-3 4-7
        {
            //display hotkey name
            lcd_api_text_gbk_display(size_12, display_left, 3, 40, (u8 *)hotkey_name[hotkey_current_item * 4 + 0], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3, 60, (u8 *)hotkey_name[hotkey_current_item * 4 + 1], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3, 80, (u8 *)hotkey_name[hotkey_current_item * 4 + 2], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3, 100, (u8 *)hotkey_name[hotkey_current_item * 4 + 3], COLOR_FONT, COLOR_BACKGROUND);

            //display key type
            lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3, 40, (u8 *)display_hotkey_type[poc.hotkey_info[hotkey_current_item * 4 + 0].hotkey_type], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3, 60, (u8 *)display_hotkey_type[poc.hotkey_info[hotkey_current_item * 4 + 1].hotkey_type], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3, 80, (u8 *)display_hotkey_type[poc.hotkey_info[hotkey_current_item * 4 + 2].hotkey_type], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3, 100, (u8 *)display_hotkey_type[poc.hotkey_info[hotkey_current_item * 4 + 3].hotkey_type], COLOR_FONT, COLOR_BACKGROUND);

            if (poc.hotkey_info[hotkey_current_item * 4 + 0].hotkey_type == 0)
            {
                lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3 + 6 * 9 + 3, 40, (u8 *)" - - - - -", COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, 3 + 6 * 3 + 3 + 6 * 7 + 3, 40, (u8 *)poc.hotkey_info[hotkey_current_item * 4 + 0].hotkey_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }

            if (poc.hotkey_info[hotkey_current_item * 4 + 1].hotkey_type == 0)
            {
                lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3 + 6 * 9 + 3, 60, (u8 *)" - - - - -", COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, 3 + 6 * 3 + 3 + 6 * 7 + 3, 60, (u8 *)poc.hotkey_info[hotkey_current_item * 4 + 1].hotkey_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }

            if (poc.hotkey_info[hotkey_current_item * 4 + 2].hotkey_type == 0)
            {
                lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3 + 6 * 9 + 3, 80, (u8 *)" - - - - -", COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, 3 + 6 * 3 + 3 + 6 * 7 + 3, 80, (u8 *)poc.hotkey_info[hotkey_current_item * 4 + 2].hotkey_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }

            if (poc.hotkey_info[hotkey_current_item * 4 + 3].hotkey_type == 0)
            {
                lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3 + 6 * 9 + 3, 100, (u8 *)" - - - - -", COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, 3 + 6 * 3 + 3 + 6 * 7 + 3, 100, (u8 *)poc.hotkey_info[hotkey_current_item * 4 + 3].hotkey_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        else //display 8-9
        {
            //display hotkey name
            lcd_api_text_gbk_display(size_12, display_left, 3, 40, (u8 *)hotkey_name[hotkey_current_item * 4 + 0], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3, 60, (u8 *)hotkey_name[hotkey_current_item * 4 + 1], COLOR_FONT, COLOR_BACKGROUND);

            //display key type
            lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3, 40, (u8 *)display_hotkey_type[poc.hotkey_info[hotkey_current_item * 4 + 0].hotkey_type], COLOR_FONT, COLOR_BACKGROUND);
            lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3, 60, (u8 *)display_hotkey_type[poc.hotkey_info[hotkey_current_item * 4 + 1].hotkey_type], COLOR_FONT, COLOR_BACKGROUND);

            if (poc.hotkey_info[hotkey_current_item * 4 + 0].hotkey_type == 0)
            {
                lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3 + 6 * 9 + 3, 40, (u8 *)" - - - - -", COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, 3 + 6 * 3 + 3 + 6 * 7 + 3, 40, (u8 *)poc.hotkey_info[hotkey_current_item * 4 + 0].hotkey_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }

            if (poc.hotkey_info[hotkey_current_item * 4 + 1].hotkey_type == 0)
            {
                lcd_api_text_gbk_display(size_12, display_left, 3 + 6 * 3 + 3 + 6 * 9 + 3, 60, (u8 *)" - - - - -", COLOR_FONT, COLOR_BACKGROUND);
            }
            else
            {
                lcd_api_text_unicode_display(size_12, 3 + 6 * 3 + 3 + 6 * 7 + 3, 60, (u8 *)poc.hotkey_info[hotkey_current_item * 4 + 1].hotkey_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        hotkey_current_item--;
        if (hotkey_current_item < 0)
        {
            hotkey_current_item = 2;
        }

        ui_setting_hotkey(UI_OPT_PART_REFRESH);

        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        hotkey_current_item++;
        if (hotkey_current_item > 2)
        {
            hotkey_current_item = 0;
        }

        ui_setting_hotkey(UI_OPT_PART_REFRESH);
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_tx_time_limit(ui_change_type opt)
{
#ifdef LANGUAGE_ENGLISH
#define TX_TIME_LIMIT_ITEM_NUM_MAX 2
    static int tx_time_limit_current_item = 0;
    char *item[TX_TIME_LIMIT_ITEM_NUM_MAX] = {"120s", "180s"};

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_TX_TIME_LIMIT;
        ui_clear();
        ui_show_title((u8 *)c_tx_time_limit);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        lcd_api_normal_item_disp(tx_time_limit_current_item, item, TX_TIME_LIMIT_ITEM_NUM_MAX, poc.flash_data.tx_time_limit, REFLASH_ALL);

        break;
    case UI_OPT_KEY_UP:
        tx_time_limit_current_item--;
        if (tx_time_limit_current_item < 0)
        {
            tx_time_limit_current_item = TX_TIME_LIMIT_ITEM_NUM_MAX - 1;
        }
        lcd_api_normal_item_disp(tx_time_limit_current_item, item, TX_TIME_LIMIT_ITEM_NUM_MAX, poc.flash_data.tx_time_limit, REFLASH_KEY_UP);

        break;
    case UI_OPT_KEY_DOWN:
        tx_time_limit_current_item++;
        if (tx_time_limit_current_item > TX_TIME_LIMIT_ITEM_NUM_MAX - 1)
        {
            tx_time_limit_current_item = 0;
        }
        lcd_api_normal_item_disp(tx_time_limit_current_item, item, TX_TIME_LIMIT_ITEM_NUM_MAX, poc.flash_data.tx_time_limit, REFLASH_KEY_DOWN);

        break;
    case UI_OPT_KEY_ENTER:
        poc.flash_data.tx_time_limit = tx_time_limit_current_item;
        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        lcd_api_normal_item_disp(tx_time_limit_current_item, item, TX_TIME_LIMIT_ITEM_NUM_MAX, poc.flash_data.tx_time_limit, REFLASH_ALL);

        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
#else
    ui.page = UI_PAGE_SETTING_TX_TIME_LIMIT;
    ui_clear();
    ui_show_title((u8 *)c_tx_time_limit);
    ui_show_bottom_button(BUTTOM_BACK);

    lcd_api_text_gbk_display(size_12, display_middle, 0, 58, (u8 *)c_No_this_feature, COLOR_WIGHT, COLOR_BACKGROUND);
    lcd_api_text_gbk_display(size_12, display_middle, 0, 81, (u8 *)c_Please_press_the_return_key, COLOR_WIGHT, COLOR_BACKGROUND);
#endif
}

void ui_setting_8k_encode(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SETTING_8K_ENCODE;
        ui_clear();
        ui_show_title((u8 *)c_8k_encode);
        ui_show_bottom_button(BUTTOM_SWITCH_BACK);
        lcd_api_text_gbk_display(size_12, display_left, 80 - 14 - 6 * strlen(c_Close) - 12, 60 + 3, (u8 *)c_Close, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 80 + 14 + 12, 60 + 3, (u8 *)c_Open, COLOR_FONT, COLOR_BACKGROUND);
        if (poc.flash_data.encode_8k_toggle == 0)
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        else
        {
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (poc.flash_data.encode_8k_toggle == 1)
        {
            poc.flash_data.encode_8k_toggle = 0;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_off, 28, 17);
        }
        else
        {
            poc.flash_data.encode_8k_toggle = 1;
            lcd_api_bmp(80 - 14, 60, gImage_toggle_on, 28, 17);
        }
        zzd_poc_set_8k_encode();
        //数据保存至flash
        save_flash_poc_data();
        delay_ms(100);
        read_flash_poc_data();

        break;
    case UI_OPT_KEY_BACK:
        ui_setting(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void ui_setting_air_code_writing(ui_change_type opt)
{
    static bool enter_air_code_flag_old = false;
    static u8 meid_flag_old = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一页
        ui.page = UI_PAGE_AIR_CODE_WRITING;
        lcd_api_clear(0, 0, COLOR_ICO_BACKGROUND, X_MAX, Y_MAX);
        ui_clear();
        ui_show_title((u8 *)c_Air_code_writing);
        lcd_api_clear(0, Y_MAX - Y_BOTTOM_SPACE, COLOR_ICO_BACKGROUND, X_MAX, Y_BOTTOM_SPACE);
        lcd_api_text_gbk_display(size_12, display_left, 5, 40, (u8 *)c_Account, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)c_Account), 40, (u8 *)poc.flash_data.user_id, COLOR_FONT, COLOR_BACKGROUND);

        lcd_api_text_gbk_display(size_12, display_left, 5, 60, (u8 *)"IP:", COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)"IP:"), 60, poc.flash_data.server_ip, COLOR_FONT, COLOR_BACKGROUND);

        lcd_api_text_gbk_display(size_12, display_left, 5, 80, (u8 *)"MEID:", COLOR_FONT, COLOR_BACKGROUND);
        if (poc.get_meid_flag == 2)
        {
            lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)"MEID:"), 80, poc.meid_number, COLOR_FONT, COLOR_BACKGROUND);
        }

        if (poc.enter_air_code_flag == false)
        {
            lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)c_Status_waiting, COLOR_FONT, COLOR_BACKGROUND);
        }
        else
        {
            lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)c_Status_Ready, COLOR_FONT, COLOR_BACKGROUND);
        }
        break;
    case UI_OPT_KEY_KEYBOARD: //实时刷新
        if (ui.page == UI_PAGE_AIR_CODE_WRITING)
        {
            //set_gpio_state(GPIO_PA_EN, on);
            //SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
            if (enter_air_code_flag_old != poc.enter_air_code_flag)
            {
                enter_air_code_flag_old = poc.enter_air_code_flag;
                lcd_api_clear_a_line(size_12, 100);
                if (poc.enter_air_code_flag == false)
                {
                    lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)c_Status_waiting, COLOR_FONT, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_text_gbk_display(size_12, display_left, 5, 100, (u8 *)c_Status_Ready, COLOR_FONT, COLOR_BACKGROUND);
                }
            }

            if (meid_flag_old != poc.get_meid_flag)
            {
                meid_flag_old = poc.get_meid_flag;
                if (poc.get_meid_flag == 2)
                {
                    lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)"MEID:"), 80, poc.meid_number, COLOR_FONT, COLOR_BACKGROUND);
                }
            }

            if (g_air_code_account_update_flag == true)
            {
                g_air_code_account_update_flag = false;
                lcd_api_clear_a_line(size_12, 40);
                lcd_api_text_gbk_display(size_12, display_left, 5, 40, (u8 *)c_Account, COLOR_FONT, COLOR_BACKGROUND);
                lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)c_Account), 40, (u8 *)poc.flash_data.user_id, COLOR_FONT, COLOR_BACKGROUND);

                lcd_api_clear_a_line(size_12, 60);
                lcd_api_text_gbk_display(size_12, display_left, 5, 60, (u8 *)"IP:", COLOR_FONT, COLOR_BACKGROUND);
                lcd_api_text_gbk_display(size_12, display_left, 5 + 6 * strlen((char *)"IP:"), 60, poc.flash_data.server_ip, COLOR_FONT, COLOR_BACKGROUND);
            }
        }
        break;
    case UI_OPT_KEY_UP: //上切换项目
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        break;
    default:
        break;
    }
}

void ui_dial(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        ui.page = UI_PAGE_DIAL;
        ui_clear();
        ui_show_title((u8 *)c_DIAL);
        ui_show_bottom_button(BUTTOM_BACK);
        break;
    case UI_OPT_KEY_UP: //上切换项目
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        break;
    case UI_OPT_KEY_ENTER: //确认保存数据
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        ui_menu(UI_OPT_FIRST_REFRESH);
        break;
    default:
        break;
    }
}

void ui_on_call(ui_change_type opt)
{
    static u8 dtmf_buf[50];
    static u8 num_buf_len = 0;
    static char current_data_old = 0;
    char current_data = 0;
    // static bool mute_state = false; //静音开关 默认:false 不静音  true：静音

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        ui.page = UI_PAGE_ON_CALL;
        ui_clear();
        ui_show_title((u8 *)c_on_call);
        ui_show_bottom_button(BUTTOM_BACK);
        // mute_state = false;
        set_gpio_state(GPIO_PA_EN, on);
        SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        g_call_state = true;

        //send at
        if (s_contacts_or_record_flag == 3) //来电
        {
        }
        else
        {
            if (s_contacts_or_record_flag == 0) //拨号获取号码
            {
                memset(poc.dial.name, 0, sizeof(poc.dial.name));
            }
            else if (s_contacts_or_record_flag == 1) //联系人获取号码
            {
                memcpy(poc.dial.name, call_contacts.current_contact[s_phone_contact_current_item].name, sizeof(call_contacts.current_contact[s_phone_contact_current_item].name));
                memcpy(poc.dial.phone, call_contacts.current_contact[s_phone_contact_current_item].phone, sizeof(call_contacts.current_contact[s_phone_contact_current_item].phone));
            }
            else if (s_contacts_or_record_flag == 2) //呼叫记录获取号码
            {
                memcpy(poc.dial.name, call_record.current_contact[s_call_log_current_item].name, sizeof(call_record.current_contact[s_call_log_current_item].name));
                memcpy(poc.dial.phone, call_record.current_contact[s_call_log_current_item].phone, sizeof(call_record.current_contact[s_call_log_current_item].phone));
            }
            else
            {
                /* code */
            }

            module_at_tx("\r\n");
            delay_ms(30);
            set_volume_level(poc.volume_level); //解决问题：当处于通话界面时，限制最大音量，电话时增益过大会导致，语音卡顿（截断导致）
            delay_ms(10);
            char sprintf_str_1[100];
            memset(sprintf_str_1, 0, sizeof(sprintf_str_1));
            sprintf(sprintf_str_1, AT_TX_CALL_SOMEONE, (char *)poc.dial.phone);
            module_at_tx(sprintf_str_1);
        }
        lcd_api_text_gbk_display(size_16, display_middle, 0, 50, poc.dial.name, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, poc.dial.phone, COLOR_FONT, COLOR_BACKGROUND);

        memset(dtmf_buf, 0, sizeof(dtmf_buf));
        num_buf_len = 0;

        break;
    case UI_OPT_KEY_KEYBOARD: //显示时间&键盘输入
        if (ui.page == UI_PAGE_ON_CALL)
        {
            //set_gpio_state(GPIO_PA_EN, on); //开启喇叭

            display_phone_time_count_down(g_call_state);

            if (num_buf_len < 15) //密码最大8位
            {
                if (get_keyboard_number() != 0)
                {
                    current_data = get_keyboard_number();
                    keyboard_number_clear();
                }

                if (current_data == 0)
                {
                    current_data_old = 0;
                }
                else
                {
                    if (current_data_old != current_data)
                    {
                        current_data_old = current_data;

                        dtmf_buf[num_buf_len] = current_data;
                        num_buf_len++;
                        lcd_api_clear_a_line(size_16, 90);
                        lcd_api_text_gbk_display(size_16, display_middle, 0, 90, dtmf_buf, COLOR_FONT, COLOR_BACKGROUND);
                        send_dtmf_code(current_data); //通话过程中DTMF处理
                    }
                }
                dispaly_red_vertical_line(80 + 8 * num_buf_len / 2, 90 - 1);
            }
        }
        break;
    case UI_OPT_KEY_ENTER:
#if 0
        if (mute_state == false)
        {
            mute_state = true;
            set_gpio_state(GPIO_PA_EN, off);
            SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
            ui_show_bottom_button(BUTTOM_RED_MUTE_BACK);
        }
        else
        {
            mute_state = false;
            set_gpio_state(GPIO_PA_EN, on);
            SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
            ui_show_bottom_button(BUTTOM_MUTE_BACK);
        }
#endif
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        set_gpio_state(GPIO_PA_EN, off);
        SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        g_call_state = false;
        display_phone_time_count_down(g_call_state);
        module_at_tx("\r\n");
        delay_ms(30);
        module_at_tx(AT_TX_CALL_END1);
        delay_ms(100);
        module_at_tx(AT_TX_CALL_END2);
        delay_ms(100);
        module_at_tx(AT_TX_CALL_END3);
        delay_ms(100);

        //存入呼叫记录
        deposit_call_record();

        if (g_desktop_or_phone_dial_page == false)
        {
            ui_phone(UI_OPT_PART_REFRESH);
        }
        else
        {
            g_desktop_or_phone_dial_page = false;
            back_to_desktop();
        }
        set_volume_level(poc.volume_level); //挂电话主动设置一次音量增益
        delay_ms(100);
        break;
    default:
        break;
    }
}

//来电界面
void ui_incoming_call(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入
        ui.page = UI_PAGE_INCOMING_CALL;
        ui_clear();
        ui_show_title((u8 *)c_Incoming_call);
        ui_show_bottom_button(BUTTOM_OK_BACK);
        s_contacts_or_record_flag = 3;
        break;
    case UI_OPT_PART_REFRESH: //刷新电话号码
        lcd_api_text_gbk_display(size_16, display_middle, 0, 50, poc.dial.name, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_16, display_middle, 0, 70, poc.dial.phone, COLOR_FONT, COLOR_BACKGROUND);
        break;
    case UI_OPT_KEY_KEYBOARD: //显示时间&键盘输入
        if (ui.page == UI_PAGE_INCOMING_CALL)
        {
            set_gpio_state(GPIO_PA_EN, on); //开启喇叭
            // SEGGER_RTT_printf(0, "%s %s-%d:PA-ON%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        }
        break;
    case UI_OPT_KEY_ENTER:
        poc.miss_call_state = 0;

        module_at_tx(AT_TX_CALL_ANSWER);
        delay_ms(100);
        ui_on_call(UI_OPT_FIRST_REFRESH);
        delay_ms(100);
        set_volume_level(poc.volume_level); //解决问题：当处于通话界面时，限制最大音量，电话时增益过大会导致，语音卡顿（截断导致）
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        poc.miss_call_state = 0;
        module_at_tx("\r\n");
        delay_ms(30);
        module_at_tx(AT_TX_CALL_END1);
        delay_ms(100);
        module_at_tx(AT_TX_CALL_END2);
        delay_ms(100);
        module_at_tx(AT_TX_CALL_END3);
        delay_ms(100);

        //存入呼叫记录
        deposit_call_record();
        //显示未接电话图标

        back_to_desktop();
        set_volume_level(poc.volume_level); //挂电话主动设置一次音量增益
        delay_ms(100);
        break;
    default:
        break;
    }
}

void ui_invite(ui_change_type opt)
{
    static bool receive_voice_old = false;
    static bool im_talking_flag_old = false;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //第一次进入

        //解决BUG：A组呼B，B进入单呼，显示异常，然后退出单呼，第一次组呼无声，第二次组呼声音有延时的问题
        poc.im_talking_flag = false;
        poc.can_i_speak_flag = true;
        poc.receive_voice = false;
        led_state_set(STANDBY);

        ui.page = UI_PAGE_INVITE;
        ui_clear();
        ui_show_title((u8 *)c_INVITE);
        ui_show_bottom_button(BUTTOM_BACK);

        // lcd_api_bmp(70, 60, gImage_member_20x20, 20, 20);
        // lcd_api_text_unicode_display(size_12, 10, 80, (u8 *)poc.talk_user_name, POC_NAME_LENTH_MAX, COLOR_FONT, COLOR_BACKGROUND);

        //显示对方图标+文本
        lcd_api_bmp(32, 62, gImage_member_20x20, 20, 20);
        lcd_api_text_scroll_display(10, 90, 6, false, (char *)poc.talk_user_name);

        //箭头
        lcd_api_bmp(77, 67, gImage_two_way_arrow16x16, 16, 16);

        //显示本机图标+文本
        lcd_api_bmp(124, 60, gImage_member_20x20, 20, 20);
        lcd_api_text_gbk_display(size_12, display_left, 124, 90, (u8 *)c_Local, COLOR_FONT, COLOR_BACKGROUND);
        break;
    case UI_OPT_OTHER_REFRESH:                                                    //实时刷新
        lcd_api_text_scroll_display(10, 90, 6, true, (char *)poc.talk_user_name); //滚动显示

        if (ui.page == UI_PAGE_INVITE)
        {
            if (receive_voice_old != poc.receive_voice)
            {
                receive_voice_old = poc.receive_voice;

                if (poc.receive_voice == true)
                {
                    lcd_api_bmp_with_one_color(35, 40, gImage_speaker_16x16, 16, 16, COLOR_FONT, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_bmp_with_one_color(35, 40, gImage_speaker_16x16, 16, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }

            if (im_talking_flag_old != poc.im_talking_flag)
            {
                im_talking_flag_old = poc.im_talking_flag;

                if (poc.im_talking_flag == true)
                {
                    lcd_api_bmp_with_one_color(125, 40, gImage_speaker_16x16, 16, 16, COLOR_RED, COLOR_BACKGROUND);
                }
                else
                {
                    lcd_api_bmp_with_one_color(125, 40, gImage_speaker_16x16, 16, 16, COLOR_BACKGROUND, COLOR_BACKGROUND);
                }
            }

            //display_talking_count_down(poc.im_talking_flag);
        }

        break;
    case UI_OPT_KEY_UP: //上切换项目
        break;
    case UI_OPT_KEY_DOWN: //下切换项目
        break;

    case UI_OPT_KEY_BACK: //返回上一级
#ifdef POC_ZZD
    {
        char sprintf_str[100];
        memset(sprintf_str, 0, sizeof(sprintf_str));
        sprintf(sprintf_str, AT_TX_ENTERY_GROUP, (char *)poc.my_group_id);
        module_at_tx("\r\n");
        delay_ms(30);
        module_at_tx(sprintf_str);
        delay_ms(50);
    }
#else
        delay_ms(50);
        module_at_tx(AT_TX_TYT_EXIT_INVITE_CALL);
        delay_ms(50);
#endif
        poc.can_i_speak_flag = true; //解决被呼时优先级低的机器主动退出单呼，再按PTT无显示发射的问题
        poc.im_talking_flag = false; //解决A单呼B,A在对讲，B退出单呼时，A发射没松开，还一直显示在发射状态
        led_state_set(STANDBY);
        back_to_desktop();
        break;
    case UI_OPT_PART_REFRESH: //收到指令退出单呼

        poc.can_i_speak_flag = true; //解决被呼时优先级低的机器主动退出单呼，再按PTT无显示发射的问题
        poc.im_talking_flag = false; //解决A单呼B,A在对讲，B退出单呼时，A发射没松开，还一直显示在发射状态
        led_state_set(STANDBY);
        back_to_desktop();
        break;
    default:
        break;
    }
}

//0:正常关机 1:低电关机
void ui_shutdown(u8 state)
{
    ui.page = UI_PAGE_SHUTDOWN;
    ui_clear_all();
    lcd_api_text_gbk_display(size_12, display_middle, 0, 60, (u8 *)c_Shutting_down, COLOR_FONT, COLOR_BACKGROUND);

    //关机处理流程
    set_gpio_state(GPIO_GREEN_LED, on);
    set_gpio_state(GPIO_RED_LED, on);

    module_at_tx(AT_TX_POC_LOGOUT);
    delay_ms(100);

    set_gpio_state(GPIO_MODULE_PWRKEY, off);
    delay_ms(100);

    module_at_tx(AT_TX_MODULE_POWER_OFF);
#if (THIS_VERSION == KER_VERSION)
    delay_ms(11000);
#else
    delay_ms(13000);
#endif

    led_state_set(RED_OFF);
    led_state_set(GREEN_OFF);
    set_gpio_state(GPIO_LCD_BL, off);
    ui_clear_all(); //背光熄灭还隐约看到字

    set_gpio_state(GPIO_PA_EN, off); //9
    SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
    set_gpio_state(GPIO_MODULE_PWR_EN, off);
    set_gpio_state(GPIO_GPS_EN, off);
    set_gpio_state(GPIO_USB_EN, off);  //关闭adb
    set_gpio_state(GPIO_PWR_CTL, off); //整机供电开关
    delay_ms(100);

    if (state == 0)
    {
        NVIC_SystemReset();
    }
    else
    {
        while (1)
        {
            u16 battery_adc_value = 0;

            battery_adc_value = all_adc.bat_adc_value;

            if (battery_adc_value >= 2550) // BATTERY_ADC_VALUE_LEVEL_2
            {
                NVIC_SystemReset();
            }

            poc.csq_level = poc.csq_level;
        }
    }
}

/*****************************************  通知栏图标  *******************************************************************/

void ui_ico_csq(ui_ico_change_type type)
{
    // #define ICO_X_START_CSQ 3
    // #define ICO_CSQ_Y_START 4
    static u8 csq_level_old = 0xff;

    if (poc.csq_value == 99 || poc.csq_value <= 3 || poc.csq_value >= 32) //no signal
    {
        poc.csq_level = 0;
    }
    else if (poc.csq_value > 3 && poc.csq_value <= 10)
    {
        poc.csq_level = 1;
    }
    else if (poc.csq_value > 10 && poc.csq_value <= 18)
    {
        poc.csq_level = 2;
    }
    else if (poc.csq_value > 18 && poc.csq_value <= 24)
    {
        poc.csq_level = 3;
    }
    else if (poc.csq_value > 24 && poc.csq_value < 32)
    {
        poc.csq_level = 4;
    }
    else
    {
        /* code */
    }

    if (type == ICO_REFRESH_ON_CHANGE)
    {
        if (csq_level_old != poc.csq_level)
        {
            csq_level_old = poc.csq_level;
            dispaly_csq_ico(poc.csq_level);
        }
    }
    else if (ICO_REFRESH_ALL)
    {
        dispaly_csq_ico(poc.csq_level);
    }
    else
    {
        /* code */
    }
}

//信号图标显示子函数
static void dispaly_csq_ico(u8 level)
{

    if (csq_basic_flag == false)
    {
        csq_basic_flag = true;
        lcd_api_tan(2, 4, 12, 4, 7, 9, 1, COLOR_ICO_FONT);
        lcd_api_y_line(6, 9, 6, 2, COLOR_ICO_FONT);
    }

    switch (level)
    {
    case 0:
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 0, ICO_CSQ_Y_START + 2 * 3, 5 + 2 * 0, 2, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 1, ICO_CSQ_Y_START + 2 * 2, 5 + 2 * 1, 2, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 2, ICO_CSQ_Y_START + 2 * 1, 5 + 2 * 2, 2, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 3, ICO_CSQ_Y_START + 2 * 0, 5 + 2 * 3, 2, COLOR_ICO_BACKGROUND);
        //lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_CSQ + 6 + 4, ICO_CSQ_Y_START + 3, (u8 *)"x", COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 1:
        //lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_CSQ + 6 + 4, ICO_CSQ_Y_START + 3, (u8 *)"x", COLOR_ICO_BACKGROUND, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 0, ICO_CSQ_Y_START + 2 * 3, 5 + 2 * 0, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 1, ICO_CSQ_Y_START + 2 * 2, 5 + 2 * 1, 2, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 2, ICO_CSQ_Y_START + 2 * 1, 5 + 2 * 2, 2, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 3, ICO_CSQ_Y_START + 2 * 0, 5 + 2 * 3, 2, COLOR_ICO_BACKGROUND);
        break;
    case 2:
        //lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_CSQ + 6 + 4, ICO_CSQ_Y_START + 3, (u8 *)"x", COLOR_ICO_BACKGROUND, COLOR_ICO_BACKGROUND);

        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 0, ICO_CSQ_Y_START + 2 * 3, 5 + 2 * 0, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 1, ICO_CSQ_Y_START + 2 * 2, 5 + 2 * 1, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 2, ICO_CSQ_Y_START + 2 * 1, 5 + 2 * 2, 2, COLOR_ICO_BACKGROUND);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 3, ICO_CSQ_Y_START + 2 * 0, 5 + 2 * 3, 2, COLOR_ICO_BACKGROUND);
        break;
    case 3:
        //lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_CSQ + 6 + 4, ICO_CSQ_Y_START + 3, (u8 *)"x", COLOR_ICO_BACKGROUND, COLOR_ICO_BACKGROUND);

        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 0, ICO_CSQ_Y_START + 2 * 3, 5 + 2 * 0, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 1, ICO_CSQ_Y_START + 2 * 2, 5 + 2 * 1, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 2, ICO_CSQ_Y_START + 2 * 1, 5 + 2 * 2, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 3, ICO_CSQ_Y_START + 2 * 0, 5 + 2 * 3, 2, COLOR_ICO_BACKGROUND);
        break;
    case 4:
        //lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_CSQ + 6 + 4, ICO_CSQ_Y_START + 3, (u8 *)"x", COLOR_ICO_BACKGROUND, COLOR_ICO_BACKGROUND);

        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 0, ICO_CSQ_Y_START + 2 * 3, 5 + 2 * 0, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 1, ICO_CSQ_Y_START + 2 * 2, 5 + 2 * 1, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 2, ICO_CSQ_Y_START + 2 * 1, 5 + 2 * 2, 2, COLOR_ICO_FONT);
        lcd_api_y_line(ICO_X_START_CSQ + 6 + 4 * 3, ICO_CSQ_Y_START + 2 * 0, 5 + 2 * 3, 2, COLOR_ICO_FONT);
        break;
    default:
        break;
    }
}

void ui_ico_network_type(ui_ico_change_type type)
{
    static u8 network_type_old = 0xff;

    if (type == ICO_REFRESH_ON_CHANGE)
    {
        if (network_type_old != poc.network_type)
        {
            network_type_old = poc.network_type;
            dispaly_network_type(poc.network_type);
        }
    }
    else if (ICO_REFRESH_ALL)
    {
        dispaly_network_type(poc.network_type);
    }
    else
    {
        /* code */
    }
}

//信号图标显示子函数
static void dispaly_network_type(u8 type)
{

    switch (type)
    {
    case 2:
        lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_NETWORK_TYPE, ICO_NETWORK_TYPE_Y_START, (u8 *)"2G", COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 3:
        lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_NETWORK_TYPE, ICO_NETWORK_TYPE_Y_START, (u8 *)"3G", COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    case 4:
        lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_NETWORK_TYPE, ICO_NETWORK_TYPE_Y_START, (u8 *)"4G", COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    default:
        lcd_api_text_gbk_display(size_12, display_left, ICO_X_START_NETWORK_TYPE, ICO_NETWORK_TYPE_Y_START, (u8 *)"  ", COLOR_ICO_FONT, COLOR_ICO_BACKGROUND);
        break;
    }
}

void ui_ico_miss_call(void)
{
    static u8 miss_call_state_old = 0xff;

    if (miss_call_state_old != poc.miss_call_state)
    {
        miss_call_state_old = poc.miss_call_state;

        if (poc.miss_call_state == 2)
        {
            lcd_api_bmp_with_one_color(ICO_X_START_MISS_CALL, ICO_MISS_CALL_TYPE_Y_START, gImage_miss_call_12x12, 12, 12, COLOR_RED, COLOR_ICO_BACKGROUND);
        }
        else
        {
            lcd_api_bmp_with_one_color(ICO_X_START_MISS_CALL, ICO_MISS_CALL_TYPE_Y_START, gImage_miss_call_12x12, 12, 12, COLOR_ICO_BACKGROUND, COLOR_ICO_BACKGROUND);
        }
    }
}

//计算UNICODE显示的长度
u8 cala_unicode_name_len(u8 *input_buf)
{
    static u8 len = 0;

    len = 0;
    for (u8 i = 0; i < POC_NAME_LENTH_MAX; i++)
    {
        if (input_buf[i] != 0x00)
        {
            len++;
        }
        else
        {
            if (input_buf[i - 1] == 0x00 && input_buf[i] == 0x00 && input_buf[i + 1] == 0x00)
            {
                return i;
            }
        }
    }
    return len;
}

//index: 根据item位置显示opt menu
void ui_son_menu(ui_change_type opt, u8 index)
{
#define SON_MENU_ITEM_NUM_MAX 2
    static int son_menu_current_item = 0;
    static u8 index_old = 0;

    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH:
        ui.page = UI_PAGE_SON_MENU;
        index_old = index;
        son_menu_current_item = 0;
        son_menu(OPT_CALL, index_old);
        break;
    case UI_OPT_KEY_UP:
        son_menu_current_item--;
        if (son_menu_current_item < 0)
        {
            son_menu_current_item = SON_MENU_ITEM_NUM_MAX - 1;
        }

        if (son_menu_current_item == 0)
        {
            son_menu(OPT_CALL, index_old);
        }
        else
        {
            son_menu(OPT_DELETE, index_old);
        }

        break;
    case UI_OPT_KEY_DOWN:
        son_menu_current_item++;
        if (son_menu_current_item > SON_MENU_ITEM_NUM_MAX - 1)
        {
            son_menu_current_item = 0;
        }

        if (son_menu_current_item == 0)
        {
            son_menu(OPT_CALL, index_old);
        }
        else
        {
            son_menu(OPT_DELETE, index_old);
        }
        break;
    case UI_OPT_KEY_ENTER:
        if (son_menu_current_item == 0) //呼叫
        {
            ui_on_call(UI_OPT_FIRST_REFRESH);
        }
        else //删除
        {
            if (s_contacts_or_record_flag == 1) //联系人
            {
                delete_one_contact_item(s_phone_contact_current_item);
                if (s_phone_contact_current_item != 0) //fixed bug: 在删完第一项后箭头就会消失，如果还删除会出现重启现象。
                {
                    s_phone_contact_current_item = s_phone_contact_current_item - 1;
                }

                ui_phone_contact(UI_OPT_PART_REFRESH);
            }
            else if (s_contacts_or_record_flag == 2) //呼叫记录
            {
                delete_one_call_record_item(s_call_log_current_item);
                if (s_call_log_current_item != 0) //fixed bug: 在删完第一项后箭头就会消失，如果还删除会出现重启现象。
                {
                    s_call_log_current_item = s_call_log_current_item - 1;
                }

                ui_phone_call_log(UI_OPT_PART_REFRESH);
            }
            else
            {
                /* code */
            }
        }
        break;
    case UI_OPT_KEY_BACK: //返回上一级
        if (s_contacts_or_record_flag == 1)
        {
            ui_phone_contact(UI_OPT_PART_REFRESH);
        }
        else
        {
            ui_phone_call_log(UI_OPT_PART_REFRESH);
        }

        break;
    default:
        break;
    }
}

static void son_menu(item_opt_t opt, u8 index)
{
    lcd_api_clear(100, 40 + 12 * index, COLOR_ICO_BACKGROUND, 6 * 6 + 2, 12 * 2 + 3);

    if (opt == OPT_CALL)
    {
        lcd_api_text_gbk_display(size_12, display_left, 100 + 1, 1 + 40 + 12 * index, (u8 *)c_Call, COLOR_BACKGROUND, COLOR_FONT);
        lcd_api_text_gbk_display(size_12, display_left, 100 + 1, 1 + 40 + 12 * (index + 1), (u8 *)c_Delete, COLOR_FONT, COLOR_BACKGROUND);
    }
    else
    {
        lcd_api_text_gbk_display(size_12, display_left, 100 + 1, 1 + 40 + 12 * index, (u8 *)c_Call, COLOR_FONT, COLOR_BACKGROUND);
        lcd_api_text_gbk_display(size_12, display_left, 100 + 1, 1 + 40 + 12 * (index + 1), (u8 *)c_Delete, COLOR_BACKGROUND, COLOR_FONT);
    }
}

//实际的倒计时
void at_0c_talking_count_down(void)
{
    static u16 talking_second = 0;
    u16 talk_time_max = 0;
    if (poc.im_talking_flag == true)
    {

#ifdef LANGUAGE_ENGLISH
#ifdef EC25_USA
        if (poc.flash_data.tx_time_limit == 0) //120s
        {
            talk_time_max = 120;
        }
        else
        {
            talk_time_max = 180;
        }
#else
        talk_time_max = 120;
#endif
#else
        talk_time_max = 50;
#endif

        if (talking_second > talk_time_max)
        {
            delay_ms(50);
            module_at_tx(AT_TX_0C0000);
            delay_ms(50);
            //poc.poc_0c_flag = true;
            SEGGER_RTT_printf(0, "%s %s-%d:AT_TX_0C0000%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
            poc.im_talking_flag = false;
            talking_second = 0;
        }
        else
        {
            talking_second++;
        }
    }
    else
    {
        if (talking_second != 0)
        {
            talking_second = 0;
        }
    }
}

//显示的倒计时
void display_talking_count_down(bool ptt_talk_flag)
{
    static u8 talk_status = 0;
    static u32 timer;
    static u8 xpos = 160 - 5 * 6;
    static u8 ypos = 25;

#ifdef LANGUAGE_ENGLISH
#ifdef EC25_USA
    static u16 talk_time_max = 0;
    if (poc.flash_data.tx_time_limit == 0) //120s
    {
        talk_time_max = 120;
    }
    else
    {
        talk_time_max = 180;
    }
#else
    static u16 talk_time_max = 120;
#endif
#else
    static u16 talk_time_max = 50;
#endif

    if (talk_status == 0)
    {
        if (ptt_talk_flag) //进入对讲
        {
            char first_disp_buf[5];
            memset(first_disp_buf, 0, sizeof(first_disp_buf));
            sprintf(first_disp_buf, "%ds", talk_time_max);

            lcd_api_text_gbk_display(size_12, display_left, xpos, ypos, (u8 *)first_disp_buf, COLOR_RED, COLOR_BACKGROUND);
            talk_status = 1;
            timer = SysTick_Cnt;
        }
    }
    else
    {
        if (ptt_talk_flag == false) //退出对讲
        {
            lcd_api_text_gbk_display(size_12, display_left, xpos, ypos, (u8 *)"    ", COLOR_RED, COLOR_BACKGROUND);
            talk_status = 0;
        }
        else //在对讲中要读秒
        {
            static u8 cur_sec = 0, old_sec = 0;
            cur_sec = ((SysTick_Cnt - timer) / 1000) % 255;
            if (cur_sec <= talk_time_max)
            {
                cur_sec = talk_time_max - cur_sec;

                if (old_sec != cur_sec)
                {
                    u8 disp_sec[5] = "000s";
                    old_sec = cur_sec;
                    disp_sec[0] = (cur_sec / 100) + '0';
                    disp_sec[1] = (cur_sec / 10) % 10 + '0';
                    disp_sec[2] = cur_sec % 10 + '0';
                    disp_sec[3] = 's';
                    disp_sec[4] = 0x0;
                    //2位数字的时候99-10
                    if (disp_sec[0] == '0' && disp_sec[1] != '0')
                    {
                        disp_sec[0] = disp_sec[1];
                        disp_sec[1] = disp_sec[2];
                        disp_sec[2] = 's';
                        disp_sec[3] = ' ';
                        disp_sec[4] = 0x0;
                    }

                    //1位数字的时候9-0
                    if (disp_sec[0] == '0' && disp_sec[1] == '0')
                    {
                        disp_sec[0] = disp_sec[2];
                        disp_sec[1] = 's';
                        disp_sec[2] = ' ';
                        disp_sec[3] = 0x0;
                        disp_sec[4] = 0x0;
                    }
                    lcd_api_text_gbk_display(size_12, display_left, xpos, ypos, disp_sec, COLOR_RED, COLOR_BACKGROUND);
                }
            }
        }
    }
}

void display_phone_time_count_down(bool start_end_flag)
{
    static u32 cur_sec = 0, old_sec = 0;
    static u8 talk_status = 0;
    static u32 timer;
    u8 xpos, ypos;

    xpos = 80 - 4 * 6;
    ypos = 40 - 5;

    if (talk_status == 0)
    {
        if (start_end_flag) //进入计时
        {
            lcd_api_text_gbk_display(size_12, display_left, xpos, ypos, (u8 *)"00:00:00", COLOR_RED, COLOR_BACKGROUND);
            talk_status = 1;
            timer = SysTick_Cnt;
        }
    }
    else
    {
        if (start_end_flag == false) //结束计时
        {
            lcd_api_text_gbk_display(size_12, display_left, xpos, ypos, (u8 *)"        ", COLOR_RED, COLOR_BACKGROUND);
            talk_status = 0;
            cur_sec = 0;
            old_sec = 0;
        }
        else //在对讲中要读秒
        {
            //cur_sec = ((SysTick_Cnt - timer) / 1000) % 255;
            cur_sec = ((SysTick_Cnt - timer) / 1000);

            if (old_sec != cur_sec)
            {
                u8 disp_sec[9] = "00:00:00";
                old_sec = cur_sec;
                disp_sec[0] = (cur_sec / 36000) + '0';
                disp_sec[1] = (cur_sec / 3600) % 10 + '0';
                disp_sec[2] = ':';
                disp_sec[3] = ((cur_sec % 3600) / 600) + '0';
                disp_sec[4] = (((cur_sec % 3600) / 60) % 10) + '0';
                disp_sec[5] = ':';
                disp_sec[6] = ((cur_sec % 60) / 10) + '0';
                disp_sec[7] = ((cur_sec % 60) % 10) + '0';
                disp_sec[8] = 0;

                lcd_api_text_gbk_display(size_12, display_left, xpos, ypos, disp_sec, COLOR_RED, COLOR_BACKGROUND);
            }
        }
    }
}

void timeout_back_to_desktop(void)
{
    if (ui.page >= UI_PAGE_MENU && ui.page <= UI_PAGE_SETTING_TX_TIME_LIMIT)
    {
        if (ui.page == UI_PAGE_PHONE_NEW_CONTACT ||
            ui.page == UI_PAGE_PHONE_NEW_CONTACT_SON ||
            ui.page == UI_PAGE_SETTING_FACTORY ||
            ui.page == UI_PAGE_SETTING_MODULE_UPDATE_STEP1 ||
            ui.page == UI_PAGE_SETTING_MODULE_UPDATE_STEP2 ||
            ui.page == UI_PAGE_SETTING_MCU_UPDATE_STEP1 ||
            ui.page == UI_PAGE_SETTING_MCU_UPDATE_STEP2) //UI_PAGE_PHONE_DIAL
        {
            //输入号码界面超时不返回桌面
        }
        else
        {
            back_to_desktop();
        }
    }
}

//输入单个键值（非数组）
#if 1
void send_dtmf_code(char input_value)
{
    u8 telphonenum[12] = "AT+VTS=\"\"\r\n"; //AT_TX_CALL_SEND_DTMF

    u8 *send_buf = NULL;
    u8 tmp_buf[2];

    memset(send_buf, 0, strlen((char *)send_buf));
    tmp_buf[0] = input_value;
    tmp_buf[1] = 0;

    send_buf = insert((u8 *)&telphonenum, tmp_buf, 8); //

    module_at_tx((char *)send_buf);
}
#else
void send_dtmf_code(u8 *input_buf)
{
    u8 send_buf[50];

    memset(send_buf, 0, strnlen(send_buf));
    send_buf = insert(AT_TX_CALL_SEND_DTMF, input_buf, 8);

    module_at_tx(send_buf);
}
#endif
