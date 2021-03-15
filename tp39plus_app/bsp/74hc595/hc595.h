#ifndef __HC595_H
#define __HC595_H

#include "tom_data.h"
#include "tom_interface.h"
typedef enum
{
  LCD_BL_PIN = 0x1,      //背光
  SIM_TOGGLE_PIN = 0x02, //SIM卡选择
  GREEN_LED_PIN = 0x04,  //绿灯
  RED_LED_PIN = 0x08,    //红灯
  PA_EN_PIN = 0x10,      //喇叭开关
  MD_PWR_EN_PIN = 0x20,  //
  GPS_EN_PIN = 0x40,     //GPS开关
  USB_EN_PIN = 0x80      //USB开关
} HC595_OUT_PIN;

#ifdef HC595
#define HC595_EXTERN
#else
#define HC595_EXTERN extern
#endif

HC595_EXTERN void hc595_init(void);
HC595_EXTERN void hc595_set(onoff_t onoff, HC595_OUT_PIN pin_name);

#endif
