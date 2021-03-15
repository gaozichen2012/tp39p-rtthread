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
#define TTS_Group_selection "7FA47EC4900962E9"  //Ⱥ��ѡ��
#define TTS_Member_selection "62105458900962E9" //��Աѡ��
#define TTS_Friend_selection "597D53CB900962E9" //����ѡ��
#define TTS_gps "5B9A4F4D4FE1606F"              //��λ��Ϣ
#define TTS_phone "75358BDD529F80FD"            //�绰����
#define TTS_setting "672C673A8BBE7F6E"          //��������
#define TTS_LOCKED "5DF24E0A9501"               //������
#define TTS_UNLOCKED "59D095016210529F"         //�����ɹ�

#define TTS_quit "900051fa"                                                    //�˳�
#define TTS_enter "8FDB5165"                                                   //����
#define TTS_not_online "4E0D57287EBF"                                          //������
#define TTS_Please_insert_SIM_card "8bf763d25361"                              //��忨
#define TTS_Play_calling_success "547c53eb6210529f"                            //���гɹ�
#define TTS_Exit_Single_call "900051fa4e3465f6547c53eb"                        //�˳���ʱ����
#define TTS_Single_call_function_is_not_enabled "4E39547C529F80FD672A5F00542F" //��������δ����
#define TTS_No_one_online "65E04EBA57287EBF002E"                               //��������
#define TTS_Shortcut_1_is_set_successfully "5feb6377952e00318bbe7f6e6210529f"  // ��ݼ�(�ո�)XX���óɹ�
#define TTS_Button_not_defined "6309952E672A5B9A4E49"                          //����δ����
#if (THIS_VERSION == KER_VERSION)
#define TTS_welcome_to_use_4G_IP_radios "60A86B6357284F7F7528516C7F5196C67FA45BF98BB2673A" //��ӭʹ�ù�����Ⱥ�Խ���
#else
#define TTS_welcome_to_use_4G_IP_radios "6B228FCE4F7F7528003457fa5BF98BB2673A" //��ӭʹ��4G�Խ�ϵͳ
#endif
#define TTS_Call_failed "547C53EB59318D25"                                            //����ʧ��
#define TTS_No_call "79816B62547C51FA"                                                // ��ֹ����
#define TTS_Incoming_call "75358BDD547C5165"                                          //�绰����
#define TTS_Empty_list "5F53524D597D53CB521788684E3A7A7A"                             //��ǰ�����б�Ϊ��
#define TTS_No_phone_function "56db5bc46a215f0f79816b6275358bdd529f80fd"              //4gģʽ��ֹ�绰����
#define TTS_Query_failed "67E58BE259318D25FF0C900051FA"                               //��ѯʧ�ܣ��˳�
#define TTS_Wrong_password_please_re_enter "5BC6780195198BEFFF0C8BF791CD65B08F935165" //��ʾ:�����������������
#define TT_No_related_numbers_found_please_re_edit "672A627E523076F8517353F77801FF0C8BF791CD65B07F168F91"
#define TTS_Duplicate_friends "91CD590D597D53CB"                              //�ظ�����
#define TTS_Deleting "6B63572852209664"                                       //ɾ���ɹ�
#define TTS_Deleted_successfully "522096646210529F"                           //ɾ���ɹ�
#define TTS_Set_up_successfully_please_restart "8BBE7F6E6210529F8BF791CD542F" //
#define TT_No_related_numbers_found_please_re_edit "672A627E523076F8517353F77801FF0C8BF791CD65B07F168F91"
#define TTS_Low_power "753591CF4F4EFF0C8BF751457535" //������
#define TTS_battery_power_20 "753591CF767E52064E4B4E8C5341"
#define TTS_battery_power_40 "753591CF767E52064E4B56DB5341"
#define TTS_battery_power_60 "753591CF767E52064E4B516D5341"
#define TTS_battery_power_80 "753591CF767E52064E4B516B5341"
#define TTS_battery_power_100 "753591CF767E52064E4B767E"

#define TTS_UNICODE_Battery "753591CF0020"               //Battery
#define TTS_UNICODE_percent "767E52064E4B0020"           //�ٷ�֮
#define TTS_UNICODE_Current_Group "5F53524D7FA47EC40020" //��ǰȺ��
#define TTS_UNICODE_Current_User "5F53524D752862370020"  //��ǰ�û�

#else
#define TTS_Group_selection "Group selection"   //Ⱥ��ѡ��
#define TTS_Member_selection "Member selection" //��Աѡ��
#define TTS_Friend_selection "Friend selection" //����ѡ��
#define TTS_gps "GPS"                           //��λ��Ϣ
#define TTS_phone "phone"                       //�绰
#define TTS_setting "settings"                  //��������
#define TTS_LOCKED "Locked"                     //������
#define TTS_UNLOCKED "Unlocking succeeded"      //�����ɹ�

#define TTS_quit "quit"                                                               //�˳�
#define TTS_enter "enter"                                                             //����
#define TTS_not_online "not online"                                                   //������
#define TTS_Please_insert_SIM_card "No SIM card"                                      //��忨
#define TTS_Play_calling_success "Play calling success"                               //���гɹ�
#define TTS_Exit_Single_call "Exit Single call"                                       //�˳���ʱ����
#define TTS_Single_call_function_is_not_enabled "Single call function is not enabled" //��������δ����
#define TTS_No_one_online "No one online"                                             //��������

#define TTS_welcome_to_use_4G_IP_radios "welcome to use 4G IP radios, Network searching..." //��ӭʹ��4G�Խ�ϵͳ
#define TTS_Call_failed "Call failed"                                                       //����ʧ��
#define TTS_No_call "No call"                                                               // ��ֹ����
#define TTS_Incoming_call "Incoming call"                                                   //�绰����
#define TTS_Empty_list "Empty list"                                                         //��ǰ�����б�Ϊ��
#define TTS_No_phone_function "No phone function"                                           //4gģʽ��ֹ�绰����
#define TTS_Query_failed "Query failed"                                                     //��ѯʧ�ܣ��˳�
#define TTS_Wrong_password_please_re_enter "Wrong password, please re-enter"                //��ʾ:�����������������
#define TT_No_related_numbers_found_please_re_edit "No related numbers found, please re-edit"
#define TTS_Duplicate_friends "Duplicate friends"                           //�ظ�����
#define TTS_Deleting "Deleting"                                             //ɾ���ɹ�
#define TTS_Deleted_successfully "Deleted successfully"                     //ɾ���ɹ�
#define TTS_Shortcut_1_is_set_successfully "Shortcut 1 is set successfully" // ��ݼ�(�ո�)XX���óɹ�
#define TTS_Button_not_defined "Button not defined"                         //����δ����
#define TTS_Set_up_successfully_please_restart "Set up successfully, please restart"
#define TT_No_related_numbers_found_please_re_edit "No related numbers found, please re-edit"

#define TTS_Low_power "Low power" //������
#define TTS_battery_power_20 "battery power 20 percent"
#define TTS_battery_power_40 "battery power 40 percent"
#define TTS_battery_power_60 "battery power 60 percent"
#define TTS_battery_power_80 "battery power 80 percent"
#define TTS_battery_power_100 "battery power 100 percent"

#define TTS_UNICODE_Battery "00420061007400740065007200790020"                           //Battery
#define TTS_UNICODE_percent "002000700065007200630065006e0074"                           //percent
#define TTS_UNICODE_Current_Group "00430075007200720065006e0074002000470072006f00750070" //��ǰȺ��
#define TTS_UNICODE_Current_User "00430075007200720065006e007400200075007300650072"      //��ǰ�û�
#endif

#ifndef LANGUAGE_ENGLISH
#define c_Menu "�˵�"
#define c_Back "����"
#define c_Okey "ȷ��"
#define c_Switch "�л�"
#define c_Cancel "ȡ��"
#define c_Mute "����"
#define c_Checking_module "���ڲ�ѯģ��... "
#define c_Checking_SIM_card "���ڲ�ѯSIM��..."
#define c_No_SIM_card "��忨!"
#define c_Network_searching "������������... "
#define c_Logging_in "  ���ڵ�½...   "
#define c_Has_logged "��¼�ɹ�"
#define c_Offline "����"
#define c_Idle "����"
#define c_I_am_talking "��������˵��..."
#define c_GROUP "Ⱥ��"
#define c_MEMBER "��Ա"
#define c_FRIEND "����"
#define c_GPS "��λ"
#define c_PHONE "�绰"
#define c_Setting "����"
#define c_DIAL "��ݺ���"
#define c_INVITE "����"

#define c_Contact "��ϵ��"
#define c_New_contact "�½���ϵ��"
#define c_Phone_dial "����"
#define c_Call_log "���м�¼"

#define c_PTT_tone_setting "PTT������"
#define c_SIM_card_toggle "SIM���л�"
#define c_GPS_switch "GPS����"
#define c_USB_switch "USB����"
#define c_Key_lock_mode "����ģʽ"
#define c_Local_information "������Ϣ"
#define c_Backlight_setting "��������"
#define c_Network_setting "��������"
#define c_Time_zone_setting "ʱ������"
#define c_Factory "�ָ�����"
#define c_Module_upgrade "ģ������"
#define c_MCU_upgrade "��Ƭ������"
#define c_hot_key "��ݼ�"
#define c_8k_encode "8k����"
#define c_Air_code_writing "����д��"
#define c_Status_Ready "״̬������д��"
#define c_Status_waiting "״̬�������"
#define c_Status_Successfully_coded "״̬��д��ɹ�"
#define c_Open_USB "�ѿ���д��ģʽ"
#define c_Querying1 "��ѯ����Ⱥ��"
#define c_Querying2 "��ѯ��������"
#define c_Single_call_is_disable "δ�򿪵�������"
#define c_Please_return "�뷵��..."
#define c_Please_wait "���Ժ�..."
#define c_Local "����"
#define c_Open "��"
#define c_Close "�ر�"
#define c_SIM_1 "SIM��1����"
#define c_SIM_2 "SIM��2����"
#define c_sec_10 "10���Զ�"
#define c_sec_20 "20���Զ�"
#define c_sec_30 "30���Զ�"
#define c_Always_open "����ģʽ"
#define c_Manual "�ֶ�����"
#define c_Automatic "�Զ�����"
#define c_Shutting_down "���ڹػ�..."
#define c_Account "�˺�: "
#define c_Name "����: "
#define c_Group "Ⱥ��: "
#define c_Ver "�汾: "
#define c_Module "ģ��: "
#define c_latitude "γ��: "
#define c_longitude "����: "
#define c_satellite "��Ч����: "
#define c_gps_switch "GPS����: "

#define c_Please_restart "�������豸!"
#define c_Locked "*������ "
#define c_Enter_password "���������룺"
#define c_Enter_number_and_press_OK "���������֣�Ȼ��ȷ�ϼ�"
#define c_Restoring "���ڻָ�..."
#define c_Status "��ǰ״̬: "
#define c_Downloading "  ��������  "
#define c_Update_successed "ģ�������ɹ�"
#define c_Is_the_latest_version "�������°汾"
#define c_Restart_and_try_again "����������"
#define c_download_failed "����ʧ��  "

#define c_undefined "δ����"
#define c_group_switching "Ⱥ��  "
#define c_member_call "��Ա  "
#define c_friend_call "����  "

#define c_on_call "ͨ����"
#define c_Incoming_call "����"
#define c_no_records "���޼�¼"
#define c_Call " ���� "
#define c_Delete " ɾ�� "

#define c_name "����"
#define c_number "����"

#define c_Network_Automatic "�Զ�ѡ��"
#define c_only_4G "4Gģʽ  "
#define c_only_3G "3Gģʽ  "
#define c_only_2G "2Gģʽ  "
#define c_only_23G "2/3Gģʽ"

#define c_week_1 "����һ"
#define c_week_2 "���ڶ�"
#define c_week_3 "������"
#define c_week_4 "������"
#define c_week_5 "������"
#define c_week_6 "������"
#define c_week_7 "������"
#define c_tx_time_limit "������ʱ"
#define c_No_this_feature "�޴˹���"
#define c_Please_press_the_return_key "�밴�����ء�"
//old
#define c_Play_last "������һ��"
#define c_Play_next "������һ��"
#define c_Delete_all_files "ɾ������¼��"

#define c_no_sdcard "(���SD��)"

#define c_Press_OK_to_upgrade "�����밴��ȷ�ϡ�"
#define c_Press_Back_to_exit "�˳��밴�����ء�"
#define c_Note_Do_not_power_off "ע��:1 ��;��ϵ���ϵ�"

#define c_NET "����: "
#define c_Firmware "Ӳ��: "

#define c_Group_selection "Ⱥ��ѡ��"
#define c_Member_selection "��Աѡ��"
#define c_Friend_selection "����ѡ��"
#define c_Friends_List "�����б�"

#define c_No_one_online "��ʱ��������"

/* ģ�� */
#define c_account "�˺�: "
#define c_Switch "�л�"
#define c_Month_Day "%d��%d��"
#define c_China_Mobile "�й��ƶ�"
#define c_China_Unicom "�й���ͨ"
#define c_China_Telecom "�й�����"
#define c_unknown "δ֪"
#define c_VFO_channel "VFOƵ�ʣ�"
#define c_channel "�ŵ���"
#define c_Save_VFO_channel "���������޸��ŵ�!"
#define c_Saved_successfully "����ɹ���"
#define c_Please_wait_saving "���ڱ��棬���Ժ�"
#define c_Save_failed "����ʧ�ܣ�"
#define c_Enter_by_keyboard "�밴�������룡"
#define c_Press_ok_to_save "��ȷ�ϼ����棡"
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

/* ģ�� */
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

#define c_Engineering_mode_1 "����ģʽ����"
#define c_Engineering_mode_2 "���������룺"
#define c_Engineering_mode_3 "���������֣�Ȼ��ȷ�ϼ�"
