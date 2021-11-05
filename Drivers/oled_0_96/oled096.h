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
#define OLED_adress 0x78
#define OLED_MAX_TRIALS 3

#define OLED_SETCONTRAST 0x81
#define OLED_SETCONTRAST_MAX 0xFF
#define OLED_SETCONTRAST_MIN 0x00

#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5

#define OLED_COLOURS_INV	0xA6
#define OLED_COLOURS_INV_TRUE	0x01
#define OLED_COLOURS_INV_FALSE	0x00

#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3



#define OLED_SETCOMPINS 0xDA
#define OLED_SETCOMPINS_COM_CONF_SEQ 0x02
#define OLED_SETCOMPINS_COM_CONF_ALT 0x12
#define OLED_SETCOMPINS_COM_CONF_SEQ 0x02
#define OLED_SETCOMPINS_COM_REMAP_NONE 0x02
#define OLED_SETCOMPINS_COM_REMAP_TRUE 0x22


#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETVCOMDETECT_TOP 0x30
#define OLED_SETVCOMDETECT_MID 0x20
#define OLED_SETVCOMDETECT_LOW 0x00


#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_DISPLAYCLOCKFREQ_MIN 0x00
#define OLED_DISPLAYCLOCKFREQ_300 0x60
#define OLED_DISPLAYCLOCKFREQ_400 0xC0
#define OLED_DISPLAYCLOCKFREQ_MAX 0xF0

#define OLED_SETPRECHARGE 0xD9
#define OLED_SETPRECHARGE_PHASE_1 0xF0
#define OLED_SETPRECHARGE_PHASE_2 0x01


#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10

#define OLED_SETSTARTLINE 		0x40
#define OLED_STARTLINE_DEFAULT  0x00

//------------------IMAGE ORIENTATION COMMANDS--------------//
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8

#define OLED_SEGREMAP 			0xA0
#define OLED_SEGREMAP_0_SEG0 	0x00
#define OLED_SEGREMAP_127_SEG0 	0x01
//------------------IMAGE ORIENTATION COMMANDS END-----------//


#define OLED_MEMORYMODE 0x20
#define OLED_MEMORYMODE_PGE 0x10
#define OLED_MEMORYMODE_HOR 0x00
#define OLED_MEMORYMODE_VER 0x01

#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR   0x22

#define OLED_SET_ADDR_PAGE_MODE 0xB0
#define OLED_SET_CLMN_PAGE_MODE_L 0x0F
#define OLED_SET_CLMN_PAGE_MODE_H 0x10



#define OLED_CHARGEPUMP 0x8D
#define OLED_CHARGEPUMP_ON 0x14
#define OLED_CHARGEPUMP_OFF 0x10

#define OLED_SWITCHCAPVCC 0x2
#define OLED_NOP 0xE3


/* OLED COMMANDS END DEFINES */

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
	SUCCES		= 0x00U,
	FAULT 		= 0x01U
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


