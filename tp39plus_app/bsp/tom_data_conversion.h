#ifndef __TOM_DATA_CONVERSION_H
#define __TOM_DATA_CONVERSION_H

#include "tom_interface.h"


#define swap16(x) ((x & 0XFF) << 8 | (x & 0XFF00) >> 8) //高低字节交换宏定义

#ifdef TOM_DATA_CONVERSION
#define TOM_DATA_CONVERSION_EXTERN
#else
#define TOM_DATA_CONVERSION_EXTERN extern
#endif

#if 1 //已经验证测试ok的函数
TOM_DATA_CONVERSION_EXTERN void tom_test(void);
TOM_DATA_CONVERSION_EXTERN void mem_clear(void *buffer);                         //清空数组
//TOM_DATA_CONVERSION_EXTERN void mem_cpy(void *str1, const void *str2, size_t n); //复制数组，str2 复制 n 个字符到存储区 str1
TOM_DATA_CONVERSION_EXTERN void int2str_dec(void *dst, int num);                 //  十进制整数转字符串，例：132->"132"
TOM_DATA_CONVERSION_EXTERN void int2str_hex(void *dst, int num);                 //十六进制整数转字符串，例：0x12D->"12D"
TOM_DATA_CONVERSION_EXTERN int str2int_dec(const char *str);                     //字符串转十进制数，例："123"->123
TOM_DATA_CONVERSION_EXTERN int str2int_hex(const char *str);                     //字符串转十六进制数，例："12D"->0x12D
#endif

TOM_DATA_CONVERSION_EXTERN void hex_to_str(u8 *pbDest, u8 *pbSrc, u16 nLen);
TOM_DATA_CONVERSION_EXTERN void str_to_hex(u8 *string, u8 *pout, u16 strlen);
TOM_DATA_CONVERSION_EXTERN int str2str2int_dec(const char *str);

TOM_DATA_CONVERSION_EXTERN u8 *insert(u8 *s1, u8 *s2, int n);
#endif
