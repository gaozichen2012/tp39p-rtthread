#ifndef __TELEPHONE_H
#define __TELEPHONE_H

#include "tom_data.h"
#define PHONE_CONTACT_NAME_LEN (18)
#define CALL_RECORD_NUM_MAX (40)

typedef enum
{
    OPT_CALL,
    OPT_DELETE,
} item_opt_t;

#if 1 //联系人结构体
typedef struct
{
    u8 name[PHONE_CONTACT_NAME_LEN];
    u8 phone[20]; //char phone[14];
} one_contact_item_t;

typedef struct
{
    u8 count;
    one_contact_item_t current_contact[50];
} contacts_t;
#endif

#if 1 //呼叫记录结构体
typedef struct
{
    u8 name[PHONE_CONTACT_NAME_LEN];
    u8 phone[20]; //char phone[14];
    u8 time[12];
    u8 miss_call_flag;
} one_record_item_t;

typedef struct
{
    u8 count;
    one_record_item_t current_contact[CALL_RECORD_NUM_MAX]; //40条记录接近2k，最大40条
    u8 had_call_record_num_max;//0:少于40条记录 1:>=40条记录 呼叫记录是否到达最大值，超过最大值则覆盖
} call_record_t;
#endif

#ifdef TELEPHONE
#define TELEPHONE_EXTERN
#else
#define TELEPHONE_EXTERN extern
#endif

TELEPHONE_EXTERN one_contact_item_t new_caontact; //新建联系人
TELEPHONE_EXTERN contacts_t call_contacts;

TELEPHONE_EXTERN call_record_t call_record;

TELEPHONE_EXTERN void telephone_init(void);
TELEPHONE_EXTERN void deposit_call_record(void);
TELEPHONE_EXTERN void deposit_new_contact(void);

TELEPHONE_EXTERN void delete_one_contact_item(u8 index);
TELEPHONE_EXTERN void delete_one_call_record_item(u8 index);
#endif
