/**
  ******************************************************************************
  * @file    PWR_STANDBY/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_dma.h"
#include "stdlib.h"
#include "string.h"
#include "key.h"
#include "gps.h"
#include "poc.h"
#include "keyboard.h"
#include "date_and_time.h"
#include "knob.h"
#ifdef DEVICE_ADC
extern Adc_t Adc;
#endif

uint16_t uart1_recv_timer = 0;

uint32_t SysTick_Cnt = 0;
//extern KEY_VALUE_T key_value_interrupt;

/** @addtogroup STM32F0_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup PWR_STANDBY
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static uint8_t buffer[USARTx_RXBUF_SIZE];
//static int Len=0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	__disable_irq();
	NVIC_SystemReset();
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	//	NVIC_SystemReset();
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
		__disable_irq();
		NVIC_SystemReset();
	}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static u32 custom_date_time_count = 0;

#ifdef SYSTEM_CLOCL_8M
	if (poc.system_clock_flag == CLOCK_8M)
	{
		SysTick_Cnt += 6;
	}
	else
#endif
	{
		SysTick_Cnt++;
	}

	if (custom_date_time_count + 1000 < SysTick_Cnt) //1ms*1000=1s
	{
		custom_date_time_count = SysTick_Cnt;
		//1s进一次
		custom_date_time_timer();
	}

#ifdef UART1_DEVICE
	if (uart1_recv_timer > 0)
		uart1_recv_timer--;
	else
	{
		if (gps_device.gps_data.status == UART_STATUS_RECVING)
			gps_device.gps_data.status = UART_STATUS_RECVED;
	}
#endif

	if (while_timeout > 0)
	{
		while_timeout--;
	}
}

/*******************************************************************************
* Function Name  : DMAChannel1_IRQHandler
* Description    : This function handles DMA Stream 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
	/*�ж�DMA��������ж�*/
	if (DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
#ifdef DEVICE_ADC
		Adc.Flag = 1;
#endif
	}
	/*���DMA�жϱ�־λ*/
	DMA_ClearITPendingBit(DMA1_IT_TC1);
}

/*******************************************************************************
* Function Name  : DMAChannel3_IRQHandler
* Description    : This function handles DMA Stream 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : DMAChannel_4_5_IRQHandler
* Description    : This function handles DMA Stream 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_5_IRQHandler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
#ifdef UART1_DEVICE
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		if (((gps_device.gps_data.write_p + 1) % UART_RXBUF_LEN_MAX) != gps_device.gps_data.read_p)
		{
			gps_device.gps_data.recv_buf[gps_device.gps_data.write_p++] = (uint8_t)USART_ReceiveData(USART1);
		}
		else //������
		{
			USART_ReceiveData(USART1);
			// LED_ON(1);
		}
		gps_device.gps_data.write_p %= UART_RXBUF_LEN_MAX;
		gps_device.gps_data.status = UART_STATUS_RECVING;
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		uart1_recv_timer = 3;
	}
#endif
	USART_ClearFlag(USART1, 0xFFFFFFFF);
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
#ifdef UART2_DEVICE
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if (((poc.module_data.write_p + 1) % UART_RXBUF_LEN_MAX) != poc.module_data.read_p)
		{
			poc.module_data.recv_buf[poc.module_data.write_p++] = (uint8_t)USART_ReceiveData(USART2);
		}
		else //������
		{
			USART_ReceiveData(USART2);
			// LED_ON(1);
		}
		poc.module_data.write_p %= UART_RXBUF_LEN_MAX;
		poc.module_data.status = UART_STATUS_RECVING;
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

#endif
	USART_ClearFlag(USART2, 0xFFFFFFFF);
}

void EXTI0_1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(0xFFF0);
	EXTI_ClearFlag(0xFFF0);
}

void EXTI2_3_IRQHandler(void)
{
	EXTI_ClearITPendingBit(0xFFF0);
	EXTI_ClearFlag(0xFFF0);
}

void EXTI4_15_IRQHandler(void)
{
#ifdef UART3_DEVICE

	uint8_t i, k;
	__disable_irq();
	//if(EXTI_GetITStatus(EXTI_Line15)!=RESET || EXTI_GetITStatus(EXTI_Line7)!=RESET)
	if (EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
	}
	EXTI_ClearFlag(EXTI_Line7);
	__enable_irq();
#endif

#ifdef KEY_INTRRUPT_PIN
	if (EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		if (GPIO_ReadInputDataBit(KEY_INTRRPUT_PORT1, KEY_INTRRPUT_PIN1) == 0) //������½����ж�
		{
			if (GPIO_ReadInputDataBit(KEY_INTRRPUT_PORT2, KEY_INTRRPUT_PIN2) == 0)
				key_value_interrupt = KEY_VALUE_INTRRPUT1;
			else
				key_value_interrupt = KEY_VALUE_INTRRPUT2;
		}
		else //�������ж�
		{
			if (GPIO_ReadInputDataBit(KEY_INTRRPUT_PORT2, KEY_INTRRPUT_PIN2) == 0)
				key_value_interrupt = KEY_VALUE_INTRRPUT2;
			else
				key_value_interrupt = KEY_VALUE_INTRRPUT1;
		}

		EXTI_ClearFlag(EXTI_Line11);
	}
#endif


#ifdef EXTI_DEVICE
if (EXTI_GetITStatus(EXTI_Line11) != RESET)
{
	knob_line_A_detection();
	EXTI_ClearFlag(EXTI_Line11);
}

if (EXTI_GetITStatus(EXTI_Line12) != RESET)
{
	knob_line_B_detection();
	EXTI_ClearFlag(EXTI_Line12);
}

#endif
	EXTI_ClearITPendingBit(0xFFF0);
	EXTI_ClearFlag(0xFFF0);
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */

#if DEVICE_TIM3
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
		if (tim3_Cnt++ > 2000) //20��bit��û�в����ж�  416us*20/4.16 = 2000
		{
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
	}
}

#endif

void TIM6_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		//1ms进一次
		key_scan();
		keyboard_value_identify();

		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		RTC_TimeTypeDef RTC_TimeStructure;
		/* Toggle on LED1 */

		/* Set the time to 00h 00mn 00s AM */
		RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours = 0x00;
		RTC_TimeStructure.RTC_Minutes = 0x00;
		RTC_TimeStructure.RTC_Seconds = 0x00;

		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
	}
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
