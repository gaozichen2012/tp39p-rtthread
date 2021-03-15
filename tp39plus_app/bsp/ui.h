#ifndef __UI_H
#define __UI_H

#include "tom_data.h"
#include "version.h"
#define Y_MAX 128
#define X_MAX 160
#define X_PADDING 5
#define Y_NOTIFICATION_SPACE 16
#define Y_TITLE_SPACE 16
#define Y_BOTTOM_SPACE 14

/* ICO位置 */
#define ICO_X_START_CSQ 3
#define ICO_X_START_NETWORK_TYPE 26
#define ICO_X_START_SIM_CARD_TYPE (ICO_X_START_NETWORK_TYPE + 12)
#define ICO_X_START_HEADPHONE (ICO_X_START_SIM_CARD_TYPE + 16)
#define ICO_X_START_TIME (X_MAX - 20 - 20 - 12 - 12 - 30)
#define ICO_X_START_MISS_CALL (X_MAX - 20 - 20 - 12 - 12)
#define ICO_X_START_GPS (X_MAX - 20 - 20 - 12)
#define ICO_X_START_VOLUME_LEVEL (X_MAX - 20 - 20)
#define ICO_X_START_BATTARY (X_MAX - 20)

#define ICO_CSQ_Y_START 4
#define ICO_NETWORK_TYPE_Y_START 3
#define ICO_MISS_CALL_TYPE_Y_START 2
#define ICO_GPS_Y_START 2
typedef enum
{
    UI_PAGE_BOOT_SCREEN, //开机界面
    UI_PAGE_BOOT,        //开机初始化登录页面
    UI_PAGE_DESKTOP,     //默认主界面

    UI_PAGE_MENU,    //主菜单
    UI_PAGE_GROUP,   //一级菜单-群组选择
    UI_PAGE_USER,    //一级菜单-成员选择
    UI_PAGE_FRIEND,  //一级菜单-好友选择
    UI_PAGE_PHONE,   //一级菜单-电话
    UI_PAGE_GPS,     //一级菜单-位置
    UI_PAGE_SETTING, //一级菜单-设置

    //电话二级菜单
    UI_PAGE_PHONE_CONTACT,     //联系人
    UI_PAGE_PHONE_NEW_CONTACT, //新建联系人
    UI_PAGE_PHONE_NEW_CONTACT_SON,
    UI_PAGE_PHONE_DIAL,    //拨号
    UI_PAGE_PHONE_RECORDS, //通话记录

    //设置二级菜单
    UI_PAGE_SETTING_PTTTONE,
    UI_PAGE_SETTING_SIM_TOGGLE,
    UI_PAGE_SETTING_GPS_SWITCH,
    UI_PAGE_SETTING_USB_SWITCH,
    UI_PAGE_SETTING_LOCK_MODE,
    UI_PAGE_SETTING_DEVICE,
    UI_PAGE_SETTING_BL,
    UI_PAGE_SETTING_NET,
    UI_PAGE_SETTING_TIME_ZONE,
    UI_PAGE_SETTING_FACTORY,             //出厂设置
    UI_PAGE_SETTING_MODULE_UPDATE_STEP1, //模块升级步骤1-输入密码
    UI_PAGE_SETTING_MODULE_UPDATE_STEP2, //模块升级步骤2-正在升级
    UI_PAGE_SETTING_MCU_UPDATE_STEP1,    //MCU升级步骤1-输入密码
    UI_PAGE_SETTING_MCU_UPDATE_STEP2,    //MCU升级步骤2-正在升级
    UI_PAGE_SETTING_HOTKEY,              //快捷键
    UI_PAGE_SETTING_TX_TIME_LIMIT,
    UI_PAGE_SETTING_8K_ENCODE,

    //特殊界面
    UI_PAGE_SPECIAL_START,    //特殊界面开始
    UI_PAGE_AIR_CODE_WRITING, //空中写码
    UI_PAGE_DIAL,             //拨号
    UI_PAGE_ON_CALL,          //通话界面（主呼和接听公用）
    UI_PAGE_INCOMING_CALL,    //被呼界面
    UI_PAGE_INVITE,           //临时组
    UI_PAGE_SHUTDOWN,         //关机
    UI_PAGE_SON_MENU,
    UI_PAGE_SPECIAL_END, //特殊界面结束

} ui_page_t;

typedef enum
{
    SETTING_PTTTONE,
    SETTING_SIM_CARD,
    SETTING_USB,
    SETTING_LOCK_MODE,
    SETTING_ABOUT,
    SETTING_BACKLIGHT,
    SETTING_NETWORK,
    SETTING_FACTORY,
    SETTING_MODULE_UPGRADE,
    SETTING_MCU_UPGRADE,
    SETTING_HOTKEY,
#ifdef LANGUAGE_ENGLISH
    SETTING_TIMEZONE,
#endif

#ifdef POC_ZZD
    SETTING_8KCODE,
#endif

#if defined(LANGUAGE_ENGLISH) && defined(EC25_USA)
    SETTING_TOT,
#endif
} setting_type;

/* 屏幕刷新机制 */
typedef enum
{
    ICO_REFRESH_ALL,       //图标全部强制刷新
    ICO_REFRESH_ON_CHANGE, //实时刷新-图标值改变时刷新
} ui_ico_change_type;

/* 屏幕刷新机制 */
typedef enum
{
    UI_OPT_FIRST_REFRESH, //第一次进入全局刷新
    UI_OPT_KEY_UP,        //上切换项目
    UI_OPT_KEY_DOWN,      //下切换项目
    UI_OPT_KEY_ENTER,     //确认项目
    UI_OPT_KEY_BACK,      //返回上一级
    UI_OPT_KEY_KEYBOARD,  //数字键盘输入（实时）
    UI_OPT_PART_REFRESH,  //其他部分刷新操作
    UI_OPT_OTHER_REFRESH, //特殊刷新
} ui_change_type;

typedef enum
{
    BUTTOM_OK_BACK,
    BUTTOM_BACK,
    BUTTOM_MENU,
    BUTTOM_SWITCH_BACK,
    BUTTOM_LOCKED,
    BUTTOM_MUTE_BACK,
    BUTTOM_RED_MUTE_BACK
} bottom_type;

typedef struct
{
    ui_page_t page;

} ui_t;

#ifdef UI
#define UI_EXTERN
#else
#define UI_EXTERN extern
#endif

UI_EXTERN bool g_air_code_account_update_flag;
UI_EXTERN bool g_call_state; //0：不在通话状态 1：正在通话

UI_EXTERN ui_t ui;

UI_EXTERN void ui_clear(void);
UI_EXTERN void ui_clear_all(void);
UI_EXTERN void ui_show_title(u8 *title_name);
UI_EXTERN void ui_init(void);
UI_EXTERN void back_to_desktop(void);

UI_EXTERN void ui_boot_screen(ui_change_type opt);
UI_EXTERN void ui_logging(ui_change_type opt);
UI_EXTERN void ui_desktop(ui_change_type opt);
UI_EXTERN void ui_menu(ui_change_type opt);
UI_EXTERN void ui_group(ui_change_type opt);
UI_EXTERN void ui_user(ui_change_type opt);
UI_EXTERN void ui_friend(ui_change_type opt);
UI_EXTERN void ui_gps(ui_change_type opt);
UI_EXTERN void ui_phone(ui_change_type opt);
UI_EXTERN void ui_phone_contact(ui_change_type opt);
//UI_EXTERN void ui_phone_new_contact(ui_change_type opt);
UI_EXTERN void ui_phone_dial(ui_change_type opt);
UI_EXTERN void ui_phone_call_log(ui_change_type opt);

UI_EXTERN void ui_setting(ui_change_type opt);
UI_EXTERN void ui_setting_ptttone(ui_change_type opt);
UI_EXTERN void ui_setting_sim_card_toggle(ui_change_type opt);
UI_EXTERN void ui_setting_usb_switch(ui_change_type opt);
UI_EXTERN void ui_setting_lock_mode(ui_change_type opt);
UI_EXTERN void ui_setting_local_info(ui_change_type opt);
UI_EXTERN void ui_setting_backlight(ui_change_type opt);
UI_EXTERN void ui_setting_network(ui_change_type opt);
UI_EXTERN void ui_setting_timezone(ui_change_type opt);
UI_EXTERN void ui_setting_factory(ui_change_type opt);
UI_EXTERN void ui_setting_module_upgrade(ui_change_type opt);
UI_EXTERN void ui_setting_mcu_upgrade(ui_change_type opt);
UI_EXTERN void ui_setting_hotkey(ui_change_type opt);
UI_EXTERN void ui_setting_tx_time_limit(ui_change_type opt);
UI_EXTERN void ui_setting_8k_encode(ui_change_type opt);
UI_EXTERN void ui_setting_air_code_writing(ui_change_type opt);

UI_EXTERN void ui_dial(ui_change_type opt);
UI_EXTERN void ui_on_call(ui_change_type opt);
UI_EXTERN void ui_incoming_call(ui_change_type opt);
UI_EXTERN void ui_invite(ui_change_type opt);
UI_EXTERN void ui_shutdown(u8 state);
UI_EXTERN void ui_son_menu(ui_change_type opt, u8 index);
UI_EXTERN void ui_show_bottom_button(bottom_type buttom);

UI_EXTERN void ui_ico_csq(ui_ico_change_type type);
UI_EXTERN void ui_ico_network_type(ui_ico_change_type type);
UI_EXTERN void ui_ico_miss_call(void);
UI_EXTERN u8 cala_unicode_name_len(u8 *input_buf);
UI_EXTERN void display_talking_count_down(bool ptt_talk_flag);
UI_EXTERN void display_phone_time_count_down(bool start_end_flag);
UI_EXTERN void timeout_back_to_desktop(void);
UI_EXTERN void send_dtmf_code(char input_value);
UI_EXTERN void at_0c_talking_count_down(void);
#endif
