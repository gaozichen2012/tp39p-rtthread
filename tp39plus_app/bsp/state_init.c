#include "Zpoc.h"
#include "state.h"
#include "state_init.h"
#include <rtthread.h>
#define POC_SETING_VALID_FLG 0xa0a0a0a0

void init_msg_timer_process(void)
{
}

short init_state_proc(unsigned int sig_set, unsigned int arg1, void *pArg2)
{
	switch (sig_set)
	{
	case ZPOC_MSG_AT_RECV:
		break;

	case ZPOC_MSG_KEY:
		break;

	case ZPOC_MSG_TIMER:
		break;

	default:
		break;
	}

	return 0;
}

short init_state_init(unsigned int arg1)
{
	int i=0;
	
	i++;
	if(i==1)
	{
		STATE_MgrChangeState(POC_STATE_DESKTOP, 0);
		rt_thread_mdelay(500);
	}
	return 0;
}

short init_state_term(void)
{
	return 0;
}

POC_STATE InitStateMgr =
	{
		init_state_init,
		init_state_term,
		init_state_proc};
