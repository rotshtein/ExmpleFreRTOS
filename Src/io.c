
#include "io.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include "cmsis_os.h"

 _Bool is_button_pressed()
 {
	 //return (HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin));
 }

 void set_green_led(_Bool state)
 {
	 if (state)
	 {
		 light_green_led();
	 }
	 else
	 {
		 clean_green_led();
	 }
 }

 void clean_green_led()
 {
	 HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
 }

 void light_green_led()
 {
	 HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
 }
