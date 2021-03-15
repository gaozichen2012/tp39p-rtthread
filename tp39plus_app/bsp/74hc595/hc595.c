#define HC595
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "hc595.h"
//74HC595口线高低电平宏定义
#define HC595_DATA_HIGH GPIO_SetBits(HC595_DATA_GPIO_Port, HC595_DATA_Pin)
#define HC595_DATA_LOW GPIO_ResetBits(HC595_DATA_GPIO_Port, HC595_DATA_Pin)
#define HC595_SCLK_HIGH GPIO_SetBits(HC595_SCLK_GPIO_Port, HC595_SCLK_Pin)
#define HC595_SCLK_LOW GPIO_ResetBits(HC595_SCLK_GPIO_Port, HC595_SCLK_Pin)
#define HC595_LCLK_HIGH GPIO_SetBits(HC595_LCLK_GPIO_Port, HC595_LCLK_Pin)
#define HC595_LCLK_LOW GPIO_ResetBits(HC595_LCLK_GPIO_Port, HC595_LCLK_Pin)

static void hc595_send_data(u8 data);

//74HC595的引脚初始化
void hc595_init(void)
{
    //下面这一串设置等同于hc595_send_data(0x00);
    hc595_set(off, LCD_BL_PIN);
    hc595_set(off, SIM_TOGGLE_PIN);
    hc595_set(off, GREEN_LED_PIN);
    hc595_set(off, RED_LED_PIN);
    hc595_set(off, PA_EN_PIN);
    hc595_set(off, MD_PWR_EN_PIN);
    hc595_set(off, GPS_EN_PIN);
    hc595_set(off, USB_EN_PIN);
}

//设置8个输出脚高低电平
void hc595_set(onoff_t state, HC595_OUT_PIN pin_name)
{
    static u8 data_old = 0;
    if (state == on)
    {
        data_old |= pin_name; //将指定位置1
    }
    else
    {
        data_old &= ~pin_name; //将指定位置0
    }
    hc595_send_data(data_old);
}

/*
将输入的数据输出到74HC595的引脚
data：1 byte data

hc595_send_data(0xFF); //74HC595输出端全部为高电平,LED全部熄灭
hc595_send_data(0x00); //74HC595输出端全部为低电平,LED全部点亮
*/
static void hc595_send_data(u8 data)
{
    u8 i;                   //发送数据时做循环使用临时变量
    for (i = 0; i < 8; i++) //将8位数据按位发送,先发送高字节后发送低字节
    {
        HC595_SCLK_LOW;  //时钟线低电平
        if (data & 0x80) //判断数据高低位
        {
            HC595_DATA_HIGH; //数据线高电平
        }
        else
        {
            HC595_DATA_LOW; //数据线低电平
        }
        data = data << 1; //数据左移1位
        HC595_SCLK_HIGH;  //时钟线高电平
    }
    //上升沿输出数据
    HC595_LCLK_LOW;
    HC595_LCLK_HIGH;
}
