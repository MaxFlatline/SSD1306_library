/**
  ******************************************************************************
  * @file           : oled096.h
  * @brief          : Header for oled096.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  *  Created on: Sep 18, 2021
  *      Author: Maxim Hodnenko
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "i2c.h"
#include <stdint.h>
#include "ssd1306.h"
#include "Font.c"
/* USER CODE END Includes */

/* OLED PARAMETERS BEGIN DEFINES */
/* OLED parameters ---------------------------------------------------------*/
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_096_SEGS 128
#define OLED_096_PAGES 8

#define OLED_BUFFERSIZE (OLED_WIDTH*OLED_HEIGHT)/8
#define OLED_DEFAULT_SPACE 0
/* OLED PARAMETERS END DEFINES */

/* OLED COMMANDS BEGIN DEFINES */
/* OLED commands ---------------------------------------------------------*/
#define OLED_ADRESS 0x78
#define OLED_MAX_TRIALS 3



/* USER CODE BEGIN PV */
/* Imported variables ---------------------------------------------------------*/
extern uint8_t temp_char[7];
extern unsigned char LCD_X,LCD_Y;  //Cursor coordinates
/* private variables ---------------------------------------------------------*/

/* USER CODE END PV */



/* Private types -------------------------------------------------------------*/

/**
  * @brief  OLED communication statuses
  */
typedef enum
{
	OLED_OK       = 0x00U,
	OLED_ERROR    = 0x01U,
	OLED_BUSY     = 0x02U,
	OLED_TIMEOUT  = 0x03U,
	OLED_DESCFAIL = 0x04U
} OLED_StatusTypeDef;
/**
  *  END
  */



/**
  * @brief  OLED Error Management
  */
typedef enum
{
	HANDLE_SUCCESS		= 0x00U,
	HANDLE_FAULT 		= 0x01U
}OLED_ErrorHandlerType;
/**
  *  END
  */



typedef enum
{
	COMMAND		  = 0x00U,
	DATA    	  = 0x40U
} OLED_DataType;

/**
  * @brief  OLED data transmit function type
  */
typedef OLED_StatusTypeDef (*DataSend)(OLED_DataType Descriptor, uint8_t AddressI2C, uint8_t *Data, size_t length);
/**
  *  END
  */

/**
  * @brief  OLED Cursor current position
  */
typedef struct
{
	uint8_t Page;

	uint8_t Segment;

} OLED_CursorCoordTypeDef;

/**
  * @brief  OLED main structure
  */
typedef struct
{
	uint8_t AddressI2C; 				// Can be 111100 or 111101, default 111100(for STM HAL left shift needed)

	DataSend DataSend;					// Abstract data send function

	uint8_t OLEDErrorSolvingTrials;		// Amount of tries to reach OLED

	uint8_t * FrameMem;					// Pointer to heap area for frame buffer

	uint8_t Width;						// Definition of display in 8 bit segments

	uint8_t Heigth;						// Definition of display in pages

	uint16_t FrameSize;					// Pages X Segments - in bytes

	OLED_CursorCoordTypeDef * Cursor;	// Active segment and page in GDDR for new data

}OLED_HandleTypeDef;









/**
  * @brief  HAL Lock structures definition
  */
typedef enum
{
  OLED_UNLOCKED = 0x00U,
  OLED_LOCKED   = 0x01U
} OLED_LockTypeDef;



extern OLED_HandleTypeDef OLED096;


/* Private function prototypes -----------------------------------------------*/
/* OLED FUNCTIONS BEGIN PROTOTYPES */


OLED_StatusTypeDef OLED_Init(OLED_HandleTypeDef *OLED);
OLED_StatusTypeDef OLED_DeInit(OLED_HandleTypeDef *OLED);
OLED_StatusTypeDef OLED_FrameRefresh (OLED_HandleTypeDef *OLED);
OLED_StatusTypeDef OLED_DrawTestImage(OLED_HandleTypeDef *OLED);


void OLED_Set_Contrast(OLED_HandleTypeDef* OLED, uint8_t *value);

void LCD_Char(uint8_t c);

void OLED_Set_Cursor(OLED_HandleTypeDef* OLED, uint8_t Byte, uint8_t Page);

void OLED_string(char *string);
void OLED_num_to_str(unsigned int value, unsigned char nDigit);
/* OLED FUNCTIONS END PROTOTYPES */

/* OLED Function refactored */


