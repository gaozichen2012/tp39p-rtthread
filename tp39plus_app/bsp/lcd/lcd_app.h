#ifndef __LCD_APP_H
#define __LCD_APP_H

#include "tom_data.h"

#ifdef LCD_APP
#define LCD_APP_EXTERN
#else
#define LCD_APP_EXTERN extern
#endif
LCD_APP_EXTERN void lcd_app_display_font_data(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 font_color, u16 back_color);
LCD_APP_EXTERN void lcd_app_display_init(void);
LCD_APP_EXTERN void lcd_app_display_clear(u8 x, u8 y, u16 colorData, u8 width, u8 height);
LCD_APP_EXTERN void lcd_app_display_bmp(u8 x, u8 y, const u8 *bmp, u8 width, u8 height);
LCD_APP_EXTERN void lcd_app_display_bmp_with_background(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 back_color);
LCD_APP_EXTERN void lcd_app_display_bmp_with_one_color(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 colorData, u16 back_color);
LCD_APP_EXTERN void lcd_app_display_x_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color);
LCD_APP_EXTERN void lcd_app_display_y_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color);
LCD_APP_EXTERN void lcd_app_display_tan(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 X2, u16 Y2, u16 LineWidth, u16 Color);
LCD_APP_EXTERN void lcd_app_display_chinese_15x16(u16 x, u16 y, char *gb2312_dat, u16 black_color, u16 text_color);
LCD_APP_EXTERN void lcd_app_display_chinese_11x12(u16 x, u16 y, char *gb2312_dat, u16 black_color, u16 text_color);
LCD_APP_EXTERN void lcd_app_string_uincode_to_gb2312(u8 *user_id_hex_unicode, u8 lenth, u8 *user_id_hex_gb2312);
#endif
