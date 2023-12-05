/*******************************************************************************
 * Title                 :   Suspension Telemetry
 * Filename              :   ADXL345.c
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 *******************************************************************************/

/** \file ADXL345.c
 * \brief This module contains the ...
 */

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "ADXL345.h"
#include "i2c.h"
#include "File_Handling_RTOS.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
/**
 * Constants to ...
 */
#define   CONSTANT					5
#define adxl_address 0x53 << 1
/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/

/******************************************************************************
 * Module Typedefs
 *******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/

uint8_t chipid = 0;
uint16_t axdlDataBuffer[128];
/******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void adxl_write(uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit(&hi2c1, adxl_address, data, 2, 100);
}

void adxl_read_values(uint8_t reg)
{
	//HAL_I2C_Mem_Read (&hi2c1, adxl_address, reg, 1, (uint8_t *)data_rec, 6, 100);
}

void adxl_read_address(uint8_t reg)
{
	HAL_I2C_Mem_Read(&hi2c1, adxl_address, reg, 1, &chipid, 1, 100);
}

void adxl_init(void)
{
	adxl_read_address(0x00); // read the DEVID
	adxl_write(0x31, 0x11);  // data_format range= +- 16g
	adxl_write(0x2d, 0x00);  // reset all bits
	adxl_write(0x2d, 0x08);  // power_cntl measure and wake up 8hz

}

/*************** END OF FUNCTIONS ***************************************************************************/
