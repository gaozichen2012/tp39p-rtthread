/****************************************Copyright (c)****************************************************
**                                   Guangzhou NTO Co.,LTD.
**
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Kingway Huang
** Created date:        2010-04-08
** Version:             V1.00
** Descriptions:             
*********************************************************************************************************/
#include <stdio.h>

#include "poc.h"
#include "state.h"

extern POC_STATE InitStateMgr;
extern POC_STATE DesktopStateMgr;

static POC_STATE *state_entries[] =
    {
        &InitStateMgr,
        &DesktopStateMgr,
		};

/** State manager definition */
typedef struct POC_STATE_MGR_TAG
{
    const POC_STATE *currState; // Current state
    POC_STATE_ID statePreId;
    POC_STATE_ID stateCurrId;
} POC_STATE_MGR;

static POC_STATE_MGR state_mgr;

short STATE_MgrInitialize(POC_STATE_ID id)
{
    state_mgr.currState = NULL;
    state_mgr.statePreId = id;
    state_mgr.stateCurrId = id;
    STATE_MgrChangeState(id, (unsigned int)0);
    return 0;
}

short STATE_MgrTerminate(void)
{
    short result = 0;
    if (state_mgr.currState)
    {
        result = state_mgr.currState->Terminate();
    }
    return result;
}

short STATE_MgrChangeState(POC_STATE_ID id, unsigned int arg1)
{
    short result = 0;

    if (state_mgr.currState)
    {
        result = state_mgr.currState->Terminate();
    }

    state_mgr.statePreId = state_mgr.stateCurrId;
    state_mgr.stateCurrId = id;
    state_mgr.currState = state_entries[id];

    if (state_mgr.currState)
    {
        result = state_mgr.currState->Initialize(arg1);
    }
    return result;
}

POC_STATE_ID STATE_MgrGetPreStateId(void)
{
    return state_mgr.statePreId;
}

POC_STATE_ID STATE_MgrGetCurStateId(void)
{
    return state_mgr.stateCurrId;
}

short STATE_MgrProcessMsg(unsigned int sig_set, unsigned int arg1, void *pArg2)
{
    short result;
    result = state_mgr.currState->Proc(sig_set, arg1, pArg2);
    return result;
}
