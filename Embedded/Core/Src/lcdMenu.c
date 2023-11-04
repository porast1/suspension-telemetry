/*******************************************************************************
* Title                 :   Suspension Telemetry
* Filename              :   lcdMenu.c
* Author                :   patryk.grzywnowicz@gmail.com
* Compiler              :   GCC
* Target                :   data_acquisition_module
*******************************************************************************/

/** \file lcdMenu.c
 * \brief This module contains the ...
 */
 
/******************************************************************************
* Includes
*******************************************************************************/
#include "lcdMenu.h"
#include "liquidcrystal_i2c.h"
#include "stdio.h"
#include "stdint.h"
#include "cmsis_os.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
/**
 * Constants to ...
 */
#define   CONSTANT					5

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
void lcdMenuSagStart(int32_t * result, int32_t * resultPressure)
{
	char lcdFirstLineTravel[8] =
	{ 0 };
	char lcSecondLineTravel[8] =
	{ 0 };
	char lcdFirstLinePressure[8] =
	{ 0 };
	char lcSecondLinePressure[8] =
	{ 0 };
	sprintf(lcdFirstLineTravel, "FT: %ld", result[0]);
	sprintf(lcSecondLineTravel, "RT: %ld", result[1]);
	sprintf(lcdFirstLinePressure, "FP: %ld", resultPressure[0]);
	sprintf(lcSecondLinePressure, "RP: %ld", resultPressure[1]);
	HD44780_NoBlink();
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr(lcdFirstLineTravel);
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr(lcSecondLineTravel);
	HD44780_SetCursor(9, 0);
	HD44780_PrintStr(lcdFirstLinePressure);
	HD44780_SetCursor(9, 1);
	HD44780_PrintStr(lcSecondLinePressure);
	osDelay(1000);
}
void lcdMenuFinishedNotification(char *notification)
{
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr(notification);
	osDelay(2000);
}

void lcdMenuStart()
{
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("SAG");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("LEFT");
	HD44780_SetCursor(9, 0);
	HD44780_PrintStr("MESSURE");
	HD44780_SetCursor(11, 1);
	HD44780_PrintStr("RIGHT");
}

void lcdMenuSag()
{
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("CALIB");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("LEFT");
	HD44780_SetCursor(10, 0);
	HD44780_PrintStr("START");
	HD44780_SetCursor(10, 1);
	HD44780_PrintStr("SELECT");
}

void lcdStartMeasurement()
{
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("START MESSURE");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("SELECT");
}

void lcdCalibration()
{
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("CALIBRATION");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("START");
	HD44780_SetCursor(10, 1);
	HD44780_PrintStr("SELECT");
}

void lcdMeasurementStart()
{
	HD44780_Clear();
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("STARTING");
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("FINISH");
	HD44780_SetCursor(10, 1);
	HD44780_PrintStr("SELECT");
}

/*************** END OF FUNCTIONS ***************************************************************************/
