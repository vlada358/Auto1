/* Standard includes. */
#include <stdio.h>l.g 
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/* Driver includes */


static const char character[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x06D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5e };
/* Priorities at which the tasks are created. */
#define task_prioritet		( tskIDLE_PRIORITY + 2 )
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "extint.h"
#include "HW_access.h"

static SemaphoreHandle_t binSem;
/* A software timer that is started from the tick hook. */
static BaseType_t myTask;
//local function declaration
static void Display_Toggle();
static void task(void* pvParams);

static void Display_Toggle()
{
	static uint16_t display_value = 0;
	select_7seg_digit(0); //aktivna prva cifru displeja sa leve srane
	if (display_value == 0)
	{
		set_7seg_digit(character[display_value]);
		display_value = 1;
	}
	else
	{
		set_7seg_digit(character[display_value]);
		display_value = 0;
	}
}

static void task(void* pvParams)
{
	while (1) {
		vTaskDelay(pdMS_TO_TICKS(500));  //task menja 0 i 1 svake 0,5s
		Display_Toggle();
	}
}

void main_demo(void)
{
	/* Inicijalizacija drajvera za displej*/

	init_7seg_comm();
	// kreiranje taska
	myTask = xTaskCreate(task,			       /* funkcija koja iplementira task */
		NULL, 							/* tekstualni naziv taska, nije nepohodan, koristi se samo za debug */
		configMINIMAL_STACK_SIZE, 		/* velicina steka u bajtovima za ovaj task  */
		NULL, 							/* parametar koji se prosledjuje tasku, ovde se ne koristi*/
		task_prioritet,                 /* prioritet ovog taska, sto veci broj veci prioritet*/
		NULL);							/* referenca (handle) ovog taska, ovde se ne koristi */
	if (myTask != pdPASS) { while (1); } // proveravamo da li se task pravilno kreirao, ako nije zakucaj program

	vTaskStartScheduler();
	while (1);
}