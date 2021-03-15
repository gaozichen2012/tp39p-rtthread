#ifndef __DATE_AND_TIME_H
#define __DATE_AND_TIME_H

#include "tom_data.h"
#include "version.h"
typedef struct
{
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minite;
    u8 second;
    u8 week;
} date_and_time_t;

#ifdef DATE_AND_TIME
#define DATE_AND_TIME_EXTERN
#else
#define DATE_AND_TIME_EXTERN extern
#endif

DATE_AND_TIME_EXTERN date_and_time_t datetime;

DATE_AND_TIME_EXTERN void custom_date_time_timer(void);
DATE_AND_TIME_EXTERN void display_time_icon(u8 refush_flag);
DATE_AND_TIME_EXTERN void display_date_and_week(void);
#endif
