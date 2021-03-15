#ifndef __BATTERY_H
#define __BATTERY_H

#include "tom_data.h"
#include "ui.h"

#ifdef BATTERY
#define BATTERY_EXTERN
#else
#define BATTERY_EXTERN extern
#endif

BATTERY_EXTERN void dispaly_battery_ico(int8_t level);
BATTERY_EXTERN void battery_process(ui_ico_change_type type);
BATTERY_EXTERN int8_t get_battery_level(void);

#endif
