#ifndef __NEW_UI_DATA_H
#define __NEW_UI_DATA_H

#include "version.h"

#ifndef NEW_UI_DATA
#define NEW_UI_DATA_EXTERN extern
#else
#define NEW_UI_DATA_EXTERN
#endif

NEW_UI_DATA_EXTERN const unsigned char gImage_toggle_on[952];
NEW_UI_DATA_EXTERN const unsigned char gImage_toggle_off[952];

NEW_UI_DATA_EXTERN const unsigned char gImage_two_way_arrow16x16[512];

#ifdef LENOVO //6个主界面图标
NEW_UI_DATA_EXTERN const unsigned char gImage_group_dark_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_group_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_member_dark_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_member_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_friend_dark_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_friend_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_gps_dark_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_gps_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_phone_dark_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_phone_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_settings_dark_53x48[5088];
NEW_UI_DATA_EXTERN const unsigned char gImage_settings_53x48[5088];
#else
NEW_UI_DATA_EXTERN const unsigned char gImage_group[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_member[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_friend[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_gps[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_phone[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_settings[1568];
#endif

NEW_UI_DATA_EXTERN const unsigned char gImage_group_28x28[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_member_28x28[1568];
//NEW_UI_DATA_EXTERN const unsigned char gImage_fiend_28x28[1568];
NEW_UI_DATA_EXTERN const unsigned char gImage_group_20x20[800];
NEW_UI_DATA_EXTERN const unsigned char gImage_member_20x20[800];

//单色
NEW_UI_DATA_EXTERN const unsigned char gImage_triangle[16];
NEW_UI_DATA_EXTERN const unsigned char gImage_triangle_8x16[16];

NEW_UI_DATA_EXTERN const unsigned char gImage_backgrund[60];
NEW_UI_DATA_EXTERN const unsigned char bits[][12];
NEW_UI_DATA_EXTERN const unsigned char gImage_true[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_0[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_1[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_2[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_3[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_4[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_5[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_battery_charge[40];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_0[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_1[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_2[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_3[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_4[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_5[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_6[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_7[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_8[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_volume_9[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_sim_card1_16x16[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_sim_card2_16x16[32];
NEW_UI_DATA_EXTERN const unsigned char gImage_speaker_16x16[32];

NEW_UI_DATA_EXTERN const unsigned char gImage_gps_12x12[24];
NEW_UI_DATA_EXTERN const unsigned char gImage_miss_call_12x12[24];

#if (THIS_VERSION == TYT_VERSION || THIS_VERSION == KER_VERSION)
NEW_UI_DATA_EXTERN const unsigned char gImage_HYSY4GDJJ_146x66[19272];
#elif (THIS_VERSION == LENOVO_VERSION || THIS_VERSION == LENOVO_ZZD_VERSION)
NEW_UI_DATA_EXTERN const unsigned char gImage_red2_lenovo_102x32[6528];
//NEW_UI_DATA_EXTERN const unsigned char gImage_red_lenovo_104x24[4992];
#elif (THIS_VERSION == ENG_VERSION || THIS_VERSION == EUX_VERSION || THIS_VERSION == USA_VERSION || THIS_VERSION == ZZD_ENG_VERSION)
NEW_UI_DATA_EXTERN const unsigned char gImage_LTE_4G_88x28[4928];
NEW_UI_DATA_EXTERN const unsigned char gImage_Tytpilot_96x26[4992];
#elif (THIS_VERSION == HWL_VERSION)
NEW_UI_DATA_EXTERN const unsigned char gImage_HWL_160x128[40960];
#elif (THIS_VERSION == ZZD_VERSION)
NEW_UI_DATA_EXTERN const unsigned char gImage_smartptt_142x21[5964];
NEW_UI_DATA_EXTERN const unsigned char gImage_LTE_4G_88x28[4928];
#endif

#endif
