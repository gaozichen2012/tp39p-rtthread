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

/* 消息队列控制块 */
static struct rt_messagequeue mq;
/* 消息队列中用到的放置消息的内存池 */
static rt_uint8_t msg_pool[2048];
	
int app_vector_redirection(void)
{
	rt_memcpy((void*)0x20000000, (void*)0x08004000, 48*4); 
	SYSCFG->CFGR1 |= 0x03;
	return 0;
}
INIT_BOARD_EXPORT(app_vector_redirection);

static void redled_shine_entry(void *parameter)
{
	int result;
	char buf = 'A';
	
    while(1)
    {
/* 发送消息到消息队列中 */
				buf++;
				result = rt_mq_send(&mq, &buf, 1);
				if (result != RT_EOK)
				{
						//rt_kprintf("rt_mq_send ERR\n");
				}
        set_gpio_state(GPIO_RED_LED, on);
        rt_thread_mdelay(500);
        set_gpio_state(GPIO_RED_LED, off);
        rt_thread_mdelay(500);
    }
}

static void greenled_shine_entry(void *parameter)
{
	char buf = 0;
	
    while(1)
    {
			/* 从消息队列中接收消息 */
        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
				{
						set_gpio_state(GPIO_GREEN_LED, off);
						rt_thread_mdelay(500);
						set_gpio_state(GPIO_GREEN_LED, on);
						rt_thread_mdelay(500);
        }
    }
}

int main(void)
{
	rt_err_t result;
	rt_thread_t redled_thread,greenled_thread;
	SysTick_Config(48000);

#if (DEF_IWDG)
//init_iwdg();
#endif

		tom_interface_init();

    /* 初始化消息队列 */
    result = rt_mq_init(&mq,
                        "mqt",
                        &msg_pool[0],             /* 内存池指向 msg_pool */
                        1,                          /* 每个消息的大小是 1 字节 */
                        sizeof(msg_pool),        /* 内存池的大小是 msg_pool 的大小 */
                        RT_IPC_FLAG_FIFO);       /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
	
	
    redled_thread = rt_thread_create("ledshine", redled_shine_entry, RT_NULL,
                                  192, RT_THREAD_PRIORITY_MAX / 2, 20);
	    if (redled_thread != RT_NULL)
    {
        rt_thread_startup(redled_thread);
    }
		
		greenled_thread = rt_thread_create("ledshine", greenled_shine_entry, RT_NULL,
                                  192, RT_THREAD_PRIORITY_MAX / 2, 20);
    if (greenled_thread != RT_NULL)
    {
        rt_thread_startup(greenled_thread);
    }

		return 1;
		#if 0
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
	#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
