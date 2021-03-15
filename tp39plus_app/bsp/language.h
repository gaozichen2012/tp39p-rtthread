#ifndef _ENGLISH_TEXT_H
#define _ENGLISH_TEXT_H

#include "version.h"

//ip&password
#ifdef LANGUAGE_ENGLISH
#ifdef EC25_USA
#define USER_IP "47.254.88.91" //the United States of America
#else
#define USER_IP "47.74.211.230" //Singapore
#endif
#define USER_PASSWORD "111111"
#else
#ifdef LENOVO
#ifdef POC_ZZD
#define USER_IP "0.0.0.0"
#define USER_PASSWORD "123456"
#else
#define USER_IP "47.100.169.173"
#define USER_PASSWORD "111111"
#endif
#else
#ifdef POC_ZZD
#define USER_IP "0.0.0.0"
#define USER_PASSWORD "123456"
#else
#define USER_IP "114.141.132.4"
#define USER_PASSWORD "111111"
#endif
#endif

#endif

#ifndef LANGUAGE_ENGLISH
#define TTS_Group_selection "7FA47EC4900962E9"  //群组选择
#define TTS_Member_selection "62105458900962E9" //成员选择
#define TTS_Friend_selection "597D53CB900962E9" //好友选择
#define TTS_gps "5B9A4F4D4FE1606F"              //定位信息
#define TTS_phone "75358BDD529F80FD"            //电话功能
#define TTS_setting "672C673A8BBE7F6E"          //本机设置
#define TTS_LOCKED "5DF24E0A9501"               //已上锁
#define TTS_UNLOCKED "59D095016210529F"         //解锁成功

#define TTS_quit "900051fa"                                                    //退出
#define TTS_enter "8FDB5165"                                                   //进入
#define TTS_not_online "4E0D57287EBF"                                          //不在线
#define TTS_Please_insert_SIM_card "8bf763d25361"                              //请插卡
#define TTS_Play_calling_success "547c53eb6210529f"                            //呼叫成功
#define TTS_Exit_Single_call "900051fa4e3465f6547c53eb"                        //退出临时呼叫
#define TTS_Single_call_function_is_not_enabled "4E39547C529F80FD672A5F00542F" //单呼功能未开启
#define TTS_No_one_online "65E04EBA57287EBF002E"                               //无人在线
#define TTS_Shortcut_1_is_set_successfully "5feb6377952e00318bbe7f6e6210529f"  // 快捷键(空格)XX设置成功
#define TTS_Button_not_defined "6309952E672A5B9A4E49"                          //按键未定义
#if (THIS_VERSION == KER_VERSION)
#define TTS_welcome_to_use_4G_IP_radios "60A86B6357284F7F7528516C7F5196C67FA45BF98BB2673A" //欢迎使用公网集群对讲机
#else
#define TTS_welcome_to_use_4G_IP_radios "6B228FCE4F7F7528003457fa5BF98BB2673A" //欢迎使用4G对讲系统
#endif
#define TTS_Call_failed "547C53EB59318D25"                                            //呼叫失败
#define TTS_No_call "79816B62547C51FA"                                                // 禁止呼出
#define TTS_Incoming_call "75358BDD547C5165"                                          //电话呼入
#define TTS_Empty_list "5F53524D597D53CB521788684E3A7A7A"                             //当前好友列表为空
#define TTS_No_phone_function "56db5bc46a215f0f79816b6275358bdd529f80fd"              //4g模式禁止电话功能
#define TTS_Query_failed "67E58BE259318D25FF0C900051FA"                               //查询失败，退出
#define TTS_Wrong_password_please_re_enter "5BC6780195198BEFFF0C8BF791CD65B08F935165" //提示:密码错误，请重新输入
#define TT_No_related_numbers_found_please_re_edit "672A627E523076F8517353F77801FF0C8BF791CD65B07F168F91"
#define TTS_Duplicate_friends "91CD590D597D53CB"                              //重复好友
#define TTS_Deleting "6B63572852209664"                                       //删除成功
#define TTS_Deleted_successfully "522096646210529F"                           //删除成功
#define TTS_Set_up_successfully_please_restart "8BBE7F6E6210529F8BF791CD542F" //
#define TT_No_related_numbers_found_please_re_edit "672A627E523076F8517353F77801FF0C8BF791CD65B07F168F91"
#define TTS_Low_power "753591CF4F4EFF0C8BF751457535" //电量低
#define TTS_battery_power_20 "753591CF767E52064E4B4E8C5341"
#define TTS_battery_power_40 "753591CF767E52064E4B56DB5341"
#define TTS_battery_power_60 "753591CF767E52064E4B516D5341"
#define TTS_battery_power_80 "753591CF767E52064E4B516B5341"
#define TTS_battery_power_100 "753591CF767E52064E4B767E"

#define TTS_UNICODE_Battery "753591CF0020"               //Battery
#define TTS_UNICODE_percent "767E52064E4B0020"           //百分之
#define TTS_UNICODE_Current_Group "5F53524D7FA47EC40020" //当前群组
#define TTS_UNICODE_Current_User "5F53524D752862370020"  //当前用户

#else
#define TTS_Group_selection "Group selection"   //群组选择
#define TTS_Member_selection "Member selection" //成员选择
#define TTS_Friend_selection "Friend selection" //好友选择
#define TTS_gps "GPS"                           //定位信息
#define TTS_phone "phone"                       //电话
#define TTS_setting "settings"                  //本机设置
#define TTS_LOCKED "Locked"                     //已上锁
#define TTS_UNLOCKED "Unlocking succeeded"      //解锁成功

#define TTS_quit "quit"                                                               //退出
#define TTS_enter "enter"                                                             //进入
#define TTS_not_online "not online"                                                   //不在线
#define TTS_Please_insert_SIM_card "No SIM card"                                      //请插卡
#define TTS_Play_calling_success "Play calling success"                               //呼叫成功
#define TTS_Exit_Single_call "Exit Single call"                                       //退出临时呼叫
#define TTS_Single_call_function_is_not_enabled "Single call function is not enabled" //单呼功能未开启
#define TTS_No_one_online "No one online"                                             //无人在线

#define TTS_welcome_to_use_4G_IP_radios "welcome to use 4G IP radios, Network searching..." //欢迎使用4G对讲系统
#define TTS_Call_failed "Call failed"                                                       //呼叫失败
#define TTS_No_call "No call"                                                               // 禁止呼出
#define TTS_Incoming_call "Incoming call"                                                   //电话呼入
#define TTS_Empty_list "Empty list"                                                         //当前好友列表为空
#define TTS_No_phone_function "No phone function"                                           //4g模式禁止电话功能
#define TTS_Query_failed "Query failed"                                                     //查询失败，退出
#define TTS_Wrong_password_please_re_enter "Wrong password, please re-enter"                //提示:密码错误，请重新输入
#define TT_No_related_numbers_found_please_re_edit "No related numbers found, please re-edit"
#define TTS_Duplicate_friends "Duplicate friends"                           //重复好友
#define TTS_Deleting "Deleting"                                             //删除成功
#define TTS_Deleted_successfully "Deleted successfully"                     //删除成功
#define TTS_Shortcut_1_is_set_successfully "Shortcut 1 is set successfully" // 快捷键(空格)XX设置成功
#define TTS_Button_not_defined "Button not defined"                         //按键未定义
#define TTS_Set_up_successfully_please_restart "Set up successfully, please restart"
#define TT_No_related_numbers_found_please_re_edit "No related numbers found, please re-edit"

#define TTS_Low_power "Low power" //电量低
#define TTS_battery_power_20 "battery power 20 percent"
#define TTS_battery_power_40 "battery power 40 percent"
#define TTS_battery_power_60 "battery power 60 percent"
#define TTS_battery_power_80 "battery power 80 percent"
#define TTS_battery_power_100 "battery power 100 percent"

#define TTS_UNICODE_Battery "00420061007400740065007200790020"                           //Battery
#define TTS_UNICODE_percent "002000700065007200630065006e0074"                           //percent
#define TTS_UNICODE_Current_Group "00430075007200720065006e0074002000470072006f00750070" //当前群组
#define TTS_UNICODE_Current_User "00430075007200720065006e007400200075007300650072"      //当前用户
#endif

#ifndef LANGUAGE_ENGLISH
#define c_Menu "菜单"
#define c_Back "返回"
#define c_Okey "确定"
#define c_Switch "切换"
#define c_Cancel "取消"
#define c_Mute "静音"
#define c_Checking_module "正在查询模块... "
#define c_Checking_SIM_card "正在查询SIM卡..."
#define c_No_SIM_card "请插卡!"
#define c_Network_searching "正在搜索网络... "
#define c_Logging_in "  正在登陆...   "
#define c_Has_logged "登录成功"
#define c_Offline "离线"
#define c_Idle "空闲"
#define c_I_am_talking "本机正在说话..."
#define c_GROUP "群组"
#define c_MEMBER "成员"
#define c_FRIEND "好友"
#define c_GPS "定位"
#define c_PHONE "电话"
#define c_Setting "设置"
#define c_DIAL "快捷呼叫"
#define c_INVITE "单呼"

#define c_Contact "联系人"
#define c_New_contact "新建联系人"
#define c_Phone_dial "拨号"
#define c_Call_log "呼叫记录"

#define c_PTT_tone_setting "PTT音设置"
#define c_SIM_card_toggle "SIM卡切换"
#define c_GPS_switch "GPS开关"
#define c_USB_switch "USB开关"
#define c_Key_lock_mode "锁键模式"
#define c_Local_information "本机信息"
#define c_Backlight_setting "背光设置"
#define c_Network_setting "网络设置"
#define c_Time_zone_setting "时区设置"
#define c_Factory "恢复出厂"
#define c_Module_upgrade "模块升级"
#define c_MCU_upgrade "单片机升级"
#define c_hot_key "快捷键"
#define c_8k_encode "8k编码"
#define c_Air_code_writing "空中写码"
#define c_Status_Ready "状态：可以写码"
#define c_Status_waiting "状态：检测中"
#define c_Status_Successfully_coded "状态：写码成功"
#define c_Open_USB "已开启写码模式"
#define c_Querying1 "查询所有群组"
#define c_Querying2 "查询在线人数"
#define c_Single_call_is_disable "未打开单呼功能"
#define c_Please_return "请返回..."
#define c_Please_wait "请稍后..."
#define c_Local "本机"
#define c_Open "打开"
#define c_Close "关闭"
#define c_SIM_1 "SIM卡1优先"
#define c_SIM_2 "SIM卡2优先"
#define c_sec_10 "10秒自动"
#define c_sec_20 "20秒自动"
#define c_sec_30 "30秒自动"
#define c_Always_open "常开模式"
#define c_Manual "手动锁键"
#define c_Automatic "自动锁键"
#define c_Shutting_down "正在关机..."
#define c_Account "账号: "
#define c_Name "名称: "
#define c_Group "群组: "
#define c_Ver "版本: "
#define c_Module "模块: "
#define c_latitude "纬度: "
#define c_longitude "经度: "
#define c_satellite "有效卫星: "
#define c_gps_switch "GPS开关: "

#define c_Please_restart "请重启设备!"
#define c_Locked "*键解锁 "
#define c_Enter_password "请输入密码："
#define c_Enter_number_and_press_OK "请输入数字，然后按确认键"
#define c_Restoring "正在恢复..."
#define c_Status "当前状态: "
#define c_Downloading "  正在下载  "
#define c_Update_successed "模块升级成功"
#define c_Is_the_latest_version "已是最新版本"
#define c_Restart_and_try_again "重启后再试"
#define c_download_failed "下载失败  "

#define c_undefined "未定义"
#define c_group_switching "群组  "
#define c_member_call "成员  "
#define c_friend_call "好友  "

#define c_on_call "通话中"
#define c_Incoming_call "来电"
#define c_no_records "暂无记录"
#define c_Call " 呼叫 "
#define c_Delete " 删除 "

#define c_name "姓名"
#define c_number "号码"

#define c_Network_Automatic "自动选择"
#define c_only_4G "4G模式  "
#define c_only_3G "3G模式  "
#define c_only_2G "2G模式  "
#define c_only_23G "2/3G模式"

#define c_week_1 "星期一"
#define c_week_2 "星期二"
#define c_week_3 "星期三"
#define c_week_4 "星期四"
#define c_week_5 "星期五"
#define c_week_6 "星期六"
#define c_week_7 "星期日"
#define c_tx_time_limit "发射限时"
#define c_No_this_feature "无此功能"
#define c_Please_press_the_return_key "请按【返回】"
//old
#define c_Play_last "播放上一条"
#define c_Play_next "播放下一条"
#define c_Delete_all_files "删除所有录音"

#define c_no_sdcard "(请插SD卡)"

#define c_Press_OK_to_upgrade "升级请按【确认】"
#define c_Press_Back_to_exit "退出请按【返回】"
#define c_Note_Do_not_power_off "注意:1 中途勿断电勿断电"

#define c_NET "网络: "
#define c_Firmware "硬件: "

#define c_Group_selection "群组选择"
#define c_Member_selection "成员选择"
#define c_Friend_selection "好友选择"
#define c_Friends_List "好友列表"

#define c_No_one_online "暂时无人在线"

/* 模拟 */
#define c_account "账号: "
#define c_Switch "切换"
#define c_Month_Day "%d月%d日"
#define c_China_Mobile "中国移动"
#define c_China_Unicom "中国联通"
#define c_China_Telecom "中国电信"
#define c_unknown "未知"
#define c_VFO_channel "VFO频率："
#define c_channel "信道："
#define c_Save_VFO_channel "键盘输入修改信道!"
#define c_Saved_successfully "保存成功！"
#define c_Please_wait_saving "正在保存，请稍候"
#define c_Save_failed "保存失败！"
#define c_Enter_by_keyboard "请按键盘输入！"
#define c_Press_ok_to_save "按确认键保存！"
#else
#define c_Menu "Menu"
#define c_Back "Back"
#define c_Okey "Okey"
#define c_Switch "Switch"
#define c_Cancel "Back"
#define c_Mute "Mute"
#define c_Checking_module "Checking module"
#define c_Checking_SIM_card "Checking SIM card"
#define c_No_SIM_card "No SIM card!"
#define c_Network_searching "Network searching"
#define c_Logging_in "Logging in..."
#define c_Has_logged "Has logged"
#define c_Offline "Offline"
#define c_Idle "Idle"
#define c_I_am_talking "I'm talking..."
#define c_GROUP "GROUP"
#define c_MEMBER "MEMBER"
#define c_FRIEND "FRIEND"
#define c_GPS "GPS"
#define c_PHONE "PHONE"
#define c_Setting "SETTING"
#define c_DIAL "DIAL"
#define c_INVITE "Single call"

#define c_Contact "Contact"
#define c_New_contact "New contact"
#define c_Phone_dial "Phone dial"
#define c_Call_log "Call log"

#define c_PTT_tone_setting "PTT tone"
#define c_SIM_card_toggle "SIM card toggle"
#define c_GPS_switch "GPS switch"
#define c_USB_switch "USB switch"
#define c_Key_lock_mode "Key lock mode"
#define c_Local_information "Local INFO"
#define c_Backlight_setting "Backlight"
#define c_Network_setting "Network"
#define c_Time_zone_setting "Time zone"
#define c_Factory "Reset"
#define c_Module_upgrade "Module upgrade"
#define c_MCU_upgrade "MCU upgrade"
#define c_hot_key "Hot key"
#define c_8k_encode "8k encode"
#define c_Air_code_writing "Air code"
#define c_Status_Ready "Status: Ready"
#define c_Status_waiting "Status: waiting"
#define c_Status_Successfully_coded "Status: Successfully coded"
#define c_Open_USB "Code writing mode"

#define c_Querying1 "Querying..."
#define c_Querying2 "Querying..."
#define c_Single_call_is_disable "Single call is disable"
#define c_Please_return "Please return..."
#define c_Please_wait "Please wait..."
#define c_Local "local"
#define c_Open "Open"
#define c_Close "Close"
#define c_SIM_1 "SIM card 1 first"
#define c_SIM_2 "SIM card 2 first"
#define c_sec_10 "10 sec     "
#define c_sec_20 "20 sec     "
#define c_sec_30 "30 sec     "
#define c_Always_open "Always open"
#define c_Manual "Manual"
#define c_Automatic "Automatic"
#define c_Shutting_down "Shutting down..."
#define c_Account "Account: "
#define c_Name "Name: "
#define c_Group "Group: "
#define c_Ver "Ver: "
#define c_Module "Module: "
#define c_latitude "Lat:"
#define c_longitude "Lng:"
#define c_satellite "satellite:"
#define c_gps_switch "GPS switch:"
#define c_Please_restart "Please restart!"
#define c_Locked "Press * to unlock"
#define c_Enter_password "Enter password:"
#define c_Enter_number_and_press_OK "Enter number and press OK"

#define c_Restoring "Restoring..."
#define c_Status "Status: "
#define c_Downloading "Downloading"
#define c_Update_successed "   Update successed   "
#define c_Is_the_latest_version "Is the latest version"
#define c_Restart_and_try_again "Restart and try again"
#define c_download_failed "download failed"

#define c_undefined "undefined"
#define c_group_switching "Group "
#define c_member_call "Member"
#define c_friend_call "Friend"

#define c_on_call "calling"
#define c_Incoming_call "Incoming call"
#define c_no_records "No records"
#define c_Call " Call "
#define c_Delete "Delete"

#define c_name "name"
#define c_number "number"

#define c_Network_Automatic "Auto    "
#define c_only_4G "4G only "
#define c_only_3G "3G only "
#define c_only_2G "2G only "
#define c_only_23G "2/3G    "

#define c_week_1 " Mon. "
#define c_week_2 " Tue. "
#define c_week_3 " Wed. "
#define c_week_4 " Thu. "
#define c_week_5 " Fri. "
#define c_week_6 " Sat. "
#define c_week_7 " Sun. "
#define c_tx_time_limit "tx time limit"
#define c_No_this_feature "No this feature"
#define c_Please_press_the_return_key "Please press the return key"

//old

#define c_Play_last "Play last"
#define c_Play_next "Play next"
#define c_Delete_all_files "Delete all files"

#define c_no_sdcard "(No SD card)"

#define c_Press_OK_to_upgrade "Press OK to upgrade"
#define c_Press_Back_to_exit "Press Back to exit"
#define c_Note_Do_not_power_off "Note: Do not power off"

#define c_NET "NET: "
#define c_Firmware "Firmware: "

#define c_Group_selection "Group"
#define c_Member_selection "Member"
#define c_Friend_selection "Friend"
#define c_Friends_List "Friends List"

#define c_No_one_online "No one online"

/* 模拟 */
#define c_account "Account:"

#define c_Month_Day "%d-%d-%d"
#define c_China_Mobile "China Mobile"
#define c_China_Unicom "China Unicom"
#define c_China_Telecom "China Telecom"
#define c_unknown "unknown"
#define c_VFO_channel "VFO channel:"
#define c_channel "channel"
#define c_Save_VFO_channel "Save VFO channel?"
#define c_Saved_successfully "Saved successfully"
#define c_Please_wait_saving "Please wait, saving"
#define c_Enter_by_keyboard "Enter by keyboard!"
#define c_Press_ok_to_save "Press ok to save!"
#define c_Save_failed "Save failed!"
#endif
#endif

#define c_Engineering_mode_1 "工程模式密码"
#define c_Engineering_mode_2 "请输入密码："
#define c_Engineering_mode_3 "请输入数字，然后按确认键"
