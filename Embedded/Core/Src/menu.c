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
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
/**
 * Constants to ...
 */

#define FILE_NAME_SIZE 20

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
char frontSensor[FILE_NAME_SIZE];
char rearSensor[FILE_NAME_SIZE];
char frontPressureSensor[FILE_NAME_SIZE];
char rearPressureSensor[FILE_NAME_SIZE];
/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void menuSelector(button_t button)
{

	switch (selector)
	{

	case (MENU_START):
		if (BUTTON_LEFT == button)
		{
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("CALIB");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("LEFT");
			HD44780_SetCursor(10,0);
			HD44780_PrintStr("START");
			HD44780_SetCursor(10,1);
			HD44780_PrintStr("SELECT");
			selector = MENU_SAG;

		}
		else if (BUTTON_RIGHT == button)
		{
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("START MESSURE");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("SELECT");
			selector = MENU_MEASURMENT;

		}
		else
		{
				 HD44780_Clear();
				 HD44780_SetCursor(0,0);
				 HD44780_PrintStr("SAG");
				 HD44780_SetCursor(0,1);
				 HD44780_PrintStr("LEFT");
				 HD44780_SetCursor(9,0);
				 HD44780_PrintStr("MESSURE");
				 HD44780_SetCursor(11,1);
				 HD44780_PrintStr("RIGHT");
		}
		stopAdcDma();
		break;

	case (MENU_SAG):
		if (BUTTON_SELECT == button)
		{
			startAdcDma();
			HD44780_Clear();
			selector = MENU_SAG_START;
			puts("pomiar rozpoczety, kliknij select aby zakonczyc\n");
		}
		else if (BUTTON_LEFT == button)
		{
			selector = MENU_CALIBRATION;
			HD44780_Init(2);
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("CALIBRATION");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("START");
			HD44780_SetCursor(10,1);
			HD44780_PrintStr("SELECT");
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
			travelPressureSensorCalibration();
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("CAL FINISH");
			osDelay(2000);
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("SAG");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("LEFT");
			HD44780_SetCursor(9,0);
			HD44780_PrintStr("MESSURE");
			HD44780_SetCursor(11,1);
			HD44780_PrintStr("RIGHT");
			selector = MENU_START;

		}
		else if (BUTTON_RIGHT == button)
		{
			selector = MENU_START;
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("SAG");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("LEFT");
			HD44780_SetCursor(9,0);
			HD44780_PrintStr("MESSURE");
			HD44780_SetCursor(11,1);
			HD44780_PrintStr("RIGHT");
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
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("SAG");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("LEFT");
			HD44780_SetCursor(9,0);
			HD44780_PrintStr("MESSURE");
			HD44780_SetCursor(11,1);
			HD44780_PrintStr("RIGHT");
		}
		else
		{
			puts("nacisnij select aby wrocic do menu start");
		}
		break;

	case (MENU_MEASURMENT):
		if (BUTTON_SELECT == button)
		{
			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("STARTING");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("FINISH");
			HD44780_SetCursor(10,1);
			HD44780_PrintStr("SELECT");
			setPath(dir, frontSensor, rearSensor, frontPressureSensor,
					rearPressureSensor, path);
			createNewFile(dir, frontSensor, rearSensor, frontPressureSensor,
					rearPressureSensor, &path);
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

			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("FINISHED");
			osDelay(2000);

			HD44780_Clear();
			HD44780_SetCursor(0,0);
			HD44780_PrintStr("SAG");
			HD44780_SetCursor(0,1);
			HD44780_PrintStr("LEFT");
			HD44780_SetCursor(9,0);
			HD44780_PrintStr("MESSURE");
			HD44780_SetCursor(11,1);
			HD44780_PrintStr("RIGHT");
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
void menuCalculateBlock(void)
{
	switch (selector)
	{

	case (MENU_SAG_START):
		int16_t result[2] =
		{ 0 };
		int16_t resultPressure[2] =
		{ 0 };
		char lcdFirstLineTravel[8] = {0};
		char lcSecondLineTravel[8] = {0};
		char lcdFirstLinePressure[8] = {0};
		char lcSecondLinePressure[8] = {0};
		processDataSag(result, resultPressure);
		sprintf(lcdFirstLineTravel,"FT: %d", result[1]);
		sprintf(lcSecondLineTravel,"RT: %d", result[0]);
		sprintf(lcdFirstLinePressure, "FP: %d", resultPressure[1]);
		sprintf(lcSecondLinePressure, "RP: %d", resultPressure[0]);
		HD44780_NoBlink();
		HD44780_Clear();
		HD44780_SetCursor(0,0);
		HD44780_PrintStr(lcdFirstLineTravel);
		HD44780_SetCursor(0,1);
		HD44780_PrintStr(lcSecondLineTravel);
		HD44780_SetCursor(9,0);
		HD44780_PrintStr(lcdFirstLinePressure);
		HD44780_SetCursor(9,1);
		HD44780_PrintStr(lcSecondLinePressure);
		osDelay(1000);
		break;
	case (MENU_MEASURMENT_START):
		processData(frontSensor, rearSensor, frontPressureSensor,
				rearPressureSensor);
		break;
	default:
	}

}

/*************** END OF FUNCTIONS ***************************************************************************/
