#ifndef __FLASH_H_
#define __FLASH_H_

#if 1 //TP39P

#define FLASH_SIZE_1K		(1024)   			/* FLASH Page Size 1K*/
#define FLASH_SIZE_2K		(2048)   			/* FLASH Page Size 2K*/

#define FLASH_END_ADDR (0x08040000)
//Custom area start addr calc by mcu end addr and Custom area size
#define FLASH_USER_START_ADDR (FLASH_END_ADDR - FLASH_CONFIG_INFO_SIZE - FLASH_HOT_KEY_INFO_SIZE - FLASH_CALL_CONTACTS_SIZE - FLASH_CALL_RECORD_SIZE - FLASH_BOOT_INFO_SIZE)

//�˺�������Ϣ
#define FLASH_CONFIG_INFO_START_ADDR FLASH_USER_START_ADDR
#define FLASH_CONFIG_INFO_SIZE FLASH_SIZE_2K

//��ݼ��ڴ�
#define FLASH_HOT_KEY_INFO_START_ADDR (FLASH_CONFIG_INFO_START_ADDR + FLASH_CONFIG_INFO_SIZE)
#define FLASH_HOT_KEY_INFO_SIZE FLASH_SIZE_2K //ʵ��ʹ�ø�ʽ (1�ֽڿ�ݼ��������� +  9�ֽ�ID��� + 48�ֽ�name) X 10 =580�ֽ�
#define FLASH_HOT_KEY_INFO_1KEY_SIZE 58       //��������Ҫ�洢�������Ϣ�ֽ���

//�绰��ϵ��
#define FLASH_CALL_CONTACTS_START_ADDR (FLASH_HOT_KEY_INFO_START_ADDR + FLASH_HOT_KEY_INFO_SIZE)
#define FLASH_CALL_CONTACTS_SIZE FLASH_SIZE_2K
#define FLASH_CALL_CONTACTS_1CONTACT_SIZE

//�绰��¼
#define FLASH_CALL_RECORD_START_ADDR (FLASH_CALL_CONTACTS_START_ADDR + FLASH_CALL_CONTACTS_SIZE)
#define FLASH_CALL_RECORD_SIZE FLASH_SIZE_2K
#define FLASH_CALL_RECORD_1RECORD_SIZE


//Զ����������������־λ+�汾�ţ�
#define FLASH_BOOT_INFO_START_ADDR (FLASH_CALL_RECORD_START_ADDR + FLASH_CALL_RECORD_SIZE)
#define FLASH_BOOT_INFO_SIZE FLASH_SIZE_2K
#define FLASH_BOOT_INFO_BOOTFLAG_SIZE 4
#define FLASH_BOOT_INFO_VER_START_ADDR (FLASH_BOOT_INFO_START_ADDR + FLASH_BOOT_INFO_BOOTFLAG_SIZE)
#endif

#define FLASH_ADDR_RESERVE    (0x08000000 +1024*248)          //248K  803E000
#define FLASH_ADDR_BOOT_FLAG  FLASH_BOOT_INFO_START_ADDR  
#define FLASH_ADDR_USER_START FLASH_CONFIG_INFO_START_ADDR  //2k


void Flash_Erase(uint32_t StartAddr,uint32_t Size);
void Flash_Updata(uint32_t StartAddr,uint8_t * Data,uint32_t Size);
void Flash_Write(uint32_t StartAddr,uint8_t * Data,uint32_t Size);//ע��Ҫ��4�ֽڶ���
void Flash_Read(uint32_t StartAddr,uint8_t * Buffer, uint32_t Size);
void Flash_Move(uint32_t DecAddr, uint32_t SrcAddr, uint32_t Size);
void Flash_rdp(void);


#endif





