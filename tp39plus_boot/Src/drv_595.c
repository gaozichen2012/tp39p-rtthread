#include "drv_595.h"

const _stTag595 ioTag595={
.GPIOx_DAT=GPIOC,\
.GPIOx_LCLK=GPIOC,\
.GPIOx_SCLK=GPIOC,\
.Pin_DAT=GPIO_Pin_13,\
.Pin_LCLK=GPIO_Pin_14,\
.Pin_SCLK=GPIO_Pin_15
};

__IO _unSTBit595 conr_val595;

void init_595IO(_stTag595 *const pTag)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = pTag->Pin_DAT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef*)(pTag->GPIOx_DAT), &GPIO_InitStructure);	
	pTag->GPIOx_DAT->BRR = pTag->Pin_DAT;

	GPIO_InitStructure.GPIO_Pin = pTag->Pin_LCLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef*)(pTag->GPIOx_LCLK), &GPIO_InitStructure);	
	pTag->GPIOx_LCLK->BSRR = pTag->Pin_LCLK;

	GPIO_InitStructure.GPIO_Pin = pTag->Pin_SCLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef*)(pTag->GPIOx_SCLK), &GPIO_InitStructure);	
	pTag->GPIOx_SCLK->BRR = pTag->Pin_SCLK;
}


static void EX595_Delay(void)
{
	uint16_t i;
	for(i=10;i>0;i--)
		__NOP();
}


void Upate_595(_stTag595 *const pTag,uint8_t SendVal,FlagStatus UpateFlag)
{
	__IO uint8_t i;
	static uint8_t Prev_val;

	if((UpateFlag==RESET) && (Prev_val == SendVal))
	{
		return;
	}
	__disable_irq();

	pTag->GPIOx_LCLK->BRR = pTag->Pin_LCLK;
	EX595_Delay();
	for(i=0;i<8;i++)
	{
		if((SendVal & (0x80>>i)) != 0x00)
		{
			pTag->GPIOx_DAT->BSRR = pTag->Pin_DAT;
		}
		else 
		{
			pTag->GPIOx_DAT->BRR = pTag->Pin_DAT;
		}
		EX595_Delay();
		pTag->GPIOx_SCLK->BRR = pTag->Pin_SCLK;
		EX595_Delay();
		pTag->GPIOx_SCLK->BSRR = pTag->Pin_SCLK;
		EX595_Delay();
	}
	pTag->GPIOx_LCLK->BSRR = pTag->Pin_LCLK;
	EX595_Delay();

	__enable_irq();

	Prev_val = SendVal;
}


