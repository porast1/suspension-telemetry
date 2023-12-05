/****************************************************************************
 * Title                 :   Suspension Telemetry 
 * Filename              :   button.h
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 * Notes                 :   None
 *****************************************************************************/

/** \file button.h
 *  \brief This module contains ...
 * 
 *  This is the header file for the definition for tasks that ...
 */
#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "gpio.h"
/******************************************************************************
 * Preprocessor Constants
 *******************************************************************************/
/**
 * This constant is
 */
typedef enum button
{
	BUTTON_LEFT = 0U,
	BUTTON_RIGHT,
	BUTTON_LEFT_PRESSED,
	BUTTON_RIGHT_PRESSED,
	BUTTON_SNA
} button_t;

/******************************************************************************
 * Configuration Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/

/******************************************************************************
 * Variables
 *******************************************************************************/

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
button_t readButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
#endif /* INC_BUTTON_H_ */

/*** End of File **************************************************************/
