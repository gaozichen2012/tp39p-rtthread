#ifndef __EC20_AT_CMD_H
#define __EC20_AT_CMD_H

#include "version.h"

#define AT_TX_SET_TONE "AT+POC=49000000\r\n"   //01表示大PTT音 00表示小
#define AT_TX_SET_TONE_1 "AT+POC=49000001\r\n" //01表示大PTT音 00表示小
#define AT_TX_SET_TONE_0 "AT+POC=49000001\r\n" //01表示大PTT音 00表示小

#ifdef POC_ZZD
#define AT_TX_QTTS0 "AT+QTTS=0\r\n"
#define AT_TX_SET_ZZD_TONE_OPEN "AT+POC=7C000001\r\n"            //开启PTT TONE
#define AT_TX_SET_ZZD_TONE_CLOSE "AT+POC=7C000000\r\n"           //关闭PTT TONE
#define AT_TX_SET_ZZD_TONE_LEVEL_DEFAULT "AT+POC=2500000064\r\n" //设置tone音大小
#define AT_TX_SET_ZZD_TONE_LEVEL_100 "AT+POC=25000001F4\r\n"

#define AT_TX_SET_POC_TYPE "AT+QCFGPOC=1\r\n"  //=1设置为卓智达平台，TYT平台无需设置
#define AT_TX_READ_POC_VER "AT+POC=7D0000\r\n" //zzd poc version
#define AT_TX_POC_UPGRADE "AT+POC=260000\r\n"
#define AT_TX_SET_ZZD_4K_CODING "AT+POC=30000001\r\n" //设置4K音质
#define AT_TX_SET_ZZD_8K_CODING "AT+POC=30000002\r\n" //设置8K音质
#define AT_TX_SET_ZZD_OPEN_LOG "AT+POC=7E000001\r\n"  //打开日志

#else
#define AT_TX_READ_POC_VER "AT+POC=460000\r\n" //tyt poc version
#define AT_TX_POC_UPGRADE "AT+POC=410000\r\n"

#endif

#if 1 //TOM整理 AT指令放一起
#ifdef POC_ZZD

#define AT_TX_SETID "AT+POC=01000069703d302e302e302e303b69643d545954303030313b7077643d3131313131313b00\r\n" //临时设置账号
#endif

#endif

/* AT TX的指令 */
#ifdef LANGUAGE_ENGLISH
#define AT_SET_TTS_LANGUAGE "AT+QTTSETUP=1,3,1\r\n"
#else
#define AT_SET_TTS_LANGUAGE "AT+QTTSETUP=1,3,0\r\n"
#endif
//开机时(TX)
#define AT_TX_ATE0 "ATE0\r\n"
#define AT_TX_READ_SIM_CARD "AT+CPIN?\r\n" //查询SIM卡
#define AT_TX_READ_MEID "AT^MEID\r\n"      //MEID
#define AT_TX_READ_GSN "AT+GSN\r\n"        //EC25-EUX用GSN空中写码（IMEI）
#define AT_TX_READ_IMSI "AT+CIMI\r\n"      // IMSI 获取卡号：4600...
#define AT_TX_READ_MODULE_VER "ATI\r\n"    //读取模块版本Revision: EC20CEFDR01A01M4G

#define AT_TX_READ_ACCOUNT_INFO "AT+POC=020000\r\n"
#define AT_TX_ENTER_AIR_CODE_MODE "AT+POC=450000\r\n"

//gain
#define AT_SET_EARPHONE_VOL_GAIN "AT+QIIC=0,0x1B,0x02,2,0x1010\r\n"
#define AT_SET_SPRAKER_VOL_GAIN "AT+QIIC=0,0x1B,0x03,2,0x1010\r\n"
#define AT_SET_ALL_VOL_GAIN_DEFAULT "at+qiic=0,0x1b,0x19,2,0xafaf\r\n" //默认值0xafaf
#define AT_SET_ALL_VOL_GAIN_2 "at+qiic=0,0x1b,0x19,2,0x9999\r\n"       //默认值0xafaf 0xa4a4 0x9999 0x8383 0x57d7   9999-39321
#define AT_SET_alc5616_ulgain "AT+QAUDCFG=\"alc5616/ulgain\",58\r\n"   //up咪头
// #define AT_SET_alc5616_dlgain "AT+QAUDCFG=\"alc5616/dlgain\",90\r\n" //down接收,此增益与QIIC冲突，无需设置
#define AT_SET_QAUDRDGAIN "AT+QAUDRDGAIN=65071\r\n" //fixed EUX BUG：read default gain is 65071, but invalid
#ifdef EC25_EUX
#define AT_SET_TTS_GAIN "AT+QTTSETUP=1,2,-31000\r\n" //eux 按账振奋要求改为-31000（tts音变小）
#else
#define AT_SET_TTS_GAIN "AT+QTTSETUP=1,2,-10000\r\n" //原为-20000，振奋要求改为-10000
#endif

#if 0
AT+QIIC=0,0x1B,0x03,2,0x0a8a

查询喇叭增益：
AT+QIIC=1,0x1B,0x03,2
#endif
#define AT_SET_OPEN_ADB "at+qcfg=\"usbcfg\",0x2C7C,0x125,1,1,1,1,1,1,1\r\n"
#define AT_SET_TONE_INCOMING "AT+QCFG=\"tone/incoming\",2\r\n" //打开来电提示音
#define AT_SET_POC_OPEN "AT+POC=000000010101\r\n"
#define AT_SET_POC_OPEN_WITH_NO_TTS "AT+POC=000000000100\r\n"
#define AT_SET_ZZD_ENGLISH_POC_OPEN "AT+POC=00000001010101\r\n"
//实时(TX)
#define AT_TX_READ_CSQ "AT+CSQ\r\n"               //查询网络强度
#define AT_TX_READ_HDRCSQ "AT^HDRCSQ\r\n"         //3G
#define AT_TX_READ_QCSQ "AT+QCSQ\r\n"             //4G
#define AT_TX_READ_NET_TYPE "AT+QNWINFO\r\n"      //查询网络类型
#define AT_TX_READ_TIME "AT+CCLK?\r\n"            //查询时间（未用到）
#define AT_TX_READ_POC_TIME "AT+POC=28000000\r\n" //查询时间（TYT ZZD通用）
//登录后(TX)
#define AT_TX_0B0000 "AT+POC=0B0000\r\n"
#define AT_TX_0C0000 "AT+POC=0C0000\r\n"
#define AT_TX_GROUP_REFRESH "AT+POC=0D0000\r\n"
#define AT_TX_ENTERY_GROUP "AT+POC=090000%s\r\n"
#define AT_TX_GET_ONLINE_USERS "AT+POC=0E0000%s\r\n"
#define AT_TX_GET_ONLINE_USERS2 "AT+POC=0E0000%s%s\r\n"
#define AT_TX_CALL_USER "AT+POC=0A0000%s\r\n"
#define AT_TX_TYT_EXIT_INVITE_CALL "AT+POC=0A000000000000\r\n"
#define AT_TX_SET_4G_AUTO "AT+QCFG=\"nwscanmode\",0,1\r\n"
#define AT_TX_SET_4G_ONLY "AT+QCFG=\"nwscanmode\",3,1\r\n"
#define AT_TX_SET_3G_CM "AT+QCFG=\"nwscanmode\",4,1\r\n"
#define AT_TX_SET_3G_CU "AT+QCFG=\"nwscanmode\",2,1\r\n"
#define AT_TX_SET_3G_CT "AT+QCFG=\"nwscanmode\",7,1\r\n"
#define AT_TX_SET_2G_CT "AT+QCFG=\"nwscanmode\",6,1\r\n"
#define AT_TX_SET_2G_GSM "AT+QCFG=\"nwscanmode\",1,1\r\n"
#define AT_TX_SET_3G_2G_AUTO_CT "AT+QCFG=\"nwscanmode\",8,1\r\n"

#define AT_TX_SET_ACCOUNT "AT+POC=010000%s\r\n"
#define AT_TX_POC_LOGOUT "AT+POC=040000\r\n"
#define AT_TX_MODULE_POWER_OFF "AT+QPOWD=0\r\n"
#define AT_TX_SET_TIME_ZONE "AT+POC=600000"

#define AT_TX_TTS_CHINESE "AT+QTTS=1,\"%s\"\r\n"
#define AT_TX_TTS_ENGLISH "AT+QTTS=2,\"%s\"\r\n"
#define AT_TX_TTS_CHINESE_WITH_2PARAM "AT+QTTS=1,\"%s%s\"\r\n"
#define AT_TX_TTS_ENGLISH_WITH_2PARAM "AT+QTTS=2,\"%s%s\"\r\n"

#define AT_TX_POWEROFF "AT+CFUN=0\r\n" //Switch phone to minimum functionality
#define AT_TX_POWERON "AT+CFUN=1\r\n"  //Switch phone to full functionality

#define AT_TX_CALL_ENABLE "AT+QCFG=\"disablesrlte\",0\r\n" //解决电信和联通在自动模式下不能正常拨打电话的bug(模块内部默认为1  且设置后模块重启生效)
#define AT_TX_CALL_SOMEONE "ATD%s;\r\n"
#define AT_TX_CALL_END1 "ATH0\r\n"
#define AT_TX_CALL_END2 "ATH\r\n"
#define AT_TX_CALL_END3 "AT+CHUP\r\n"
#define AT_TX_CALL_CLCC "AT+CLCC\r\n"
#define AT_TX_CALL_ANSWER "ATA\r\n"
#define AT_TX_CALL_SEND_DTMF "AT+VTS=\"\"\r\n"

#if 0                                //set and read apn
#define AT_TX_READ_APN "AT+QICSGP=1" //+QICSGP: 1,"ctnet","ctwap@mycdma.cn","vnet.mobi",2
#define AT_TX_SET_APN "AT+QICSGP=1,1," UNINET "," \
                      ","                         \
                      ",1"
#endif

/* AT RX的指令 */
//开机时(RX)
#define AT_RX_OK "OK"
#define AT_RX_CPIN_READY "+CPIN: READY"
#define AT_RX_CME_ERROR "+CME ERROR: 10"
#define AT_RX_MEID_1 "0x00A1" //ec20:A100005DBFCC45
#define AT_RX_MEID_2 "A1"     //ec20:A100005DB70E6A
#define AT_RX_MEID_3 "0x80"   //ec25:0x80F365F7
#define AT_RX_IMSI "460"      //中国460，其他国家未知
#define AT_RX_MODULE_VER "Revision:"

#define AT_RX_QCSQ_LTE "+QCSQ: \"LTE\""
#define AT_RX_HDRCSQ "^HDRCSQ:"
#define AT_RX_CSQ "+CSQ: "
#define AT_RX_QCSQ_NOSERVICE "+QCSQ: \"NOSERVICE\""

#define AT_RX_QNWINFO "+QNWINFO: "
#define AT_RX_QTTS0 "+QTTS: 0"

#define AT_RX_POC_02 "+POC:02"       //获取ip账号
#define AT_RX_POC_26 "+POC:26000000" //卓智达模块升级-应答26

#define AT_RX_POC_41 "+POC:41" //TYT poc升级进度
#define AT_RX_POC_45 "+POC:45" //enter air code

#define AT_RX_POC_46 "+POC:46" //get poc ver
#define AT_RX_POC_48 "+POC:48" //获取账号密码

#define AT_RX_POC_7d00 "+POC:7d00" //ZZD poc version

#define AT_RX_POC_80 "+POC:80"     //get group list name
#define AT_RX_POC_81 "+POC:81"     //get user list name
#define AT_RX_POC_82 "+POC:82"     //
#define AT_RX_POC_83 "+POC:83"     //
#define AT_RX_POC_84 "+POC:84"     //notify info
#define AT_RX_POC_86 "+POC:86"     //enter group
#define AT_RX_POC_8600 "+POC:8600" //ZZD enter group
#define AT_RX_POC_8601 "+POC:8601" //ZZD enter invite
#define AT_RX_POC_89 "+POC:89"     //Notify Privilege

#define AT_RX_POC_8b "+POC:8b" //pa states
#define AT_RX_POC_96 "+POC:96"
#define AT_RX_POC_0B "+POC:0b0000"
#define AT_RX_POC_0C "+POC:0c0000"
#define AT_RX_POC_0D "+POC:0d"    //get group list number
#define AT_RX_POC_0E "+POC:0e"    //get group list number
#define AT_RX_POC_FE0 "+POC:fe00" //卓智达模块升级-应答fe00-最新版本
#define AT_RX_POC_FE1 "+POC:fe01" //卓智达模块升级-应答fe01-升级成功

#define AT_RX_POC_8c "+POC:8c"     //TYT开机下发一次8c时间
#define AT_RX_POC_28 "+POC:280000" //获取时间（时区）
#define AT_RX_CCLK "+CCLK: \""     //获取世界时间（未用到）

//call
#define AT_RX_NO_CARRIER "NO CARRIER"
#define AT_RX_NO_ANSWER "NO ANSWER"
#define AT_RX_CLCC "+CLCC:"
#define AT_RX_RING "RING"

#define AT_RX_NORMAL_POWER_DOWN "NORMAL POWER DOWN"
//实时(RX)

//登录后(RX)

// #define AT_RX_
// #define AT_RX_
// #define AT_RX_
// #define AT_RX_
// #define AT_RX_
// #define AT_RX_
//#define AT_SET_urcport_uart1 "AT+QURCCFG=\"urcport\",\"uart1\"\r\n"
#endif
