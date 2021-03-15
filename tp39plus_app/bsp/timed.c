#define TIMED
#include "timed.h"
#include "tom_interface.h"
#include "ec20_at_cmd.h"
#include "poc.h"
#include "gps.h"
#include "ui.h"
#include "battery.h"
#include "pa_led_volume.h"
#include "typewriting.h"
#include "language.h"
#include "date_and_time.h"

u32 g_air_code_adb_timer_count = 0;

static void send_at_cmd_with_init(void);
static void send_at_cmd_with_login(void);

static void send_at_cmd_with_real_time(void);
static void some_flash_flag_init_with_logged(void);
static void air_code_timer_100ms_process(void);
static void get_poc_time(void);

void timed_main_process(void)
{
    static u32 time_100ms_count = 0;
    static u32 time_250ms_count = 0;
    static u32 time_500ms_count = 0;
    static u32 time_1s_count = 0;
    static u32 time_2s_count = 0;
    static u32 time_5s_count = 0;
    static u32 time_7s_count = 0;
    static u32 time_10s_count = 0;
    static u32 time_60s_count = 0;
    static u32 time_60min_count = 0;

    /*实时*/
    //uart2_buf_handle();
    // ptt_500ms_delay_process(false);
    ui_desktop(UI_OPT_PART_REFRESH); //实时刷新
    pinyin_process();
    ui_phone_new_contact(UI_OPT_KEY_KEYBOARD);
    ui_on_call(UI_OPT_KEY_KEYBOARD);
    ui_incoming_call(UI_OPT_KEY_KEYBOARD);
    ui_phone_dial(UI_OPT_KEY_KEYBOARD);
    ui_phone_dial(UI_OPT_OTHER_REFRESH);
    ui_setting_factory(UI_OPT_KEY_KEYBOARD);
    ui_setting_module_upgrade(UI_OPT_KEY_KEYBOARD);
    ui_setting_mcu_upgrade(UI_OPT_KEY_KEYBOARD);
    ui_setting_air_code_writing(UI_OPT_KEY_KEYBOARD);

    led_bling_timer();
    pa_timer_process();
    //wake_up_module();
    if (ui.page != UI_PAGE_BOOT_SCREEN)
    {
        gps_poc_report_timer_process();
        volume_timer_process();
    }

    if (SysTick_Cnt - time_100ms_count >= 100)
    {
        time_100ms_count = SysTick_Cnt;

        /* 100ms进程 */
        air_code_timer_100ms_process();
        battery_process(ICO_REFRESH_ON_CHANGE);

        if (ui.page != UI_PAGE_BOOT_SCREEN)
        {
            headphone_control_process();
        }
    }

    if (SysTick_Cnt - time_250ms_count >= 300)
    {
        time_250ms_count = SysTick_Cnt;

        /* 250ms进程 */
        bug_fix_ptt_end_with_0c00();
        volume_level_control();
    }

    if (SysTick_Cnt - time_500ms_count >= 450)
    {
        time_500ms_count = SysTick_Cnt;

        /* 400ms进程 */
        send_at_cmd_with_init();
        display_time_icon(0);
    }

    if (SysTick_Cnt - time_1s_count >= 1000)
    {
        time_1s_count = SysTick_Cnt;

        /* 1秒进程 */
        backlight_timer_1s();
        invite_idle_timer_1s();
        at_0c_talking_count_down();
        if (ui.page != UI_PAGE_BOOT_SCREEN)
        {
            ui_ico_csq(ICO_REFRESH_ON_CHANGE);
            ui_ico_network_type(ICO_REFRESH_ON_CHANGE);
            ui_ico_miss_call();
        }
    }

    if (SysTick_Cnt - time_2s_count >= 2567)
    {
        time_2s_count = SysTick_Cnt;

        some_flash_flag_init_with_logged();
        /* 2秒进程 */
    }

    if (SysTick_Cnt - time_5s_count >= 4567)
    {
        time_5s_count = SysTick_Cnt;

        /* 5秒进程 */
        ui_gps(UI_OPT_PART_REFRESH); //5s freash 1 time
        send_at_cmd_with_login();
    }

    if (SysTick_Cnt - time_7s_count >= 7890)
    {
        time_7s_count = SysTick_Cnt;

        send_at_cmd_with_real_time();
        /* 7秒进程 */
        if (poc.has_logged_flag == true && poc.get_account_flag == false)
        {
            module_at_tx(AT_TX_READ_ACCOUNT_INFO);
        }
    }

    if (SysTick_Cnt - time_10s_count >= 3456)
    {
        time_10s_count = SysTick_Cnt;

        /* 10秒进程 */
        get_poc_time();
    }

    if (SysTick_Cnt - time_60s_count >= 60 * 1000)
    {
        time_60s_count = SysTick_Cnt;

        /* 60s process */
        time_60min_count++;
        //SEGGER_RTT_printf(0, "time_60min_count = %d!!!%s-%d\r\n", time_60min_count, __FILE__, __LINE__);
        if (time_60min_count >= 60)
        {
            time_60min_count = 0;

            /* 60min process */
            poc.got_time_flag = 0; //过一小时会刷新一次时间，解决待机时间会变慢和日期/星期不会更新的问题
        }
    }
}

#ifdef CUSTOM_TIME
static void get_poc_time(void)
{
    if (poc.has_logged_flag == true && poc.got_time_flag == 0)
    {
        module_at_tx(AT_TX_READ_POC_TIME);
    }
}
#else
//获取时间
//1h获取1次：息屏
//1min获取1次：亮屏
//10s获取1次：亮屏瞬间；开机；设置时区
static void get_poc_time(void)
{
    static u16 get_time_count = 0; //10s累加1次

    if (poc.has_logged_flag == true)
    {
        if (poc.got_time_flag == 0 ||                                //开机获取；设置时区；
            (poc.got_time_flag == 2 && poc.backlight_state == true)) //亮屏瞬间
        {
            if (ui.page != UI_PAGE_ON_CALL && ui.page != UI_PAGE_INCOMING_CALL)
            {
                module_at_tx(AT_TX_READ_POC_TIME); //首次每隔10s获取一次时间
                get_time_count = 0;
            }
        }
        else
        {
            get_time_count++;

            if (poc.backlight_state == true)
            {
                //亮屏1分钟获取一次时间
                if (get_time_count >= 6) //(60/10=6)
                {
                    get_time_count = 0;
                    module_at_tx(AT_TX_READ_POC_TIME);
                }
            }
            else
            {
                //息屏1小时更新一次时间
                if (get_time_count >= 360) //(60*60/10=360)
                {
                    get_time_count = 0;
                    module_at_tx(AT_TX_READ_POC_TIME); //
                }
            }
        }
    }
}
#endif
//某些标志位首次刷程序或登录时需要设置一次
static void some_flash_flag_init_with_logged(void)
{
    static bool init_flag = false;

    if (poc.got_time_flag == 1 && init_flag == false)
    {
        init_flag = true;
        //时区（第一次登陆或第一次刷程序）
        delay_ms(50);
        poc_set_time_zone(poc.flash_data.time_zone_flag);
    }
}

static void send_at_cmd_with_init(void)
{
    if (ui.page != UI_PAGE_INCOMING_CALL && ui.page != UI_PAGE_ON_CALL)
    {
        if (poc.has_logged_flag == false)
        {
            if (poc.module_ready_flag == false)
            {
                module_at_tx(AT_TX_ATE0);
            }
            else
            {
                if (poc.module_init_flag == false)
                {
                    delay_ms(200);
                    module_at_tx(AT_SET_ALL_VOL_GAIN_DEFAULT);
                    delay_ms(200);
                    module_at_tx(AT_SET_ALL_VOL_GAIN_DEFAULT);
                    delay_ms(200);
                    module_at_tx(AT_SET_TTS_GAIN);
                    delay_ms(200);
                    module_at_tx(AT_SET_alc5616_ulgain);
                    delay_ms(200);
                    module_at_tx(AT_SET_TONE_INCOMING);
                    delay_ms(200);
                    module_at_tx(AT_SET_OPEN_ADB);
                    delay_ms(200);
                    module_at_tx(AT_TX_CALL_ENABLE);
                    delay_ms(200);
                    module_at_tx(AT_SET_TTS_LANGUAGE);
                    delay_ms(200);
#ifdef POC_ZZD
                    module_at_tx(AT_TX_SET_POC_TYPE);
                    delay_ms(200);
                    poc_set_ptt_tone();
                    delay_ms(200);
                    zzd_poc_set_8k_encode();
                    delay_ms(200);
                    module_at_tx(AT_TX_SET_ZZD_OPEN_LOG);
#endif

#ifdef LANGUAGE_ENGLISH
                    //EC25需要设置
                    module_at_tx(AT_SET_QAUDRDGAIN);
                    delay_ms(200);
#endif
                    volume_timer_process();
                    set_volume_level(poc.volume_level);
                    delay_ms(200);

                    if (ui.page == UI_PAGE_AIR_CODE_WRITING) //如果是空中写码模式，则不播报
                    {
                        set_gpio_state(GPIO_PA_EN, off);
                        SEGGER_RTT_printf(0, "%s %s-%d:PA-OFF%s\r\n", RTT_CTRL_BG_BRIGHT_GREEN, __FILE__, __LINE__, RTT_CTRL_RESET);
                    }
                    else
                    {
                        play_tts(2, TTS_welcome_to_use_4G_IP_radios);
                    }

                    poc.module_init_flag = true;

                    if (ui.page == UI_PAGE_BOOT_SCREEN)
                    {
                        ui_logging(UI_OPT_FIRST_REFRESH);
                    }
                }
                else
                {
                    if (poc.get_module_ver_flag == false)
                    {
                        module_at_tx(AT_TX_READ_MODULE_VER);
                    }
                    else
                    {
                        if (poc.sim_card_flag == FIRST_CHECK_SIM_CARD)
                        {
                            module_at_tx(AT_TX_READ_SIM_CARD);
                        }
                        else if (poc.sim_card_flag == FIRST_NO_SIM_CARD)
                        {
                            /* code */
                        }
                        else if (poc.sim_card_flag == SECOND_CHECK_SIM_CARD)
                        {
                            module_at_tx(AT_TX_READ_SIM_CARD);
                        }
                        else if (poc.sim_card_flag == SECOND_NO_SIM_CARD)
                        {
                            static u8 time_count = 10;

                            time_count++;

                            if (time_count >= 10)
                            {
                                time_count = 0;
                                play_tts(2, TTS_Please_insert_SIM_card);
                            }
                        }
                        else if (poc.sim_card_flag == HAVE_SIM_CARD_1 || poc.sim_card_flag == HAVE_SIM_CARD_2)
                        {
                            if (poc.get_meid_flag != 2)
                            {
#if (defined EC25_EUX) || (defined EC25_USA) //EC25使用GSN替代MEID
                                poc.get_meid_flag = 1;
                                module_at_tx(AT_TX_READ_GSN); //864977041641493
#else
                                module_at_tx(AT_TX_READ_MEID);
#endif
                            }
                            else
                            {
                                if (poc.get_imsi_sim_card_type == 0)
                                {
                                    module_at_tx(AT_TX_READ_IMSI);
                                }
                                else
                                {
                                    //if (poc.csq_level != 0)
                                    {
                                        if (ui.page == UI_PAGE_AIR_CODE_WRITING)
                                        {
                                            if (poc.enter_air_code_flag == false)
                                            {
                                                module_at_tx(AT_TX_ENTER_AIR_CODE_MODE);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* code */
                        }
                    }
                }
            }
        }
    }
}

static void send_at_cmd_with_login(void)
{
    static bool set_ptt_tone_flag = false;
    if (ui.page != UI_PAGE_INCOMING_CALL && ui.page != UI_PAGE_ON_CALL)
    {
        if (poc.has_logged_flag == false)
        {
            if (poc.module_ready_flag == false)
            {
            }
            else
            {
                if (poc.module_init_flag == false)
                {
                }
                else
                {
                    if (poc.get_module_ver_flag == false)
                    {
                    }
                    else
                    {
                        if (poc.sim_card_flag == FIRST_CHECK_SIM_CARD)
                        {
                        }
                        else if (poc.sim_card_flag == FIRST_NO_SIM_CARD)
                        {
                            /* code */
                        }
                        else if (poc.sim_card_flag == SECOND_CHECK_SIM_CARD)
                        {
                        }
                        else if (poc.sim_card_flag == SECOND_NO_SIM_CARD)
                        {
                        }
                        else if (poc.sim_card_flag == HAVE_SIM_CARD_1 || poc.sim_card_flag == HAVE_SIM_CARD_2)
                        {
                            if (poc.get_meid_flag != 2)
                            {
                            }
                            else
                            {
                                if (poc.get_imsi_sim_card_type == 0)
                                {
                                }
                                else
                                {
                                    if (ui.page == UI_PAGE_AIR_CODE_WRITING)
                                    {
                                    }
                                    else
                                    {
                                        if (poc.offline_test_flag == 1)
                                        {
                                            poc.offline_test_flag = 4;
                                        }
                                        else if (poc.offline_test_flag == 2)
                                        {
                                            poc.offline_test_flag = 5;
                                        }
                                        else if (poc.offline_test_flag == 3)
                                        {
                                            poc.offline_test_flag = 6;
                                        }
                                        else
                                        {
                                            poc.offline_test_flag = 7;
                                        }

                                        delay_ms(50);
                                        if (poc.logged_before_flag == true)
                                        {
#ifdef POC_ZZD
                                            //卓智达掉线后重新登录不播TTS，会导致进组和单呼无TTS提醒
                                            module_at_tx(AT_SET_POC_OPEN);
#else
                                            //掉线后重新登录不播TTS
                                            module_at_tx(AT_SET_POC_OPEN_WITH_NO_TTS);
#endif
                                        }
                                        else
                                        {
                                            module_at_tx(AT_SET_POC_OPEN);
                                        }
#ifdef POC_ZZD
                                        if (set_ptt_tone_flag == false)
                                        {
                                            set_ptt_tone_flag = true;
                                            delay_ms(50);
                                            module_at_tx(AT_TX_SET_TONE);
                                        }
#else
                                        delay_ms(50);
                                        module_at_tx(AT_TX_SET_TONE);
#endif
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static void air_code_timer_100ms_process(void)
{
    if (ui.page == UI_PAGE_BOOT_SCREEN)
    {
        g_air_code_adb_timer_count++;
    }
}

//获取信号值&网络类型
static void send_at_cmd_with_real_time(void)
{
    static u8 send_count = 0;

    if (ui.page != UI_PAGE_ON_CALL && ui.page != UI_PAGE_INCOMING_CALL)
    {
#if 1
        if (poc.backlight_state == true &&
            poc.im_talking_flag == false &&
            poc.receive_voice == false &&
            (poc.sim_card_flag == HAVE_SIM_CARD_1 || poc.sim_card_flag == HAVE_SIM_CARD_2))
        {
            if (poc.has_logged_flag == false || poc.network_type == 0 || poc.csq_level == 0)
            {
                if (send_count == 0)
                {
                    send_count = 1;
                    module_at_tx("\r\n");
                    delay_ms(30);

                    if (poc.network_type == 4)
                    {
                        module_at_tx(AT_TX_READ_QCSQ);
                    }
                    else if (poc.network_type == 3)
                    {
                        module_at_tx(AT_TX_READ_HDRCSQ);
                    }
                    else
                    {
                        module_at_tx(AT_TX_READ_CSQ);
                    }
                }
                else
                {
                    send_count = 0;
                    module_at_tx("\r\n");
                    delay_ms(30);
                    module_at_tx(AT_TX_READ_NET_TYPE);
                }
            }
            else
            {
                module_at_tx("\r\n");
                delay_ms(30);
                if (poc.network_type == 4)
                {
                    module_at_tx(AT_TX_READ_QCSQ);
                }
                else if (poc.network_type == 3)
                {
                    module_at_tx(AT_TX_READ_HDRCSQ);
                }
                else
                {
                    module_at_tx(AT_TX_READ_CSQ);
                }
            }
        }
#else
        if (poc.has_logged_flag == false || poc.network_type == 0 || poc.csq_level == 0)
        {
            if (poc.sim_card_flag == HAVE_SIM_CARD_1 || poc.sim_card_flag == HAVE_SIM_CARD_2)
            {
                //未登录且获取到SIM卡后，10s获取一次信号值&10获取一次网络类型
                if (send_count == 0)
                {
                    send_count = 1;
                    module_at_tx("\r\n");
                    delay_ms(30);
                    module_at_tx(AT_TX_READ_CSQ);
                }
                else
                {
                    send_count = 0;
                    module_at_tx("\r\n");
                    delay_ms(30);
                    module_at_tx(AT_TX_READ_NET_TYPE);
                }
            }
        }
#endif
    }
}

//一秒进一次
void backlight_timer_1s(void)
{
#if 1 //对讲时也会息屏
    if (poc.has_logged_flag == true)
#else
    if (poc.has_logged_flag == true && poc.im_talking_flag == false && poc.receive_voice == false)
#endif
    {
        poc.standby_idle_count++;
        if (poc.backlight_flash_time == 0xff) //常亮
        {
            /* code */
        }
        else
        {
            //超时息屏控制
            if (poc.standby_idle_count >= poc.backlight_flash_time && poc.backlight_state == true)
            {
                if (ui.page == UI_PAGE_ON_CALL ||
                    ui.page == UI_PAGE_INCOMING_CALL ||
                    ui.page == UI_PAGE_INVITE ||
                    ui.page == UI_PAGE_PHONE_DIAL ||
                    ui.page == UI_PAGE_PHONE_NEW_CONTACT ||
                    ui.page == UI_PAGE_PHONE_NEW_CONTACT_SON ||
                    ui.page == UI_PAGE_SETTING_FACTORY ||
                    ui.page == UI_PAGE_SETTING_MODULE_UPDATE_STEP1 ||
                    ui.page == UI_PAGE_SETTING_MODULE_UPDATE_STEP2 ||
                    ui.page == UI_PAGE_SETTING_MCU_UPDATE_STEP1 ||
                    ui.page == UI_PAGE_SETTING_MCU_UPDATE_STEP2)
                {
                    //特殊状态不息屏上锁
                }
                else
                {
                    set_gpio_state(GPIO_LCD_BL, off);
                    if (poc.flash_data.key_lock_toggle != 0) //≠0 自动
                    {
                        poc.lock_flag = true; //息屏则锁屏
                        ui_show_bottom_button(BUTTOM_LOCKED);
                    }
                }
            }
        }

        //息屏待机120秒后，进入降频模式（未开启GPS）
        if (poc.standby_idle_count >= 5 && poc.backlight_state == false && poc.flash_data.gps_enable_flag == 0 && poc.system_clock_flag == CLOCK_48M) //120s
        {
            select_system_clock_48M_or_8M(CLOCK_8M);
        }

        //超时返回桌面
        if (poc.standby_idle_count == 20 || poc.standby_idle_count == poc.backlight_flash_time)
        {
            timeout_back_to_desktop();
        }
    }
    else
    {
        backlight_idle_clear();
    }
}

void invite_idle_timer_1s(void)
{
#ifdef POC_ZZD
    //只有卓智达要主动退出（TYT平台会自动退出）
    if (poc.has_logged_flag == true &&
        ui.page == UI_PAGE_INVITE &&
        poc.im_talking_flag == false &&
        poc.receive_voice == false)
    {
        poc.invite_idle_count++;

        if (poc.invite_idle_count >= 20)
        {
            //超时退出单呼，进入群组
            ui_invite(UI_OPT_KEY_BACK);
        }
    }
    else
    {
        poc.invite_idle_count = 0;
    }

#endif
}

//息屏计时清零（按下按键、进组）
void backlight_idle_clear(void)
{
    set_gpio_state(GPIO_LCD_BL, on);
    poc.standby_idle_count = 0;
    poc.invite_idle_count = 0;

    //亮屏进入正常主频模式
    if (poc.system_clock_flag == CLOCK_8M)
    {
        select_system_clock_48M_or_8M(CLOCK_48M);
    }
}

void wake_up_module(void)
{
    static bool bl_state_old = false;

    if (bl_state_old != poc.backlight_state)
    {
        bl_state_old = poc.backlight_state;

        if (poc.backlight_state == true)
        {
            //亮屏时如果空闲按钮超过1分钟发AT唤醒
            if (poc.standby_idle_count >= 60)
            {
                module_at_tx(AT_TX_ATE0);
                SEGGER_RTT_printf(0, "module wake up\r\n");
            }
        }
    }
}
