#define KNOB
#include "knob.h"
#include "tom_interface.h"

char first_line = 0;      //第一次跳变的线（A 0r B）
char first_line_edge = 0; //跳边沿

void knob_line_A_detection(void)
{
#if 1
    if (get_gpio_state(GPIO_BTN_4) == off) //A线产生下降沿
    {
        if (first_line == 'B')
        {
            if (first_line_edge == 0)
            {
                first_line = 0;
                //先B下降沿，然后A下降沿，判断为逆时针转动一次
                step_switch = STEP_RIGHT;
            }
            else
            {
                //先B上升沿，然后A下降沿，判断为顺时针转动一次
                first_line = 0;
                step_switch = STEP_LEFT;
            }
        }
        else
        {
            //首次A下降沿
            first_line = 'A';
            first_line_edge = 0;
        }
    }
    else //A线产生上升沿
    {
        if (first_line == 'B')
        {
            if (first_line_edge == 0)
            {
                //先B下降沿，然后A上升沿，判断为顺时针转动一次
                first_line = 0;
                step_switch = STEP_LEFT;
            }
            else
            {
                first_line = 0;
                //先B上升沿，然后A上升沿，判断为逆时针转动一次
                step_switch = STEP_RIGHT;
            }
        }
        else
        {
            //首次A上升沿
            first_line = 'A';
            first_line_edge = 1;
        }
    }

#else
    if (get_gpio_state(GPIO_BTN_4) == off) //A线产生下降沿
    {
        if (first_line == 'B')
        {
            if (first_line_edge == 0)
            {
                first_line = 0;
                //先B下降沿，然后A下降沿，判断为逆时针转动一次
                step_switch = STEP_RIGHT;
            }
            else
            {
                //无效状态，算为首次A下降
                first_line = 'A';
                first_line_edge = 0;
            }
        }
        else
        {
            //首次A下降沿
            first_line = 'A';
            first_line_edge = 0;
        }
    }
    else //A线产生上升沿
    {
        if (first_line == 'B')
        {
            if (first_line_edge == 0)
            {
                //无效状态：算为A首次上升
                first_line = 'A';
                first_line_edge = 1;
            }
            else
            {
                first_line = 0;
                //先B上升沿，然后A上升沿，判断为逆时针转动一次
                step_switch = STEP_RIGHT;
            }
        }
        else
        {
            //首次A上升沿
            first_line = 'A';
            first_line_edge = 1;
        }
    }
#endif
}

void knob_line_B_detection(void)
{
#if 1
    if (get_gpio_state(GPIO_BTN_5) == off) //B线产生下降沿
    {
        if (first_line == 'A')
        {
            if (first_line_edge == 0)
            {
                first_line = 0;
                //先A下降沿，然后B下降沿，判断为顺时针转动一次
                step_switch = STEP_LEFT;
            }
            else
            {
                //先A上升沿，然后B下降沿，判断为逆时针转动一次
                first_line = 0;
                step_switch = STEP_RIGHT;
            }
        }
        else
        {
            //首次B上升沿
            first_line = 'B';
            first_line_edge = 0;
        }
    }
    else //B线产生上升沿
    {
        if (first_line == 'A')
        {
            if (first_line_edge == 0)
            {
                //先A下降沿，然后B上升沿，判断为逆时针转动一次
                first_line = 0;
                step_switch = STEP_RIGHT;
            }
            else
            {
                first_line = 0;
                //先A上升沿，然后B上升沿，判断为顺时针转动一次
                step_switch = STEP_LEFT;
            }
        }
        else
        {
            //首次B上升沿
            first_line = 'B';
            first_line_edge = 1;
        }
    }

#else
    if (get_gpio_state(GPIO_BTN_5) == off) //B线产生下降沿
    {
        if (first_line == 'A')
        {
            if (first_line_edge == 0)
            {
                first_line = 0;
                //先A下降沿，然后B下降沿，判断为顺时针转动一次
                step_switch = STEP_LEFT;
            }
            else
            {
                //无效状态：算是首次B首次下降沿
                first_line = 'B';
                first_line_edge = 0;
            }
        }
        else
        {
            //首次B上升沿
            first_line = 'B';
            first_line_edge = 0;
        }
    }
    else //B线产生上升沿
    {
        if (first_line == 'A')
        {
            if (first_line_edge == 0)
            {
                //无效状态：算是A首次上升沿
                first_line = 'B';
                first_line_edge = 1;
            }
            else
            {
                first_line = 0;
                //先A上升沿，然后B上升沿，判断为顺时针转动一次
                step_switch = STEP_LEFT;
            }
        }
        else
        {
            //首次B上升沿
            first_line = 'B';
            first_line_edge = 1;
        }
    }
#endif
}
