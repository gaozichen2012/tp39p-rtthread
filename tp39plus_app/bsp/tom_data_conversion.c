/*
	2020.02.27 tom update
	dec-Decimal 十进制
	hex-Hexadecimal十六进制
*/
#define TOM_DATA_CONVERSION
#include "tom_data_conversion.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#if 1 //已经验证测试ok的函数

void tom_test(void)
{
	//	char  str3[20];
	//	char  str4[20];
	//	int num1=1234567890;
	//	int num2=0x12;
	//
	//	int2str_dec(str3,num1);//12345→"12345"
	//	int2str_hex(str4,num2);//0x123→"0123"
	//	num3=str2int_dec(str3);//"12345"→12345
	//	mem_clear(str3);
	//	mem_clear(str3);
}

//清空数组
void mem_clear(void *buffer)
{
	memset(buffer, 0, sizeof(buffer));
}

/*
 十进制整数转字符串，例：123->"123"
 dst 存放转换后的字符串
 num 要转换的数
 */
void int2str_dec(void *dst, int num)
{
	itoa(num, (char *)dst, 10);
}

/*
 十六进制整数转字符串，例：0x12D->"12D"
 dst 存放转换后的字符串
 num 要转换的数
 */
void int2str_hex(void *dst, int num)
{
	itoa(num, (char *)dst, 16);
}

/*
 字符串转十进制数，例："123"->123
 num 存放转换后的数
 str 要转换的字符串
*/
int str2int_dec(const char *str)
{
	return atoi(str);
}

/*
 字符串转十六进制数，例："12D"->0x12D
 num 存放转换后的数
 str 要转换的字符串
*/

static int tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return c + 'a' - 'A';
	}
	else
	{
		return c;
	}
}

int str2int_hex(const char *str)
{
	int i;
	int n = 0;
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		i = 2;
	}
	else
	{
		i = 0;
	}
	for (; (str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'); ++i)
	{
		if (tolower(str[i]) > '9')
		{
			n = 16 * n + (10 + tolower(str[i]) - 'a');
		}
		else
		{
			n = 16 * n + (tolower(str[i]) - '0');
		}
	}
	return n;
}

#endif

//将字符串"123"转为"313233"
void hex_to_str(u8 *pbDest, u8 *pbSrc, u16 nLen)
{
	u8 ddl, ddh;
	u16 i;
	for (i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57)
			ddh = ddh + 7;
		if (ddl > 57)
			ddl = ddl + 7;
		pbDest[i * 2] = ddh;
		pbDest[i * 2 + 1] = ddl;
	}
	pbDest[nLen * 2] = '\0';
}

//将字符串"313233"转为"123"
void str_to_hex(u8 *string, u8 *pout, u16 strlen)
{
	u16 i;
	unsigned char highByte, lowByte;

	for (i = 0; i < strlen; i += 2)
	{
		highByte = toupper(string[i]);
		lowByte = toupper(string[i + 1]);

		if (highByte > 0x39)
			highByte -= 0x37;
		else
			highByte -= 0x30;

		if (lowByte > 0x39)
			lowByte -= 0x37;
		else
			lowByte -= 0x30;

		pout[i / 2] = (highByte << 4) | lowByte;
	}
	return;
}

/*
 字符串转十进制数，例："313233"->123
 num 存放转换后的数
 str 要转换的字符串
*/
int str2str2int_dec(const char *str)
{
	char tmp_buf[100];
	
	memset(tmp_buf,0,sizeof(tmp_buf));
	str_to_hex((u8*)str, (u8*)tmp_buf, strlen(str));

	return atoi(tmp_buf);
}

/***************************************************************************************
功能://将一个字符串插入到另一个字符串的指定位置
输入:	s1		->		源字符串
		s2		->		要插入的字符串
		n		->		位置
	例:s1="abcdef";s2="tyu";n=3;    结果:abctyudef
输出:插入后的字符串
***************************************************************************************/
u8 *insert(u8 *s1, u8 *s2, int n)
{
	int len1 = 0, len2 = 0, j = 0, k = 0;
	char s4[30];

	if (s1 == NULL)
		return NULL;
	if (s2 == NULL)
		return s1;
	len1 = strlen((const char *)s1);
	len2 = strlen((const char *)s2);
	if (n > len1)
		return NULL;
	for (int i = 0; i < n; i++)
	{
		j++;
	}
	for (int i = 0; i < len1; i++)
	{
		s4[k++] = s1[i];
	}
	for (int i = 0; i < len2; i++)
	{
		s1[j++] = s2[i];
	}
	for (int i = n; i < len1; i++)
	{
		s1[j++] = s4[i];
	}
	s1[j] = '\0';

	return s1;
}
