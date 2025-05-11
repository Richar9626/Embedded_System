/*
 * MyNewTask.c
 *
 *  Created on: 26 oct 2021
 *      Author: j_dan
 */


#include "MyNewTask.h"

osaEventId_t mMyEvents;

tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

osaTaskId_t gMyTaskHandler_ID;

static uint8_t ledsState = 0;


OSA_TASK_DEFINE(My_Task,gMyTaskPriority_c,1,gMyTaskStackSize_c,FALSE);

void My_Task(osaTaskParam_t argument){

	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();

	while(1){

		OSA_EventWait(mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &customEvent);
		if(!gUseRtos_c && !customEvent){
			break;
		}
		switch(customEvent){
			case gMyNewTaskEvent1_c:
				TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
				     1000, /* Timer's Timeout */
				     myTaskTimerCallback, /* pointer to
				myTaskTimerCallback function */
				     NULL
				   );
				TurnOffLeds();
				break;
			case gMyNewTaskEvent2_c:
				if(!ledsState){
					TurnOnLeds();
					ledsState = 1;
				}else{
					TurnOffLeds();
					ledsState = 0;
				}
				break;
			case gMyNewTaskEvent3_c:
				ledsState = 0;
				TurnOffLeds();
				TMR_StopTimer(myTimerID);
				break;
			default:
				break;
		}
	}
}

void MyTask_Init(void){

	mMyEvents = OSA_EventCreate(TRUE);
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);

}

void myTaskTimerCallback(void *param)
{
 OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}

void MyTaskTimer_Stop(void)
{
 OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}

void MyTaskTimer_Start(){
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}
