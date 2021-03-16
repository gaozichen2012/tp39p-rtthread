/*
在使用HAL_UART_Transmit传输数据时会进行加锁HAL_LOCK(huart)。
此时来了数据,进入了中断HAL_UART_RxCpltCallback,在中断里又继续调用HAL_UART_Receive_IT来开启下一个数据接受中断,而在HAL_UART_Receive_IT里也会先进行加锁,
此时发现锁已经被HAL_UART_Transmit占用了,所以函数调用失败了,结果以后就再也不会接受数据了
*/

#define TOM_INTERFACE
#include "tom_interface.h"
#include "poc.h"
#include "gps.h"
#include "key.h"
#include "keyboard.h"
#include "date_and_time.h"
#include "knob.h"
#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_tim.h"
#include <rtthread.h>
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
//extern DMA_HandleTypeDef hdma_usart2_tx;
//extern DMA_HandleTypeDef hdma_usart2_rx;
#if 1						 //尝试使用UART2+DMA
volatile uint8_t rx_len = 0; //接收一帧数据的长度
//volatile uint8_t recv_end_flag = 0; //一帧数据接收完成标志

uint8_t usart2_rx_buffer[BUF_TEMP_SIZE];

// uint8_t usart2_rx_buffer_2[BUF_TEMP_SIZE];
uint16_t usart2_tx_len = 0;
#endif

static void lcd_gpio_init(void);
static void else_output_gpio_init(void);
static void else_input_gpio_init(void);
static void adc_gpio_init(void);
static void adc_device_init(void);
static void ADC_DMA_Init(void);
static void TIM6_init(void);
//static void TIM6_init_for_8M(void);
static void uart1_config_9600(void);
static void uart2_config(void);
static void knob_init(void);

#if 1 //普通gpio相关设置（目标为了方便移植，将和硬件相关的操作集中在此）

void tom_interface_init(void)
{
	lcd_gpio_init();
	else_output_gpio_init();
	else_input_gpio_init();
	adc_gpio_init();
	adc_device_init();
	TIM6_init();

	uart1_config_9600();
	uart2_config();
	uart2_buf_clear();
	knob_init();
}

//获取按键状态
onoff_t get_gpio_state(GPIO_T pin)
{
	static uint8_t state = 0;

	switch (pin)
	{
	case GPIO_KEY_PTT: //ptt
		state = GPIO_ReadInputDataBit(PTT_GPIO_Port, PTT_Pin);
		break;
	case GPIO_KEY_UP: //侧上键
		state = GPIO_ReadInputDataBit(KEY_UP_GPIO_Port, KEY_UP_Pin);
		break;
	case GPIO_KEY_DN: //侧下键
		state = GPIO_ReadInputDataBit(KEY_DN_GPIO_Port, KEY_DN_Pin);
		break;
	case GPIO_HPH_DET: //耳机检测
		state = GPIO_ReadInputDataBit(HPH_DET_GPIO_Port, HPH_DET_Pin);
		break;
	case GPIO_BTN_4:
		state = GPIO_ReadInputDataBit(BTN4_GPIO_Port, BTN4_Pin);
		break;
	case GPIO_BTN_5:
		state = GPIO_ReadInputDataBit(BTN5_GPIO_Port, BTN5_Pin);
		break;
	default:
		state = 0;
		break;
	}

	return (onoff_t)state;
}

//设置引脚状态
void set_gpio_state(GPIO_T pin, onoff_t state)
{
	switch (pin)
	{
	case GPIO_LCD_BL: //背光
		if (state == on)
		{
			poc.backlight_state = true;
			hc595_set(on, LCD_BL_PIN);
		}
		else
		{
			poc.backlight_state = false;
			hc595_set(off, LCD_BL_PIN);
		}
		//state == on ? hc595_set(on, LCD_BL_PIN) : hc595_set(off, LCD_BL_PIN);
		break;
	case GPIO_SIM_TOGGLE: //SIM卡选择
		state == on ? hc595_set(on, SIM_TOGGLE_PIN) : hc595_set(off, SIM_TOGGLE_PIN);
		break;
	case GPIO_GREEN_LED: //绿灯
		state == on ? hc595_set(on, GREEN_LED_PIN) : hc595_set(off, GREEN_LED_PIN);
		break;
	case GPIO_RED_LED: //红灯
		state == on ? hc595_set(on, RED_LED_PIN) : hc595_set(off, RED_LED_PIN);
		break;
	case GPIO_PA_EN: //喇叭开关
#if 0				 //TEST always open PA
        hc595_set(on, PA_EN_PIN);
#else
		if (state == on)
		{
			if (poc.headphone_state == on)
			{
				//插入耳机不开PA
				hc595_set(off, PA_EN_PIN);
			}
			else
			{
				hc595_set(on, PA_EN_PIN);
			}
		}
		else
		{
			hc595_set(off, PA_EN_PIN);
		}
#endif
		//state == on ? hc595_set(on, PA_EN_PIN) : hc595_set(on, PA_EN_PIN);
		break;
	case GPIO_MODULE_PWR_EN: //模块供电开关
		state == on ? hc595_set(on, MD_PWR_EN_PIN) : hc595_set(off, MD_PWR_EN_PIN);
		break;
	case GPIO_GPS_EN: //GPS开关
		state == on ? hc595_set(on, GPS_EN_PIN) : hc595_set(off, GPS_EN_PIN);
		break;
	case GPIO_USB_EN: //USB开关
		state == on ? hc595_set(on, USB_EN_PIN) : hc595_set(off, USB_EN_PIN);
		break;
	case GPIO_PWR_CTL: //整机供电开关
		state == on ? GPIO_SetBits(PWR_CTL_GPIO_Port, PWR_CTL_Pin) : GPIO_ResetBits(PWR_CTL_GPIO_Port, PWR_CTL_Pin);
		break;
	case GPIO_MODULE_PWRKEY: //模块的PWRKEY
		state == on ? GPIO_SetBits(ONOFF_GPIO_Port, ONOFF_Pin) : GPIO_ResetBits(ONOFF_GPIO_Port, ONOFF_Pin);
		break;
	default:
		break;
	}
}
#endif

#if 1 //外部中断设置（旋钮）
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#if 1 //双EXTI中断
	//BTN_4/BTN_5引脚已设置为双边捕获
	if (GPIO_Pin == BTN4_Pin)
	{
		knob_line_A_detection();
	}

	if (GPIO_Pin == BTN5_Pin)
	{
		knob_line_B_detection();
	}
#else
	//BTN3引脚已设置为双边捕获
	if (GPIO_Pin == BTN4_Pin)
	{
		if (get_gpio_state(GPIO_BTN_4) == on)
		{
			if (get_gpio_state(GPIO_BTN_5) == on)
			{
				step_switch = STEP_LEFT; //顺时针
			}
			else
			{
				step_switch = STEP_RIGHT; //逆时针
			}
		}
		else
		{
			if (get_gpio_state(GPIO_BTN_5) == on)
			{
				step_switch = STEP_RIGHT; //逆时针
			}
			else
			{
				step_switch = STEP_LEFT; //顺时针
			}
		}
	}
#endif
}

#endif

#if 1 //串口相关设置
FlagStatus UartReady;
//发指令给模块函数

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    UartReady = SET;
//}

//uart中断初始化（必须调用一次HAL_UART_Receive_IT,触发HAL_UART_RxCpltCallback）

#if 1
void uart2_buf_handle(void)
{
	//    if (recv_end_flag == 1) //接收完成标志
	//    {
	//        for (u16 i = 0; i < rx_len; i++)
	//        {
	//            //拷贝数据
	//            if (((poc.module_data.write_p + 1) % UART_RXBUF_LEN_MAX) != poc.module_data.read_p)
	//            {
	//                poc.module_data.recv_buf[poc.module_data.write_p++] = usart2_rx_buffer[i]; //接收数据
	//            }
	//            else
	//            {
	//                /* code */
	//            }
	//            poc.module_data.write_p %= UART_RXBUF_LEN_MAX;
	//            poc.module_data.status = UART_STATUS_RECVING;
	//        }

	//        rx_len = 0;        //清除计数
	//        recv_end_flag = 0; //清除接收结束标志位

	//        // memset(usart2_rx_buffer_2, 0, BUF_TEMP_SIZE);
	//        // memcpy(usart2_rx_buffer_2, usart2_rx_buffer, BUF_TEMP_SIZE); //接收数据
	//        memset(usart2_rx_buffer, 0, sizeof(usart2_rx_buffer));
	//    }
}
#endif

#if 0 //尝试使用UART2+DMA
void UsartReceive_IDLE(UART_HandleTypeDef *huart)
{
      recv_end_flag = 1;                  // 接受完成标志位置1
      __HAL_UART_CLEAR_IDLEFLAG(&huart2); //清除标志位
      HAL_UART_DMAStop(&huart2);
      temp = __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);         // 获取DMA中未传输的数据个数
      rx_len = BUF_TEMP_SIZE - temp;                         //总计数减去未传输的数据个数，得到已经接收的数据个数
      HAL_UART_Receive_DMA(&huart1, rx_buffer, BUF_TEMP_SIZE); //重新打开DMA接收
}
#else
//void UsartReceive_IDLE(UART_HandleTypeDef *huart)
//{
//    uint32_t temp;
//    recv_end_flag = 1;                  // 接受完成标志位置1 
//    __HAL_UART_CLEAR_IDLEFLAG(&huart2); //清除标志位

//    //空闲回调函数里不要用HAL_DMA_STOP()，而是用HAL_UART_AbortReceive()来代替。
//    //原因是，HAL_DMA_STOP()关闭DMA后，到再次打开DMA接收前，如果有数据过来，会出现ORE错误，引起错误中断，进而无法触发DMA请求。
//    //而HAL_UART_AbortReceve()里，关闭了CR3寄存器的EIE，不会再触发过载中断。
//    HAL_UART_AbortReceive(&huart2);

//    temp = __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);                  // 获取DMA中未传输的数据个数
//    rx_len = BUF_TEMP_SIZE - temp;                                  //总计数减去未传输的数据个数，得到已经接收的数据个数
//    HAL_UART_Receive_DMA(&huart2, usart2_rx_buffer, BUF_TEMP_SIZE); //重新打开DMA接收
//}

#endif

/*
uart接收回调函数

触发此回调函数必须先调用一次HAL_UART_Receive_IT
HAL_UART_Receive_IT通过设置接收缓冲区和需要接收的数据个数。
当数据接收达到设定个数后引发一次中断调用回调函数HAL_UART_RxCpltCallback。
由于只引发一次中断，如果需要连续接收，则需要在HAL_UART_RxCpltCallback再调用HAL_UART_Receive_IT。
这种定长的接收可能并不是想要的，往往传输的数据都是不定长的，将HAL_UART_Receive_IT长度设置为1，然后自己根据接收的数据判断。
*/
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    static u8 one_data = 0;

//    if (huart == &huart1) //由于回调函数没有指明是哪个串口引发的中断，因此有必要在回调函数中做判断
//    {
//        gps_device.gps_data.rx_buf[gps_device.gps_data.rx_buf_len++] = one_data; //接收数据

//        //当判断以0x0D和0x0A收尾时，算一个数据包
//        if (gps_device.gps_data.rx_buf[gps_device.gps_data.rx_buf_len - 1] == '\r' ||
//            gps_device.gps_data.rx_buf[gps_device.gps_data.rx_buf_len - 1] == '\n')
//        {
//            gps_device.gps_data.rx_buf_len = 0;
//            gps_device.gps_data.one_package_rx_end_flag = true;
//        }
//        else
//        {
//            gps_device.gps_data.one_package_rx_end_flag = false;
//        }

//        HAL_UART_Receive_IT(huart, (uint8_t *)&one_data, 1);
//    }
//}
#endif

#if 1 //定时器相关设置
//tim中断初始化（必须调用一次 HAL_TIM_Base_Start_IT ,触发 HAL_TIM_PeriodElapsedCallback ）
void tim_it_init(void)
{
	//HAL_TIM_Base_Start_IT(&htim6);
}

/*
定时器回调函数

按键扫描使用定时器TIM6
TIM6的输入频率为48Mhz，Prescaler=4800-1，Period=10-1
output_freq=48000000/(4800*10)=1000hz
output_time=1/output_freq=1ms
TIM6中断1ms进一次

按键扫描使用定时器TIM6
TIM6的输入频率为24Mhz，Prescaler=4800-1，Period=10-1
output_freq=24000000/(4800*10)=500hz
output_time=1/output_freq=2ms
TIM6中断1ms进一次

*/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    static u32 custom_date_time_count = 0;

//    if (htim->Instance == htim6.Instance)
//    {
//        key_scan();
//        keyboard_value_identify();

//        custom_date_time_count++;
//        if (custom_date_time_count > 500) //2ms*500=1s
//        {
//            custom_date_time_count = 0;

//            //1s进一次
//            custom_date_time_timer();
//        }
//    }
//}

#endif

#if 1 //adc相关设置

void adc_start_init(void)
{
	//获取4组adc放入all_adc结构体
	//HAL_ADC_Start_DMA(&hadc, (u32 *)&all_adc.bat_adc_value, 4);
}

#endif

void SystemClock_Config8M(void)
{
#ifdef SYSTEM_CLOCL_8M
	RCC_DeInit();
	RCC_HSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
		;
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_LSEConfig(RCC_LSE_OFF);
	RCC_PLLCmd(DISABLE);

	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLKConfig(RCC_SYSCLK_Div1);

	while (RCC_GetSYSCLKSource() != 0x00)
		;

#endif
}

void SystemClock_Config48M(void)
{
#ifdef SYSTEM_CLOCL_8M
	/* Enable Prefetch Buffer and set Flash Latency */
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	/* HCLK = SYSCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

	/* PCLK = HCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;

	/* PLL configuration = (HSI/2) * 12 = ~48 MHz */
	RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL12); //48M
	//RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL4);
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

	/* Wait till PLL is ready */
	while ((RCC->CR & RCC_CR_PLLRDY) == 0)
	{
	}
	/* Select PLL as system clock source */
	RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

	/* Wait till PLL is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
	{
	}
#endif
}

//时钟选择（48MHz->8MHz）
void select_system_clock_48M_or_8M(clock_type type)
{
#ifdef SYSTEM_CLOCL_8M
	u8 one_data = 0;
	if (type == CLOCK_48M)
	{
		delay_ms(50); //错开时间，解决被抢登录时收不到8200指令，测试生效
		poc.system_clock_flag = CLOCK_48M;
		SystemClock_Config48M();
		uart1_config_9600();
		uart2_config();
		//TIM6_init();
		SEGGER_RTT_printf(0, "%s!!! INTO 48M !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);
	}
	else if (type == CLOCK_8M)
	{
		poc.system_clock_flag = CLOCK_8M;
		SystemClock_Config8M();
		uart1_config_9600();
		uart2_config();
		//TIM6_init_for_8M();可能会导致TIM6_init降频唤醒，不显示用户名的问题
		SEGGER_RTT_printf(0, "%s!!! INTO 8M !!!%s\r\n", RTT_CTRL_BG_BRIGHT_RED, RTT_CTRL_RESET);
	}
	else
	{
		/* code */
	}
#endif
}

void delay_ms(u32 ms)
{
	#if 1
	rt_thread_mdelay(ms);
	#else
	uint32_t i, j;

	SEGGER_RTT_printf(0, "delay_ms(%d)\r\n", ms);
	if (poc.system_clock_flag == CLOCK_8M) //8M
	{
		for (j = 0; j < ms; j++)
		{
			for (i = 0; i < 1000; i++)
				;
		}
	}
	else //48M
	{
		for (j = 0; j < ms; j++)
		{
			for (i = 0; i < 8000; i++)
				;
		}
	}
	#endif
}

#ifdef UART1_DEVICE
static void uart1_config_9600(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{
	}
	USART_DirectionModeCmd(USART1, USART_Mode_Rx, ENABLE);
	USART_RequestCmd(USART1, USART_Request_RXFRQ, ENABLE);

	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	gps_device.gps_data.read_p = 0;
	gps_device.gps_data.write_p = 0;
	memset((void *)gps_device.gps_data.recv_buf, 0, UART_RXBUF_LEN_MAX);
	gps_device.gps_data.status = UART_STATUS_NORMAL;
}
#endif

#ifdef UART2_DEVICE

void uart2_buf_clear(void)
{
	poc.module_data.read_p = 0;
	poc.module_data.write_p = 0;
	memset((void *)poc.module_data.recv_buf, 0, UART_RXBUF_LEN_MAX);
	poc.module_data.status = UART_STATUS_NORMAL;
}

static void uart2_config(void)
{
	// PA2, PA3 , 115200 8-N-1

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{
	}
	USART_DirectionModeCmd(USART2, USART_Mode_Rx, ENABLE);
	USART_RequestCmd(USART2, USART_Request_RXFRQ, ENABLE);

	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

#define UART_POC_SEND_DELAY									//发送一条指令后延时
void uart2_send_string(const uint8_t *str, uint16_t strlen) //poc模块 每次发送指令需要延时一段时间
{
#ifdef UART_POC_SEND_DELAY
	static uint32_t last_send_timer = 0;
	if (SysTick_Cnt > last_send_timer && SysTick_Cnt - last_send_timer < 25)
	{
		while (SysTick_Cnt - last_send_timer > 25)
			;
		last_send_timer = SysTick_Cnt;
	}
#endif
	while (strlen != 0)
	{
		while_timeout = 10;
		while (!((USART2->ISR) & (1 << 7)))
			; //等待发送完
		USART2->TDR = *str;
		str++;
		strlen--;
		if (while_timeout == 0x00)
		{
			USART_DeInit(USART2);
			uart2_config();
			break;
		}
	}
}
#endif

void module_at_tx(char *cmd)
{
	//    HAL_UART_Transmit(&huart2, (u8 *)cmd, strlen((char *)cmd), 0xffff);
	uart2_send_string((u8 *)cmd, strlen((char *)cmd));
	SEGGER_RTT_printf(0, "TX->%s\r\n", cmd);
}

void uart_it_init(void)
{
}

#ifdef LCD_PIN_INIT

#define LCD_PIN_NUM 13

//8080 时序，需要 14个GPIO    8根数据线， CS RS RW RST RD BK
const uint16_t lcd_pin[LCD_PIN_NUM] = {
	LCD_RST_Pin,
	LCD_CS_Pin,
	LCD_RS_Pin,
	LCD_WR_Pin,
	LCD_RD_Pin,
	LCD_DAT0_Pin,
	LCD_DAT1_Pin,
	LCD_DAT2_SPI_MOSI_Pin,
	LCD_DAT3_Pin,
	LCD_DAT4_Pin,
	LCD_DAT5_Pin,
	LCD_DAT6_Pin,
	LCD_DAT7_Pin};

const GPIO_TypeDef *lcd_port[LCD_PIN_NUM] = {
	LCD_RST_GPIO_Port,
	LCD_CS_GPIO_Port,
	LCD_RS_GPIO_Port,
	LCD_WR_GPIO_Port,
	LCD_RD_GPIO_Port,
	LCD_DAT0_GPIO_Port,
	LCD_DAT1_GPIO_Port,
	LCD_DAT2_SPI_MOSI_GPIO_Port,
	LCD_DAT3_GPIO_Port,
	LCD_DAT4_GPIO_Port,
	LCD_DAT5_GPIO_Port,
	LCD_DAT6_GPIO_Port,
	LCD_DAT7_GPIO_Port};

static void lcd_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i;
	for (i = 0; i < LCD_PIN_NUM; i++)
	{
		rcc_init_GPIO_en((GPIO_TypeDef *)lcd_port[i]);
		GPIO_InitStructure.GPIO_Pin = lcd_pin[i];
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init((GPIO_TypeDef *)lcd_port[i], &GPIO_InitStructure);
		GPIO_ResetBits((GPIO_TypeDef *)lcd_port[i], lcd_pin[i]);
	}
}
#endif

#ifdef ALL_PIN_INIT

static void else_output_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	rcc_init_GPIO_en((GPIO_TypeDef *)HC595_DATA_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = HC595_DATA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)HC595_DATA_GPIO_Port, &GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)HC595_DATA_GPIO_Port, HC595_DATA_Pin);

	rcc_init_GPIO_en((GPIO_TypeDef *)HC595_LCLK_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = HC595_LCLK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)HC595_LCLK_GPIO_Port, &GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)HC595_LCLK_GPIO_Port, HC595_LCLK_Pin);

	rcc_init_GPIO_en((GPIO_TypeDef *)HC595_SCLK_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = HC595_SCLK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)HC595_SCLK_GPIO_Port, &GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)HC595_SCLK_GPIO_Port, HC595_SCLK_Pin);

	rcc_init_GPIO_en((GPIO_TypeDef *)ONOFF_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = ONOFF_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)ONOFF_GPIO_Port, &GPIO_InitStructure);
	GPIO_SetBits((GPIO_TypeDef *)ONOFF_GPIO_Port, ONOFF_Pin);

	rcc_init_GPIO_en((GPIO_TypeDef *)SPI_SCLK_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = SPI_SCLK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)SPI_SCLK_GPIO_Port, &GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)SPI_SCLK_GPIO_Port, SPI_SCLK_Pin);

	rcc_init_GPIO_en((GPIO_TypeDef *)SPI_CS_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = SPI_CS_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)SPI_CS_GPIO_Port, &GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)SPI_CS_GPIO_Port, SPI_CS_Pin);

	rcc_init_GPIO_en((GPIO_TypeDef *)PWR_CTL_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = PWR_CTL_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)PWR_CTL_GPIO_Port, &GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)PWR_CTL_GPIO_Port, PWR_CTL_Pin);
}

static void else_input_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	rcc_init_GPIO_en(HPH_DET_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = HPH_DET_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(HPH_DET_GPIO_Port, &GPIO_InitStructure);

	rcc_init_GPIO_en(PTT_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = PTT_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PTT_GPIO_Port, &GPIO_InitStructure);

	rcc_init_GPIO_en(SPI_MISO_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStructure);

	rcc_init_GPIO_en(KEY_UP_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = KEY_UP_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_UP_GPIO_Port, &GPIO_InitStructure);

	rcc_init_GPIO_en(KEY_DN_GPIO_Port);
	GPIO_InitStructure.GPIO_Pin = KEY_DN_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(KEY_DN_GPIO_Port, &GPIO_InitStructure);
}

#endif

#ifdef ADC_DEVICE

static void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = BAT_DET_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BAT_DET_GPIO_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = VOL_CTL_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VOL_CTL_GPIO_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = K1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(K1_GPIO_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = K2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(K2_GPIO_Port, &GPIO_InitStructure);
}

static void adc_device_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/*设置ADC分频因子为4 ,ADC最大时间不能超过14M*/
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);
	/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC1 DeInit */
	ADC_DeInit(ADC1);

	/*初始化ADC结构体，此句必须加，不加的话多路ADC数据会交换*/
	ADC_StructInit(&ADC_InitStructure);

	/*配置ADC分辨率为12位*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

	/*开启连续转换*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	/*禁止触发检测，使用软件触发*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	/*ADC采集数据右对齐*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

	/*向上扫描*/
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;

	/*ADC初始化*/
	ADC_Init(ADC1, &ADC_InitStructure);

	/*配置采样通道及时间*/
	ADC_ChannelConfig(ADC1, BAT_DET_Pin, ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, VOL_CTL_Pin, ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, K1_Pin, ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1, K2_Pin, ADC_SampleTime_239_5Cycles);

	/* ADC 校准 */
	ADC_GetCalibrationFactor(ADC1);

	/* 循环模式下的 ADC DMA 请求 */
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

	/* 使能 ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 等待 ADCEN 标志 */
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
		;

	/*  ADC1 常规软件启动转换 */
	ADC_StartOfConversion(ADC1);

	ADC_DMA_Init();
}

static void ADC_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* 使能DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* 复位DMA1_Channel1 */
	DMA_DeInit(DMA1_Channel1);

	/*DMA外设ADC基地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40012440; //ADC1_DR_Address

	/*DMA内存基地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&all_adc.bat_adc_value;

	/*外设作为数据传输源*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

	/*DMA通道的DMA缓存的大小*/
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNELS * 1; //ADC_SAMPLE_COUNT

	/*外设地址寄存器不变*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*内存地址寄存器递增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/*外设取值大小设置为半字*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

	/*数据大小设置为半字*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

	/*DMA循环模式，即完成后重新开始覆盖*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	/*DMA优先级设置为高*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

	/*DMA通道x禁用内存到内存传输*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*DMA初始化*/
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/*清除一次DMA中断标志*/
	DMA_ClearITPendingBit(DMA1_IT_TC1);

	/*使能DMA传输完成中断*/
	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);

	/*选择DMA1通道中断*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;

	/*中断使能*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/*优先级设为0 */
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;

	/*使能 DMA 中断*/
	NVIC_Init(&NVIC_InitStructure);

	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
#endif

#ifdef TIM6_DEVICE
/*
定时器回调函数

按键扫描使用定时器TIM6
TIM6的输入频率为48Mhz，Prescaler=4800-1，Period=10-1
output_freq=48000000/(4800*10)=1000hz
output_time=1/output_freq=1ms
TIM6中断1ms进一次

按键扫描使用定时器TIM6
TIM6的输入频率为24Mhz，Prescaler=4800-1，Period=10-1
output_freq=24000000/(4800*10)=500hz
output_time=1/output_freq=2ms
TIM6中断1ms进一次

*/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    static u32 custom_date_time_count = 0;

//    if (htim->Instance == htim6.Instance)
//    {
//        key_scan();
//        keyboard_value_identify();

//        custom_date_time_count++;
//        if (custom_date_time_count > 500) //2ms*500=1s
//        {
//            custom_date_time_count = 0;

//            //1s进一次
//            custom_date_time_timer();
//        }
//    }
//}

static void TIM6_init(void)
{
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_Cmd(TIM6, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	PrescalerValue = 1;
	PrescalerValue = PrescalerValue;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 4800 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM6, TIM_FLAG_Update);

	/* Prescaler configuration */
	///TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* TIM IT enable */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	TIM6->CNT = 0;
	TIM_Cmd(TIM6, ENABLE);
}

//static void TIM6_init_for_8M(void)
//{
//	uint16_t PrescalerValue = 0;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	TIM_Cmd(TIM6, DISABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

//	/* Enable the TIM3 global Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	PrescalerValue = 1;
//	PrescalerValue = PrescalerValue;
//	/* Time base configuration */
//	TIM_TimeBaseStructure.TIM_Period = 10 - 1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 800 - 1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

//	TIM_ClearFlag(TIM6, TIM_FLAG_Update);

//	/* Prescaler configuration */
//	///TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

//	/* TIM IT enable */
//	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

//	TIM6->CNT = 0;
//	TIM_Cmd(TIM6, ENABLE);
//}
#endif

#ifdef EXTI_DEVICE

static void knob_init(void)
{
	//BTN_4/BTN_5引脚设置为双边捕获
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//初始化中断输入脚
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

#endif
