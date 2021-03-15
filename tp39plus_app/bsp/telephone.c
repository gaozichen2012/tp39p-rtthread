#define TELEPHONE
#include "telephone.h"
#include "tom_flash.h"
#include "poc.h"
#include "date_and_time.h"

void telephone_init(void)
{
    read_flash_call_record_data();
    read_flash_call_contacts_data();

    if (call_contacts.count == 0xff)
    {
        //读取的总数为0xff，表示未初始化
        call_contacts.count = 0x0;
        save_flash_call_contacts_data();
    }

    if (call_record.count == 0xff)
    {
        //读取的总数为0xff，表示未初始化
        call_record.count = 0x0;
        call_record.had_call_record_num_max = 0;
        save_flash_call_record_data();
    }
}

/*存入呼叫记录*/
void deposit_call_record(void)
{
    u8 time_buf[64];

    /*存入呼叫记录*/
    if (strlen((char *)poc.dial.phone) != 0)
    {
        if (call_record.count >= CALL_RECORD_NUM_MAX)
        {
            call_record.count = 0;
            call_record.had_call_record_num_max = 1;
        }

        memcpy(call_record.current_contact[call_record.count].phone, poc.dial.phone, sizeof(poc.dial.phone));
        memcpy(call_record.current_contact[call_record.count].name, poc.dial.name, sizeof(poc.dial.name));

        memset(time_buf, 0, sizeof(time_buf));
        memset(call_record.current_contact[call_record.count].time, 0, sizeof(call_record.current_contact[call_record.count].time));

        time_buf[0] = datetime.hour / 10 + 0x30;
        time_buf[1] = datetime.hour % 10 + 0x30;
        time_buf[2] = ':';
        time_buf[3] = datetime.minite / 10 + 0x30;
        time_buf[4] = datetime.minite % 10 + 0x30;
        time_buf[5] = 2;
        time_buf[6] = datetime.month / 10 + 0x30;
        time_buf[7] = datetime.month % 10 + 0x30;
        time_buf[8] = '-';
        time_buf[9] = datetime.day / 10 + 0x30;
        time_buf[10] = datetime.day % 10 + 0x30;
        time_buf[11] = 0;
        memcpy(call_record.current_contact[call_record.count].time, time_buf, sizeof(call_record.current_contact[call_record.count].time));

        if (poc.miss_call_state == 2)
        {
            call_record.current_contact[call_record.count].miss_call_flag = 1; //表示为未接电话，此标志位用于最后显示红色呼叫记录
        }
        else
        {
            call_record.current_contact[call_record.count].miss_call_flag = 0;
        }

        call_record.count++;
        save_flash_call_record_data();
        read_flash_call_record_data();
    }
}

/*存入新联系人记录*/
void deposit_new_contact(void)
{
    memcpy(call_contacts.current_contact[call_contacts.count].name, new_caontact.name, sizeof(new_caontact.name));
    memcpy(call_contacts.current_contact[call_contacts.count].phone, new_caontact.phone, sizeof(new_caontact.phone));
    memset(new_caontact.name, 0, sizeof(new_caontact.name));
    memset(new_caontact.phone, 0, sizeof(new_caontact.phone));

    call_contacts.count++;
    save_flash_call_contacts_data();
    read_flash_call_contacts_data();
}

//先修改结构体数据，在整体存入
void delete_one_contact_item(u8 index)
{
    if (index < call_contacts.count)
    {
        for (u8 i = 0; i < call_contacts.count; i++)
        {
            if (i < index)
            {
                //小于选中项不处理
            }
            else if (i >= index)
            {
                //所有项向前移一位
                memcpy(&call_contacts.current_contact[i], &call_contacts.current_contact[i + 1], sizeof(one_record_item_t));

                if (i == call_contacts.count - 1)
                {
                    //如果是最后一项，则删除
                    memset(&call_contacts.current_contact[i], 0, sizeof(one_record_item_t));
                }
            }
            else
            {
                /* code */
            }
        }
    }

    //总项目数-1
    call_contacts.count = call_contacts.count - 1;
    save_flash_call_contacts_data();
    read_flash_call_contacts_data();
}

//先修改结构体数据，在整体存入
void delete_one_call_record_item(u8 index)
{
    if (index < call_record.count)
    {
        for (u8 i = 0; i < call_record.count; i++)
        {
            if (i < index)
            {
                //小于选中项不处理
            }
            else if (i >= index)
            {
                //所有项向前移一位
                memcpy(&call_record.current_contact[i], &call_record.current_contact[i + 1], sizeof(one_record_item_t));

                if (i == call_record.count - 1)
                {
                    //如果是最后一项，则删除
                    memset(&call_record.current_contact[i], 0, sizeof(one_record_item_t));
                }
            }
            else
            {
                /* code */
            }
        }
    }

    //总项目数-1
    if (call_record.had_call_record_num_max == 1)
    {
        call_record.count = CALL_RECORD_NUM_MAX - 1;
        call_record.had_call_record_num_max = 0;
    }
    else
    {
        call_record.count = call_record.count - 1;
    }

    save_flash_call_record_data();
    read_flash_call_record_data();
}
