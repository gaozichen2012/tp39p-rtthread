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
#include "uart.h"


extern device_uart_t   device_uart2;

#ifdef DEVICE_GPS_UART1
extern device_uart_t   device_uart1;
#endif

extern device_uart_t   device_uart3;


uint32_t SysTick_Cnt = 0;
uint32_t tim3_Cnt = 0;

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
		SysTick_Cnt++;

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
	/*判断DMA传输完成中断*/ 
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)                        
	{ 
#if DEVICE_ADC		
		Adc.Flag = 1;
#endif
	}	
	/*清除DMA中断标志位*/	
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
#ifdef DEVICE_GPS_UART1

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
	  	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			if(((device_uart1.write_p+1)%UART_RXBUF_LEN_MAX) != device_uart1.read_p)
			{
				device_uart1.recv_buf[device_uart1.write_p++] = (uint8_t)USART_ReceiveData(USART1);	
				
			}
			else//丢包了
			{
				USART_ReceiveData(USART1);	
				// LED_ON(1);
			}
			device_uart1.write_p %= UART_RXBUF_LEN_MAX;
			device_uart1.status = UART_STATUS_RECVING;
			USART_ClearFlag(USART1,USART_FLAG_RXNE);
	}

	USART_ClearFlag(USART1,0xFFFFFFFF);
	
#endif
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

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
	  	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			if(((device_uart2.write_p+1)%UART_RXBUF_LEN_MAX) != device_uart2.read_p)
			{
				device_uart2.recv_buf[device_uart2.write_p++] = (uint8_t)USART_ReceiveData(USART2);	
				
			}
			else//丢包了
			{
				USART_ReceiveData(USART2);	
				// LED_ON(1);
			}
			device_uart2.write_p %= UART_RXBUF_LEN_MAX;
			device_uart2.status = UART_STATUS_RECVING;
			USART_ClearFlag(USART2,USART_FLAG_RXNE);
	}
//	if(USART_GetITStatus(USART2, USART_IT_RTO) != RESET) 
	//{
	//	USART_ClearITPendingBit(USART2, USART_IT_RTO);	
//	}
	USART_ClearFlag(USART2,0xFFFFFFFF);
	

}


void EXTI0_1_IRQHandler(void)
{  
	uint8_t i,k;
	__disable_irq() ;	
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)  
	{
		if(device_uart3.status == UART_STATUS_NORMAL )
		{
			device_uart3.status = UART_STATUS_RECVING;	
			device_uart3.write_p = 0;
			while(1)
			{
				uint16_t timeout = 0;				
				for(k=0;k<32;k++); //读起始位
				if(UART3_RXPIN_READ()) //起始位是高就退出
					break;
				for(i=0;i<8;i++)
				{
					for(k=0;k<62;k++);	
					if(UART3_RXPIN_READ()) 
						device_uart3.recv_buf[device_uart3.write_p] |= (uint8_t)(0x01<<i);
					else
						device_uart3.recv_buf[device_uart3.write_p] &= ~((uint8_t)(0x01<<i));
				}
				device_uart3.write_p++;
				for(k=0;k<52;k++);//结束位
				while(1) //等起始位
				{
					if(UART3_RXPIN_READ() == 0)
						break;
					if(timeout++ > 1000)
						break;
				}
			}
 			device_uart3.status = UART_STATUS_RECVED;	
		}
	}
	EXTI_ClearFlag(EXTI_Line0); 
	__enable_irq() ;
}


void EXTI2_3_IRQHandler(void)
{

}

void EXTI4_15_IRQHandler(void)
{


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
		if(tim3_Cnt++ > 2000)//20个bit都没有产生中断  416us*20/4.16 = 2000
		{
			if(device_uart3.status == UART_STATUS_RECVING)
				device_uart3.status = UART_STATUS_RECVED;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
  }
}

#endif

void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
    RTC_TimeTypeDef RTC_TimeStructure;
    /* Toggle on LED1 */
    
    /* Set the time to 00h 00mn 00s AM */
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x00;
    RTC_TimeStructure.RTC_Minutes = 0x00;
    RTC_TimeStructure.RTC_Seconds = 0x00;  
  
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
    
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
  } 

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
