#include "Zpoc.h"
#include "state.h"
#include "state_desktop.h"
#include <rtthread.h>
void desktop_msg_timer_process(void)
{
}

short desktop_state_proc(unsigned int sig_set, unsigned int arg1, void *pArg2)
{
	switch (sig_set)
	{
	case ZPOC_MSG_AT_RECV:
		break;

	case ZPOC_MSG_KEY:
		break;

	case ZPOC_MSG_TIMER:
		desktop_msg_timer_process();
		break;

	case ZPOC_MSG_GPS:
		break;

	case ZPOC_MSG_BATLOW:
		break;
	}

	return 0;
}

short desktop_state_init(unsigned int arg1) //
{
		int i=0;

	if (STATE_MgrGetPreStateId() == POC_STATE_INIT)
	{
		i++;
		if(i==1)
		{
//			STATE_MgrChangeState(POC_STATE_INIT, 0);
//			rt_thread_mdelay(500);
		}
	}
	return 0;
}

short desktop_state_term(void)
{
	return 0;
}

POC_STATE DesktopStateMgr =
	{
		desktop_state_init,
		desktop_state_term,
		desktop_state_proc};
