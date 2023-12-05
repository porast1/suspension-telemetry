/****************************************************************************
 * Title                 :   Suspension Telemetry 
 * Filename              :   menu.h
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 * Notes                 :   None
 *****************************************************************************/

/** \file menu.h
 *  \brief This module contains ...
 * 
 *  This is the header file for the definition for tasks that ...
 */
#ifndef INC_MENU_H_
#define INC_MENU_H_

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "button.h"

/******************************************************************************
 * Preprocessor Constants
 *******************************************************************************/
/**
 * This constant is
 */

/******************************************************************************
 * Configuration Constants
 *******************************************************************************/
typedef enum menu
{
	MENU_START = 0U,
	MENU_SAG,
	MENU_CALIBRATION,
	MENU_SAG_START,
	MENU_MEASURMENT,
	MENU_MEASURMENT_START

} menu_t;

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
menu_t getMenuSelector(void);
void setMenuSelector(menu_t selectorState);
char* getSensorDataFileName(void);
void menuSelector(button_t buttonLeft, button_t buttonRight);
void menuCalculateBlock(void);
#endif /* INC_MENU_H_ */

/*** End of File **************************************************************/
