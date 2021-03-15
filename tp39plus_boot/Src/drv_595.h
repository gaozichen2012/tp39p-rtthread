#ifndef		__DRV_595_H
#define		__DRV_595_H

#include "stm32f0xx.h"

typedef struct __Tag_595
{
	 GPIO_TypeDef* GPIOx_DAT;
	 GPIO_TypeDef* GPIOx_LCLK;
	 GPIO_TypeDef* GPIOx_SCLK;
	 uint16_t Pin_DAT;
	 uint16_t Pin_LCLK;
	 uint16_t Pin_SCLK;
}_stTag595;

typedef struct __BITOF595
{
	uint8_t LCD_BL:1;
	uint8_t SIM_SEL:1;
	uint8_t LED_BLUE:1;
	uint8_t LED_RED:1;
	uint8_t PA_EN:1;
	uint8_t PWR_EN:	1;
	uint8_t GPS_EN:	1;
	uint8_t USE_EN:	1;
}_stBit595;

typedef union __unstBit595
{
	_stBit595 conr_bit;
	uint8_t contun;
}_unSTBit595;


extern const _stTag595 ioTag595;
extern __IO _unSTBit595 conr_val595;

void init_595IO(_stTag595 *const pTag);
void Upate_595(_stTag595 *const pTag,uint8_t SendVal,FlagStatus UpateFlag);


#endif

