#ifndef __LCD_DRV_H
#define __LCD_DRV_H

#include "tom_data.h"

#ifdef LCD_DRV
#define LCD_DRV_EXTERN
#else
#define LCD_DRV_EXTERN extern
#endif

LCD_DRV_EXTERN void lcd_init(void);
LCD_DRV_EXTERN void drv_lcd_set_display_range(u16 x, u16 y, u16 width, u16 height);
LCD_DRV_EXTERN void lcd_write_cmd8(u8 cmd);
LCD_DRV_EXTERN void lcd_write_dat8(u8 cmd);
LCD_DRV_EXTERN void lcd_write_dat16(u16 Data);
#endif
