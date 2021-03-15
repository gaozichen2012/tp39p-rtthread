
#define MIAN_PROCESS
#include "main_process.h"
#include "key.h"
#include "ui.h"
#include "poc.h"
#include "pa_led_volume.h"
#include "timed.h"
#include "tom_flash.h"
#include "language.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "typewriting.h"

static u8 s_current_key_number = 0;         //数字键短按值
static u8 s_current_long_key_number = 0xff; //数字键短按值

static void process_step_switch_up(void);
static void process_step_switch_down(void);
static void process_key_ok(void);
static void process_key_back(void);
static void process_key_up(void);
static void process_key_down(void);
static void process_key_star(void);

static void keyboard_0_to_9_process(void);
static void long_hot_key_process(void);
void main_process_init(void)
{
}

void main_process_key(void)
{
    if (all_key.key_up.state == KEY_SHORT_PRESS)
    {
        if (ui.page == UI_PAGE_BOOT_SCREEN)
        {
            /* code */
        }
        else
        {
            backlight_idle_clear();
            //成员选择
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                if (poc.im_talking_flag == false)
                {
                    if ((ui.page == UI_PAGE_USER || ui.page == UI_PAGE_FRIEND) && poc.getting_users_flag == true)
                    {
                        //解决连续按会刷出重复好友的问题
                    }
                    else
                    {
                        ui_user(UI_OPT_FIRST_REFRESH);
                    }
                }
            }
        }
        all_key.key_up.state = KEY_IDLE;
    }
    else if (all_key.key_up.state == KEY_LONG_PRESS)
    {
        if (ui.page == UI_PAGE_BOOT_SCREEN)
        {
            /* code */
        }
        else
        {
            backlight_idle_clear();
            //群组选择
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                if (ui.page == UI_PAGE_GROUP && poc.getting_group_flag == true)
                {
                    //解决连续按会刷出重复好友的问题
                }
                else
                {
                    if (poc.im_talking_flag == false) //解决林振奋需求：按发射键时，其他按键限制操作。
                    {
                        ui_group(UI_OPT_FIRST_REFRESH);
                    }
                }
            }
        }

        all_key.key_up.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.key_down.state == KEY_SHORT_PRESS)
    {
        if (ui.page == UI_PAGE_BOOT_SCREEN)
        {
            /* code */
        }
        else
        {
            backlight_idle_clear();
            //好友选择
            if (poc.has_logged_flag == false)
            {
                play_tts(2, TTS_not_online);
            }
            else
            {
                if (poc.im_talking_flag == false)
                {
                    if ((ui.page == UI_PAGE_USER || ui.page == UI_PAGE_FRIEND) && poc.getting_users_flag == true)
                    {
                        //解决连续按会刷出重复好友的问题
                    }
                    else
                    {
                        ui_friend(UI_OPT_FIRST_REFRESH);
                    }
                }
            }
        }
        all_key.key_down.state = KEY_IDLE;
    }
    else if (all_key.key_down.state == KEY_LONG_PRESS)
    {
        if (ui.page == UI_PAGE_BOOT_SCREEN && g_air_code_adb_timer_count < 1)
        {
            ui_boot_screen(UI_OPT_PART_REFRESH);
        }
        else
        {
            backlight_idle_clear();
            //播报-当前群组-本机姓名-电量
            if (poc.im_talking_flag == false)
            {
                play_local_info_tts();
            }
        }
        all_key.key_down.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.key_ptt.state == SIMPLE_KEY_PRESS)
    {
        if (ui.page == UI_PAGE_GROUP)
        {
            ui_group(UI_OPT_KEY_ENTER);
        }
#ifndef POC_ZZD
        else if (ui.page == UI_PAGE_BOOT_SCREEN && g_air_code_adb_timer_count < 1)
        {
            //ui_setting_air_code_writing(UI_OPT_FIRST_REFRESH);//解决开机过程中一直按PTT会导致白屏的问题
        }
#endif
        else if (ui.page == UI_PAGE_USER)
        {
            ui_user(UI_OPT_KEY_ENTER);
        }
        else if (ui.page == UI_PAGE_FRIEND)
        {
            ui_friend(UI_OPT_KEY_ENTER);
        }
        else if (ui.page == UI_PAGE_ON_CALL || ui.page == UI_PAGE_INCOMING_CALL || ui.page == UI_PAGE_AIR_CODE_WRITING)
        {
            backlight_idle_clear();
        }
        else
        {
            backlight_idle_clear();

            //if (poc.poc_0c_flag == false) //尝试解决未收到0c00时发射0B00，无发射的问题
            {
                module_at_tx("\r\n");
                delay_ms(100); //100ms；解决频繁收发导致模块死掉的问题（接收结束瞬间，连续发两次会导致卡死）
#ifdef POC_ZZD
                if (poc.tts_state == true)
                {
                    module_at_tx(AT_TX_QTTS0);
                    delay_ms(100);
                }
#endif
                module_at_tx(AT_TX_0B0000);
                SEGGER_RTT_printf(0, "%s %s-%d:AT_TX_0B0000%s\r\n", RTT_CTRL_BG_BRIGHT_RED, __FILE__, __LINE__, RTT_CTRL_RESET);

                if (poc.can_i_speak_flag == true)
                {
                    if (poc.flash_data.ptt_tone_toggle == false)
                    {
                        pa_on_timer(200); //-3
                        SEGGER_RTT_printf(0, "%s %s-%d:pa_on_timer(200)%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                    }
                }
            }
        }

        all_key.key_ptt.state = SIMPLE_KEY_HOLD;
    }
    else if (all_key.key_ptt.state == SIMPLE_KEY_RELEASE)
    {
        if (ui.page == UI_PAGE_BOOT_SCREEN ||
            ui.page == UI_PAGE_GROUP ||
            ui.page == UI_PAGE_USER ||
            ui.page == UI_PAGE_FRIEND ||
            ui.page == UI_PAGE_ON_CALL ||
            ui.page == UI_PAGE_INCOMING_CALL ||
            ui.page == UI_PAGE_AIR_CODE_WRITING)
        {
            /* code */
        }
        else
        {
            backlight_idle_clear();
            //module_at_tx("\r\n");
            delay_ms(400);
            module_at_tx(AT_TX_0C0000);
            poc.bug_fix_send_at_0c_flag = true;
            SEGGER_RTT_printf(0, "%s %s-%d:AT_TX_0C0000%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
        }

        all_key.key_ptt.state = SIMPLE_KEY_IDLE;
        all_key.key_ptt.state_old = SIMPLE_KEY_IDLE;
    }

    if (all_key.keyboard_ok.state == SIMPLE_KEY_RELEASE)
    {
        backlight_idle_clear();
        process_key_ok();
        all_key.keyboard_ok.state = SIMPLE_KEY_IDLE;
        all_key.keyboard_ok.state_old = SIMPLE_KEY_IDLE;
    }

    if (all_key.keyboard_back.state == SIMPLE_KEY_RELEASE)
    {
        backlight_idle_clear();
        process_key_back();
        all_key.keyboard_back.state = SIMPLE_KEY_IDLE;
        all_key.keyboard_back.state_old = SIMPLE_KEY_IDLE;
    }

    if (all_key.keyboard_up.state == SIMPLE_KEY_RELEASE)
    {
        backlight_idle_clear();
        process_key_up();
        all_key.keyboard_up.state = SIMPLE_KEY_IDLE;
        all_key.keyboard_up.state_old = SIMPLE_KEY_IDLE;
    }

    if (all_key.keyboard_down.state == SIMPLE_KEY_RELEASE)
    {
        backlight_idle_clear();
        process_key_down();
        all_key.keyboard_down.state = SIMPLE_KEY_IDLE;
        all_key.keyboard_down.state_old = SIMPLE_KEY_IDLE;
    }

    if (all_key.keyboard_well.state == SIMPLE_KEY_RELEASE)
    {
        backlight_idle_clear();
        s_current_key_number = '#';
        //process_key_star();
        all_key.keyboard_well.state = SIMPLE_KEY_IDLE;
        all_key.keyboard_well.state_old = SIMPLE_KEY_IDLE;
    }

    if (step_switch == STEP_LEFT) //up
    {
        backlight_idle_clear();
        process_step_switch_up();
        step_switch = STEP_NONE;
    }

    if (step_switch == STEP_RIGHT) //down
    {
        backlight_idle_clear();
        process_step_switch_down();
        step_switch = STEP_NONE;
    }

    keyboard_0_to_9_process();
    long_hot_key_process();
}

static void keyboard_0_to_9_process(void)
{
    if (all_key.keyboard_star.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        if (ui.page == UI_PAGE_DESKTOP)
        {
            process_key_star();
        }
        else
        {
            s_current_key_number = '*';
        }
        all_key.keyboard_star.state = KEY_IDLE;
    }
    else if (all_key.keyboard_star.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        if (ui.page == UI_PAGE_DESKTOP)
        {
            process_key_star();
        }
        all_key.keyboard_star.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_0.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '0';
        all_key.keyboard_0.state = KEY_IDLE;
    }
    else if (all_key.keyboard_0.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 0;
        all_key.keyboard_0.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_1.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '1';
        all_key.keyboard_1.state = KEY_IDLE;
    }
    else if (all_key.keyboard_1.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 1;
        all_key.keyboard_1.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_2.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '2';
        all_key.keyboard_2.state = KEY_IDLE;
    }
    else if (all_key.keyboard_2.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 2;
        all_key.keyboard_2.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_3.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '3';
        all_key.keyboard_3.state = KEY_IDLE;
    }
    else if (all_key.keyboard_3.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 3;
        all_key.keyboard_3.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_4.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '4';
        all_key.keyboard_4.state = KEY_IDLE;
    }
    else if (all_key.keyboard_4.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 4;
        all_key.keyboard_4.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_5.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '5';
        all_key.keyboard_5.state = KEY_IDLE;
    }
    else if (all_key.keyboard_5.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 5;
        all_key.keyboard_5.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_6.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '6';
        all_key.keyboard_6.state = KEY_IDLE;
    }
    else if (all_key.keyboard_6.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 6;
        all_key.keyboard_6.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_7.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '7';
        all_key.keyboard_7.state = KEY_IDLE;
    }
    else if (all_key.keyboard_7.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 7;
        all_key.keyboard_7.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_8.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '8';
        all_key.keyboard_8.state = KEY_IDLE;
    }
    else if (all_key.keyboard_8.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 8;
        all_key.keyboard_8.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }

    if (all_key.keyboard_9.state == KEY_SHORT_PRESS)
    {
        backlight_idle_clear();
        s_current_key_number = '9';
        all_key.keyboard_9.state = KEY_IDLE;
    }
    else if (all_key.keyboard_9.state == KEY_LONG_PRESS)
    {
        backlight_idle_clear();
        s_current_long_key_number = 9;
        all_key.keyboard_9.state = KEY_IDLE;
    }
    else
    {
        /* code */
    }
}
//旋转编码器-顺时针-up
static void process_step_switch_up(void)
{
    switch (ui.page)
    {
    case UI_PAGE_DESKTOP:
        if (poc.has_logged_flag == false)
        {
            play_tts(2, TTS_not_online);
        }
        else
        {
            if (ui.page == UI_PAGE_GROUP && poc.getting_group_flag == true)
            {
                //解决连续按会刷出重复好友的问题
            }
            else
            {
                if (poc.im_talking_flag == false) //解决林振奋需求：按发射键时，其他按键限制操作。
                {
                    ui_group(UI_OPT_FIRST_REFRESH);
                }
            }
        }
        break;
    case UI_PAGE_MENU: //主菜单
        ui_menu(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_GROUP: //一级菜单-群组选择
        ui_group(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_USER: //一级菜单-成员选择
        ui_user(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_FRIEND: //一级菜单-好友选择
        ui_friend(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE: //一级菜单-电话
        ui_phone(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING: //一级菜单-设置
        ui_setting(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE_CONTACT: //联系人
        break;
    case UI_PAGE_PHONE_RECORDS: //通话记录
        break;
    default:
        break;
    }
}

static void process_step_switch_down(void)
{
    switch (ui.page)
    {
    case UI_PAGE_DESKTOP:
        if (poc.has_logged_flag == false)
        {
            play_tts(2, TTS_not_online);
        }
        else
        {
            if (ui.page == UI_PAGE_GROUP && poc.getting_group_flag == true)
            {
                //解决连续按会刷出重复好友的问题
            }
            else
            {
                if (poc.im_talking_flag == false) //解决林振奋需求：按发射键时，其他按键限制操作。
                {
                    ui_group(UI_OPT_FIRST_REFRESH);
                }
            }
        }
        break;
    case UI_PAGE_MENU: //主菜单
        ui_menu(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_GROUP: //一级菜单-群组选择
        ui_group(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_USER: //一级菜单-成员选择
        ui_user(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_FRIEND: //一级菜单-好友选择
        ui_friend(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE: //一级菜单-电话
        ui_phone(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING: //一级菜单-设置
        ui_setting(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE_CONTACT: //联系人
        break;
    case UI_PAGE_PHONE_RECORDS: //通话记录
        break;
    default:
        break;
    }
}

static void process_key_ok(void)
{
    switch (ui.page)
    {
    case UI_PAGE_BOOT: //开机初始化登录页面
        ui_logging(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_DESKTOP: //默认主界面
        ui_desktop(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_MENU: //主菜单
        ui_menu(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_GROUP: //一级菜单-群组选择
        ui_group(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_USER: //一级菜单-成员选择
        ui_user(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_FRIEND: //一级菜单-好友选择
        ui_friend(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_PHONE: //一级菜单-电话
        ui_phone(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_GPS: //一级菜单-位置
        ui_gps(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING: //一级菜单-设置
        ui_setting(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_PHONE_CONTACT: //联系人
        ui_phone_contact(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT: //新建联系人
        ui_phone_new_contact(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT_SON:
        ui_phone_new_contact_son_page(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_PHONE_DIAL: //拨号
        ui_phone_dial(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_PHONE_RECORDS: //通话记录
        ui_phone_call_log(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_PTTTONE:
        ui_setting_ptttone(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_SIM_TOGGLE:
        ui_setting_sim_card_toggle(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_USB_SWITCH:
        ui_setting_usb_switch(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_LOCK_MODE:
        ui_setting_lock_mode(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_DEVICE:
        ui_setting_local_info(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_BL:
        ui_setting_backlight(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_NET:
        ui_setting_network(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_TIME_ZONE:
        ui_setting_timezone(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_FACTORY: //出厂设置
        ui_setting_factory(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP1: //模块升级步骤1-输入密码
        ui_setting_module_upgrade(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP2: //模块升级步骤2-正在升级
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP1: //MCU升级步骤1-输入密码
        ui_setting_mcu_upgrade(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP2: //MCU升级步骤2-正在升级
        break;

    case UI_PAGE_SETTING_HOTKEY: //快捷键
        break;
    case UI_PAGE_SETTING_TX_TIME_LIMIT:
        ui_setting_tx_time_limit(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SETTING_8K_ENCODE:
        ui_setting_8k_encode(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_DIAL: //拨号
        break;
    case UI_PAGE_INVITE: //临时组
        break;
    case UI_PAGE_ON_CALL:
        ui_on_call(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_INCOMING_CALL:
        ui_incoming_call(UI_OPT_KEY_ENTER);
        break;
    case UI_PAGE_SON_MENU:
        ui_son_menu(UI_OPT_KEY_ENTER, 0);
        break;
    default:
        break;
    }
}

static void process_key_back(void)
{
    switch (ui.page)
    {
    case UI_PAGE_BOOT: //开机初始化登录页面
        ui_logging(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_DESKTOP: //默认主界面
        ui_desktop(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_MENU: //主菜单
        ui_menu(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_GROUP: //一级菜单-群组选择
        ui_group(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_USER: //一级菜单-成员选择
        ui_user(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_FRIEND: //一级菜单-好友选择
        ui_friend(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_PHONE: //一级菜单-电话
        ui_phone(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_GPS: //一级菜单-位置
        ui_gps(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING: //一级菜单-设置
        ui_setting(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_PHONE_CONTACT: //联系人
        ui_phone_contact(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT: //新建联系人
        ui_phone_new_contact(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT_SON:
        ui_phone_new_contact_son_page(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_PHONE_DIAL: //拨号
        ui_phone_dial(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_PHONE_RECORDS: //通话记录
        ui_phone_call_log(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_PTTTONE:
        ui_setting_ptttone(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_SIM_TOGGLE: //SIM卡切换
        ui_setting_sim_card_toggle(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_USB_SWITCH:
        ui_setting_usb_switch(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_LOCK_MODE:
        ui_setting_lock_mode(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_DEVICE:
        ui_setting_local_info(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_BL:
        ui_setting_backlight(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_NET:
        ui_setting_network(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_TIME_ZONE:
        ui_setting_timezone(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_FACTORY: //出厂设置
        ui_setting_factory(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP1: //模块升级步骤1-输入密码
        ui_setting_module_upgrade(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP2: //模块升级步骤2-正在升级
        ui_setting_module_upgrade(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP1: //MCU升级步骤1-输入密码
        ui_setting_mcu_upgrade(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP2: //MCU升级步骤2-正在升级
        ui_setting_mcu_upgrade(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_HOTKEY: //快捷键
        ui_setting_hotkey(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_TX_TIME_LIMIT:
        ui_setting_tx_time_limit(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SETTING_8K_ENCODE:
        ui_setting_8k_encode(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_AIR_CODE_WRITING:
        ui_setting_air_code_writing(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_DIAL: //拨号
        break;
    case UI_PAGE_INVITE: //临时组
        ui_invite(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_ON_CALL:
        ui_on_call(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_INCOMING_CALL:
        ui_incoming_call(UI_OPT_KEY_BACK);
        break;
    case UI_PAGE_SON_MENU:
        ui_son_menu(UI_OPT_KEY_BACK, 0);
        break;
    default:
        break;
    }
}

static void process_key_up(void)
{
    switch (ui.page)
    {
    case UI_PAGE_BOOT: //开机初始化登录页面
        ui_logging(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_DESKTOP: //默认主界面
        ui_desktop(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_MENU: //主菜单
        ui_menu(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_GROUP: //一级菜单-群组选择
        ui_group(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_USER: //一级菜单-成员选择
        ui_user(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_FRIEND: //一级菜单-好友选择
        ui_friend(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE: //一级菜单-电话
        ui_phone(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_GPS: //一级菜单-位置
        ui_gps(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING: //一级菜单-设置
        ui_setting(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE_CONTACT: //联系人
        ui_phone_contact(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT: //新建联系人
        ui_phone_new_contact(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT_SON:
        ui_phone_new_contact_son_page(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE_DIAL: //拨号
        ui_phone_dial(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_PHONE_RECORDS: //通话记录
        ui_phone_call_log(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_PTTTONE:
        break;
    case UI_PAGE_SETTING_SIM_TOGGLE: //SIM卡切换
        ui_setting_sim_card_toggle(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_GPS_SWITCH:
        break;
    case UI_PAGE_SETTING_USB_SWITCH:
        break;
    case UI_PAGE_SETTING_LOCK_MODE:
        ui_setting_lock_mode(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_DEVICE:
        ui_setting_local_info(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_BL:
        ui_setting_backlight(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_NET:
        ui_setting_network(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_TIME_ZONE:
        ui_setting_timezone(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_FACTORY: //出厂设置
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP1: //模块升级步骤1-输入密码
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP2: //模块升级步骤2-正在升级
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP1: //MCU升级步骤1-输入密码
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP2: //MCU升级步骤2-正在升级
        break;
    case UI_PAGE_SETTING_HOTKEY: //快捷键
        ui_setting_hotkey(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_TX_TIME_LIMIT:
        ui_setting_tx_time_limit(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_SETTING_8K_ENCODE:
        ui_setting_8k_encode(UI_OPT_KEY_UP);
        break;
    case UI_PAGE_DIAL: //拨号
        break;
    case UI_PAGE_INVITE: //临时组
        break;
    case UI_PAGE_SON_MENU:
        ui_son_menu(UI_OPT_KEY_UP, 0);
        break;
    default:
        break;
    }
}

static void process_key_down(void)
{
    switch (ui.page)
    {
    case UI_PAGE_BOOT: //开机初始化登录页面
        ui_logging(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_DESKTOP: //默认主界面
        ui_desktop(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_MENU: //主菜单
        ui_menu(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_GROUP: //一级菜单-群组选择
        ui_group(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_USER: //一级菜单-成员选择
        ui_user(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_FRIEND: //一级菜单-好友选择
        ui_friend(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE: //一级菜单-电话
        ui_phone(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_GPS: //一级菜单-位置
        ui_gps(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING: //一级菜单-设置
        ui_setting(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE_CONTACT: //联系人
        ui_phone_contact(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT: //新建联系人
        ui_phone_new_contact(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE_NEW_CONTACT_SON:
        ui_phone_new_contact_son_page(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE_DIAL: //拨号
        ui_phone_dial(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_PHONE_RECORDS: //通话记录
        ui_phone_call_log(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_PTTTONE:
        break;
    case UI_PAGE_SETTING_SIM_TOGGLE: //SIM卡切换
        ui_setting_sim_card_toggle(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_GPS_SWITCH:
        break;
    case UI_PAGE_SETTING_USB_SWITCH:
        break;
    case UI_PAGE_SETTING_LOCK_MODE:
        ui_setting_lock_mode(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_DEVICE:
        ui_setting_local_info(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_BL:
        ui_setting_backlight(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_NET:
        ui_setting_network(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_TIME_ZONE:
        ui_setting_timezone(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_FACTORY: //出厂设置
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP1: //模块升级步骤1-输入密码
        break;
    case UI_PAGE_SETTING_MODULE_UPDATE_STEP2: //模块升级步骤2-正在升级
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP1: //MCU升级步骤1-输入密码
        break;
    case UI_PAGE_SETTING_MCU_UPDATE_STEP2: //MCU升级步骤2-正在升级
        break;
    case UI_PAGE_SETTING_HOTKEY: //快捷键
        ui_setting_hotkey(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_TX_TIME_LIMIT:
        ui_setting_tx_time_limit(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_SETTING_8K_ENCODE:
        ui_setting_8k_encode(UI_OPT_KEY_DOWN);
        break;
    case UI_PAGE_DIAL: //拨号
        break;
    case UI_PAGE_INVITE: //临时组
        break;
    case UI_PAGE_SON_MENU:
        ui_son_menu(UI_OPT_KEY_DOWN, 0);
        break;
    default:
        break;
    }
}

static void process_key_star(void)
{
    switch (ui.page)
    {
    case UI_PAGE_DESKTOP: //默认主界面
        ui_desktop(UI_OPT_OTHER_REFRESH);
        break;
    default:
        break;
    }
}

u8 get_keyboard_number(void)
{
    if (poc.im_talking_flag == true) //解决林振奋需求：按发射键时，其他按键限制操作。
    {
        s_current_key_number = 0;
    }

    return s_current_key_number;
}

void keyboard_number_clear(void)
{
    s_current_key_number = 0;
}

u8 get_long_keyboard_number(void)
{
    if (poc.im_talking_flag == true) //解决林振奋需求：按发射键时，其他按键限制操作。
    {
        s_current_long_key_number = 0xff;
    }

    return s_current_long_key_number;
}

static void long_hot_key_process(void)
{
    char tts_buf[100];

    //当获取到长键值，存储相关信息并保存至flash
    if (get_long_keyboard_number() != 0xff)
    {
        memset(tts_buf, 0, sizeof(tts_buf));
        memcpy(tts_buf, TTS_Shortcut_1_is_set_successfully, sizeof(TTS_Shortcut_1_is_set_successfully));
#ifdef LANGUAGE_ENGLISH
        tts_buf[9] = get_long_keyboard_number() + 0x30;
#else
        tts_buf[15] = get_long_keyboard_number() + 0x30;
#endif

        read_flash_hotkey_data();
        if (ui.page == UI_PAGE_GROUP)
        {
            if (poc.group_current_item != 0xFFFF)
            {
                poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_type = 1;
                memset(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_id, 0, sizeof(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_id));
                memset(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_name, 0, sizeof(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_name));
                memcpy(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_id, poc.group_list_id[poc.group_current_item], 8);
                memcpy(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_name, poc.group_list_name[poc.group_current_item], POC_NAME_LENTH_MAX);

                play_tts(2, tts_buf);
                save_flash_hotkey_data();
            }
        }
        else if (ui.page == UI_PAGE_USER || ui.page == UI_PAGE_FRIEND)
        {
            if (poc.flash_data.invite_enable_toggle != 0 && poc.user_current_item != 0xFFFF) //单呼功能开启
            {
                if (ui.page == UI_PAGE_USER)
                {
                    poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_type = 2;
                }
                else
                {
                    poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_type = 3;
                }

                memset(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_id, 0, sizeof(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_id));
                memset(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_name, 0, sizeof(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_name));
                memcpy(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_id, poc.user_list_id[poc.user_current_item], 8);
                memcpy(poc.hotkey_info[get_long_keyboard_number() % 10].hotkey_name, poc.user_list_name[poc.user_current_item], POC_NAME_LENTH_MAX);

                play_tts(2, tts_buf);
                save_flash_hotkey_data();
            }
        }
        else if (ui.page == UI_PAGE_DESKTOP && poc.lock_flag == false) //主界面按快捷键操作
        {
            char sprintf_str[100];
            char sprintf_str_1[100];

            switch (poc.hotkey_info[get_long_keyboard_number()].hotkey_type)
            {
            case 0: //undefined
                play_tts(2, TTS_Button_not_defined);
                break;
            case 1: //group
                memset(sprintf_str, 0, sizeof(sprintf_str));
                sprintf(sprintf_str, AT_TX_ENTERY_GROUP, (char *)poc.hotkey_info[get_long_keyboard_number()].hotkey_id);
                module_at_tx("\r\n");
                delay_ms(30);
                module_at_tx(sprintf_str);
                break;
            case 2: //user
            case 3: //friend
                if (poc.flash_data.invite_enable_toggle == 0)
                {
                    play_tts(2, TTS_Single_call_function_is_not_enabled);
                }
                else
                {
                    memset(sprintf_str_1, 0, sizeof(sprintf_str_1));
                    sprintf(sprintf_str_1, AT_TX_CALL_USER, (char *)poc.hotkey_info[get_long_keyboard_number()].hotkey_id);
                    module_at_tx("\r\n");
                    delay_ms(30);
                    module_at_tx(sprintf_str_1);

                    //cpy invite user name
                    memset(poc.talk_user_name, 0, sizeof(poc.talk_user_name)); //4
                    memcpy(poc.talk_user_name, poc.hotkey_info[get_long_keyboard_number()].hotkey_name, sizeof(poc.talk_user_name));
                    ui_invite(UI_OPT_FIRST_REFRESH);
                }

                break;
            default:
                break;
            }
        }
        else
        {
            /* code */
        }
        s_current_long_key_number = 0xff;
    }
}
