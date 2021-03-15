#include "stm32f0xx.h"
#include "stm32f0xx_flash.h"
#include "flash.h"
#include "string.h"

void Flash_Erase(uint32_t StartAddr,uint32_t Size)
{
	uint8_t i;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	for(i=0;i<Size/0x800;i++)
		FLASH_ErasePage(StartAddr+i*0x800); //������ҳ

  FLASH_Lock(); 	
	
}

void Flash_Write(uint32_t StartAddr,uint8_t * Data,uint32_t Size)
{
	uint32_t i;
	uint32_t Data32;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	
	for(i=0;i<Size;i+=4)//д����
	{
		Data32 = (uint32_t)Data[i] + (uint32_t)(Data[i+1] << 8)+ (uint32_t)(Data[i+2] << 16)+ (uint32_t)(Data[i+3] << 24);
		FLASH_ProgramWord( StartAddr+i, Data32);
	}	
	
	
  FLASH_Lock(); 	
}


void Flash_Move(uint32_t DecAddr, uint32_t SrcAddr, uint32_t Size)
{
	uint32_t EndAddr=SrcAddr+Size; 
	uint32_t PageStart = DecAddr/0x800 ; 
	uint32_t PageEnd = (EndAddr-1)/0x800 ; 
	uint32_t EraseCounter = 0x0;
	uint32_t addr_size = SrcAddr - DecAddr;
	uint32_t i;
	static uint32_t Data32;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	

	for(EraseCounter=0; EraseCounter< (PageEnd - PageStart+1); EraseCounter++)
	{
		FLASH_ErasePage(FLASH_ADDR_RESERVE); //��������ҳ
		FLASH_ErasePage(FLASH_ADDR_RESERVE+0x800); //��������ҳ
		//д����ҳ
		for(i=0;i<0x200;i++)
		{
			Flash_Read( DecAddr/0x800*0x800+i*4 + EraseCounter*0x800,(uint8_t*)&Data32, 4);
			FLASH_ProgramWord( FLASH_ADDR_RESERVE+i*4, Data32);
		}				
		FLASH_ErasePage(DecAddr+ EraseCounter*0x800); //������ǰҳ
		for(i=0;i<0x800;i+=4)
		{
			if(i>=(DecAddr%0x800) )
				Flash_Read( FLASH_ADDR_RESERVE+i+addr_size,(uint8_t*)&Data32, 4);
			else
				Flash_Read( FLASH_ADDR_RESERVE+i,(uint8_t*)&Data32, 4);
			FLASH_ProgramWord( DecAddr/0x800*0x800+i+ EraseCounter*0x800, Data32);
		}					
	}


  FLASH_Lock(); 	
}


void Flash_Updata(uint32_t StartAddr,uint8_t * Data,uint32_t Size)
{
	uint32_t EndAddr=StartAddr+Size; 
	uint32_t PageStart = StartAddr/0x800 ; 
	uint32_t PageEnd = (EndAddr-1)/0x800 ; 
	uint32_t EraseCounter = 0x0;
	uint32_t i,j;
	static uint32_t Data32;
	FLASH_Unlock(); 
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR );
	

	for(EraseCounter=0; EraseCounter< (PageEnd - PageStart+1); EraseCounter++)
	{
		FLASH_ErasePage(FLASH_ADDR_RESERVE); //��������ҳ
		if(EraseCounter == 0)//��һҳ
		{
			if(PageEnd == PageStart)//ֻ��һҳ����
			{
				//д����ҳ
				for(i=0;i<0x200;i++)
				{
					Flash_Read( StartAddr/0x800*0x800+i*4,(uint8_t*)&Data32, 4);
					FLASH_ProgramWord( FLASH_ADDR_RESERVE+i*4, Data32);
				}		
				FLASH_ErasePage(StartAddr); //������ǰҳ
				for(i=0,j=0;i<0x800;i+=4)
				{
					if(i>=(StartAddr%0x800) && i < (StartAddr%0x800)+Size)
					{
						memcpy((uint8_t*)&Data32,Data+j,4);
						j+=4;
					}
					else
						Flash_Read( FLASH_ADDR_RESERVE+i,(uint8_t*)&Data32, 4);
					FLASH_ProgramWord( StartAddr/0x800*0x800+i, Data32);
				}				
			}
			else
			{
				
			}

		
		}
		else if(EraseCounter == PageEnd - PageStart)//���һҳ
		{
		}
		else//�м�ҳ
		{
			
		}
	}


  FLASH_Lock(); 

}

void Flash_rdp(void)
{
	//FlagStatus status = FLASH_OB_GetRDP();
	//if(status == RESET)
	{
		FLASH_Unlock();
		FLASH_OB_Unlock();
		FLASH_OB_RDPConfig(OB_RDP_Level_1);
		FLASH_OB_Lock();
		FLASH_Lock();
		FLASH_OB_Launch();
	}
	
}


void Flash_Read(uint32_t StartAddr,uint8_t * Buffer, uint32_t Size)
{
	uint8_t *p;
	p = (uint8_t*)StartAddr;
	memcpy(Buffer,p,Size);
}
