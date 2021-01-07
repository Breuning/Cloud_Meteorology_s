/*
 * ResetTask.c
 *
 *  Created on: 2020��8��31��
 *      Author: Breuning
 */

#include "main.h"
#include "string.h"
#include "ResetTask.h"
#include "NBiotTask.h"

BOOL ResetTaskTimerFlag = FALSE;


void Reset_Task()
{
	if(ResetTaskTimerFlag == TRUE)
	{
		ResetTaskTimerFlag = FALSE;

		NBiot_POWD_PEN();
		HAL_Delay(200);
		McuReset();
	}
}
