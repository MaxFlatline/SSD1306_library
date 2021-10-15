/*
 * oled096.h
 *
 *  Created on: Sep 18, 2021
 *      Author: Maxim Hodnenko
 */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "i2c.h"
#include <stdint.h>
/* USER CODE END Includes */

/* OLED PARAMETERS BEGIN DEFINES */
/* OLED parameters ---------------------------------------------------------*/
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_BUFFERSIZE (OLED_WIDTH*OLED_HEIGHT)/8
#define OLED_DEFAULT_SPACE 0
/* OLED PARAMETERS END DEFINES */

/* OLED COMMANDS BEGIN DEFINES */
/* OLED commands ---------------------------------------------------------*/
#define OLED_adress 0x78
#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR   0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D
#define OLED_SWITCHCAPVCC 0x2
#define OLED_NOP 0xE3

//#define COMMAND 0x00
//#define DATA   0x40
/* OLED COMMANDS END DEFINES */

/* USER CODE BEGIN PV */
/* Imported variables ---------------------------------------------------------*/
extern uint8_t LCD_Buffer[OLED_WIDTH * OLED_HEIGHT / 8];

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

typedef enum
{
	COMMAND		  = 0x00U,
	DATA    	  = 0x40U
} OLED_DataType;

typedef struct __OLED_HandleTypeDef
{
	uint8_t AddressI2C; // Can be 111100 or 111101, default 111100(for STM HAL left shift needed)

	OLED_StatusTypeDef (*DataSend)(OLED_DataType Descriptor, const uint8_t * Data);//Abstract data send function


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
void i2c_init(void);
void OLED_init(OLED_HandleTypeDef* oled);
void sendCommand(unsigned char command);
void LCD_Clear(void);
void Set_Contrast(uint8_t value);

void LCD_Char(uint8_t c);

void LCD_Goto(unsigned char x, unsigned char y);
void LCD_DrawImage(unsigned char num_image);
void OLED_string(char *string);
void OLED_num_to_str(unsigned int value, unsigned char nDigit);
/* OLED FUNCTIONS END PROTOTYPES */

/* OLED Function refactored */






OLED_StatusTypeDef OLED_SendData (OLED_DataType Descriptor, const uint8_t * Data);

