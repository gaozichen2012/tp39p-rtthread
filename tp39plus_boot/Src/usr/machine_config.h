#ifndef _MACHINE_CONFIG_
#define _MACHINE_CONFIG_

//#define RELEASE //正式发布版本

#define THIS_VERSION   TYT_VERSION

#define TYT_VERSION		1	//特易通版本
#define ENG_VERSION		2 //英文版

/*出厂默认版本号*/
#if THIS_VERSION==TYT_VERSION

#elif THIS_VERSION==ENG_VERSION
#define LANGUAGE_ENGLISH
#endif

#endif

