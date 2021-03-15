/*
键盘通用驱动
提供：2路adc采集值
输出：所有键盘识别
*/
#define KEYBOARD
#include "keyboard.h"

#define BASIC_VALUE 4095 - 4035 = 60 = 4065
/*
不同键对应的键盘值：
null： 4028 4028
ok  :2388 4028
up  :3659 4028
dn  :4028 2390
back:4028 3656
star:4028 3360
well:4028 2750
KB_0:4028 3079
kb_1:1984 4028
kb_2:3360 4028
KB_3:4028 1989
KB_4:1685 4028
KB_5:3076 4028
KB_6:4028 1708
KB_7:1320 4028
KB_8:2750 4028
KB_9:4028 1325

键盘值排序：间隔在300左右，取值在当前值的±50即可，其他值不取
4028 4028(368)
3659 3656(300)
3360 3360(284)
3076 3079(326)
2750 2750(362)
2388 2390(404)
1984 1989(299)
1685 1708(300)
1320 1325(365)

*/

#define KB_VALUE_SPACE 80

static keyboard_value_t keyboard_value = KB_NULL;
//键盘兼职识别
#if 0 //减少跑圈程序运行
void keyboard_value_identify(void)
{
    if (all_adc.keyboard2_adc_value > 4095 - KB_VALUE_SPACE &&
        all_adc.keyboard2_adc_value < 4095 + KB_VALUE_SPACE) //当keyboard2_adc_value为空时
    {
        if (all_adc.keyboard1_adc_value > 4095 - KB_VALUE_SPACE &&
            all_adc.keyboard1_adc_value < 4095 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_NULL;
            return; //尽可能减少tim6的程序，8M时会影响uart接收数据
        }
        else if (all_adc.keyboard1_adc_value > 2455 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 2455 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_OK;
        }
        else if (all_adc.keyboard1_adc_value > 3723 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 3723 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_UP;
        }
        else if (all_adc.keyboard1_adc_value > 2049 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 2049 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_1;
        }
        else if (all_adc.keyboard1_adc_value > 3420 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 3420 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_2;
        }
        else if (all_adc.keyboard1_adc_value > 1753 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 1753 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_4;
        }
        else if (all_adc.keyboard1_adc_value > 3144 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 3144 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_5;
        }
        else if (all_adc.keyboard1_adc_value > 1386 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 1386 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_7;
        }
        else if (all_adc.keyboard1_adc_value > 2812 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 2812 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_8;
        }
        else
        {
            /* code */
        }
    }

    if (all_adc.keyboard1_adc_value > 4095 - KB_VALUE_SPACE &&
        all_adc.keyboard1_adc_value < 4095 + KB_VALUE_SPACE) //当keyboard1_adc_value为空时
    {
        if (all_adc.keyboard2_adc_value > 4095 - KB_VALUE_SPACE &&
            all_adc.keyboard2_adc_value < 4095 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_NULL;
        }
        else if (all_adc.keyboard2_adc_value > 2459 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 2459 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_DN;
        }
        else if (all_adc.keyboard2_adc_value > 3721 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 3721 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_BACK;
        }
        else if (all_adc.keyboard2_adc_value > 3421 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 3421 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_STAR;
        }
        else if (all_adc.keyboard2_adc_value > 2812 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 2812 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_WELL;
        }
        else if (all_adc.keyboard2_adc_value > 3142 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 3142 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_0;
        }
        else if (all_adc.keyboard2_adc_value > 2047 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 2047 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_3;
        }
        else if (all_adc.keyboard2_adc_value > 1763 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 1763 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_6;
        }
        else if (all_adc.keyboard2_adc_value > 1382 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 1382 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_9;
        }
        else
        {
            /* code */
        }
    }
}
#else
void keyboard_value_identify(void)
{
    if (all_adc.keyboard2_adc_value > 4095 - KB_VALUE_SPACE &&
        all_adc.keyboard2_adc_value < 4095 + KB_VALUE_SPACE) //当keyboard2_adc_value为空时
    {
        if (all_adc.keyboard1_adc_value > 2455 - KB_VALUE_SPACE &&
            all_adc.keyboard1_adc_value < 2455 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_OK;
        }
        else if (all_adc.keyboard1_adc_value > 3723 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 3723 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_UP;
        }
        else if (all_adc.keyboard1_adc_value > 2049 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 2049 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_1;
        }
        else if (all_adc.keyboard1_adc_value > 3420 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 3420 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_2;
        }
        else if (all_adc.keyboard1_adc_value > 1753 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 1753 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_4;
        }
        else if (all_adc.keyboard1_adc_value > 3144 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 3144 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_5;
        }
        else if (all_adc.keyboard1_adc_value > 1386 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 1386 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_7;
        }
        else if (all_adc.keyboard1_adc_value > 2812 - KB_VALUE_SPACE &&
                 all_adc.keyboard1_adc_value < 2812 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_8;
        }
        else
        {
            keyboard_value = KB_NULL;
        }
    }

    if (all_adc.keyboard1_adc_value > 4095 - KB_VALUE_SPACE &&
        all_adc.keyboard1_adc_value < 4095 + KB_VALUE_SPACE) //当keyboard1_adc_value为空时
    {
        if (all_adc.keyboard2_adc_value > 2459 - KB_VALUE_SPACE &&
            all_adc.keyboard2_adc_value < 2459 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_DN;
        }
        else if (all_adc.keyboard2_adc_value > 3721 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 3721 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_BACK;
        }
        else if (all_adc.keyboard2_adc_value > 3421 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 3421 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_STAR;
        }
        else if (all_adc.keyboard2_adc_value > 2812 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 2812 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_WELL;
        }
        else if (all_adc.keyboard2_adc_value > 3142 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 3142 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_0;
        }
        else if (all_adc.keyboard2_adc_value > 2047 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 2047 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_3;
        }
        else if (all_adc.keyboard2_adc_value > 1763 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 1763 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_6;
        }
        else if (all_adc.keyboard2_adc_value > 1382 - KB_VALUE_SPACE &&
                 all_adc.keyboard2_adc_value < 1382 + KB_VALUE_SPACE)
        {
            keyboard_value = KB_9;
        }
        else
        {
            keyboard_value = KB_NULL;
        }
    }
}
#endif

keyboard_value_t get_keyboard_value(void)
{
    return keyboard_value;
}
