
#define LCD_APP
#include "lcd_app.h"
#include "lcd_drv.h"
#include "font.h"
#include "tom_interface.h"
#include <string.h>

static void display_line(u16 StartX, u16 StartY, u16 EndX, u16 EndY, u16 Width, u16 Color);

//显示字库点阵数据（字体颜色、背景可设置）
void lcd_app_display_font_data(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 font_color, u16 back_color)
{
    u8 i, j;
    drv_lcd_set_display_range(x, y, width - 1, height - 1);
    lcd_write_cmd8(0x2c); //以下开始数据传输

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if ((*(bmp + j + (i / 8) * width)) & (0x01 << (i % 8))) //写colordata
                lcd_write_dat16(font_color);
            else //写back_color
                lcd_write_dat16(back_color);
        }
    }
}

void lcd_app_display_init(void)
{
    lcd_init();
}

//设置底色或清屏
void lcd_app_display_clear(u8 x, u8 y, u16 colorData, u8 width, u8 height)
{
    int i, j;
    drv_lcd_set_display_range(x, y, width - 1, height - 1);
    lcd_write_cmd8(0x2c); //以下开始数据传输
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            lcd_write_dat16(colorData);
        }
    }
}

//彩色图标显示（水平扫描，16位真彩色，高位在前）（原398的显示函数，简洁）
void lcd_app_display_bmp(u8 x, u8 y, const u8 *bmp, u8 width, u8 height)
{
    u8 i, j;
    drv_lcd_set_display_range(x, y, width - 1, height - 1);
    lcd_write_cmd8(0x2c); //以下开始数据传输

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            lcd_write_dat8(*(bmp++)); //发送数据高8位
            lcd_write_dat8(*(bmp++)); //发送数据低8位
        }
    }
}

//彩色图标显示（可设置背景色）
void lcd_app_display_bmp_with_background(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 back_color)
{
    u8 i, j;

    drv_lcd_set_display_range(x, y, width - 1, height - 1);
    lcd_write_cmd8(0x2c); //以下开始数据传输

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (*(bmp) == 0xff && *(bmp + 1) == 0xff)
            {
                bmp++;
                lcd_write_dat8(back_color >> 8);
                bmp++;
                lcd_write_dat8(back_color);
            }
            else
            {
                lcd_write_dat8(*(bmp++));
                lcd_write_dat8(*(bmp++));
            }
        }
    }
}

/*
输入单色位图数据（测试正常）
横向扫描， 垂直8bit数据
数据水平，字节垂直，字节内像素反序
*/
void lcd_app_display_bmp_with_one_color(u8 x, u8 y, const u8 *bmp, u8 width, u8 height, u16 colorData, u16 back_color)
{
    u8 i, j;
    drv_lcd_set_display_range(x, y, width - 1, height - 1);
    lcd_write_cmd8(0x2c); //以下开始数据传输

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if ((*(bmp + j + (i / 8) * width)) & (0x01 << (i % 8))) //写colordata
                lcd_write_dat16(colorData);
            else //写back_color
                lcd_write_dat16(back_color);
        }
    }
}

//显示横线
void lcd_app_display_x_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color)
{
    u16 i = 0, j = 0;
    drv_lcd_set_display_range(StartX, StartY, Length - 1, Width - 1);
    lcd_write_cmd8(0x2c);
    for (i = 0; i < Width; i++)
    {
        for (j = 0; j < Length; j++)
            lcd_write_dat16(Color);
    }
}

//显示竖线
void lcd_app_display_y_line(u16 StartX, u16 StartY, u16 Length, u16 Width, u16 Color)
{
    u16 i = 0, j = 0;
    drv_lcd_set_display_range(StartX, StartY, Width - 1, Length - 1);
    lcd_write_cmd8(0x2c);
    for (i = 0; i < Width; i++)
    {
        for (j = 0; j < Length; j++)
            lcd_write_dat16(Color);
    }
}

//显示一个三角形（信号ico使用）
void lcd_app_display_tan(u16 X0, u16 Y0, u16 X1, u16 Y1, u16 X2, u16 Y2, u16 LineWidth, u16 Color)
{
    display_line(X0, Y0, X1 - 1, Y1, LineWidth, Color);
    display_line(X1, Y1, X2 - 1, Y2, LineWidth, Color);
    display_line(X2, Y2, X0, Y0, LineWidth, Color);
}

static void display_line(u16 StartX, u16 StartY, u16 EndX, u16 EndY, u16 Width, u16 Color)
{
    int y = 0, i = 0;
    int x0, y0, x1; //,y1;
    if (StartX < EndX)
    {
        x0 = StartX;
        x1 = EndX;
        y0 = StartY;
        //y1 = EndY;
    }
    else
    {
        x0 = EndX;
        x1 = StartX;
        y0 = EndY;
        //y1 = StartY;
    }
    for (i = x0; i <= x1; i++)
    {
        y = (int)((((double)(i - x0)) / ((double)(EndX - StartX))) * ((double)EndY - (double)StartY));
        lcd_app_display_clear(i, (u16)(y0 + y), Color, 1, Width);
    }
}

/*
 显示15*16中英文(单行显示，超出则丢弃)
输入x,y,坐标，中文2302，背景色，文字颜色
 */
void lcd_app_display_chinese_15x16(u16 x, u16 y, char *gb2312_dat, u16 black_color, u16 text_color)
{
    u16 i, j, lenth;
    u8 disp_buf[32] = {0};
    lenth = strlen(gb2312_dat);
    for (i = 0; i < lenth; i++)
    {
        if (gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
        {
            font_get_ascii_pixel_data_8x16(disp_buf, *(gb2312_dat + i));
            //把最下面的三行空白点移到上面去
            for (j = 0; j < 8; j++)
            {
                disp_buf[j + 8] <<= 3;
                disp_buf[j + 8] |= ((disp_buf[j] >> 5) & 0x07);
                disp_buf[j] = ((disp_buf[j] << 3) & 0xF8);
            }
            lcd_app_display_font_data(x + i * 8, y, disp_buf, 8, 8, text_color, black_color);
            lcd_app_display_font_data(x + i * 8, y + 8, &disp_buf[8], 8, 8, text_color, black_color);
        }
        else
        {
            if (font_get_gb2312_pixel_data_15x16(disp_buf, (u8 *)(gb2312_dat + i)))
            {
                lcd_app_display_font_data(x + i * 8, y, disp_buf, 16, 8, text_color, black_color);
                lcd_app_display_font_data(x + i * 8, y + 8, &disp_buf[16], 16, 8, text_color, black_color);
                i++;
            }
            else //出错了，找不到字库
            {
                //if(gb2312_dat[i])
            }
        }
    }
}

/*
显示11*12中英文(单行显示，超出则丢弃)
输入x,y,坐标，中文2302，背景色，文字颜色
 */
void lcd_app_display_chinese_11x12(u16 x, u16 y, char *gb2312_dat, u16 black_color, u16 text_color)
{
    u16 i, lenth;
    u8 disp_buf[32] = {0};
    lenth = strlen(gb2312_dat);

    for (i = 0; i < lenth; i++)
    {
        if (gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
        {
            font_get_ascii_pixel_data_6x12(disp_buf, *(gb2312_dat + i));
            lcd_app_display_font_data(x + i * 6, y, disp_buf, 6, 8, text_color, black_color);
            lcd_app_display_font_data(x + i * 6, y + 8, &disp_buf[6], 6, 5, text_color, black_color);
        }
        else
        {
            if (font_get_gb2312_pixel_data_11x12(disp_buf, (u8 *)(gb2312_dat + i)))
            {
                lcd_app_display_font_data(x + i * 6, y, disp_buf, 12, 8, text_color, black_color);
                lcd_app_display_font_data(x + i * 6, y + 8, &disp_buf[12], 12, 4, text_color, black_color);
                i++;
            }
            else //出错了，找不到字库
            {
#ifdef JIANPUZHAI
                uin_dat16 = (gb2312_dat[i]) + (gb2312_dat[(i + 1)] << 8); //0x17、0x80合成为0x1780
                if (uin_dat16 >= 0x1780 && uin_dat16 <= 0x17FF)
                {
                    lcd_disp_bit1bmp_SingleByte(x + i * 8, y, gmy_16_16_bmp_data[uin_dat16 - 0x1780], 16, 16, text_color, black_color);
                }
                i++;
#endif
            }
        }
    }
}

void lcd_app_string_uincode_to_gb2312(u8 *user_id_hex_unicode, u8 lenth, u8 *user_id_hex_gb2312)
{
    uint16_t gb2312_dat16, uin_dat16;
    u8 j, k = 0;
    //unicode string to GB2312 dat
    for (j = 0; j < lenth; j += 2)
    {
        if (user_id_hex_unicode[j + 1] != 0x00)
        {
            uin_dat16 = (user_id_hex_unicode[j]) + (user_id_hex_unicode[(j + 1)] << 8); //0x17、0x80合成为0x1780

            {
                gb2312_dat16 = unicode_to_gb2312(uin_dat16);
                memcpy(&user_id_hex_gb2312[k], (u8 *)&gb2312_dat16, 2);
            }
            k += 2;
        }
        else
        {
            user_id_hex_gb2312[k++] = (user_id_hex_unicode[j]);
        }
        if (k >= lenth)
            return;
    }
}
