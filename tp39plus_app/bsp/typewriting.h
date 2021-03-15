#ifndef __TYPEWRITING_H
#define __TYPEWRITING_H

#include "tom_data.h"
#include "ui.h"
#include "lcd_api.h"
#include "lcd_app.h"
#include "version.h"

#define PY_PAGE_MAX_NUM 7
typedef enum
{
	K_NULL = 0, //空键，不存在
	K0,
	K1,
	K2,
	K3,
	K4,
	K5,
	K6,
	K7,
	K8,
	K9,
	K10,
	K11,
	K12,
	K13,
	K14,
	K15,
	K16,
	K17,
	K18,
	K19,
	K20,
	K21,
	K22,
	K23,
	K24,
	K25,
	K26,
	K27,
	K28,
	K29,
	K30,
	K31,
	P1,
	P2,
	P3
} KeyCode_t;
typedef struct
{
	KeyCode_t Code;
} Key_t, *Key_p;

typedef enum
{
	KB_STATE_OFF = 0,
	KB_STATE_ON,
} KeyBoardState_t;

#ifdef LANGUAGE_ENGLISH //英文版不用中文输入法
typedef enum
{
	KB_NUMBER = 0, //数字
	KB_ENGLISH,	   //英文
	KB_SYMBLE,	   //符号
	KB_PINYIN,	   //拼音
} KeyBoardType_t;
#else
typedef enum
{
	KB_NUMBER = 0, //数字
	KB_ENGLISH,	   //英文
	KB_PINYIN,	   //拼音
	KB_SYMBLE,	   //符号
} KeyBoardType_t;
#endif

typedef struct
{
	Key_t KeyBuf[10];
	///uint16_t KeyDownTimer[10];		/// 按键按下记数【缓存多个按键】
	u8 InCharBuf[3]; /// 0:更新标志  1: 重选标志 2：输入字符
	u8 KeyIndex_Head;
	u8 KeyIndex_Rear;
	KeyBoardType_t Type;
	KeyBoardType_t PreType;
	KeyBoardState_t state;
} KeyBoard_t, *KeyBoard_p;

typedef enum
{
	PINYIN_EN, //英文
	PINYIN_CN, //中文
} PinyinMode_t;

typedef struct
{
	bool Enable;
	char InputBuf[7];
	char DataBuf[128];
	u8 SelPage;
	char SelData[3];
	int8_t SelIndex; //待选字-序号
	PinyinMode_t Mode;
	u8 UpdateFlag; //更新
} Pinyin_t, *Pinyin_p;

typedef struct
{
	bool ShowOn;

	bool Enable;
	u8 SelValue;
	int8_t SelIndex;
} Symble_t, *Symble_p;

#ifdef TYPEWRITING
#define TYPEWRITING_EXTERN
#else
#define TYPEWRITING_EXTERN extern
#endif

TYPEWRITING_EXTERN Pinyin_t Pinyin;
TYPEWRITING_EXTERN Symble_t Symble;
TYPEWRITING_EXTERN KeyBoard_t KeyBoard; //键盘值

TYPEWRITING_EXTERN void Symble_Init(Symble_p Symble, bool En);
TYPEWRITING_EXTERN char Symble_GetSymbleValue(u8 SymbleIndex);

TYPEWRITING_EXTERN u8 Symble_GetSymbleCount(void);
TYPEWRITING_EXTERN void Symble_SetSelectSymble(Symble_p Symble);
TYPEWRITING_EXTERN char Symble_GetSelectedSymble(Symble_p Symble);
TYPEWRITING_EXTERN char Symble_GetSymbleValue(u8 SymbleIndex);
TYPEWRITING_EXTERN bool KeyBoard_GetInputChar(char *CharData, char *ReSelFlag);
TYPEWRITING_EXTERN char KeyBoard_GetPinyinInputChar(void);
TYPEWRITING_EXTERN void ui_phone_new_contact(ui_change_type opt);
TYPEWRITING_EXTERN void ui_phone_new_contact_son_page(ui_change_type opt);
TYPEWRITING_EXTERN void KeyBoard_InputChar(void);
TYPEWRITING_EXTERN void pinyin_process(void);
TYPEWRITING_EXTERN char KeyBoard_KeyChar(u8 CharIndex);
TYPEWRITING_EXTERN void typewriting_text_gbk_display(SIZE_TYPE size, DISPLAY_TYPE display_type, u16 LocX, u16 LocY, u8 *Gb2312Data, u8 data_len, u16 uiFontColor, u16 uiBackColor);

TYPEWRITING_EXTERN void typewriting_display_chinese_11x12(u16 x, u16 y, char *gb2312_dat, u16 lenth, u16 black_color, u16 text_color);
#endif
