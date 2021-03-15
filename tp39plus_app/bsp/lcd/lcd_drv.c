/*
显示屏驱动

TP39PLUS的lcd驱动ST7735
TP79的lcd驱动芯片为ILI9163
398的lcd驱动ST7735

解释：drv驱动原理：通过引脚的组合设置，形成第一级的驱动函数：初始化函数、点线形函数、汉字显示驱动函数
 */
#define LCD_DRV
#include "lcd_drv.h"
#include "tom_interface.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
/*
#define ST7735S 0 //39PLUS
#define OHTER1 1
#define OHTER2 2
#define LCD_IC ST7735S
#if LCD_IC == ST7735S
#elif LCD_IC == OHTER1
#elif LCD_IC == OHTER2
#else
#endif
*/
#define LCD_SEND_DAT8(x) LCD_DATA_GPIO_Port->ODR = (LCD_DATA_GPIO_Port->ODR & 0x00ff) | (((x) << 8) & 0xff00) //data引脚1-8用来传输数据
#if 0
#define LCD_RST_HIGH LCD_RST_GPIO_Port->BSRR = LCD_RST_Pin
#define LCD_RST_LOW LCD_RST_GPIO_Port->BRR = LCD_RST_Pin

#define LCD_CS_HIGH LCD_CS_GPIO_Port->BSRR = LCD_CS_Pin
#define LCD_CS_LOW LCD_CS_GPIO_Port->BRR = LCD_CS_Pin

#define LCD_RS_HIGH LCD_RS_GPIO_Port->BSRR = LCD_RS_Pin
#define LCD_RS_LOW LCD_RS_GPIO_Port->BRR = LCD_RS_Pin

#define LCD_WR_HIGH LCD_WR_GPIO_Port->BSRR = LCD_WR_Pin
#define LCD_WR_LOW LCD_WR_GPIO_Port->BRR = LCD_WR_Pin

#define LCD_RD_HIGH LCD_RD_GPIO_Port->BSRR = LCD_RD_Pin
#define LCD_RD_LOW LCD_RD_GPIO_Port->BRR = LCD_RD_Pin
#else
#define LCD_RST_HIGH GPIO_SetBits(LCD_RST_GPIO_Port, LCD_RST_Pin)
#define LCD_RST_LOW GPIO_ResetBits(LCD_RST_GPIO_Port, LCD_RST_Pin)

#define LCD_CS_HIGH GPIO_SetBits(LCD_CS_GPIO_Port, LCD_CS_Pin)
#define LCD_CS_LOW GPIO_ResetBits(LCD_CS_GPIO_Port, LCD_CS_Pin)

#define LCD_RS_HIGH GPIO_SetBits(LCD_RS_GPIO_Port, LCD_RS_Pin)
#define LCD_RS_LOW GPIO_ResetBits(LCD_RS_GPIO_Port, LCD_RS_Pin)

#define LCD_WR_HIGH GPIO_SetBits(LCD_WR_GPIO_Port, LCD_WR_Pin)
#define LCD_WR_LOW GPIO_ResetBits(LCD_WR_GPIO_Port, LCD_WR_Pin)

#define LCD_RD_HIGH GPIO_SetBits(LCD_RD_GPIO_Port, LCD_RD_Pin)
#define LCD_RD_LOW GPIO_ResetBits(LCD_RD_GPIO_Port, LCD_RD_Pin)
#endif
static void lcd_delay_ms(u32 ms);
static void lcd_reset(void);

static void lcd_delay_ms(u32 ms)
{
    u32 i, j;
    for (j = 0; j < ms; j++)
    {
        for (i = 0; i < 8000; i++)
            ; //for 8M
    }
}

static void lcd_reset(void)
{
    LCD_RST_HIGH;
    lcd_delay_ms(1);
    LCD_RST_LOW;
    lcd_delay_ms(10);
    LCD_RST_HIGH;
    lcd_delay_ms(100);
}

/* 初始化lcd的参数 */
void lcd_init(void)
{
    lcd_reset();

    /* Start Initial Sequence */
    lcd_write_cmd8(0x11); //Exit Sleep
    lcd_delay_ms(120);
    lcd_write_cmd8(0xB1); //正常模式/全彩数据帧速率
    lcd_write_dat8(0x05);
    lcd_write_dat8(0x3C);
    lcd_write_dat8(0x3C);

    lcd_write_cmd8(0xB4); //反显控制
    lcd_write_dat8(0x00);

    /* ST7735S PowerSequence */
    lcd_write_cmd8(0xC0);
    lcd_write_dat8(0x28);
    lcd_write_dat8(0x08);
    lcd_write_dat8(0x04);

    lcd_write_cmd8(0xC1);
    lcd_write_dat8(0XC0);

    lcd_write_cmd8(0xC2);
    lcd_write_dat8(0x0D);
    lcd_write_dat8(0x00);

    lcd_write_cmd8(0xC3);
    lcd_write_dat8(0x8D);
    lcd_write_dat8(0x2A);

    lcd_write_cmd8(0xC4);
    lcd_write_dat8(0x8D);
    lcd_write_dat8(0xEE);
    /* End ST7735S Power Sequence */

    lcd_write_cmd8(0xC5); //VCOM 电压控制
    lcd_write_dat8(0x1A);

    /* 屏幕闪烁问题 */
    lcd_write_cmd8(0xC7);
    lcd_write_dat8(0xB6);

    lcd_write_cmd8(0x3a); //Set Color Format  65K 即RGB565  ，2的16次方
    lcd_write_dat8(0x05);

    lcd_write_cmd8(0x2A); //Set Column Address 横向起始像素和结束像素 160-1
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x9F);

    lcd_write_cmd8(0x2B); //Set Page Address 纵向起始像素和结束像素 128-1
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x7F);

    lcd_write_cmd8(0x36); //Set Scanning Direction --MX, MY,(水平、垂直扫描顺序)  、 RGB mode (RGB or BGR)
    lcd_write_dat8(0x60);

    /* ST7735S GammaSequence */
    lcd_write_cmd8(0xE0);
    lcd_write_dat8(0x04);
    lcd_write_dat8(0x22);
    lcd_write_dat8(0x07);
    lcd_write_dat8(0x0A);
    lcd_write_dat8(0x2E);
    lcd_write_dat8(0x30);
    lcd_write_dat8(0x25);
    lcd_write_dat8(0x2A);
    lcd_write_dat8(0x28);
    lcd_write_dat8(0x26);
    lcd_write_dat8(0x2E);
    lcd_write_dat8(0x3A);
    lcd_write_dat8(0x00);

    lcd_write_dat8(0x01);
    lcd_write_dat8(0x03);
    lcd_write_dat8(0x13);

    lcd_write_cmd8(0xE1);
    lcd_write_dat8(0x04);
    lcd_write_dat8(0x16);
    lcd_write_dat8(0x06);
    lcd_write_dat8(0x0D);
    lcd_write_dat8(0x2D);
    lcd_write_dat8(0x26);
    lcd_write_dat8(0x23);
    lcd_write_dat8(0x27);
    lcd_write_dat8(0x27);
    lcd_write_dat8(0x25);
    lcd_write_dat8(0x2D);
    lcd_write_dat8(0x3B);
    lcd_write_dat8(0x00);
    lcd_write_dat8(0x01);
    lcd_write_dat8(0x04);
    lcd_write_dat8(0x13);
    /* End ST7735S Gamma Sequence */

    lcd_delay_ms(200);
    lcd_write_cmd8(0x29); // Display On
}

void drv_lcd_set_display_range(u16 x, u16 y, u16 width, u16 height)
{
    lcd_write_cmd8(0x2a); //定义X地址的开始及结束位置
    lcd_write_dat8(x / 256);
    lcd_write_dat8(x % 256);
    lcd_write_dat8((x + width) / 256);
    lcd_write_dat8((x + width) % 256);

    lcd_write_cmd8(0x2b); //定义Y地址的开始及结束位置
    lcd_write_dat8(y / 256);
    lcd_write_dat8(y % 256);
    lcd_write_dat8((y + height) / 256);
    lcd_write_dat8((y + height) % 256);
}

/* 8080时序 */
inline void lcd_write_cmd8(u8 cmd)
{
    LCD_RD_HIGH;        //禁止读
    LCD_CS_LOW;         //使能
    LCD_RS_LOW;         //写命令
    LCD_WR_LOW;         //写时序
    LCD_SEND_DAT8(cmd); //发送数据
    LCD_WR_HIGH;
    LCD_CS_HIGH;
}

/* 8080时序 */
inline void lcd_write_dat8(u8 cmd)
{
    LCD_RD_HIGH;        //禁止读
    LCD_CS_LOW;         //使能
    LCD_RS_HIGH;        //写命令
    LCD_SEND_DAT8(cmd); //发送数据
    LCD_WR_LOW;
    LCD_WR_HIGH;
    LCD_CS_HIGH;
}

void lcd_write_dat16(u16 Data)
{
    LCD_RD_HIGH;                         //禁止读
    LCD_CS_LOW;                          //使能
    LCD_RS_HIGH;                         //写数据
    LCD_SEND_DAT8((Data >> 8) & 0x00ff); //发送数据高8位
    LCD_WR_LOW;                          //写时序
    LCD_WR_HIGH;
    LCD_SEND_DAT8(Data & 0x00ff); //发送数据低8位
    LCD_WR_LOW;                   //写时序
    LCD_WR_HIGH;
    LCD_CS_HIGH;
}
