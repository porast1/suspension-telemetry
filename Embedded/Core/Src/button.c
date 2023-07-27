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
#include "adc.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
/**
 * Constants to ...
 */
#define   BUTTON_SELECT_ADC_VAL 2880U
#define   BUTTON_LEFT_ADC_VAL   1930U
#define   BUTTON_UP_ADC_VAL     550U
#define   BUTTON_DOWN_ADC_VAL   1250U
#define   BUTTON_RIGHT_ADC_VAL  0U
#define   BUTTON_NO_BUTTON_ADC_VAL 3000U

#define   BUTTON_ADC_CHANNEL    &hadc3

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/
#define BUTTON_RANGE(X,RANGE)   (int16_t)((RANGE) - (50)) < (X) && (X) < ((RANGE) + (50))
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

button_t readButton(void)
{
	button_t button;
	static uint8_t buttonLock;
	uint16_t buttonAdcValue = BUTTON_NO_BUTTON_ADC_VAL;
	HAL_ADC_Start(BUTTON_ADC_CHANNEL);
	HAL_ADC_PollForConversion(BUTTON_ADC_CHANNEL, 10);
	buttonAdcValue = HAL_ADC_GetValue(BUTTON_ADC_CHANNEL);
	HAL_ADC_Stop(BUTTON_ADC_CHANNEL);
	if (0 == buttonLock)
	{
		if (BUTTON_RANGE(buttonAdcValue, BUTTON_SELECT_ADC_VAL))
		{
			button = BUTTON_SELECT;
		}
		else if (BUTTON_RANGE(buttonAdcValue, BUTTON_UP_ADC_VAL))
		{
			button = BUTTON_UP;
		}
		else if (BUTTON_RANGE(buttonAdcValue, BUTTON_DOWN_ADC_VAL))
		{
			button = BUTTON_DOWN;
		}
		else if (BUTTON_RANGE(buttonAdcValue, BUTTON_LEFT_ADC_VAL))
		{
			button = BUTTON_LEFT;
		}
		else if (BUTTON_RANGE(buttonAdcValue, BUTTON_RIGHT_ADC_VAL))
		{
			button = BUTTON_RIGHT;
		}
		else
		{
			button = BUTTON_SNA;
		}
		buttonLock = 1;
	}
	else
	{
		button = BUTTON_SNA;
	}
	buttonLock = (buttonAdcValue < BUTTON_NO_BUTTON_ADC_VAL) ? (1) : (0);

	return (button);
}

/*************** END OF FUNCTIONS ***************************************************************************/
