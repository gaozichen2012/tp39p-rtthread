#ifndef __KEY_H
#define __KEY_H

#include "tom_interface.h"

typedef enum
{
    SIMPLE_KEY_IDLE,    //空闲
    SIMPLE_KEY_PRESS,   //按下
    SIMPLE_KEY_HOLD,    //按住
    SIMPLE_KEY_RELEASE, //松开
} simple_key_state_type;

//单个按键结构体
typedef struct
{
    u16 tmp_press_count;             //内部使用-按键计数器
    simple_key_state_type state_old; //内部使用-对比上一个状态
    simple_key_state_type state;     //外部调用-按键长短按状态
} simple_key_t;

typedef enum
{
    KEY_IDLE,        //空闲
    KEY_SHORT_PRESS, //短按
    KEY_LONG_PRESS,  //长按
} key_state_type;

//单个按键结构体
typedef struct
{
    u16 tmp_press_count;           //内部使用-按键计数器
    bool tmp_long_press_lock_flag; //内部使用-长按锁定标志位，避免一直触发长按
    bool tmp_short_press_flag;     //内部使用-短按标志位
    key_state_type state;          //外部调用-按键长短按状态
} key_t;

//所有按键结构体
typedef struct
{
    simple_key_t key_ptt;

    key_t key_up;
    key_t key_down;

    //键盘0-9
    key_t keyboard_0;
    key_t keyboard_1;
    key_t keyboard_2;
    key_t keyboard_3;
    key_t keyboard_4;
    key_t keyboard_5;
    key_t keyboard_6;
    key_t keyboard_7;
    key_t keyboard_8;
    key_t keyboard_9;
    key_t keyboard_star;

    simple_key_t keyboard_ok;
    simple_key_t keyboard_up;
    simple_key_t keyboard_down;
    simple_key_t keyboard_back;

    simple_key_t keyboard_well;
} all_key_t;

#ifdef KEY
#define KEY_EXTERN
#else
#define KEY_EXTERN extern
#endif

KEY_EXTERN all_key_t all_key;

KEY_EXTERN void key_scan(void);

#endif
