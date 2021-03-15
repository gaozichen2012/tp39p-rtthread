#include "stm32f0xx.h"
#include "device.h"

#include "flash.h"
#include "my_string.h"
#include "string.h"
#include "machine_config.h"

//device_cfg_t  device_cfg ;


//void device_config_save_flash(void)  //set 
//{
//	Flash_Erase(FLASH_ADDR_USER_START,2*1024);
//	Flash_Write(FLASH_ADDR_USER_START, (uint8_t *)&device_cfg, sizeof(device_cfg_t));
//}



//void device_config_read_flash(void)  //set default----默认版本号
//{
//	memset((void *)&device_cfg,0x00,sizeof(device_cfg_t));
//	Flash_Read(FLASH_ADDR_USER_START, (uint8_t *)&device_cfg, sizeof(device_cfg_t));
//	if(device_cfg.init_flag != 0x12345678)		       ///结构体写入12345678 ，返回12345678
//	{
//		device_cfg.init_flag = 0x12345678;

//		memcpy(device_cfg.ver, VER_SOFT,16);  ///---------- TYT-LOGO 型号

//		
//		device_config_save_flash();
//	}
//}

//device_cfg_pt device_config_read(void)
//{
//	Flash_Read(FLASH_ADDR_USER_START, (uint8_t *)&device_cfg, sizeof(device_cfg_t));
//	return &device_cfg;
//}

//void device_init(void)
//{
//	
//	device_config_read_flash();

//}


