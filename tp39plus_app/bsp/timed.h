#ifndef __TIMED_H
#define __TIMED_H

#include "tom_data.h"

#ifdef TIMED
#define TIMED_EXTERN
#else
#define TIMED_EXTERN extern
#endif

TIMED_EXTERN u32 g_air_code_adb_timer_count;

TIMED_EXTERN void timed_main_process(void);
TIMED_EXTERN void backlight_timer_1s(void);
TIMED_EXTERN void invite_idle_timer_1s(void);
TIMED_EXTERN void backlight_idle_clear(void); //息屏计时清零（按下按键、进组）
TIMED_EXTERN void wake_up_module(void);

#endif
