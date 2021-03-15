#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_gpio.h"
#include "device_config.h"
#include "uart.h"
#include "tim3.h"
#include "string.h"
#include "stdio.h"


device_uart_t   device_uart3;
device_uart_t   device_uart2;
device_uart_t   device_uart1;



void uart3_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;  
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE); 	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 ;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	
	
	//设置中断输入 exti -> exti0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);  
	EXTI_InitStructure.EXTI_Line=EXTI_Line0; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//EXTI0中断向量配置
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_1_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPriority=0x01;  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  

	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE); 	
	//RCC->APB2ENR|=1<<0; 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  
	GPIO_SetBits(GPIOA, GPIO_Pin_15);

	uart3_send_string((uint8_t *)"uart3 init ok\r\n", 15);

	
	device_uart3.read_p = 0;
	device_uart3.write_p = 0;
	memset(device_uart3.recv_buf, 0, UART_RXBUF_LEN_MAX);
	device_uart3.status = UART_STATUS_NORMAL;		

}





void uart1_config(void)
{
#ifdef DEVICE_GPS_UART1	
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef  NVIC_InitStructure;

	#if(DEF_DEBUG)
	#else
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   #endif

   RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE ); 
   USART_InitStructure.USART_BaudRate = 115200;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART1, &USART_InitStructure);
	 
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
	USART_DirectionModeCmd(USART1, USART_Mode_Rx, ENABLE);	
	USART_RequestCmd(USART1,USART_Request_RXFRQ,ENABLE);	 	 
	 
   USART_Cmd(USART1, ENABLE);
	 USART_ITConfig(USART1 , USART_IT_RXNE, ENABLE);
		
	 device_uart1.read_p = 0;
	 device_uart1.write_p = 0;
	 memset(device_uart1.recv_buf, 0, UART_RXBUF_LEN_MAX);
	 device_uart1.status = UART_STATUS_NORMAL;		

	 
#endif
}

/*
 * 函数名：uart2_config
 * 描述    
 * 输入  ：无
 * 输出  : 无
 */
void uart2_config(void)
{
	// PA2, PA3 , 115200 8-N-1
#ifdef DEVICE_POC_UART2	
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef  NVIC_InitStructure;

   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
   GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE ); 
   USART_InitStructure.USART_BaudRate = 115200;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART2, &USART_InitStructure);
	 
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	USART_DirectionModeCmd(USART2, USART_Mode_Rx, ENABLE);	
	USART_RequestCmd(USART2,USART_Request_RXFRQ,ENABLE);	 
	 /*
	USART_SetReceiverTimeOut(USART2,115200/20);//设置接收超时
	USART_ReceiverTimeOutCmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RTO, ENABLE);//开启接收超时中断	 	 
	*/
   USART_Cmd(USART2, ENABLE);
	
	 USART_ITConfig(USART2 , USART_IT_RXNE, ENABLE);
	 
	 device_uart2.read_p = 0;
	 device_uart2.write_p = 0;
	 memset(device_uart2.recv_buf, 0, UART_RXBUF_LEN_MAX);
	 device_uart2.status = UART_STATUS_NORMAL;
	 
#endif
}


void uart2_send_byte(uint8_t byte) //发送1字节数据
{
 while(!((USART2->ISR)&(1<<7)));
 USART2->TDR=byte;	
}		

void uart2_send_string(const uint8_t *str, uint16_t strlen)
{
	/*
	#if(DEF_DEBUG)
	printf("----->");
	#endif
	*/
	while(strlen != 0)
	{
		while(!((USART2->ISR)&(1<<7)));//等待发送完
		USART2->TDR= *str;
		/*
		#if(DEF_DEBUG)
		USART1->TDR = *str;
		#endif
		*/
		str++;
		strlen--;
	}
}


void uart1_send_byte(uint8_t byte) //发送1字节数据
{
 while(!((USART1->ISR)&(1<<7)));
 USART1->TDR=byte;	
}		

void uart1_send_string(const uint8_t *str, uint16_t strlen)
{
	while(strlen != 0)
	{
		while(!((USART1->ISR)&(1<<7)));//等待发送完
		USART1->TDR= *str;
		str++;
		strlen--;
	}
}

//115200
void uart3_send_string(const uint8_t *str, uint16_t strlen)
{
	uint16_t k;
	uint8_t i ,j = 0;
	const uint8_t temp[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	__disable_irq() ;
	while(strlen != 0)
	{
		//发送起始位
		GPIOA->BRR = GPIO_Pin_15;
		for(k=0;k<68;k++);
		//发送数据
		for(i=0; i<8; i++)
		{
			if((str[j] & temp[i]) == 0)
				GPIOA->BRR = GPIO_Pin_15;
			else
				GPIOA->BSRR = GPIO_Pin_15;
			for(k=0;k<68;k++);	
		}
		//结束位
		GPIOA->BSRR = GPIO_Pin_15;
		for(k=0;k<68;k++);		
		j++;
		strlen--;
	}
	__enable_irq() ;
}


#ifdef UART_GPIO_9600
void uart3_send_string(const uint8_t *str, uint16_t strlen)
{
	uint16_t k;
	uint8_t i ,j = 0;
	const uint8_t temp[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	__disable_irq() ;
	while(strlen != 0)
	{
		//发送起始位
		GPIOA->BRR = GPIO_Pin_15;
		for(k=0;k<745;k++);
		//发送数据
		for(i=0; i<8; i++)
		{
			if((str[j] & temp[i]) == 0)
				GPIOA->BRR = GPIO_Pin_15;
			else
				GPIOA->BSRR = GPIO_Pin_15;
			for(k=0;k<750;k++);	
		}
		//结束位
		GPIOA->BSRR = GPIO_Pin_15;
		for(k=0;k<745;k++);		
		j++;
		strlen--;
	}
	__enable_irq() ;
}
#endif



void uart3_send_char(const uint8_t str)
{
	uint16_t k;
	uint8_t i ,j = 0;
	const uint8_t temp[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	__disable_irq() ;
//	while(strlen != 0)
	{
		//发送起始位
		GPIOA->BRR = GPIO_Pin_15;
		for(k=0;k<68;k++);
		//发送数据
		for(i=0; i<8; i++)
		{
			if((str & temp[i]) == 0)
				GPIOA->BRR = GPIO_Pin_15;
			else
				GPIOA->BSRR = GPIO_Pin_15;
			for(k=0;k<68;k++);	
		}
		//结束位
		GPIOA->BSRR = GPIO_Pin_15;
		for(k=0;k<68;k++);		
		j++;
	//	strlen--;
	}
	__enable_irq() ;
}

#if(DEF_DEBUG)
int fputc(int ch, FILE *stream)
{
	while(!((USART1->ISR)&(1<<7)));
 	USART1->TDR=(uint8_t)ch;
	return ch;
}
#endif



