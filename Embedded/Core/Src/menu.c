/*******************************************************************************
 * Title                 :   Suspension Telemetry
 * Filename              :   menu.c
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 *******************************************************************************/

/** \file menu.c
 * \brief This module contains the ...
 */

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "menu.h"
#include "button.h"
#include "stdint.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "File_Handling_RTOS.h"
#include "travelSensor.h"
#include "ssd1306.h"
#include "lcdMenu.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
/**
 * Constants to ...
 */

#define FILE_NAME_SIZE 20U

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/

/******************************************************************************
 * Module Typedefs
 *******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/


static menu_t selector = MENU_START;
static uint8_t path = 0;
extern calibration_t calibrationValues;
char dir[FILE_NAME_SIZE];
char sensorData[FILE_NAME_SIZE];

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
menu_t getMenuSelector(void)
{
	return (selector);
}
void setMenuSelector(menu_t selectorState)
{
	selector = selectorState;
}
char* getSensorDataFileName(void)
{
	return (sensorData);
}

void menuSelector(button_t buttonLeft, button_t buttonRight)
{

	switch (selector)
	{

	case (MENU_START):

		if (BUTTON_LEFT == buttonLeft)
		{
			lcdMenuSag();
			selector = MENU_SAG;

		}
		else if (BUTTON_RIGHT == buttonRight)
		{
			lcdStartMeasurement();
			selector = MENU_MEASURMENT;

		}
		else
		{
			lcdMenuStart();
		}

		break;

	case (MENU_SAG):
		if (BUTTON_LEFT_PRESSED == buttonLeft)
		{
			if(0 == readCalibrationData(&calibrationValues)){
			startAdcDma();
			SSD1306_Clear();
			selector = MENU_SAG_START;
			}
		}
		else if (BUTTON_LEFT == buttonLeft)
		{
			lcdCalibration();
			selector = MENU_CALIBRATION;
		}
		else if (BUTTON_RIGHT == buttonRight)
		{
			lcdMenuStart();
			selector = MENU_START;
		}
		else
		{

		}
		break;
	case (MENU_CALIBRATION):
		if (BUTTON_LEFT_PRESSED == buttonLeft)
		{
			startAdcDma();

		}
		else if (BUTTON_RIGHT == buttonRight)
		{
			selector = MENU_START;
			lcdMenuStart();
		}
		else
		{

		}
		break;
	case (MENU_SAG_START):
		if (BUTTON_LEFT_PRESSED == buttonLeft)
		{
			stopAdcDma();
			selector = MENU_START;
			lcdMenuStart();
		}
		else
		{

		}
		break;

	case (MENU_MEASURMENT):
		if (BUTTON_RIGHT_PRESSED == buttonRight)
		{
			lcdMeasurementStart();
			setPath(dir, sensorData, path);
			createNewFile(dir, sensorData, &path);
			if(0 == readCalibrationData(&calibrationValues)){
			startAdcDma();
			selector = MENU_MEASURMENT_START;

			}
		}
		else if (BUTTON_LEFT == buttonLeft)
		{
			lcdMenuStart();
			selector = MENU_START;
		}
		else
		{

		}
		break;

	case (MENU_MEASURMENT_START):
		if (BUTTON_RIGHT_PRESSED == buttonRight)
		{
			stopAdcDma();
			lcdMenuFinishedNotification("Mes Finished");
			lcdMenuStart();
			selector = MENU_START;
		}
		else
		{

		}
		break;

	default:
	}

}

/*************** END OF FUNCTIONS ***************************************************************************/
