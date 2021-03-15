#ifndef __POC_H
#define __POC_H

#include "tom_interface.h"
#include "ec20_at_cmd.h"
#include "stm32f0xx_rtc.h"
#include "tom_flash.h"
#include "telephone.h"

#include "SEGGER_RTT.h"

#ifdef LANGUAGE_ENGLISH
#define POC_NAME_LENTH_MAX (12 * 2) //英文最长13个字符
#else
#define POC_NAME_LENTH_MAX (9 * 2) //中文最长9个字符
#endif

#define ORIGINAL_NAME_LENTH_MAX (POC_NAME_LENTH_MAX * 2)
#define GROUP_NUM_MAX (150)
#define USER_NUM_MAX (150)

/* SIM自动检测 */
typedef enum
{
    FIRST_CHECK_SIM_CARD,
    FIRST_NO_SIM_CARD,
    SECOND_CHECK_SIM_CARD,
    SECOND_NO_SIM_CARD,
    HAVE_SIM_CARD_1,
    HAVE_SIM_CARD_2,
} sim_card_state_t;

/* 屏幕刷新机制 */
typedef enum
{
    TTS_GROUP,
    TTS_USER,
} tts_group_or_user_t;

typedef struct
{
    //1字节快捷键定义类型 +  9字节ID序号 + 48字节name = 58
    u8 hotkey_type; // 0未定义 1群组 2成员 3好友
    u8 hotkey_id[8 + 1];
    u8 hotkey_name[48];
} one_hotkey_flash_t;

typedef struct
{
    u32 boot_flag;      //此标志为了让boot程序识别，进入update模式，boot_flag=0x88888888则进入update
    u8 ver[32];         //版本
    u8 sim_card_select; //0:SIM1 1:SIM2
} boot_flash_t;

typedef struct
{
    //account info
    u8 server_ip[17];
    u8 user_id[33];
    u8 user_pwd[33];
    u8 dealers_pwd[33]; //经销商密码
    u8 gps_enable_flag;
    u8 invite_enable_toggle;
    u8 apn_name[33];
    u8 apn_pwd[33];

    //settings
    u8 ptt_tone_toggle; //0:on 1:off
    u8 sim_card_toggle;
    u8 key_lock_toggle;
    u8 backlight_flag;
    u8 network_flag;
    u8 time_zone_flag;
    u8 tx_time_limit;
    u8 encode_8k_toggle;
    //other

} poc_flash_t;

typedef struct
{
    //uart_rx_t module_data;
    device_uart_t module_data;

    poc_flash_t flash_data;
    one_hotkey_flash_t hotkey_info[10];
    boot_flash_t boot_info;

    u8 got_time_flag;
    RTC_TimeTypeDef cur_time; //时间
    RTC_DateTypeDef data;     //日期

    //登录中的标志位
    bool module_ready_flag;
    bool module_init_flag; //设置模块增益等

    u8 working_sim_card; //当前检测和工作的SIM卡 0：SIM卡1  1：SIM卡2
    sim_card_state_t sim_card_flag;

    u8 get_meid_flag;          //0：默认 1：正在查询 2：已获取
    u8 get_imsi_sim_card_type; //0未检测 1移动 2联通 3电信
    bool get_module_ver_flag;
    bool get_poc_ver_flag;
    bool get_account_flag;
    u8 module_ver[40];
    u8 poc_ver[40];
    u8 meid_number[40]; //
    u8 imsi_number[40]; //
    u8 csq_value;       //csq具体的值
    u8 csq_level;       //csq等级
    u8 network_type;    //2:2G 3:3G 4:4G 其他：无网络
    bool has_logged_flag;
    bool logged_before_flag; //0未登录过 1登录过 （用于识别掉线状态，离线后登陆不发TTS）
    u8 offline_test_flag;    //测试尝试解决zzd异常掉线问题

    //其他标志位
    u8 usb_toggle;
    bool enter_air_code_flag; //进入写码模式 收到85
    u8 backlight_flash_time;  //实际息屏的时间（与flash中backlight_flag一一对应）
    bool lock_flag;
    u32 standby_idle_count; //待机空闲时间计时（控制息屏）
    u32 invite_idle_count;  //单呼空闲时间计时（卓智达专用）
    u8 poc_upgrade_percent;
    u8 poc_upgrade_state;
    u8 miss_call_state;      //0:默认状态 1:收到来电 2:未接来电
    one_contact_item_t dial; //拨号界面使用，拨号联系人呼叫记录从此获取姓名号码
    bool ptt_release_flag;   //释放ptt加500s延迟
    clock_type system_clock_flag;
    bool backlight_state; //0：灭 1：亮
    //bool poc_0c_flag;     //false:默认状态，收到0c00 true：未收到0c00  （尝试解决未收到0c00时发射0B00，无发射的问题）
    bool bug_fix_send_at_0c_flag; //发送at标志位 1：发送AT  0:默认状态
    u8 volume_level;
    onoff_t headphone_state;

    bool tts_state; //1正在播放tts （用于解决卓智达播放tts时按PTT，TTS会传到对讲中）
    bool im_talking_flag;
    bool can_i_speak_flag; //1：本机可以说话 0：本机禁止说话  默认1
    bool receive_voice;    //收到语音
    u8 talk_user_name[POC_NAME_LENTH_MAX + 1];
    u8 talk_user_id[8 + 1];

    //群组用户名
    u8 my_id[8 + 1];                    //我的用户id
    u8 my_name[POC_NAME_LENTH_MAX + 1]; //我的用户
    u8 my_group_index[4 + 1];           //我的组索引（卓智达获取在线成员时会用到）
    u8 my_group_id[8 + 1];
    u8 my_group_name[POC_NAME_LENTH_MAX + 1];

    u16 group_index; //群组总数累加获取（正常不需要，此处为了规避有些群组未刷出来的问题）
    u16 total_number_of_groups;
    u8 group_list_index[GROUP_NUM_MAX][4 + 1];
    u8 group_list_id[GROUP_NUM_MAX][8 + 1];
    u8 group_list_name[GROUP_NUM_MAX][POC_NAME_LENTH_MAX + 1];

    bool getting_group_flag; //false:不在获取列表 true：正在获取列表
    bool getting_users_flag; //false:不在获取列表 true：正在获取列表
    u16 total_number_of_users;
    u16 user_index; //计数器累加获取
    u8 user_list_index[USER_NUM_MAX][4 + 1];
    u8 user_list_id[USER_NUM_MAX][8 + 1];
    u8 user_list_name[USER_NUM_MAX][POC_NAME_LENTH_MAX + 1];

    int group_current_item; //群组列表当前选中的选项(快捷键会用到)
    int user_current_item;
} poc_t;

#ifdef POC_DEFINE
#define POC_EXTERN
#else
#define POC_EXTERN extern
#endif

POC_EXTERN bool g_bug_fix_first_tts_play_flag;

POC_EXTERN poc_t poc;
POC_EXTERN void main_init(u8 flg);

POC_EXTERN void poc_init(void);
POC_EXTERN void poc_first_deal(void);
POC_EXTERN void poc_process(u8 *ptr);
POC_EXTERN void poc_set_account(u8 *ip, u8 *name, u8 *pwd, u8 gps_flag, u8 inv_flag);
POC_EXTERN void poc_set_net_mode(u8 mode);

POC_EXTERN void play_tts(u8 tts_type, char *tts_data);
POC_EXTERN void play_tts_with_2Param(u8 tts_type, char *tts_data1, char *tts_data2);
POC_EXTERN void play_tts_group_and_user(tts_group_or_user_t type, u16 group_index);

POC_EXTERN void poc_factory_reset(void);
POC_EXTERN void play_local_info_tts(void);
POC_EXTERN void play_unicode_tts(u8 *input_buf);
POC_EXTERN void poc_set_time_zone(u8 num);
POC_EXTERN void headphone_control_process(void);
POC_EXTERN void bug_fix_ptt_end_with_0c00(void);

//zzd专用函数
POC_EXTERN void poc_set_ptt_tone(void);
POC_EXTERN void zzd_poc_set_8k_encode(void);
#endif
