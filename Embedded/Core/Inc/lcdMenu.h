/****************************************************************************
 * Title                 :   Suspension Telemetry 
 * Filename              :   lcdMenu.h
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 * Notes                 :   None
 *****************************************************************************/

/** \file lcdMenu.h
 *  \brief This module contains ...
 * 
 *  This is the header file for the definition for tasks that ...
 */
#ifndef INC_LCDMENU_H_
#define INC_LCDMENU_H_

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "main.h"

/******************************************************************************
 * Preprocessor Constants
 *******************************************************************************/
/**
 * This constant is
 */

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
void lcdMenuSagStart(char* frontTravelBuf,char* rearTravelBuf, char* frontPressureBuf, char* rearPressureBuf);
void lcdMenuFinishedNotification(char *notification);
void lcdMenuStartBatVoltage(char * batteryVoltage);
void lcdMenuStart();
void lcdMenuSag();
void lcdStartMeasurement();
void lcdCalibration();
void lcdMeasurementStart();
#endif /* INC_LCDMENU_H_ */

/*** End of File **************************************************************/
