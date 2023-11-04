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
#include "liquidcrystal_i2c.h"
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
char * getSensorDataFileName(void){
	return (sensorData);
}



void menuSelector(button_t button)
{

	switch (selector)
	{

	case (MENU_START):
		if (BUTTON_LEFT == button)
		{
			lcdMenuSag();
			selector = MENU_SAG;

		}
		else if (BUTTON_RIGHT == button)
		{
			lcdStartMeasurement();
			selector = MENU_MEASURMENT;

		}
		else
		{
			lcdMenuStart();
		}
		stopAdcDma();
		break;

	case (MENU_SAG):
		if (BUTTON_SELECT == button)
		{
			startAdcDma();
			HD44780_Clear();
			puts("pomiar rozpoczety, kliknij select aby zakonczyc\n");
			selector = MENU_SAG_START;
		}
		else if (BUTTON_LEFT == button)
		{
			lcdCalibration();
			selector = MENU_CALIBRATION;
		}
		else if (BUTTON_RIGHT == button)
		{
			selector = MENU_START;
		}
		else
		{
			puts("nacisnij select aby rozpoczac pomiar\n");
		}
		break;
	case (MENU_CALIBRATION):
		if (BUTTON_SELECT == button)
		{
			startAdcDma();
			while (travelPressureSensorCalibration() != 1)
			{
				osDelay(40);
			}
			lcdMenuFinishedNotification("Cal Finished");
			lcdMenuStart();
			selector = MENU_START;

		}
		else if (BUTTON_RIGHT == button)
		{
			selector = MENU_START;
			lcdMenuStart();
		}
		else
		{
			puts("nacisnij select aby rozpoczac pomiar\n");
		}
		break;
	case (MENU_SAG_START):
		if (BUTTON_SELECT == button)
		{

			selector = MENU_START;
			lcdMenuStart();
		}
		else
		{
			puts("nacisnij select aby wrocic do menu start");
		}
		break;

	case (MENU_MEASURMENT):
		if (BUTTON_SELECT == button)
		{
			lcdMeasurementStart();
			setPath(dir, sensorData, path);
			createNewFile(dir, sensorData, &path);
			startAdcDma();
			selector = MENU_MEASURMENT_START;
			puts("pomiar ciagly rozpoczety, kliknij select aby zakonczyc\n");
		}
		else
		{
			puts("nacisnij select aby rozpoczac pomiar\n");
		}
		break;

	case (MENU_MEASURMENT_START):
		if (BUTTON_SELECT == button)
		{

			lcdMenuFinishedNotification("Mes Finished");
			lcdMenuStart();
			selector = MENU_START;
		}
		else
		{
			puts("nacisnij select aby wrocic do menu start");
		}
		break;

	default:
	}

}

/*************** END OF FUNCTIONS ***************************************************************************/
