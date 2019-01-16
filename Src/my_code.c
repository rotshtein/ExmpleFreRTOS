/*
 * my_code.c
 *
 *  Created on: Jan 10, 2019
 *      Author: uri
 */

#include "main.h"
#include "io.h"
#include "stm32f0xx_hal.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#define task1PRIORITY (configMAX_PRIORITIES - 1)
#define task2PRIORITY (configMAX_PRIORITIES - 2)

void vTask1(void const * argument);
void vTask2(void const * argument);
void vCallbackFunctionExample( TimerHandle_t xTimer );

TaskHandle_t Task1_handle,Task2_handle;
QueueHandle_t queue;
TimerHandle_t timer;
BaseType_t level;

void vInitTask(void const * argument)
{
	BaseType_t volatile ret;

	queue = xQueueCreate(1,5);
	vQueueAddToRegistry(queue, "The Q");
	//ITM_SendChar( 65 );
	ret = xTaskCreate( (TaskFunction_t)vTask1, "Task 1", 200, (void*)0, task1PRIORITY, &Task1_handle );

	//ret = xTaskCreate( (TaskFunction_t)vTask2, "Task 2", 200, (void*)0, task2PRIORITY, &Task2_handle );

	osDelay(1000 + ret);
	vTaskDelete(NULL);
}

void vTask1(void const * argument)
{

	while(1)
	{
		_Bool in;
		xQueueReceive(queue, &in, 0xFFFFFFFF);
		if (in)
		{
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
			timer = xTimerCreate("timer1",pdMS_TO_TICKS(1000), pdFALSE , 0,vCallbackFunctionExample);
			xTimerStart(timer,100);
		}
		else
		{
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
		}
	}
}

void vTask2(void const * argument)
{
	while(1)
	{
	  _Bool in = 0;//HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin);
	  xQueueSend(queue, &in, 0);
	  osDelay(10);
	}
}


void vCallbackFunctionExample( TimerHandle_t xTimer )
{
	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	_Bool in = GPIO_Pin & 1;
	xQueueSendFromISR(queue, &in, &level);

}
