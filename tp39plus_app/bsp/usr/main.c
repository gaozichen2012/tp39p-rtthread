//Includes
#include "stm32f0xx.h"
#include "tom_interface.h"
#include "lcd_api.h"
#include "poc.h"
#include "gps.h"
#include "timed.h"
#include "key.h"
#include "main_process.h"
#include "ui.h"
#include "telephone.h"
#include "typewriting.h"
#include <rtthread.h>
#if 1
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APPLICATION_ADDRESS (uint32_t)0x08004000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (defined(__CC_ARM))
__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined(__ICCARM__))
#pragma location = 0x20000000
__no_init __IO uint32_t VectorTable[48];
#elif defined(__GNUC__)
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#endif

#define __HAL_SYSCFG_REMAPMEMORY_SRAM()                                       \
	do                                                                        \
	{                                                                         \
		SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE);                            \
		SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1); \
	} while (0)
#endif

int app_vector_redirection(void)
{
	rt_memcpy((void*)0x20000000, (void*)0x08004000, 48*4); 
	SYSCFG->CFGR1 |= 0x03;
	return 0;
}
INIT_BOARD_EXPORT(app_vector_redirection);
	
int main(void)
{
	uint8_t i;
	SysTick_Config(48000);

	/* Copy the vector table from the Flash (mapped at the base of the application
		 load address 0x08004000) to the base address of the SRAM at 0x20000000. */
	for (i = 0; i < 48; i++)
	{
		VectorTable[i] = *(__IO uint32_t *)(APPLICATION_ADDRESS + (i << 2));
	}

	/* Enable the SYSCFG peripheral clock*/
	//  __HAL_RCC_SYSCFG_CLK_ENABLE();
	/* Remap SRAM at 0x00000000 */
	__HAL_SYSCFG_REMAPMEMORY_SRAM();
	NVIC_DisableIRQ(EXTI4_15_IRQn);
	delay_ms(10);

#if (DEF_IWDG)
//init_iwdg();
#endif

	/* USER CODE BEGIN 2 */
	main_init(0);
#if 1
	SEGGER_RTT_ConfigUpBuffer(0, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	SEGGER_RTT_ConfigDownBuffer(0, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	SEGGER_RTT_SetTerminal(0);
	SEGGER_RTT_printf(0, "SEGGER_RTT_run\r\n");
	//	SEGGER_RTT_printf(0,"%s-%d:ui.page=%s\r\n",__FILE__,__LINE__,uiPagebuf[UI.Page]);
	//	SEGGER_RTT_printf(0,"%s %s-%d:Poc_AudioOnOff(Poc,OFF)%s\r\n",RTT_CTRL_BG_BRIGHT_GREEN,__FILE__,__LINE__,RTT_CTRL_RESET);
	//	SEGGER_RTT_printf(0,"%s %s-%d:Poc->Config.InviteEnable=%d%s\r\n",RTT_CTRL_BG_BRIGHT_GREEN,__FILE__,__LINE__,Poc->Config.InviteEnable,RTT_CTRL_RESET);

#endif

	/* USER CODE END 2 */

	while (1)
	{
		#if 1
		set_gpio_state(GPIO_RED_LED, off);
		set_gpio_state(GPIO_GREEN_LED, on);
		rt_thread_mdelay(500);
		set_gpio_state(GPIO_RED_LED, on);
		set_gpio_state(GPIO_GREEN_LED, off);
		rt_thread_mdelay(500);
		#else
		main_process_key();

		poc_first_deal(); //poc_process(poc.module_data.rx_buf);
		gps_task();
		timed_main_process();
		ui_invite(UI_OPT_OTHER_REFRESH);

#if (DEF_IWDG)
//CLRWDT();
#endif
		#endif
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
