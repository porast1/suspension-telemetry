/*
 * File_Handling_RTOS.h
 *
 *  Created on: 30-April-2020
 *      Author: Controllerstech
 */

#ifndef FILE_HANDLING_RTOS_H_
#define FILE_HANDLING_RTOS_H_

#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "travelSensor.h"

void setPath(char *dir, char *sensorData, uint8_t path);
void createNewFile(char *dir, char *sensorData, uint8_t *pathPtr);

/* mounts the sd card*/
void Mount_SD(const TCHAR *path);

/* unmounts the sd card*/
void Unmount_SD(const TCHAR *path);

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD(char *pat);

/* Only supports removing files from home directory. Directory remover to be added soon */
FRESULT Format_SD(uint8_t dirNumber);

/* write the data to the file
 * @ name : is the path to the file*/
FRESULT Write_File(char *name, char *data);

/* read data from the file
 * @ name : is the path to the file*/
FRESULT Read_File(char *name);

/* creates the file, if it does not exists
 * @ name : is the path to the file*/
FRESULT Create_File(char *name);

/* Removes the file from the sd card
 * @ name : is the path to the file*/
FRESULT Remove_File(char *name);

/* creates a directory
 * @ name: is the path to the directory
 */
FRESULT Create_Dir(char *name);

/* checks the free space in the sd card*/
void Check_SD_Space(void);

/* updates the file. write pointer is set to the end of the file
 * @ name : is the path to the file
 */
FRESULT Update_File(char *name, char *data);

void sendDataSD(char *file, volatile float *sensor);

int readCalibrationData(calibration_t *calibration);
int writeCalibrationData(const calibration_t *calibration);

#endif /* FILE_HANDLING_RTOS_H_ */
