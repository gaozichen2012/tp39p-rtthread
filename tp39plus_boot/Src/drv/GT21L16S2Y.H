#ifndef __GT21L16S2_H_
#define __GT21L16S2_H_

#include "stm32f0xx.h"
//SPI

#define ADDR_BASE_15X16   0
#define ADDR_BASE_11X16   0X3CF80
#define ADDR_ASCII_8X16   0X3B7C0
#define ADDR_ASCII_8X12		0x66D40
#define ADDR_UNICODE_TO_GB2312	0x67D70
#define ADDR_GB2312_TO_UNICODE	0x2F00

#define SPI_CS_PORT				GPIOA
#define SPI_SCLK_PORT			GPIOB
#define SPI_MOSI_PORT			GPIOB
#define SPI_MISO_PORT			GPIOB

#define SPI_CS_PIN				GPIO_Pin_8
#define SPI_SCLK_PIN			GPIO_Pin_1
#define SPI_MOSI_PIN			GPIO_Pin_10
#define SPI_MISO_PIN			GPIO_Pin_0

#define SPI_CS_HIGHT()  GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN)
#define SPI_CS_LOW()  	GPIO_ResetBits(SPI_CS_PORT, SPI_CS_PIN)

#define SPI_CLK_HIGHT()  GPIO_SetBits(SPI_SCLK_PORT, SPI_SCLK_PIN)
#define SPI_CLK_LOW()  	GPIO_ResetBits(SPI_SCLK_PORT, SPI_SCLK_PIN)

#define SPI_MOSI_HIGHT()  GPIO_SetBits(SPI_MOSI_PORT, SPI_MOSI_PIN)
#define SPI_MOSI_LOW()  	GPIO_ResetBits(SPI_MOSI_PORT, SPI_MOSI_PIN)

#define SPI_MISO_HIGHT()  GPIO_SetBits(SPI_MISO_PORT, SPI_MISO_PIN)
#define SPI_MISO_LOW()  	GPIO_ResetBits(SPI_MISO_PORT, SPI_MISO_PIN)
#define SPI_MISO_READ()   GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN)


void gt21l16s2_init(void);

//输入一个GB2312汉字，输出15*16点阵数据
uint8_t gt21l16s2_get_gb2312_15x16(uint8_t *out_dat, uint8_t* dat_gb2312);
//输入一个ASCII，输出8*16点阵数据
uint8_t gt21l16s2_get_ascii_8x16(uint8_t *out_dat, uint8_t ascii);
uint8_t gt21l16s2_get_gb2312_11x12(uint8_t *out_dat, uint8_t* dat_gb2312);
uint8_t gt21l16s2_get_ascii_6x12(uint8_t *out_dat, uint8_t ascii);
//输入小端模式数据, uincode转GB2312
uint16_t unicode_to_gb2312(uint16_t UCSData);
//GB2312转Uincode 输出小端模式
uint16_t gb2312_to_unicode(uint16_t GBData);

#endif

