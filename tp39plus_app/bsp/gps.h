#ifndef __GPS_H
#define __GPS_H

#include "tom_data.h"
#include "tom_interface.h"
typedef enum
{
    GPS_DEVICE_NOT_DETECT, //未检测到设备
    GPS_DEVICE_NORMAL,     //正常状态
    GPS_DEVICE_MOVING,     //移动状态

} gps_device_status_t;

typedef enum
{
    ITUDE_N = 'N',
    ITUDE_S = 'S',
    ITUDE_W = 'W',
    ITUDE_E = 'E',

} itude_t;

typedef struct
{
    u8 SNR[32][4]; //信噪比，最多32个卫星
    u32 time[32];  //更新时间,超过时间 数值未更新，则清除信号强度为0
                   //u8 number[32][4];  //卫星编号
                   //仰角角度  90在头顶
                   //方位角度  0~360 水平

} gps_snr_t, *gps_snr_pt;

typedef struct
{
    device_uart_t gps_data;

    u8 NS_value[32];      //纬度 值
    itude_t ns;           //类型
    u8 EW_value[32];      //经度值
    itude_t ew;           //类型
    u8 position_state;    //定位状态 ‘1’ 定位成功，  ‘0’ 未定位
    u8 satellites_sum[4]; //卫星数量
    gps_snr_t snr;        //卫星状态信息  信噪比
    gps_device_status_t status;

} gps_device_t, *gps_device_pt;

#ifdef GPS
#define GPS_EXTERN
#else
#define GPS_EXTERN extern
#endif

GPS_EXTERN uint32_t gps_nodata_offtime;
GPS_EXTERN gps_device_t gps_device;

GPS_EXTERN void gps_init(void);
GPS_EXTERN void gps_process(u8 *ptr);
GPS_EXTERN void gps_task(void);
GPS_EXTERN void gps_poc_report_timer_process(void);
#endif
