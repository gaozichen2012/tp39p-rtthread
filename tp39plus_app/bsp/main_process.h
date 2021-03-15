#ifndef __MIAN_PROCESS_H
#define __MIAN_PROCESS_H

#include "tom_data.h"

#ifdef MIAN_PROCESS
#define MIAN_PROCESS_EXTERN
#else
#define MIAN_PROCESS_EXTERN extern
#endif

MIAN_PROCESS_EXTERN void main_process_init(void);
MIAN_PROCESS_EXTERN void main_process_key(void);

MIAN_PROCESS_EXTERN u8 get_keyboard_number(void);
MIAN_PROCESS_EXTERN void keyboard_number_clear(void);
MIAN_PROCESS_EXTERN u8 get_long_keyboard_number(void);
#endif
