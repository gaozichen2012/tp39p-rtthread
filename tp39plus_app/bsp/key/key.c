/*
按键通用驱动
提供：读取按键状态的函数和按键数
输出：按键的识别

简单按键只有按下和松开的状态 simple_key_state_type
默认按键可以识别长短键 key_state_type
ptt为简单按键
侧上下键为默认按键可以识别长短键
键盘0-9：默认按键可以识别长短键
键盘ok/up/down/back为简单按键
*/

#define KEY
#include "key.h"
#include "keyboard.h"
#include "poc.h"

// #define KEYTIME_SHORT 100 //短按的按键去抖动延时的时间
// #define KEYTIME_LONG 800  //长按的按键去抖动延时的时间

#define KEYTIME_SHORT 48 //短按的按键去抖动延时的时间
#define KEYTIME_LONG 600 //长按的按键去抖动延时的时间

#define KEYTIME_SHORT_FOR_8M 0  //短按的按键去抖动延时的时间
#define KEYTIME_LONG_FOR_8M 100 //长按的按键去抖动延时的时间

static void simple_key_press_scan(simple_key_t *key_type);
static void simple_key_clear(simple_key_t *key_type);
static void key_short_long_press_scan(key_t *key_type);
static void key_clear(key_t *key_type);

//按键扫描函数（放在定时中断里）
void key_scan(void)
{
    static keyboard_value_t keyboard_value = KB_NULL;
    static keyboard_value_t keyboard_value_old = KB_NULL;
    static keyboard_value_t tmp_value = KB_NULL;
    static u8 tmp_count = 0;
    /* 简单按键 */
    if (get_gpio_state(GPIO_KEY_PTT) == on) //侧上键
    {
        simple_key_press_scan(&all_key.key_ptt);
    }
    else
    {
        simple_key_clear(&all_key.key_ptt);
    }

    /* 长短键 */
    if (get_gpio_state(GPIO_KEY_UP) == on) //侧上键
    {
        key_short_long_press_scan(&all_key.key_up);
    }
    else
    {
        //按键未被按下，及时清零相关标志位
        key_clear(&all_key.key_up);
    }

    if (get_gpio_state(GPIO_KEY_DN) == on) //侧下键
    {
        key_short_long_press_scan(&all_key.key_down);
    }
    else
    {
        //按键未被按下，及时清零相关标志位
        key_clear(&all_key.key_down);
    }

    /* 以下为键盘模拟的按键 */
#if 1 //数字键盘滤波，尝试解决长按按键有时跳出短按操作
    if (tmp_value != get_keyboard_value())
    {
        tmp_value = get_keyboard_value();
        tmp_count = 0;
    }
    else
    {
        tmp_count++;
    }

    if (poc.system_clock_flag == CLOCK_48M)
    {
        if (tmp_count > 50)
        {
            tmp_count = 0;
            keyboard_value = tmp_value;
        }
    }
    else
    {
        if (tmp_count > 8)
        {
            tmp_count = 0;
            keyboard_value = tmp_value;
        }
    }

#else
    keyboard_value = get_keyboard_value();
#endif

#if 0
    if (keyboard_value != keyboard_value_old)
    {
        keyboard_value_old = keyboard_value;

        switch (keyboard_value)
        {
        case KB_NULL:

            key_clear(&all_key.keyboard_0);
            key_clear(&all_key.keyboard_1);
            key_clear(&all_key.keyboard_2);
            key_clear(&all_key.keyboard_3);
            key_clear(&all_key.keyboard_4);
            key_clear(&all_key.keyboard_5);
            key_clear(&all_key.keyboard_6);
            key_clear(&all_key.keyboard_7);
            key_clear(&all_key.keyboard_8);
            key_clear(&all_key.keyboard_9);
            key_clear(&all_key.keyboard_star);

            simple_key_clear(&all_key.keyboard_ok);
            simple_key_clear(&all_key.keyboard_up);
            simple_key_clear(&all_key.keyboard_down);
            simple_key_clear(&all_key.keyboard_back);

            simple_key_clear(&all_key.keyboard_well);
            break;
        case KB_OK: //确定键
            simple_key_press_scan(&all_key.keyboard_ok);
            break;
        case KB_UP: //上键
            simple_key_press_scan(&all_key.keyboard_up);
            break;
        case KB_DN: //下键
            simple_key_press_scan(&all_key.keyboard_down);
            break;
        case KB_BACK: //返回键
            simple_key_press_scan(&all_key.keyboard_back);
            break;
        case KB_WELL: //#键
            simple_key_press_scan(&all_key.keyboard_well);
            break;
        case KB_STAR: //*键
            key_short_long_press_scan(&all_key.keyboard_star);
            break;
        case KB_0: //数字键0-9
            key_short_long_press_scan(&all_key.keyboard_0);
            break;
        case KB_1:
            key_short_long_press_scan(&all_key.keyboard_1);
            break;
        case KB_2:
            key_short_long_press_scan(&all_key.keyboard_2);
            break;
        case KB_3:
            key_short_long_press_scan(&all_key.keyboard_3);
            break;
        case KB_4:
            key_short_long_press_scan(&all_key.keyboard_4);
            break;
        case KB_5:
            key_short_long_press_scan(&all_key.keyboard_5);
            break;
        case KB_6:
            key_short_long_press_scan(&all_key.keyboard_6);
            break;
        case KB_7:
            key_short_long_press_scan(&all_key.keyboard_7);
            break;
        case KB_8:
            key_short_long_press_scan(&all_key.keyboard_8);
            break;
        case KB_9:
            key_short_long_press_scan(&all_key.keyboard_9);
            break;
        default:
            break;
        }
    }
#else
    switch (keyboard_value)
    {
    case KB_NULL:

        key_clear(&all_key.keyboard_0);
        key_clear(&all_key.keyboard_1);
        key_clear(&all_key.keyboard_2);
        key_clear(&all_key.keyboard_3);
        key_clear(&all_key.keyboard_4);
        key_clear(&all_key.keyboard_5);
        key_clear(&all_key.keyboard_6);
        key_clear(&all_key.keyboard_7);
        key_clear(&all_key.keyboard_8);
        key_clear(&all_key.keyboard_9);
        key_clear(&all_key.keyboard_star);

        simple_key_clear(&all_key.keyboard_ok);
        simple_key_clear(&all_key.keyboard_up);
        simple_key_clear(&all_key.keyboard_down);
        simple_key_clear(&all_key.keyboard_back);

        simple_key_clear(&all_key.keyboard_well);
        break;
    case KB_OK: //确定键
        simple_key_press_scan(&all_key.keyboard_ok);
        break;
    case KB_UP: //上键
        simple_key_press_scan(&all_key.keyboard_up);
        break;
    case KB_DN: //下键
        simple_key_press_scan(&all_key.keyboard_down);
        break;
    case KB_BACK: //返回键
        simple_key_press_scan(&all_key.keyboard_back);
        break;
    case KB_WELL: //#键
        simple_key_press_scan(&all_key.keyboard_well);
        break;
    case KB_STAR: //*键
        key_short_long_press_scan(&all_key.keyboard_star);
        break;
    case KB_0: //数字键0-9
        key_short_long_press_scan(&all_key.keyboard_0);
        break;
    case KB_1:
        key_short_long_press_scan(&all_key.keyboard_1);
        break;
    case KB_2:
        key_short_long_press_scan(&all_key.keyboard_2);
        break;
    case KB_3:
        key_short_long_press_scan(&all_key.keyboard_3);
        break;
    case KB_4:
        key_short_long_press_scan(&all_key.keyboard_4);
        break;
    case KB_5:
        key_short_long_press_scan(&all_key.keyboard_5);
        break;
    case KB_6:
        key_short_long_press_scan(&all_key.keyboard_6);
        break;
    case KB_7:
        key_short_long_press_scan(&all_key.keyboard_7);
        break;
    case KB_8:
        key_short_long_press_scan(&all_key.keyboard_8);
        break;
    case KB_9:
        key_short_long_press_scan(&all_key.keyboard_9);
        break;
    default:
        break;
    }
#endif
}

static void simple_key_press_scan(simple_key_t *key_type)
{
    static u16 short_key_max = 0;
    key_type->tmp_press_count++; //累加定时中断次数

    if (poc.system_clock_flag == CLOCK_48M)
    {
        short_key_max = KEYTIME_SHORT;
    }
    else
    {
        short_key_max = KEYTIME_SHORT_FOR_8M;
    }

    if (key_type->tmp_press_count > short_key_max)
    {
        if (key_type->state_old != SIMPLE_KEY_PRESS) //
        {
            key_type->state = SIMPLE_KEY_PRESS;
            key_type->state_old = key_type->state;
        }
    }
}

static void simple_key_clear(simple_key_t *key_type)
{
    if (key_type->state == SIMPLE_KEY_HOLD || (key_type->state == SIMPLE_KEY_PRESS))
    {
        key_type->state = SIMPLE_KEY_RELEASE;
        key_type->state_old = key_type->state;
        key_type->tmp_press_count = 0;
    }
}

//识别长短键
static void key_short_long_press_scan(key_t *key_type)
{
    static u16 short_key_max = 0;
    static u16 long_key_max = 0;

    if (poc.system_clock_flag == CLOCK_48M)
    {
        short_key_max = KEYTIME_SHORT;
        long_key_max = KEYTIME_LONG;
    }
    else
    {
        short_key_max = KEYTIME_SHORT_FOR_8M;
        long_key_max = KEYTIME_LONG_FOR_8M;
    }

    if (key_type->tmp_long_press_lock_flag == false) //有按键按下，且是第一次被按下
    {
        key_type->tmp_press_count++; //累加定时中断次数
        if (key_type->tmp_press_count > short_key_max)
        {
            key_type->tmp_short_press_flag = true; //激活按键短按的有效标志
        }
        if (key_type->tmp_press_count > long_key_max)
        {
            key_type->tmp_short_press_flag = false; //清除按键短按的有效标志
            key_type->tmp_press_count = 0;
            key_type->tmp_long_press_lock_flag = true; //自锁按键置位,避免一直触发
            key_type->state = KEY_LONG_PRESS;          //触发1号键的长按
        }
    }
}

//清除长短键标志
static void key_clear(key_t *key_type)
{
    key_type->tmp_long_press_lock_flag = false; //按键自锁标志位清零
    key_type->tmp_press_count = 0;              //按键去抖计数器清零

    if (key_type->tmp_short_press_flag == true)
    {
        key_type->tmp_short_press_flag = false;
        key_type->state = KEY_SHORT_PRESS; //触发短按
    }
}
