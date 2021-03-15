#ifndef __FONT_H
#define __FONT_H

#include "tom_data.h"

#ifdef FONT
#define FONT_EXTERN
#else
#define FONT_EXTERN extern
#endif

FONT_EXTERN void read_font_addr(u32 addr, u8 *dat, u8 datlen);


FONT_EXTERN u8 font_get_gb2312_pixel_data_15x16(u8 *out_dat, u8 *dat_gb2312);
FONT_EXTERN u8 font_get_gb2312_pixel_data_11x12(u8 *out_dat, u8 *dat_gb2312);
FONT_EXTERN u8 font_get_ascii_pixel_data_8x16(u8 *out_dat, u8 ascii);
FONT_EXTERN u8 font_get_ascii_pixel_data_6x12(u8 *out_dat, u8 ascii);
FONT_EXTERN u8 font_get_ascii_pixel_data_5x7(u8 *out_dat, u8 ascii);
FONT_EXTERN u16 gb2312_to_unicode(u16 GBData);
FONT_EXTERN u16 unicode_to_gb2312(u16 UCSData);

#endif
