#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "tom_interface.h"

typedef enum
{
    KB_NULL, //空闲
    KB_OK,   //确定键
    KB_UP,   //上键
    KB_DN,   //下键
    KB_BACK, //返回键
    KB_STAR, //*键
    KB_WELL, //#键
    KB_0,    //数字键0-9
    KB_1,
    KB_2,
    KB_3,
    KB_4,
    KB_5,
    KB_6,
    KB_7,
    KB_8,
    KB_9,
} keyboard_value_t;

#ifdef KEYBOARD
#define KEYBOARD_EXTERN
#else
#define KEYBOARD_EXTERN extern
#endif

KEYBOARD_EXTERN void keyboard_value_identify(void);
KEYBOARD_EXTERN keyboard_value_t get_keyboard_value(void);
#endif
