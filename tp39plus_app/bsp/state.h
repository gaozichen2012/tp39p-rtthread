#ifndef _STATE_H_
#define _STATE_H_

typedef enum POC_STATE_ID_TAG
{
	POC_STATE_INIT,
	POC_STATE_DESKTOP,
	POC_STATE_MENU,

	POC_STATE_GROUP,
	POC_STATE_MEMBER,
	POC_STATE_FRIEND,
	POC_STATE_RECORD,
	POC_STATE_GPS,
	POC_STATE_SETING,
	POC_STATE_INVITE,
	POC_STATE_OTA,
	POC_STATE_SOS,
} POC_STATE_ID;

/**
 * State initialize function. When enter a state, it will be called first.
 *
 * @return 0 if success, failure otherwise.
 */
typedef short (*STATE_INITIALIZE)(unsigned int arg1);

/**
 * State terminate function. When leave this state, it will be called.
 *
 * @return 0 if success, failure otherwise.
 */
typedef short (*STATE_TERMINATE)(void);

/**
 * Dispatch events to current state. When any event occured, it will be called.
 *
 * @param id Event ID. Can be NTO_MSG_ID_TIMER or NTO_MSG_ID_KEY_DOWN
 * @param arg1 In keypad event, it is the keycode.
 * @param arg2 It's not used in timer event. In keypad event, it is the event 
 *  source. It can be MSG_ID_KBD or MSG_ID_IR.
 * @return 0 if success, failure otherwise.
 */
typedef short (*STATE_PROC)(unsigned int sig_set, unsigned int arg1, void *pArg2);

/**
 * State manager singleton object.
 */
typedef struct POC_STATE_TAG
{
	STATE_INITIALIZE Initialize; /**< Pointer to current state's initialize function */
	STATE_TERMINATE Terminate;	 /**< Pointer to current state's terminate function */
	STATE_PROC Proc;			 /**< Pointer to current state's procedure function */
} POC_STATE;

/**
 * State manager initial function, should be called when AP startup.
 *
 * @return 0 if success, failure otherwise.
 */
short STATE_MgrInitialize(POC_STATE_ID id);

/**
 * Terminate state manager, called when AP terminated. (Actually not important)
 *
 * @return 0 if success, failure otherwise.
 */
short STATE_MgrTerminate(void);

/**
 * Change state.
 *
 * @param id State ID to change to.
 * @return 0 if success, failure otherwise.
 */
short STATE_MgrChangeState(POC_STATE_ID id, unsigned int arg1);

/**
 * Get previous state ID.
 *
 * @return The previous state ID.
 */
POC_STATE_ID STATE_MgrGetPreStateId(void);

/**
 * Get current state ID.
 *
 * @return The current state ID.
 */
POC_STATE_ID STATE_MgrGetCurStateId(void);

/**
 * Dispatch events to current state.
 *
 * @param id Event ID. Can be NTO_MSG_ID_TIMER or NTO_MSG_ID_KEY_DOWN
 * @param arg1 In keypad event, it is the keycode.
 * @param arg2 It's not used in timer event. In keypad event, it is the event 
 *  source. It can be MSG_ID_KBD or MSG_ID_IR.
 * @return 0 if success, failure otherwise.
 */
short STATE_MgrProcessMsg(unsigned int sig_set, unsigned int arg1, void *pArg2);

#endif
