#ifndef __PA_LED_VOLUME_H
#define __PA_LED_VOLUME_H

#include "tom_data.h"

typedef enum
{
    LOW_BATTERY, //低电 红灯1秒闪1次
    NOT_LOGGED,  //未登录 绿灯1秒闪1次
    STANDBY,     //已登录或模拟待机状态 绿灯8秒闪1次
    TX,          //红灯常亮
    RX,          //绿灯常亮
    RED_OFF,
    GREEN_OFF,
} LED_STATES;

#ifdef PA_LED_VOLUME
#define PA_LED_VOLUME_EXTERN
#else
#define PA_LED_VOLUME_EXTERN extern
#endif

PA_LED_VOLUME_EXTERN void volume_init(void);

PA_LED_VOLUME_EXTERN void pa_timer_process(void);
PA_LED_VOLUME_EXTERN void pa_on_timer(u32 timer);

PA_LED_VOLUME_EXTERN void led_state_set(LED_STATES led_state);
PA_LED_VOLUME_EXTERN void led_bling_timer(void);

PA_LED_VOLUME_EXTERN void volume_timer_process(void);
PA_LED_VOLUME_EXTERN void volume_level_control(void);
PA_LED_VOLUME_EXTERN void set_volume_level(u8 value);
PA_LED_VOLUME_EXTERN void dispaly_volume_ico(u8 level);

#endif
