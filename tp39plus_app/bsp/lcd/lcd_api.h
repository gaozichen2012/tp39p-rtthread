#ifndef __LCD_API_H
#define __LCD_API_H

#include "tom_data.h"
#include "version.h"

#define COLOR_WIGHT 0xFFFF         //白色
#define COLOR_BLACK 0x0000         //黑色
#define COLOR_RED 0xF800           //红色
#define COLOR_GREEN 0x07E0         //绿色
#define COLOR_BLUE 0x001F          //蓝色
#define COLOR_YELLOW 0xFFE0        //黄色
#define COLOR_CYAN 0x07FF          //青色
#define COLOR_PURPLE 0x780F        //紫色
#define COLOR_BABY_BLUE1 0xEFCF    //浅蓝
#define COLOR_BABY_BLUE2 0x067A    //绿松色
#define COLOR_BABY_BLUE3 0x0213    //深蓝
#define COLOR_39P_BLUE 0x393A      //39P蓝
#define COLOR_GRAY 0X8410          //灰色
#define COLOR_NAVYBLUE 0X0009      //藏青色
#define COLOR_PINK 0X39C7          //棕黑色 0XE497 //粉色
#define COLOR_RUST 0XDE0F          //铁锈色口红色
#define COLOR_BLACK2 0x2965        //雅黑
#define COLOR_QUBLUE 0xcf9e        //接近白色
#define COLOR_SSBLUE 0x1B9D        //深蓝
#define COLOR_LOG_BACKCOLOR 0xEF5D ///开机背景 灰色编码

#define COLOR_TYT_LOG 0xEF5D //R239 G235 B239->1110111101011101
#define LENOVO_RED 0xE103    //R230 G33 B25 PS吸取RGB值转换为RGB565值
#define LENOVO_BLUE 0x2A53   //R41 G74 B154

#ifdef LENOVO
#define COLOR_BACKGROUND COLOR_WIGHT
#define COLOR_FONT LENOVO_BLUE
#define COLOR_ICO_BACKGROUND LENOVO_BLUE
#define COLOR_ICO_FONT COLOR_WIGHT
#else
#define COLOR_BACKGROUND COLOR_WIGHT
#define COLOR_FONT COLOR_39P_BLUE
#define COLOR_ICO_BACKGROUND COLOR_39P_BLUE
#define COLOR_ICO_FONT COLOR_WIGHT
#endif

/* 显示字符大小16*16 或 12*12 */
typedef enum
{
    size_12,
    size_16,
} SIZE_TYPE;

/* 显示类型 文本居左-居中-居右-自定义 */
typedef enum
{
    REFLASH_KEY_UP,
    REFLASH_KEY_DOWN,
    REFLASH_ALL, //手
} DISPLAY_REFLASH_TYPE;

/* 显示类型 文本居左-居中-居右-自定义 */
typedef enum
{
    display_left,
    display_middle,
    display_right,
    display_other, //手
} DISPLAY_TYPE;

#ifdef LCD_API
#define LCD_API_EXTERN
#else
#define LCD_API_EXTERN extern
#endif
LCD_API_EXTERN void lcd_api_init(void);
LCD_API_EXTERN void lcd_api_normal_item_disp(int current_item, char *item_name[], u8 items_num_max, u16 actual_item, DISPLAY_REFLASH_TYPE refresh_type);
LCD_API_EXTERN void lcd_api_normal_item_disp_with_group_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type);
LCD_API_EXTERN void lcd_api_normal_item_disp_with_userfriend_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type);
LCD_API_EXTERN void lcd_api_normal_item_disp_with_contacts_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type);
LCD_API_EXTERN void lcd_api_normal_item_disp_with_record_list(int current_item, u8 items_num_max, DISPLAY_REFLASH_TYPE refresh_type);
LCD_API_EXTERN void lcd_api_text_scroll_display(u8 x, u8 y, u8 max_len, bool refresh_type, char *input_text_buf);
LCD_API_EXTERN void lcd_api_text_gbk_display(SIZE_TYPE size, DISPLAY_TYPE display_type, u16 LocX, u16 LocY, u8 *Gb2312Data, u16 uiFontColor, u16 uiBackColor);
LCD_API_EXTERN void lcd_api_text_unicode_display(SIZE_TYPE size, u16 LocX, u16 LocY, u8 *unicode_data, u8 data_len, u16 uiFontColor, u16 uiBackColor);
LCD_API_EXTERN void lcd_api_clear_a_line(SIZE_TYPE size, u8 y);

LCD_API_EXTERN void lcd_api_clear(u8 x, u8 y, u16 colorData, u8 width, u8 height);
LCD_API_EXTERN void lcd_api_bmp(u8 x, u8 y, const u8 *bmp, u8 width, u8 height);
LCD_API_EXTERN void lcd_api_bmp_with_background(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 back_color);
LCD_API_EXTERN void lcd_api_bmp_with_one_color(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 colorData, u16 back_color);
LCD_API_EXTERN void lcd_api_x_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color);
LCD_API_EXTERN void lcd_api_y_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color);
LCD_API_EXTERN void lcd_api_tan(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 X2, u16 Y2, u16 LineWidth, u16 Color);

LCD_API_EXTERN void lcd_api_display_range(u16 x, u16 y, u16 width, u16 height, u16 line_color);
#endif
