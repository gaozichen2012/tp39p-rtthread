/*******************************************************************************
  * @file    PWR_STANDBY/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  *******************************************************************************/

//Includes      
#include "stm32f0xx.h"
#include "flash.h"
#include "uart.h"
#include "nv3021.h"
#include "string.h"
#include "GT21L16S2Y.H"
#include "poc.h"
#include "device.h"
#include "machine_config.h"
#include "stdio.h"
#include "drv_595.h"
#include "language.h"

extern device_uart_t   device_uart3;
/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

#define APP_START_ADDRESS    	0x08004000		  	/* 第16页， 共128页，每页大小为2k */
#define DATA_BUFFER_SIZE   		FLASH_PAGE_SIZE  	/*定义接收缓冲区长度	*/
#define FLASH_PAGE_SIZE         (0x800)	   			/* 2048 */
#define FLASH_SIZE            	(040000) 			/* 256K */

/* Private define ------------------------------------------------------------*/
/* Xmodem Frame form: <SOH><blk #><255-blk #><--128 data bytes--><CRC hi><CRC lo> */
/* 定义Xmoden控制字符 */
#define XMODEM_NUL 				0x00
#define XMODEM_SOH 				0x01
#define XMODEM_STX 				0x02
#define XMODEM_EOT 				0x04
#define XMODEM_ACK 				0x06
#define XMODEM_NAK 				0x15
#define XMODEM_CAN 				0x18
#define XMODEM_EOF 				0x1A

#define Bootdelay				2

#define LINE1_DOT_NUM			28  ///显示: 当前版本
#define LINE2_DOT_NUM			43  ///显示: 服务器版本
#define LINE3_DOT_NUM			60  ///显示: 升级状态?????
#define LINE4_DOT_NUM			75  ///显示: 进度条
#define LINE5_DOT_NUM			95  ///显示: 警告事项
#define LINE6_DOT_NUM			123 ///显示: ---------





#define lcd_color_clr		nv3021_color_clr
#define lcd_init				nv3021_init
#define lcd_disp_range		nv3021_disp_range
#define lcd_disp_box		nv3021_disp_box
#define lcd_disp_bmp_SingleByte	nv3021_disp_bmp_SingleByte
#define lcd_disp_bit1bmp_SingleByte	 nv3021_disp_bit1bmp_SingleByte
#define lcd_disp_tan		nv3021_disp_range




/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;

volatile uint32_t jiffies=0;
volatile uint32_t timeStamp = 0;
volatile uint32_t secondCount = 0;

extern uint32_t SysTick_Cnt;
 extern poc_flash_boot_t boot_info;

/* Private function prototypes -----------------------------------------------*/






//fun: 显示15*16 中文
//输入x,y,坐标，中文2302， 背景色，文字颜色
//单行显示，超出则丢弃
void ui_disp_china_15x16(uint16_t x,uint16_t y, char *gb2312_dat, uint16_t black_color, uint16_t text_color)
{
  uint16_t i,j,lenth;
	uint8_t disp_buf[32] = {0};
	lenth = strlen(gb2312_dat);
	for(i=0; i<lenth; i++)
	{
		if(gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
		{
			gt21l16s2_get_ascii_8x16(disp_buf,*(gb2312_dat+i));
			//把最下面的三行空白点移到上面去
			for(j=0;j<8;j++)
			{
				disp_buf[j+8] <<= 3 ;
				disp_buf[j+8] |= ((disp_buf[j]>>5) & 0x07);					
				disp_buf[j] = ((disp_buf[j]<<3)&0xF8);
			}
			if(x+i*8 <= 152)
			{
				lcd_disp_bit1bmp_SingleByte(x+i*8,y,disp_buf,8,8,text_color,black_color);			
				lcd_disp_bit1bmp_SingleByte(x+i*8,y+8,&disp_buf[8],8,8,text_color,black_color);		
			}				
		}			
		else
		{
			if(gt21l16s2_get_gb2312_15x16(disp_buf,( uint8_t*)(gb2312_dat+i)))	
			{
				if(x+i*8 <= 144)
				{
					lcd_disp_bit1bmp_SingleByte(x+i*8,y,disp_buf,16,8,text_color,black_color);			
					lcd_disp_bit1bmp_SingleByte(x+i*8,y+8,&disp_buf[16],16,8,text_color,black_color);
				}					
			  i++;
			}
			else//出错了，找不到字库
			{
			}
		}			
	}
}


//fun: 显示11*12 中文
//输入x,y,坐标，中文2302， 背景色，文字颜色
//单行显示，超出则丢弃
static void ui_disp_china_11x12(uint16_t x,uint16_t y, char *gb2312_dat, uint16_t black_color, uint16_t text_color)
{
  uint16_t i,lenth;
	uint8_t disp_buf[32] = {0};
	lenth = strlen(gb2312_dat);
	for(i=0; i<lenth; i++)
	{
		if(gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
		{
			gt21l16s2_get_ascii_6x12(disp_buf,*(gb2312_dat+i));
			if(x+i*6 <= 154)
			{
				lcd_disp_bit1bmp_SingleByte(x+i*6,y,disp_buf,6,8,text_color,black_color);			
				lcd_disp_bit1bmp_SingleByte(x+i*6,y+8,&disp_buf[6],6,5,text_color,black_color);				
			}
		}			
		else
		{
			if(gt21l16s2_get_gb2312_11x12(disp_buf,( uint8_t*)(gb2312_dat+i)))	
			{
				if(x+i*6 <= 148)
				{
					lcd_disp_bit1bmp_SingleByte(x+i*6,y,disp_buf,12,8,text_color,black_color);			
					lcd_disp_bit1bmp_SingleByte(x+i*6,y+8,&disp_buf[12],12,4,text_color,black_color);		
				}
				i++;
			}
			else//出错了，找不到字库
			{
			}
		}			
	}
}


void 	boot_ui_init()
{
//	device_cfg_pt cfg = device_config_read();
	lcd_color_clr(0,0,COLOR_WIGHT,160,128);
	lcd_color_clr(0,0,COLOR_BLUE,160,22);
	ui_disp_china_15x16(80-4*strlen(c_MCU_upgrade),3,c_MCU_upgrade,COLOR_BLUE,COLOR_WIGHT);
	ui_disp_china_11x12(0,LINE1_DOT_NUM,c_This_version,COLOR_WIGHT,COLOR_BLUE);
	ui_disp_china_11x12(60,LINE1_DOT_NUM,(char*)boot_info.soft_ver,COLOR_WIGHT,COLOR_BLUE);
	ui_disp_china_11x12(0,LINE2_DOT_NUM,c_Upgraded_version,COLOR_WIGHT,COLOR_BLUE);

	lcd_disp_tan(30,LINE4_DOT_NUM,101,16,1,COLOR_BLUE);
	ui_disp_china_11x12(0,LINE5_DOT_NUM,c_Note_Do_not_power_off,COLOR_WIGHT,COLOR_RED);
}



/*******************************************************************************
* Function Name  : download_code_bin
* Description    : download bin file of application code.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void download_code_bin(void)
{
	uint8_t uci;
	
	uint32_t ulcnt = 0;
	poc_init();
	while(1)
	{
		poc_device_pt poc = poc_get_para();
		poc_task();
		if(poc->status == POC_DEVICE_VER_DWONLOAD)//检测到版本,开始下载到模块
		{
			static uint8_t ui_refush_flag = 0;
			if(ui_refush_flag == 0)
			{
				ui_refush_flag = 1;
				ui_disp_china_11x12(60,LINE2_DOT_NUM,(char*)poc->server_soft_ver,COLOR_WIGHT,COLOR_BLUE);
				ui_disp_china_11x12(0,LINE3_DOT_NUM,c_Downloading,COLOR_WIGHT,COLOR_BLUE);
			}
		}
		else if(poc->status == POC_DEVICE_VER_SAME)
		{
			uint32_t delay;
			boot_info.boot_flag=0x12345678;//修改boot flag为任意值
			Flash_Updata(FLASH_ADDR_BOOT_FLAG,(uint8_t*)&boot_info, sizeof(poc_flash_boot_t));					
			ui_disp_china_11x12(0,LINE3_DOT_NUM,c_Is_the_latest_version,COLOR_WIGHT,COLOR_BLUE);
			for(uci = 0; uci < 160 ;)
			{
				lcd_disp_tan(uci,LINE6_DOT_NUM,2,2,1,COLOR_RED);
				uci += 4;
			}
			for(delay=0;delay<30000000;delay++);
			break;
		}
		else if(poc->status == POC_DEVICE_VER_DWONLOAD_SUCCESS)
		{
			uint32_t delay;
boot_info.boot_flag=0x12345678;//修改boot flag为任意值
			Flash_Updata(FLASH_ADDR_BOOT_FLAG,(uint8_t*)&boot_info, sizeof(poc_flash_boot_t));	
//			memset(cfg->ver,0,32);
//			cfg->init_flag = 0x12345678;
//			memcpy(cfg->ver,poc->server_soft_ver,18);
//			device_config_save_flash();
			ui_disp_china_11x12(0,LINE3_DOT_NUM,c_Update_successed,COLOR_WIGHT,COLOR_BLUE);
			for(uci = 0; uci < 160 ;)
			{
				lcd_disp_tan(uci,LINE6_DOT_NUM,2,2,1,COLOR_RED);
				uci += 4;
			}
			for(delay=0;delay<30000000;delay++);
			break;			
		}
		else if(poc->status ==  POC_DEVICE_VER_UPGRADE)
		{
			static uint8_t ui_refush_flag = 0;
			double a;
			static uint16_t cur_pt = 0,old_pt;
			if(ui_refush_flag == 0)
			{
				ui_refush_flag = 1;
				ui_disp_china_11x12(0,LINE3_DOT_NUM,c_upgrading,COLOR_WIGHT,COLOR_BLUE);
			}
			if(poc->pack_sum != 0)
			{
				a = poc->down_pack_cnt/(poc->pack_sum/100.0);
			}
			else
			{
				a=0.0;
			}
			cur_pt = ((uint16_t)a)%100;
			if(cur_pt != old_pt)
			{
				old_pt = cur_pt;
				lcd_color_clr(31,LINE4_DOT_NUM+1,COLOR_BLUE,cur_pt,14);
			}
		}

		
		if(poc->status !=  POC_DEVICE_VER_UPGRADE)//不在升级状态，超过5分钟则退出升级状态
		{
			uint32_t delay;
			if(SysTick_Cnt > 300000)
			{	
				boot_info.boot_flag=0x12345678;//修改boot flag为任意值
				Flash_Updata(FLASH_ADDR_BOOT_FLAG,(uint8_t*)&boot_info, sizeof(poc_flash_boot_t));	
				ui_disp_china_11x12(0,LINE3_DOT_NUM,c_Timeout_restart,COLOR_WIGHT,COLOR_BLUE);
				for(uci = 0; uci < 160 ;)
				{
					lcd_disp_tan(uci,LINE6_DOT_NUM,2,2,1,COLOR_RED);
					uci += 4;
				}
				for(delay=0;delay<30000000;delay++);
				break;
			}			
		}

		
		ulcnt++;
		if(ulcnt > 20000)
		{
			ulcnt = 0;
			lcd_disp_tan(uci,LINE6_DOT_NUM,2,2,1,COLOR_RED);
			uci +=4;
			if(uci >= 159)
			{
				uci = 0;
				lcd_disp_tan(0,LINE6_DOT_NUM,160,2,1,COLOR_WIGHT);
			}
		}

		Upate_595((_stTag595 *)&ioTag595,(conr_val595.contun),RESET);
		#if(DEF_DEBUG)
		printf_Device_status();
		#endif
		
	}
}


void boot_task()
{
	gt21l16s2_init();
	///nv3021_init();
	ILI9163C_Initial();
	//device_init();
	boot_ui_init();
	//LCD_BK_ON();
	conr_val595.conr_bit.LCD_BL = 1;
	
	//下载程序
	download_code_bin();
	
}

/*******************************************************************************
* Function Name  : run_app
* Description    : Run to the application code.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void run_app(void)
{
	/* jump to application code */
	JumpAddress = *(volatile uint32_t *)(APP_START_ADDRESS + 4);
	/* Jump to user application */
	Jump_To_Application = (pFunction) JumpAddress;
	/* Initialize user application's Stack Pointer */
	//__MSR_MSP(*(volatile uint32_t *)APP_START_ADDRESS);
	Jump_To_Application();
}


void Flash_WrRDP(void)
{
	FlagStatus status = FLASH_OB_GetRDP();
	if(status == RESET)
	{
		FLASH_Unlock();
		FLASH_OB_Unlock();
		FLASH_OB_RDPConfig(OB_RDP_Level_1);
		FLASH_OB_Launch();
		FLASH_OB_Lock();
		FLASH_Lock();
	}
}



int main(void)
 {
	SysTick_Config(48000);
	//uart3_config();
    Flash_Read(FLASH_ADDR_BOOT_FLAG,(uint8_t*)&boot_info, sizeof(poc_flash_boot_t));
    
#ifdef RELEASE
       Flash_WrRDP();
#endif

	
	if(boot_info.boot_flag == 0x88888888)
	{
		init_595IO((_stTag595 *)&ioTag595);
		conr_val595.contun=0x00;
		conr_val595.conr_bit.PWR_EN = 1;

		if(boot_info.sim_card_select == 0)
		{
		conr_val595.conr_bit.SIM_SEL = 0;//SIM1
		}
		else
		{
		conr_val595.conr_bit.SIM_SEL = 1;//SIM2
		}
		Upate_595((_stTag595 *)&ioTag595,(conr_val595.contun),SET);
		boot_task();
	}
	//uart3_send_string((uint8_t*)"goto app addr: 0x8004000\r\n",26);
	run_app();
	NVIC_SystemReset();
	while(1);

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
