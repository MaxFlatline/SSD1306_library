/*
 * oled096.c
 *
 *  Created on: Sep 18, 2021
 *      Author: Maxim Hodnenko
 */


#include "oled096.h"



/* Global variables */

OLED_HandleTypeDef OLED096;

/* Global variables END*/


//======Private function prototypes==========================================

static OLED_StatusTypeDef OLED_FrameMem_Init (OLED_HandleTypeDef * OLED);
static OLED_StatusTypeDef OLED_FrameMem_DeInit (OLED_HandleTypeDef * OLED);
static OLED_StatusTypeDef OLED_GDDR_Clear (OLED_HandleTypeDef* OLED);

typedef enum{
	SSD1306_DISPLAY_ON 	= 0xAF,
	SSD1306_DISPLAY_OFF = 0xAE,
} ssd1306_display_on_off_t;
static OLED_StatusTypeDef OLED_SetDisplayOnOff(OLED_HandleTypeDef *OLED, ssd1306_display_on_off_t value);

static OLED_StatusTypeDef OLED_SetContrast(OLED_HandleTypeDef *OLED, uint8_t value);

static OLED_ErrorHandlerType OLED_ErrorHandler (OLED_HandleTypeDef * OLED);
static OLED_StatusTypeDef OLED_SendData (OLED_DataType Descriptor, uint8_t AddressI2C, uint8_t *Data, size_t length);
//===========================================================================

/**
 * @brief Function initializes &OLED and clears it's screen
 *
 * @returns  Status of operation
 * 			 default : Same as HAL_I2C_Mem_Write
 *			 4 		: Transmit error
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_Init(OLED_HandleTypeDef* OLED)
{
	OLED_StatusTypeDef Result = OLED_OK;

	uint8_t tempBuf = 0;
	OLED->DataSend = OLED_SendData;
	OLED->AddressI2C = OLED_ADRESS;
	OLED->Heigth = OLED_096_PAGES;
	OLED->Width = OLED_096_SEGS;

	OLED->Cursor = NULL;

	// Turn display off
	Result = OLED_SetDisplayOnOff(OLED, 174);

	tempBuf = OLED_SETDISPLAYCLOCKDIV;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_DISPLAYCLOCKFREQ_MAX;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_SETMULTIPLEX;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_HEIGHT-1;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_SETDISPLAYOFFSET;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = 0x00;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SETSTARTLINE | OLED_STARTLINE_DEFAULT;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	// Horizontal memory mode
	tempBuf = OLED_MEMORYMODE;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_MEMORYMODE_HOR;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SEGREMAP | OLED_SEGREMAP_127_SEG0;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_COMSCANDEC;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_SETCOMPINS;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SETCOMPINS_COM_CONF_ALT | OLED_SETCOMPINS_COM_REMAP_NONE;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);


	Result = OLED_SetContrast(OLED, OLED_SETCONTRAST_MAX);


	tempBuf = OLED_SETPRECHARGE;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SETPRECHARGE_PHASE_1 | OLED_SETPRECHARGE_PHASE_2;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_SETVCOMDETECT;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SETVCOMDETECT_MID;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_DISPLAYALLON;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_DISPLAYALLON_RESUME;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);


	// Non-inverted COLORS of display
	tempBuf = OLED_COLORS_INV | OLED_COLORS_INV_FALSE;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	// We use internal charge pump
	tempBuf = OLED_CHARGEPUMP;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_CHARGEPUMP_ON;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	// Turn display back on
	Result = OLED_SetDisplayOnOff(OLED, 175);

	// Memory allocation for frame buffer
	Result = OLED_FrameMem_Init (OLED);

	OLED->FrameSize = OLED_096_PAGES * OLED_096_SEGS;

	Result = OLED_GDDR_Clear (OLED);
	if(Result != OLED_OK){
		  OLED_ErrorHandler (OLED);
	}

	return Result;
}

/**
 * @brief Function deinitialize &OLED and frees it's FrameMem
 *
 * @returns  Status of operation
 * 			 default 			: OLED_OK
 *			 OLED_ERROR 		: FrameMem is not initialized(NULL)
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_DeInit(OLED_HandleTypeDef* OLED){
	OLED_StatusTypeDef Result = OLED_OK;
	Result = OLED_SetDisplayOnOff(OLED, SSD1306_DISPLAY_OFF);
	Result = OLED_FrameMem_DeInit (OLED);
	HAL_I2C_DeInit(&hi2c1);
	MX_I2C1_Init();
	return Result;
}



/**
 * @brief Function refreshes OLED screen with current frame stored in &OLED
 *
 * @returns  Status of operation
 * 			 default : Same as HAL_I2C_Mem_Write
 *			 4 		: Transmit error
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_FrameRefresh (OLED_HandleTypeDef *OLED){
	OLED_StatusTypeDef Result = OLED_OK;
	if(OLED->FrameMem == NULL){
		return OLED_ERROR;
	}
	Result = OLED->DataSend(DATA, OLED->AddressI2C, OLED->FrameMem, OLED->FrameSize);
	return Result;
}


/**
 * @brief Function fills &OLED frame with test image
 *
 * @returns  Status of operation
 * 			 default    : OLED_OK
 *			 OLED_ERROR : FrameMem is not initialized(NULL)
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_DrawTestImage(OLED_HandleTypeDef *OLED){
	if(OLED->FrameMem == NULL){
		return OLED_ERROR;
	}

	uint8_t t = 0;
	uint8_t tempBuf = 0;
	for (size_t i = 0; i < OLED->FrameSize; i++)
	{

		if(t == 8)
		{
			tempBuf = ~tempBuf;
			t = 0;
		}
		*(OLED->FrameMem + i) = tempBuf;
		t++;

	}
	return OLED_OK;
}

/**
 * @brief Function sets display on/off
 *
 * @returns necessary 0 for correct execution
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1206_display_on_off_t CMD
 */
OLED_StatusTypeDef OLED_SetDisplayOnOff(OLED_HandleTypeDef *OLED, ssd1306_display_on_off_t value){
	OLED_StatusTypeDef Result = OLED_OK;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);
	return Result;
}
/*
 *
 * End function NAME_FUNCTION
 *
 */

/**
 * @brief Function sets OLED contrast to desired value
 *
 * @returns  Nothing
 *
 * @[IN]data uint8_t	 Desired contrast value (0-255)
 *
 */
OLED_StatusTypeDef OLED_SetContrast(OLED_HandleTypeDef* OLED, uint8_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETCONTRAST;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &value, 1);

	return Result;
}


/**
 *
 *
 * @brief API tool for cursor positioning
 *
 * @param &OLED - Display object
 * 		  Byte, Page - coordinates
 *
 * @returns bool status of error handling
 *
 */
void OLED_Set_Cursor(OLED_HandleTypeDef* OLED, uint8_t Byte, uint8_t Page)
{
	uint8_t tempBuf = OLED_SET_ADDR_PAGE_MODE + Page;
	OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = Byte & OLED_SET_CLMN_PAGE_MODE_L;
	OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SET_CLMN_PAGE_MODE_H | (Byte >> 4);
	OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
}



// Очистку экрана лучше всего делать с помощью отправки по i2c нулевого байта
// с DMA без автоинкремента
static OLED_StatusTypeDef OLED_GDDR_Clear (OLED_HandleTypeDef* OLED){
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = 0;
	for (uint16_t i = 0; i < (OLED->Heigth) * (OLED->Width); i++){
		Result = OLED_SendData (DATA, OLED->AddressI2C, &tempBuf, 1);
	}
	return Result;
}


/**
 *
 *
 * @brief Error management routine
 *
 * @returns bool status of error handling
 *
 */
static OLED_ErrorHandlerType OLED_ErrorHandler (OLED_HandleTypeDef * OLED){

	OLED_DeInit(OLED);
	if(OLED_Init(OLED) != OLED_OK && OLED->OLEDErrorSolvingTrials < OLED_MAX_TRIALS){
		OLED->OLEDErrorSolvingTrials++;
		return OLED_ErrorHandler(OLED);
	}
	else{
		return HANDLE_FAULT;
	}
	OLED->OLEDErrorSolvingTrials = 0;
	return HANDLE_SUCCESS;
}

/**
 * @brief Function initializes &OLED FrameMem
 *
 * @returns  Status of operation
 * 			 default 			: OLED_OK
 *			 OLED_ERROR 		: malloc error
 *
 * @[IN]data &OLED - Display object
 *
 */
static OLED_StatusTypeDef OLED_FrameMem_Init (OLED_HandleTypeDef * OLED){
	OLED->FrameMem = malloc(OLED_096_PAGES * OLED_096_SEGS);
	if(OLED->FrameMem == NULL){
		return OLED_ERROR;
	}
	return OLED_OK;
}

/**
 * @brief Function deinitialize &OLED FrameMem
 *
 * @returns  Status of operation
 * 			 default 			: OLED_OK
 *			 OLED_ERROR 		: FrameMem is not initialized(NULL)
 *
 * @[IN]data &OLED - Display object
 *
 */
static OLED_StatusTypeDef OLED_FrameMem_DeInit (OLED_HandleTypeDef * OLED){
	if(OLED->FrameMem == NULL){
		return OLED_ERROR;
	}
	free(OLED->FrameMem);
	return OLED_OK;
}
/*
 *
 * End function NAME_FUNCTION
 *
 */


/**
 * @brief Function sends data to OLED unit
 *
 * @returns  Status of operation
 * 			 default : Same as HAL_I2C_Transmit
 *			 4 		: Incorrect descriptor
 *
 * @[IN]data (enum Data/Command) Descriptor
 * 			 uint8_t 			 Data to send
 */
static OLED_StatusTypeDef OLED_SendData (OLED_DataType Descriptor, uint8_t AddressI2C, uint8_t *Data, size_t length){

	OLED_StatusTypeDef Result = OLED_OK;

	if(Descriptor == DATA || Descriptor == COMMAND){
		Result = HAL_I2C_Mem_Write(&hi2c1, (uint16_t)AddressI2C, (uint16_t)Descriptor, I2C_MEMADD_SIZE_8BIT, Data, length,100);
	}
	else{
		Result = OLED_DESCFAIL;
	}

	return Result;
}
/*
 *
 * End function NAME_FUNCTION
 *
 */
