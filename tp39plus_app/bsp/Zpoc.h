#ifndef __ZPOC_H__
#define __ZPOC_H__

typedef enum
{
	ZPOC_MSG_AT_RECV = 1,
	ZPOC_MSG_KEY = 2,
	ZPOC_MSG_TIMER = 3,
	ZPOC_MSG_UART = 4,
	ZPOC_MSG_GPS = 5,
	ZPOC_MSG_BATLOW = 6,
} ENUM_MSG_TYPE;

typedef struct
{
	unsigned long ulType;
	ENUM_MSG_TYPE enMsgType;
	void *pMsg;
	unsigned int usMsgLen;
} ZPOC_MSG_TYPE;
#endif
