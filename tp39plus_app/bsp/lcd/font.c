/*
字库IC-GT21L16S2Y的驱动，使用SPI驱动方式

通过字库ic读取字库内的点阵数据，并当做图片显示出来
 */
#define FONT
#include "font.h"
#include "tom_interface.h"
#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#define FONT_CS_HIGH GPIO_SetBits(SPI_CS_GPIO_Port, SPI_CS_Pin)
#define FONT_CS_LOW GPIO_ResetBits(SPI_CS_GPIO_Port, SPI_CS_Pin)
#define FONT_SCLK_HIGH GPIO_SetBits(SPI_SCLK_GPIO_Port, SPI_SCLK_Pin)
#define FONT_SCLK_LOW GPIO_ResetBits(SPI_SCLK_GPIO_Port, SPI_SCLK_Pin)
#define FONT_MOSI_HIGH GPIO_SetBits(LCD_DAT2_SPI_MOSI_GPIO_Port, LCD_DAT2_SPI_MOSI_Pin)
#define FONT_MOSI_LOW GPIO_ResetBits(LCD_DAT2_SPI_MOSI_GPIO_Port, LCD_DAT2_SPI_MOSI_Pin)
#define READ_FONT_MISO GPIO_ReadInputDataBit(SPI_MISO_GPIO_Port, SPI_MISO_Pin)
#define ADDR_BASE_15X16 0
#define ADDR_BASE_11X16 0X3CF80
#define ADDR_ASCII_8X16 0X3B7C0
#define ADDR_ASCII_8X12 0x66D40
#define ADDR_ASCII_5X7 0x3BFC0

#define ADDR_UNICODE_TO_GB2312 0x67D70
#define ADDR_GB2312_TO_UNICODE 0x2F00

void drv_font_init(void)
{
    gb2312_to_unicode(0xe2b2);
}

/*
读取GT21L16S数据
addr& 00ffffff  3个byte
datlen 读取长度
读取时序为 首字节 03普通读取，后三个字节为地址
 */
void read_font_addr(u32 addr, u8 *dat, u8 datlen)
{
    u8 i, j;
    addr &= 0x00ffffff;
    addr |= 0x03000000;
    memset(dat, 0, datlen);
    FONT_CS_LOW;

    for (i = 0; i < 32; i++) //发送bit数据
    {
        if (addr & (0x80000000 >> i))
            FONT_MOSI_HIGH;
        else
            FONT_MOSI_LOW;
        FONT_SCLK_LOW;
        FONT_SCLK_HIGH; //scl上升沿数据被GT21L锁定
    }
    //开始读数据， scl下降沿被移出
    for (i = 0; i < datlen; i++)
    {
        for (j = 0; j < 8; j++)
        {
            FONT_SCLK_HIGH;
            FONT_SCLK_LOW;
            if (READ_FONT_MISO)
                *(dat + i) = *(dat + i) | (0x01 << (7 - j));
            else
                *(dat + i) = *(dat + i) & ~(0x01 << (7 - j));
        }
    }
    FONT_CS_HIGH;
}

/*输入一个GB2312汉字，输出15*16点阵数据*/
u8 font_get_gb2312_pixel_data_15x16(u8 *out_dat, u8 *dat_gb2312)
{
    u32 addr;
    u8 MSB, LSB;
    MSB = *(u8 *)dat_gb2312;
    LSB = *(u8 *)(dat_gb2312 + 1);
    if (MSB >= 0XA4 && MSB <= 0XA8 && LSB >= 0XA1)
        addr = ADDR_BASE_15X16;
    else if (MSB >= 0XA1 && MSB <= 0XA9 && LSB >= 0XA1)
        addr = ((MSB - 0xa1) * 94 + (LSB - 0XA1)) * 32 + ADDR_BASE_15X16;
    else if (MSB >= 0XB0 && MSB <= 0XF7 && LSB >= 0XA1)
        addr = ((MSB - 0XB0) * 94 + (LSB - 0XA1) + 846) * 32 + ADDR_BASE_15X16;
    else
    {
        if (font_get_ascii_pixel_data_8x16(out_dat, *dat_gb2312))
        {
            if (font_get_ascii_pixel_data_8x16(out_dat + 16, *(dat_gb2312 + 1)))
                return 1;
        }
        else
            return 0;
    }
    read_font_addr(addr, out_dat, 32);
    return 1;
}

/*输入一个GB2312汉字，输出11x12点阵数据*/
u8 font_get_gb2312_pixel_data_11x12(u8 *out_dat, u8 *dat_gb2312)
{
    u32 addr;
    u8 MSB, LSB;
    MSB = *(u8 *)dat_gb2312;
    LSB = *(u8 *)(dat_gb2312 + 1);
    if (MSB >= 0XA1 && MSB <= 0XA3 && LSB >= 0XA1)
        addr = ((MSB - 0xa1) * 94 + (LSB - 0XA1)) * 24 + ADDR_BASE_11X16;
    else if (MSB == 0XA9 && LSB >= 0XA1)
        addr = (282 + (LSB - 0XA1)) * 24 + ADDR_BASE_11X16;
    else if (MSB >= 0XB0 && MSB <= 0XF7 && LSB >= 0XA1)
        addr = ((MSB - 0XB0) * 94 + (LSB - 0XA1) + 376) * 24 + ADDR_BASE_11X16;
    else
    {
        if (font_get_ascii_pixel_data_8x16(out_dat, *dat_gb2312))
        {
            if (font_get_ascii_pixel_data_8x16(out_dat + 16, *(dat_gb2312 + 1)))
                return 1;
        }
        else
            return 0;
    }
    read_font_addr(addr, out_dat, 24);
    return 1;
}

/* 输入ascii码，返回8*16点阵信息 */
u8 font_get_ascii_pixel_data_8x16(u8 *out_dat, u8 ascii)
{
    char g8x16_buffer[] = {
        0X00,
        0XF0,
        0X18,
        0X08,
        0X08,
        0X10,
        0XF8,
        0X00,
        0X00,
        0X01,
        0X13,
        0X12,
        0X12,
        0X19,
        0X0F,
        0X00,
    }; //8-数据水平-字节垂直-高位在前-上下颠倒-g4
    char j8x16_buffer[] = {
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0XF4,
        0X00,
        0X00,
        0X00,
        0X00,
        0X08,
        0X10,
        0X10,
        0X0F,
        0X00,
        0X00,
    }; //9-数据水平-字节垂直-高位在前-上下颠倒-j1
    char bar8x16_buffer[] = {
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X00,
        0X20,
        0X20,
        0X20,
        0X20,
        0X20,
        0X20,
        0X00,
    }; //10-数据水平-字节垂直-高位在前-上下颠倒-bar1

    u32 addr;
    if (ascii == 0x67) //0x67==g
    {
        memcpy(out_dat, g8x16_buffer, 16);
    }
    else if (ascii == 0x6a)
    {
        memcpy(out_dat, j8x16_buffer, 16);
    }
    else if (ascii == 0x5f)
    {
        memcpy(out_dat, bar8x16_buffer, 16);
    }
    else
    {
        if (ascii >= 0x20 && ascii <= 0x7E)
            addr = (ascii - 0X20) * 16 + ADDR_ASCII_8X16;
        else
            return 0;
        read_font_addr(addr, out_dat, 16);
    }

    return 1;
}

/* 输入ascii码，返回6*12点阵信息 */
u8 font_get_ascii_pixel_data_6x12(u8 *out_dat, u8 ascii)
{
    u32 addr;

    if (ascii >= 0x20 && ascii <= 0x7E)
        addr = (ascii - 0X20) * 12 + ADDR_ASCII_8X12;
    else
        return 0;
    read_font_addr(addr, out_dat, 12);
    return 1;
}

/* 输入ascii码，返回5*7点阵信息 */
u8 font_get_ascii_pixel_data_5x7(u8 *out_dat, u8 ascii)
{
    u32 addr;

    if (ascii >= 0x20 && ascii <= 0x7E)
        addr = (ascii - 0X20) * 8 + ADDR_ASCII_5X7;
    else
        return 0;
    read_font_addr(addr, out_dat, 8);
    return 1;
}

u16 unicode_to_gb2312(u16 UCSData)
{

    u8 result = 0;
    u16 gbcode;
    int h;
    if (UCSData < 0xa0)
        result = 1;
    else if (UCSData <= 0xf7)
        h = UCSData - 160; // 160
    else if (UCSData < 0x2c7)
        result = 1;
    else if (UCSData <= 0x2c9)
        h = UCSData - 160 - 463; //623
    else if (UCSData < 0x2010)
        result = 1;
    else if (UCSData <= 0x2312)
        h = UCSData - 160 - 463 - 7494; //8117
    else if (UCSData < 0x2460)
        result = 1;
    else if (UCSData <= 0x2642)
        h = UCSData - 160 - 463 - 7494 - 333; //8450
    else if (UCSData < 0x3000)
        result = 1;
    else if (UCSData <= 0x3017)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493; //10943
    else if (UCSData < 0x3220)
        result = 1;
    else if (UCSData <= 0x3229)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520; //11463
    else if (UCSData < 0x4e00)
        result = 1;
    else if (UCSData <= 0x9b54)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126; //18589
    else if (UCSData < 0x9c7c)
        result = 1;
    else if (UCSData <= 0x9ce2)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295; //18884
    else if (UCSData < 0x9e1f)
        result = 1;
    else if (UCSData <= 0x9fa0)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316; //19200
    else if (UCSData < 0xe76c)
        result = 1;
    else if (UCSData <= 0xe774)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379; //37579
    else if (UCSData < 0xff00)
        result = 1;
    else if (UCSData <= 0xff5f)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379 - 6027; //43606
    else if (UCSData < 0xffe0)
        result = 1;
    else if (UCSData <= 0xffe5)
        h = UCSData - 160 - 463 - 7494 - 333 - 2493 - 520 - 7126 - 295 - 316 - 18379 - 6027 - 128; //43734
    else
        result = 1;

    if (result == 0)
    {
        read_font_addr((u32)ADDR_UNICODE_TO_GB2312 + (h << 1), (u8 *)&gbcode, 2);
        gbcode = (gbcode >> 8) + (gbcode << 8);
    }
    else
    {
        gbcode = 0x0000;
    }
    //转换成小端模式
    result = (gbcode >> 8) & 0xff;
    gbcode = ((gbcode << 8) & 0xff00) | result;
    return gbcode;
}

u16 gb2312_to_unicode(u16 GBData)
{
    u8 codeH = 0;
    u8 codeL = 0;
    u32 index;
    u16 ucode;
    codeH = GBData >> 8; //输入小端模式
    codeL = GBData & 0x00ff;
    if ((codeH >= 0xA1) && (codeH <= 0xa3) && (codeL >= 0xA1))
    {
        index = (codeH - 0xA1) * 94 + (codeL - 0xA1);
    }
    else if ((codeH == 0xA9) && (codeL >= 0xA1) && (codeL <= 0xf6))
    {
        index = 282 + (codeL - 0xA1);
    }
    else if ((codeH >= 0xb0) && (codeH <= 0xf7) && (codeL >= 0xA1))
    {
        index = (codeH - 0xB0) * 94 + (codeL - 0xA1) + 368;
    }
    else
    {
        index = 0;
    }
    read_font_addr((u32)ADDR_GB2312_TO_UNICODE + (index << 1), (u8 *)&ucode, 2);
    ucode = (ucode >> 8) + (ucode << 8);
    return ucode;
}
