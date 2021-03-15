#ifndef __KNOB_H
#define __KNOB_H

#include "tom_data.h"

#ifdef KNOB
#define KNOB_EXTERN
#else
#define KNOB_EXTERN extern
#endif

KNOB_EXTERN void knob_line_A_detection(void);
KNOB_EXTERN void knob_line_B_detection(void);

#endif
