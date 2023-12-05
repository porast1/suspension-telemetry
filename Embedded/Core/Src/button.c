/*******************************************************************************
 * Title                 :   Suspension Telemetry
 * Filename              :   button.c
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 *******************************************************************************/

/** \file button.c
 * \brief This module contains the ...
 */

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "button.h"
#include "stdint.h"

#include "cmsis_os.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
/**
 * Constants to ...
 */
#define BUTTON_DELAY				30U
#define BUTTON_PRESSED_COUNT		20U
/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/

/******************************************************************************
 * Module Typedefs
 *******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/

button_t readButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	button_t button = BUTTON_SNA;
	static uint8_t buttonPressedCounter = 0;
	while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)
			&& (BUTTON_PRESSED_COUNT >= buttonPressedCounter))
	{
		if ( BUTTON_PRESSED_COUNT > buttonPressedCounter)
		{

			++buttonPressedCounter;
			button =
					(LEFT_BUTTON_Pin == GPIO_Pin) ?
							(BUTTON_LEFT) : (BUTTON_RIGHT);
		}
		else
		{
			button =
					(LEFT_BUTTON_Pin == GPIO_Pin) ?
							(BUTTON_LEFT_PRESSED) : (BUTTON_RIGHT_PRESSED);
		}
		osDelay(BUTTON_DELAY);
	}
	buttonPressedCounter = 0;
	return (button);
}

/*************** END OF FUNCTIONS ***************************************************************************/
