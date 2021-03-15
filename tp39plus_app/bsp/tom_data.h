#ifndef __TOM_DATA
#define __TOM_DATA

#ifndef u8
typedef unsigned char u8;
#endif

#ifndef u16
typedef unsigned short u16;
#endif

#ifndef u32
typedef unsigned int u32;
#endif

#ifndef u64
typedef unsigned long u64;
#endif

#ifndef bool
typedef enum
{
    false,
    true
} bool;
#endif

typedef enum
{
    on,
    off
} onoff_t;

typedef signed char int8_t;

#endif
