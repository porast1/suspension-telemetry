/*
 * File_Handling_RTOS.c
 *
 *  Created on: 30-April-2020
 *      Author: Controllerstech
 */

#include "File_Handling_RTOS.h"
#include "stm32f4xx_hal.h"
#include "ff.h"
#include "travelSensor.h"

#define UART_NO_ACTIVE

#ifdef UART_ACTIVE
extern UART_HandleTypeDef huart2;
#define UART &huart2
#endif


/* =============================>>>>>>>> NO CHANGES AFTER THIS LINE =====================================>>>>>>> */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

void setPath(char *dir, char* sensorData,  uint8_t path)
{
	sprintf(dir, "Data%d", path);
	sprintf(sensorData, "Data%d/Data%d.txt", path, path);

}
void createNewFile(char *dir, char* sensorData,  uint8_t *pathPtr)
{
	uint8_t path = *pathPtr;
	Mount_SD("/");
	Format_SD(path);
	Create_Dir(dir);
	Create_File(sensorData);

	Unmount_SD("/");
	(*pathPtr)++;
}

void Send_Uart(char *string)
{
#ifdef UART_ACTIVE
	HAL_UART_Transmit(UART, (uint8_t*) string, strlen(string), HAL_MAX_DELAY);
#endif
}

void Mount_SD(const TCHAR *path)
{
	fresult = f_mount(&fs, path, 1);
#ifdef UART_ACTIVE
	if (fresult != FR_OK)
		Send_Uart("ERROR!!! in mounting SD CARD...\n\n");
	else
		Send_Uart("SD CARD mounted successfully...\n");
#endif
}

void Unmount_SD(const TCHAR *path)
{
	fresult = f_mount(NULL, path, 1);
#ifdef UART_ACTIVE
	if (fresult == FR_OK)
		Send_Uart("SD CARD UNMOUNTED successfully...\n\n\n");
	else
		Send_Uart("ERROR!!! in UNMOUNTING SD CARD\n\n\n");
#endif
}

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD(char *pat)
{
	DIR dir;
	UINT i;
	char *path = pvPortMalloc(20 * sizeof(char));
	sprintf(path, "%s", pat);

	fresult = f_opendir(&dir, path); /* Open the directory */
	if (fresult == FR_OK)
	{
		for (;;)
		{
			fresult = f_readdir(&dir, &fno); /* Read a directory item */
			if (fresult != FR_OK || fno.fname[0] == 0)
				break; /* Break on error or end of dir */
			if (fno.fattrib & AM_DIR) /* It is a directory */
			{
				if (!(strcmp("SYSTEM~1", fno.fname)))
					continue;
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(30 * sizeof(char));
				sprintf(buf, "Dir: %s\r\n", fno.fname);
				Send_Uart(buf);
				vPortFree(buf);
#endif
				i = strlen(path);
				sprintf(&path[i], "/%s", fno.fname);
				fresult = Scan_SD(path); /* Enter the directory */
				if (fresult != FR_OK)
					break;
				path[i] = 0;
			}
			else
			{ /* It is a file. */
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(30 * sizeof(char));
				sprintf(buf, "File: %s/%s\n", path, fno.fname);
				Send_Uart(buf);
				vPortFree(buf);
#endif
			}
		}
		f_closedir(&dir);
	}
	vPortFree(path);
	return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_SD(uint8_t dirNumber)
{
	DIR dir;
	FILINFO fno;
	char *path = pvPortMalloc(20 * sizeof(char));
	sprintf(path, "/Data%d", dirNumber);
	fresult = f_opendir(&dir, path);

	if (fresult == FR_OK)
	{
		while (1)
		{
			fresult = f_readdir(&dir, &fno); /* Read a directory item */
			if (fresult != FR_OK || fno.fname[0] == 0)
				break; /* Break on error or end of dir */
			if (!(strcmp(".", fno.fname)) || !(strcmp("..", fno.fname)))
			{
				continue;
			}

			sprintf(path, "/Data%d/%s", dirNumber, fno.fname);
			if (fno.fattrib & AM_DIR) /* It is a directory */
			{

				fresult = Format_SD(dirNumber);
				if (fresult != FR_OK)
					break;
			}
			else
			{
				fresult = f_unlink(path);
				if (fresult != FR_OK)
					break;
			}
		}
		f_closedir(&dir);
	}

	if (fresult == FR_OK)
	{
		memset(path, 0, 20);
		sprintf(path, "/Data%d", dirNumber);
		fresult = f_unlink(path);
	}

	vPortFree(path);
	return (fresult);
}

FRESULT Write_File(char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat(name, &fno);
	if (fresult != FR_OK)
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
		return fresult;
	}

	else
	{
		/* Create a file with read write access and open it */
		fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
			return fresult;
		}

		else
		{
			fresult = f_write(&fil, data, strlen(data), &bw);
			if (fresult != FR_OK)
			{
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(100 * sizeof(char));
				sprintf(buf,
						"ERROR!!! No. %d while writing to the FILE *%s*\n\n",
						fresult, name);
				Send_Uart(buf);
				vPortFree(buf);
#endif
			}

			/* Close file */
			fresult = f_close(&fil);
			if (fresult != FR_OK)
			{
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(100 * sizeof(char));
				sprintf(buf,
						"ERROR!!! No. %d in closing file *%s* after writing it\n\n",
						fresult, name);
				Send_Uart(buf);
				vPortFree(buf);
#endif
			}
			else
			{
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(100 * sizeof(char));
				sprintf(buf, "File *%s* is WRITTEN and CLOSED successfully\n",
						name);
				Send_Uart(buf);
				vPortFree(buf);
#endif
			}
		}
		return fresult;
	}
}

FRESULT Read_File(char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat(name, &fno);
	if (fresult != FR_OK)
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "ERRROR!!! *%s* does not exists\n\n", name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
		return fresult;
	}

	else
	{
		/* Open file to read */
		fresult = f_open(&fil, name, FA_READ);

		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
			return fresult;
		}

		/* Read data from the file
		 * see the function details for the arguments */

		char *buffer = pvPortMalloc(sizeof(f_size(&fil)));
		fresult = f_read(&fil, buffer, f_size(&fil), &br);
		if (fresult != FR_OK)
		{
			vPortFree(buffer);
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult,
					name);
			Send_Uart(buffer);
			vPortFree(buf);
#endif
		}

		else
		{
#ifdef UART_ACTIVE
			Send_Uart(buffer);
#endif
			vPortFree(buffer);

			/* Close file */
			fresult = f_close(&fil);
			if (fresult != FR_OK)
			{
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(100 * sizeof(char));
				sprintf(buf, "ERROR!!! No. %d in closing file *%s*\n\n",
						fresult, name);
				Send_Uart(buf);
				vPortFree(buf);
#endif
			}
			else
			{
#ifdef UART_ACTIVE
				char *buf = pvPortMalloc(100 * sizeof(char));
				sprintf(buf, "File *%s* CLOSED successfully\n", name);
				Send_Uart(buf);
				vPortFree(buf);
#endif
			}
		}
		return fresult;
	}
}

FRESULT Create_File(char *name)
{
	fresult = f_stat(name, &fno);
	if (fresult == FR_OK)
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",
				name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
		return fresult;
	}
	else
	{
		fresult = f_open(&fil, name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
			return fresult;
		}
		else
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf,
					"*%s* created successfully\n Now use Write_File to write data\n",
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}

		fresult = f_close(&fil);
		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR No. %d in closing file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}
		else
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "File *%s* CLOSED successfully\n", name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}
	}
	return fresult;
}

FRESULT Update_File(char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat(name, &fno);
	if (fresult != FR_OK)
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
		return fresult;

	}

	else
	{
		/* Create a file with read write access and open it */
		fresult = f_open(&fil, name, FA_OPEN_APPEND | FA_WRITE);
		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
			return fresult;
		}

		/* Writing text */
		fresult = f_write(&fil, data, strlen(data), &bw);
		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}

		else
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "*%s* UPDATED successfully\n", name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}

		/* Close file */
		fresult = f_close(&fil);
		if (fresult != FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult,
					name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}
		else
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "File *%s* CLOSED successfully\n", name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}
	}
	return fresult;
}

FRESULT Remove_File(char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat(name, &fno);
	if (fresult != FR_OK)
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
		return fresult;
	}

	else
	{
		fresult = f_unlink(name);
		if (fresult == FR_OK)
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "*%s* has been removed successfully\n", name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}

		else
		{
#ifdef UART_ACTIVE
			char *buf = pvPortMalloc(100 * sizeof(char));
			sprintf(buf, "ERROR No. %d in removing *%s*\n\n", fresult, name);
			Send_Uart(buf);
			vPortFree(buf);
#endif
		}
	}
	return fresult;
}

FRESULT Create_Dir(char *name)
{
	fresult = f_mkdir(name);
	if (fresult == FR_OK)
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "*%s* has been created successfully\n", name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
	}
	else
	{
#ifdef UART_ACTIVE
		char *buf = pvPortMalloc(100 * sizeof(char));
		sprintf(buf, "ERROR No. %d in creating directory *%s*\n\n", fresult,
				name);
		Send_Uart(buf);
		vPortFree(buf);
#endif
	}
	return fresult;
}

void Check_SD_Space(void)
{
	/* Check free space */
	f_getfree("", &fre_clust, &pfs);

	total = (uint32_t) ((pfs->n_fatent - 2) * pfs->csize * 0.5);
#ifdef UART_ACTIVE
	char *buf = pvPortMalloc(30 * sizeof(char));
	sprintf(buf, "SD CARD Total Size: \t%lu\n", total);
	Send_Uart(buf);
	vPortFree(buf);
#endif
	free_space = (uint32_t) (fre_clust * pfs->csize * 0.5);
#ifdef UART_ACTIVE
	buf = pvPortMalloc(30 * sizeof(char));
	sprintf(buf, "SD CARD Free Space: \t%lu\n", free_space);
	Send_Uart(buf);
	vPortFree(buf);
#endif
}
void sendDataSD(char *file, volatile float *sensor)
{
	char *buffer = pvPortMalloc((7*TRAVEL_SENSOR_BUFFER_SIZE/2) * sizeof(char));
	if (NULL != buffer){
		memset(buffer, 0, (7*TRAVEL_SENSOR_BUFFER_SIZE/2));
		for (int i = 0; i < (TRAVEL_SENSOR_BUFFER_SIZE) / 2; i++)
		{
			sprintf(buffer + strlen(buffer), "%.2f;","%.2f;","%.2f;","%.2f\n",
					sensor[i + FRONT_TRAVEL_BUFFER_POSITION],
					sensor[i + REAR_TRAVEL_BUFFER_POSITION],
					sensor[i + FRONT_PRESSURE_BUFFER_POSITION],
					sensor[i + REAR_PRESSURE_BUFFER_POSITION] );
		}
		Mount_SD("/");
		Update_File(file, buffer);
		Unmount_SD("/");
		vPortFree(buffer);
	}
	else{
		puts("Wrong alocation pressureTravel Buffer SD Card");
	}

}
