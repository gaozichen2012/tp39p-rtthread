#ifndef _MACHINE_CONFIG_
#define _MACHINE_CONFIG_

//#define RELEASE //��ʽ�����汾

#define THIS_VERSION   TYT_VERSION

#define TYT_VERSION		1	//����ͨ�汾
#define ENG_VERSION		2 //Ӣ�İ�

/*����Ĭ�ϰ汾��*/
#if THIS_VERSION==TYT_VERSION

#elif THIS_VERSION==ENG_VERSION
#define LANGUAGE_ENGLISH
#endif

#endif

