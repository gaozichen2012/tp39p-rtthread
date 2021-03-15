#define TOM_FLASH
#include "stm32f0xx.h"
#include "stm32f0xx_flash.h"
#include "tom_flash.h"
#include <string.h>
#include "poc.h"
#include "telephone.h"

void flash_erase(u32 StartAddr, u32 Size)
{
	uint32_t EraseCounter = 0x00;
	uint32_t NbrOfPage = 0x00;
	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

	/* Define the number of page to be erased */
	NbrOfPage = (Size+FLASH_SIZE_1K-1) / FLASH_SIZE_1K;

	/* Erase the FLASH pages */
	for (EraseCounter = 0; (EraseCounter < NbrOfPage); EraseCounter++)
	{
		if (FLASH_ErasePage(StartAddr + (FLASH_SIZE_1K * EraseCounter))!= FLASH_COMPLETE)
		{
			FLASH_Lock();
			return;
		}
	}
	FLASH_Lock();
}

void flash_write(u32 StartAddr, u8 *Data, u32 Size)
{
	#if 0
	//??
	uint32_t Address = 0x00;
	uint32_t Data32 = 0;

	uint32_t Index=0;
	uint32_t i=0;
	uint8_t Data8[4]={ 0, 0, 0, 0 };

	FLASH_Unlock();

	Address = StartAddr;
	Index = 0;
	while (Index < Size)
	{
		Data32 = (((uint32_t)Data[Index])<<24)|(((uint32_t)Data[Index+1])<<16)|(((uint32_t)Data[Index+2])<<8)|((uint32_t)Data[Index+3]);
		if (FLASH_ProgramWord(Address, Data32) == FLASH_COMPLETE)
		{
			Address +=4;
			Index+=4;
		}
		else
		{
			while (1)
			{
				return;
			}
		}
	}
	FLASH_Lock();
	Address = StartAddr;
	Index=0;
	while (Index<Size)
	{
		Data32 = *(__IO uint32_t *)Address;
		Data8[0]= (uint8_t)(Data32 >> 24);
		Data8[1]= (uint8_t)(Data32 >> 16);
		Data8[2]= (uint8_t)(Data32 >> 8);
		Data8[3]= (uint8_t)(Data32 >> 0);
		for (i=0;i<4;i++)
		{
			if (Data8[i]!=Data[Index+i])
				return;
		}
		Address +=4;
		Index +=4;
	}
	return;	
	#else
	uint32_t i;
	uint32_t Data32;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	
	for(i=0;i<Size;i+=4)//???
	{
		Data32 = (uint32_t)Data[i] + (uint32_t)(Data[i+1] << 8)+ (uint32_t)(Data[i+2] << 16)+ (uint32_t)(Data[i+3] << 24);
		FLASH_ProgramWord( StartAddr+i, Data32);
	}
  FLASH_Lock();
	#endif
}

void flash_read(u32 StartAddr, u8 *Buffer, u32 Size)
{
	#if 0
	//??
	uint32_t i=0;
	uint32_t Index=0;
	uint32_t Address = StartAddr;
	uint32_t Data32=0;

	while (1)
	{
		Data32 = *(__IO uint32_t *)Address;
		for (i=0;i<4;i++)
		{
			if (Index<Size)
				Buffer[Index++]= (uint8_t)(Data32 >> (8*(3-i)));
			else
				return;
		}
		Address +=4;
	}

	#else
	u8 *p;
	p = (uint8_t *)StartAddr;
	memcpy(Buffer, p, Size);
	#endif
}

void save_flash_poc_data(void)
{
	flash_erase(FLASH_CONFIG_INFO_START_ADDR, FLASH_CONFIG_INFO_SIZE);
	flash_write(FLASH_CONFIG_INFO_START_ADDR, (u8 *)&poc.flash_data, sizeof(poc_flash_t));
}

void read_flash_poc_data(void)
{
	flash_read(FLASH_CONFIG_INFO_START_ADDR, (u8 *)&poc.flash_data, sizeof(poc_flash_t));
}

void save_flash_hotkey_data(void)
{
	flash_erase(FLASH_HOT_KEY_INFO_START_ADDR, FLASH_HOT_KEY_INFO_SIZE);
	flash_write(FLASH_HOT_KEY_INFO_START_ADDR, (u8 *)&poc.hotkey_info, 10 * sizeof(one_hotkey_flash_t));
}

void read_flash_hotkey_data(void)
{
	flash_read(FLASH_HOT_KEY_INFO_START_ADDR, (u8 *)&poc.hotkey_info, 10 * sizeof(one_hotkey_flash_t));
}

void save_flash_call_contacts_data(void)
{
	flash_erase(FLASH_CALL_CONTACTS_START_ADDR, FLASH_CALL_CONTACTS_SIZE);
	flash_write(FLASH_CALL_CONTACTS_START_ADDR, (u8 *)&call_contacts, sizeof(contacts_t));
}

void read_flash_call_contacts_data(void)
{
	flash_read(FLASH_CALL_CONTACTS_START_ADDR, (u8 *)&call_contacts, sizeof(contacts_t));
}

void save_flash_call_record_data(void)
{
	flash_erase(FLASH_CALL_RECORD_START_ADDR, FLASH_CALL_RECORD_SIZE);
	flash_write(FLASH_CALL_RECORD_START_ADDR, (u8 *)&call_record, sizeof(call_record_t));
}

void read_flash_call_record_data(void)
{
	flash_read(FLASH_CALL_RECORD_START_ADDR, (u8 *)&call_record, sizeof(call_record_t));
}

void save_flash_bootflag_and_ver(void)
{
	flash_erase(FLASH_BOOT_INFO_START_ADDR, FLASH_BOOT_INFO_SIZE);
	flash_write(FLASH_BOOT_INFO_START_ADDR, (u8 *)&poc.boot_info, sizeof(boot_flash_t));
}

void read_flash_bootflag_and_ver(void)
{
	flash_read(FLASH_BOOT_INFO_START_ADDR, (u8 *)&poc.boot_info, sizeof(boot_flash_t));
}
