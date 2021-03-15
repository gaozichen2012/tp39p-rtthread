#ifndef __TOM_INTERFACE_H
#define __TOM_INTERFACE_H

#include "tom_data.h"
#include "tom_data_conversion.h"
#include "hc595.h"
#include <string.h>
#include <stdint.h>

#define SYSTEM_CLOCL_8M
#define LCD_PIN_INIT
#define ALL_PIN_INIT
#define ADC_DEVICE
#define UART1_DEVICE
#define UART2_DEVICE
#define TIM6_DEVICE
#define EXTI_DEVICE
#define ADC_CHANNELS 4
#define UART_BUF_LEN 120 //80

/* Private defines -----------------------------------------------------------*/
#define HC595_DATA_Pin GPIO_Pin_13
#define HC595_DATA_GPIO_Port GPIOC
#define HC595_LCLK_Pin GPIO_Pin_14
#define HC595_LCLK_GPIO_Port GPIOC
#define HC595_SCLK_Pin GPIO_Pin_15
#define HC595_SCLK_GPIO_Port GPIOC
#define PTT_Pin GPIO_Pin_0
#define PTT_GPIO_Port GPIOF
#define HPH_DET_Pin GPIO_Pin_1
#define HPH_DET_GPIO_Port GPIOF
#define PWR_CTL_Pin GPIO_Pin_0
#define PWR_CTL_GPIO_Port GPIOA
#define BAT_DET_Pin GPIO_Pin_1
#define BAT_DET_GPIO_Port GPIOA
#define VOL_CTL_Pin GPIO_Pin_4
#define VOL_CTL_GPIO_Port GPIOA
#define KEY_UP_Pin GPIO_Pin_5
#define KEY_UP_GPIO_Port GPIOA
#define K1_Pin GPIO_Pin_6
#define K1_GPIO_Port GPIOA
#define K2_Pin GPIO_Pin_7
#define K2_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_Pin_0
#define SPI_MISO_GPIO_Port GPIOB
#define SPI_SCLK_Pin GPIO_Pin_1
#define SPI_SCLK_GPIO_Port GPIOB
#define ONOFF_Pin GPIO_Pin_2
#define ONOFF_GPIO_Port GPIOB
#define LCD_DAT2_SPI_MOSI_Pin GPIO_Pin_10
#define LCD_DAT2_SPI_MOSI_GPIO_Port GPIOB
#define LCD_DAT3_Pin GPIO_Pin_11
#define LCD_DAT3_GPIO_Port GPIOB
#define LCD_DAT4_Pin GPIO_Pin_12
#define LCD_DAT4_GPIO_Port GPIOB
#define LCD_DAT5_Pin GPIO_Pin_13
#define LCD_DAT5_GPIO_Port GPIOB
#define LCD_DAT6_Pin GPIO_Pin_14
#define LCD_DAT6_GPIO_Port GPIOB
#define LCD_DAT7_Pin GPIO_Pin_15
#define LCD_DAT7_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_Pin_8
#define SPI_CS_GPIO_Port GPIOA
#define BTN5_Pin GPIO_Pin_11
#define BTN5_GPIO_Port GPIOA
#define BTN5_EXTI_IRQn EXTI4_15_IRQn
#define BTN4_Pin GPIO_Pin_12
#define BTN4_GPIO_Port GPIOA
#define BTN4_EXTI_IRQn EXTI4_15_IRQn
#define KEY_DN_Pin GPIO_Pin_15
#define KEY_DN_GPIO_Port GPIOA
#define LCD_RST_Pin GPIO_Pin_3
#define LCD_RST_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_Pin_4
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_Pin_5
#define LCD_RS_GPIO_Port GPIOB
#define LCD_WR_Pin GPIO_Pin_6
#define LCD_WR_GPIO_Port GPIOB
#define LCD_RD_Pin GPIO_Pin_7
#define LCD_RD_GPIO_Port GPIOB
#define LCD_DAT0_Pin GPIO_Pin_8
#define LCD_DAT0_GPIO_Port GPIOB
#define LCD_DAT1_Pin GPIO_Pin_9
#define LCD_DAT1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LCD_DATA_GPIO_Port GPIOB
/* USER CODE END Private defines */

//GPIO设置
typedef enum
{
    //595的8个输出脚
    GPIO_LCD_BL,        //背光
    GPIO_SIM_TOGGLE,    //SIM卡选择
    GPIO_GREEN_LED,     //绿灯
    GPIO_RED_LED,       //红灯
    GPIO_PA_EN,         //喇叭开关
    GPIO_MODULE_PWR_EN, //模块供电开关
    GPIO_GPS_EN,        //GPS开关
    GPIO_USB_EN,        //USB开关

    //默认与mcu直接连接的输出脚
    GPIO_PWR_CTL,       //整机供电开关
    GPIO_MODULE_PWRKEY, //模块的PWRKEY

    //输入脚
    GPIO_KEY_PTT, //ptt
    GPIO_KEY_UP,  //侧上键
    GPIO_KEY_DN,  //侧下键
    GPIO_HPH_DET, //耳机检测
    GPIO_BTN_4,   //旋钮1
    GPIO_BTN_5,   //旋钮2

} GPIO_T;

//GPIO设置
typedef enum
{
    STEP_NONE,  //空闲
    STEP_LEFT,  //顺时针
    STEP_RIGHT, //逆时针
} step_switch_type;

//串口结构体
typedef struct
{
    u8 rx_buf[UART_BUF_LEN]; //接收数据的数组
    //u8 rx_buf_2[UART_BUF_LEN];    //接收数据的数组
    u16 rx_buf_len;               //接收数据的长度
    bool one_package_rx_end_flag; //一个数据包结束的标志
} uart_rx_t;

#if 1 //尝试使用398串口接收程序
#define UART_RXBUF_LEN_MAX 2048
#define BUF_TEMP_SIZE 256
typedef enum
{
    UART_STATUS_NORMAL,
    UART_STATUS_RECVING,
    UART_STATUS_RECVED,
    UART_STATUS_SENDING,
} uart_status_t;

typedef struct
{
    u8 recv_buf[UART_RXBUF_LEN_MAX]; //接收buf
    u16 write_p;
    u16 read_p;
    uart_status_t status;
} device_uart_t;
#endif

//adc结构体
typedef struct
{
    //4组adc：电量adc、音量adc、键盘的2个adc
    u16 bat_adc_value;
    u16 vol_adc_value;
    u16 keyboard1_adc_value;
    u16 keyboard2_adc_value;
} all_adc_t;

//时钟主频选择
typedef enum
{
    CLOCK_48M,
    CLOCK_8M,
} clock_type;

#ifdef TOM_INTERFACE
#define TOM_INTERFACE_EXTERN
#else
#define TOM_INTERFACE_EXTERN extern
#endif

extern uint32_t SysTick_Cnt;
TOM_INTERFACE_EXTERN uint8_t while_timeout;
TOM_INTERFACE_EXTERN all_adc_t all_adc;
TOM_INTERFACE_EXTERN step_switch_type step_switch;

TOM_INTERFACE_EXTERN void tom_interface_init(void);
TOM_INTERFACE_EXTERN onoff_t get_gpio_state(GPIO_T pin);
TOM_INTERFACE_EXTERN void set_gpio_state(GPIO_T pin, onoff_t state);
TOM_INTERFACE_EXTERN void module_at_tx(char *cmd); //发指令给模块函数
TOM_INTERFACE_EXTERN void uart_it_init(void);
TOM_INTERFACE_EXTERN void tim_it_init(void);
TOM_INTERFACE_EXTERN void adc_start_init(void);
TOM_INTERFACE_EXTERN void select_system_clock_48M_or_8M(clock_type type); //时钟选择（48MHz->8MHz）

TOM_INTERFACE_EXTERN void uart2_buf_clear(void);
TOM_INTERFACE_EXTERN void delay_ms(u32 ms);
#endif
