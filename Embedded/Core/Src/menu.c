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
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
/**
 * Constants to ...
 */

#define FILE_NAME_SIZE 20


typedef enum menu{
	MENU_START = 0U,
	MENU_SAG,
	MENU_SAG_START,
	MENU_MEASURMENT,
	MENU_MEASURMENT_START

}menu_t;

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
/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void menuSelector(button_t button){

	switch(selector){
		case(MENU_START):
			if(BUTTON_LEFT == button){
				selector = MENU_SAG;
				puts("jestes w menu sag, kliknij select aby rozpoczac pomiar\n");
			}
			else if(BUTTON_RIGHT == button){
				selector = MENU_MEASURMENT;
				puts("jestes w menu measurment, kliknij select aby rozpoczac pomiar\n");
			}
			else {
				puts("jestes w menu start, kliknij:\n1.Pomiar Sagu\n2.Rozpocznij pomiary pracy zawieszenia\n");
			}
			stopAdcDma();
			break;
		case(MENU_SAG):
			if(BUTTON_SELECT == button){
				startAdcDma();
				selector = MENU_SAG_START;
				puts("pomiar rozpoczety, kliknij select aby zakonczyc\n");
			}
			else{
				puts("nacisnij select aby rozpoczac pomiar\n");
			}
			break;
		case(MENU_SAG_START):
			if(BUTTON_SELECT == button){

				selector = MENU_START;
				puts("pomiar zakonczony\njestes w menu start, kliknij:\n1.Pomiar Sagu\n2.Rozpocznij pomiary pracy zawieszenia\n");
			}
			else{
				puts("nacisnij select aby wrocic do menu start");
			}
			break;
		case(MENU_MEASURMENT):
			if(BUTTON_SELECT == button){
				setPath(dir, frontSensor, rearSensor, path);
				createNewFile(dir, frontSensor, rearSensor, &path);
				startAdcDma();
				selector = MENU_MEASURMENT_START;
				puts("pomiar ciagly rozpoczety, kliknij select aby zakonczyc\n");
			}
			else{
				puts("nacisnij select aby rozpoczac pomiar\n");
				}
			break;
		case(MENU_MEASURMENT_START):
			if(BUTTON_SELECT == button){
				selector = MENU_START;
				puts("pomiar ciagly zakonczony\njestes w menu start, kliknij:\n1.Pomiar Sagu\n2.Rozpocznij pomiary pracy zawieszenia\n");
			}
			else{
				puts("nacisnij select aby wrocic do menu start");
			}
			break;
		default:
	}


}
void menuCalculateBlock(void){
	switch(selector){

			case(MENU_SAG_START):
				uint16_t result[2] = {0};
				processDataSag(result);
				printf("REAR: %d\nFRONT: %d\n",result[0], result[1]);
				break;
			case(MENU_MEASURMENT_START):
				processData(frontSensor,rearSensor);
				break;
			default:
		}

}

/*************** END OF FUNCTIONS ***************************************************************************/
