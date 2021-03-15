/*
确保程序的所占的flash空间小于自定义flash的起始点
flash目前所占大小为103112byte=‭0x192C8，所以程序所占内存为‬0x8004000+0x192C8=0x‭801D2C8‬
目前自定义flash的起始点为‭0x803E000‬>>0x‭801D2C8‬,正常



STM32的ROM就是FLASH
STM32的代码也是存在FLASH里面，即起始地址为0x0800_0000
STM32的MCU远程升级是直接将代码下载到APP起始点0x0800_4000，并没有什么备份区
STM32F103VE flash addr is 0x0800_0000-0x0808_0000, total of 0x80000=512k
STM32F030CC flash addr is 0x0800_0000-0x0804_0000, total of 0x40000=256k

PART1:bootloader    addr:0x0800_0000-0x0800_4000    size:
PART2:code          addr:0x0800_4000-0x0800_4000    size:
PART3:Custom        addr:0x0800_4000-0x0800_4000    size:2K+1K+2K+2K
 */
#ifndef __TOM_FLASH_H
#define __TOM_FLASH_H

#include "tom_data.h"

#define FLASH_SIZE_1K (1024) /* FLASH Page Size 1K*/
#define FLASH_SIZE_2K (2048) /* FLASH Page Size 2K*/

#define FLASH_END_ADDR (0x08040000)
#define FLASH_CONFIG_INFO_SIZE FLASH_SIZE_2K
#define FLASH_HOT_KEY_INFO_SIZE FLASH_SIZE_2K //实际使用格式 (1字节快捷键定义类型 +  9字节ID序号 + 48字节name) X 10 =580字节
#define FLASH_CALL_CONTACTS_SIZE FLASH_SIZE_2K
#define FLASH_CALL_RECORD_SIZE FLASH_SIZE_2K
#define FLASH_BOOT_INFO_SIZE FLASH_SIZE_2K

//Custom area start addr calc by mcu end addr and Custom area size
#define FLASH_USER_START_ADDR (FLASH_END_ADDR - FLASH_CONFIG_INFO_SIZE - FLASH_HOT_KEY_INFO_SIZE - FLASH_CALL_CONTACTS_SIZE - FLASH_CALL_RECORD_SIZE - FLASH_BOOT_INFO_SIZE)

//账号配置信息
#define FLASH_CONFIG_INFO_START_ADDR FLASH_USER_START_ADDR

//快捷键内存
#define FLASH_HOT_KEY_INFO_START_ADDR (FLASH_CONFIG_INFO_START_ADDR + FLASH_CONFIG_INFO_SIZE)
#define FLASH_HOT_KEY_INFO_1KEY_SIZE 58 //单按键需要存储的相关信息字节数

//电话联系人
#define FLASH_CALL_CONTACTS_START_ADDR (FLASH_HOT_KEY_INFO_START_ADDR + FLASH_CALL_CONTACTS_SIZE)
#define FLASH_CALL_CONTACTS_1CONTACT_SIZE

//电话记录()
#define FLASH_CALL_RECORD_START_ADDR (FLASH_CALL_CONTACTS_START_ADDR + FLASH_CALL_RECORD_SIZE)
#define FLASH_CALL_RECORD_1RECORD_SIZE 72

//堆栈大小（未用到）//#define FLASH_STACK_SIZE FLASH_SIZE_2K * 2

#if 1 //此部分也会再boot程序中使用
//远程升级区（升级标志位+版本号）
#define FLASH_BOOT_INFO_START_ADDR (FLASH_CALL_RECORD_START_ADDR + FLASH_CALL_RECORD_SIZE)
#define FLASH_BOOT_INFO_BOOTFLAG_SIZE 4
#define FLASH_BOOT_INFO_VER_START_ADDR (FLASH_BOOT_INFO_START_ADDR + FLASH_BOOT_INFO_BOOTFLAG_SIZE)
#endif

#ifdef TOM_FLASH
#define TOM_FLASH_EXTERN
#else
#define TOM_FLASH_EXTERN extern
#endif

TOM_FLASH_EXTERN void flash_write(u32 StartAddr, u8 *Data, u32 Size);
TOM_FLASH_EXTERN void flash_read(u32 StartAddr, u8 *Buffer, u32 Size);

TOM_FLASH_EXTERN void save_flash_poc_data(void);
TOM_FLASH_EXTERN void read_flash_poc_data(void);

TOM_FLASH_EXTERN void save_flash_hotkey_data(void);
TOM_FLASH_EXTERN void read_flash_hotkey_data(void);
TOM_FLASH_EXTERN void save_flash_call_contacts_data(void);
TOM_FLASH_EXTERN void read_flash_call_contacts_data(void);
TOM_FLASH_EXTERN void save_flash_call_record_data(void);
TOM_FLASH_EXTERN void read_flash_call_record_data(void);

TOM_FLASH_EXTERN void save_flash_bootflag_and_ver(void);
TOM_FLASH_EXTERN void read_flash_bootflag_and_ver(void);
#endif
