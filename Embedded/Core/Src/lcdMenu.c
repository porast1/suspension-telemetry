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
#include "stdio.h"
#include "stdint.h"
#include "cmsis_os.h"
#include "fonts.h"
#include "ssd1306.h"
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
void lcdMenuSagStart(char* frontTravelBuf,char* rearTravelBuf, char* frontPressureBuf, char* rearPressureBuf)
{
	char lcdFirstLineTravel[9] =
	{ 0 };
	char lcSecondLineTravel[9] =
	{ 0 };
	char lcdFirstLinePressure[8] =
	{ 0 };
	char lcSecondLinePressure[8] =
	{ 0 };
	sprintf(lcdFirstLineTravel, "FT: %s", frontTravelBuf);
	sprintf(lcSecondLineTravel, "RT: %s", rearTravelBuf);
	sprintf(lcdFirstLinePressure, "FP: %s", frontPressureBuf);
	sprintf(lcSecondLinePressure, "RP: %s", rearPressureBuf);
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts(lcdFirstLineTravel, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts(lcSecondLineTravel, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(78, 10);
	SSD1306_Puts(lcdFirstLinePressure, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(78, 30);
	SSD1306_Puts(lcSecondLinePressure, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 50);
	SSD1306_Puts("FINISH hold LEFT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}
void lcdMenuFinishedNotification(char *notification)
{
	SSD1306_Clear();
	SSD1306_GotoXY(30, 30);
	SSD1306_Puts(notification, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
	osDelay(2000);
}
void lcdMenuStartBatVoltage(char * batteryVoltage){
	SSD1306_GotoXY(10, 50);
	char buf[30] = "Battery: ";
	strcat(buf, batteryVoltage);
	SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}
void lcdMenuStart()
{
	SSD1306_Clear();
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts("SAG", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts("LEFT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 10);
	SSD1306_Puts("MESSURE", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 30);
	SSD1306_Puts("RIGHT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

void lcdMenuSag()
{
	SSD1306_Clear();
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts("CALIB", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts("LEFT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 10);
	SSD1306_Puts("MENU", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 30);
	SSD1306_Puts("RIGHT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 50);
	SSD1306_Puts("START hold LEFT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

void lcdStartMeasurement()
{
	SSD1306_Clear();
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts("MENU", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts("LEFT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 10);
	SSD1306_Puts("MESSURE", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 50);
	SSD1306_Puts("START hold RIGHT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

void lcdCalibration()
{
	SSD1306_Clear();
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts("CALIB", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 10);
	SSD1306_Puts("MENU", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(68, 30);
	SSD1306_Puts("RIGHT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 50);
	SSD1306_Puts("START hold LEFT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

void lcdMeasurementStart()
{
	SSD1306_Clear();
	SSD1306_GotoXY(30, 30);
	SSD1306_Puts("STARTING", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 50);
	SSD1306_Puts("FINISH hold RIGHT", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

/*************** END OF FUNCTIONS ***************************************************************************/
