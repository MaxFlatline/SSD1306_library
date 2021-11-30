/*
 * oled096.c
 *
 *  Created on: Sep 18, 2021
 *      Authors: Maxim Hodnenko, Nulliik
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
	SSD1306_DISPLAY_OFF = 0xAE
} ssd1306_display_on_off_t;
static OLED_StatusTypeDef OLED_SetDisplayOnOff(OLED_HandleTypeDef *OLED, ssd1306_display_on_off_t value);

typedef enum{
	SSD1306_CLOCKFREQ_MIN = 0x00,
	SSD1306_CLOCKFREQ_300 = 0x60,
	SSD1306_CLOCKFREQ_400 = 0xC0,
	SSD1306_CLOCKFREQ_MAX = 0xF0
} ssd1306_display_clock_freq_t;
static OLED_StatusTypeDef OLED_Set_Display_ClockDiv(OLED_HandleTypeDef* OLED, ssd1306_display_clock_freq_t value);

typedef enum{
	SSD1306_COLORS_INV = OLED_COLORS_INV|OLED_COLORS_INV_TRUE,
	SSD1306_COLORS_NON_INV = OLED_COLORS_INV|OLED_COLORS_INV_FALSE
} ssd1306_display_color_t;
static OLED_StatusTypeDef OLED_Set_Display_ColorMode(OLED_HandleTypeDef* OLED, ssd1306_display_color_t value);

typedef enum{
	SSD1306_CHARGEPUMP_ON = OLED_CHARGEPUMP_ON,
	SSD1306_CHARGEPUMP_OFF = OLED_CHARGEPUMP_OFF
} ssd1306_chargepump_state_t;
static OLED_StatusTypeDef OLED_Set_ChargePumpOnOff(OLED_HandleTypeDef* OLED, ssd1306_chargepump_state_t value);

typedef enum{
	SSD1306_SETVCOMDETECT_TOP = OLED_SETVCOMDETECT_TOP,
	SSD1306_SETVCOMDETECT_MID = OLED_SETVCOMDETECT_MID,
	SSD1306_SETVCOMDETECT_LOW = OLED_SETVCOMDETECT_LOW
} ssd1306_vcomdetect_state_t;
static OLED_StatusTypeDef OLED_Set_VComDetect(OLED_HandleTypeDef* OLED, ssd1306_vcomdetect_state_t value);

typedef enum{
	SSD1306_COMSCANINC = OLED_COMSCANINC,
	SSD1306_COMSCANDEC = OLED_COMSCANDEC
} ssd1306_comscan_state_t;
static OLED_StatusTypeDef OLED_Set_ComScanDir(OLED_HandleTypeDef* OLED, ssd1306_comscan_state_t value);

typedef enum{
	SSD1306_SEGREMAP_0_SEG0 = OLED_SEGREMAP|OLED_SEGREMAP_0_SEG0,
	SSD1306_SEGREMAP_127_SEG0 = OLED_SEGREMAP|OLED_SEGREMAP_127_SEG0
} ssd1306_segremap_state_t;
static OLED_StatusTypeDef OLED_Set_SegRemap(OLED_HandleTypeDef* OLED, ssd1306_segremap_state_t value);

typedef enum{
	SSD1306_MEMORYMODE_PGE = OLED_MEMORYMODE|OLED_MEMORYMODE_PGE,
	SSD1306_MEMORYMODE_HORISONTAL = OLED_MEMORYMODE|OLED_MEMORYMODE_HOR,
	SSD1306_MEMORYMODE_VERTICAL = OLED_MEMORYMODE|OLED_MEMORYMODE_VER
} ssd1306_memorymode_state_t;
static OLED_StatusTypeDef OLED_Set_MemoryMode(OLED_HandleTypeDef* OLED, ssd1306_memorymode_state_t value);

typedef enum{
	SSD1306_STARTLINE_DEFAULT = OLED_SETSTARTLINE | OLED_STARTLINE_DEFAULT,
} ssd1306_startline_state_t;
static OLED_StatusTypeDef OLED_Set_StartLine(OLED_HandleTypeDef* OLED, ssd1306_startline_state_t value);

typedef enum{
	SSD1306_DISPLAYOFFSET_LOWCOLUMN = OLED_SETLOWCOLUMN,
	SSD1306_DISPLAYOFFSET_HIGHCOLUMN = OLED_SETHIGHCOLUMN,
} ssd1306_displayoffset_state_t;
static OLED_StatusTypeDef OLED_Set_DisplayOffset(OLED_HandleTypeDef* OLED, ssd1306_displayoffset_state_t value);

typedef enum{
	SSD1306_COMPINS_DEFAULT = OLED_SETCOMPINS_COM_CONF_ALT | OLED_SETCOMPINS_COM_REMAP_NONE
} ssd1306_compins_state_t;
static OLED_StatusTypeDef OLED_Set_ComPins(OLED_HandleTypeDef* OLED, ssd1306_compins_state_t value);

typedef enum{
	SSD1306_MULTIPLEXRATIO_DEFAULT = 63
} ssd1306_multiplex_state_t;
static OLED_StatusTypeDef OLED_Set_MultiplexRatio(OLED_HandleTypeDef* OLED, ssd1306_multiplex_state_t value);

static OLED_StatusTypeDef OLED_Set_PreCharge(OLED_HandleTypeDef* OLED);
static OLED_StatusTypeDef OLED_Set_Display_Use_GDDRAM(OLED_HandleTypeDef* OLED);
static OLED_StatusTypeDef OLED_SetContrast(OLED_HandleTypeDef *OLED, uint8_t value);
static OLED_StatusTypeDef OLED_Set_Cursor(OLED_HandleTypeDef* OLED, uint8_t Byte, uint8_t Page);

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

	OLED->DataSend = OLED_SendData;
	OLED->AddressI2C = OLED_ADRESS;
	OLED->Heigth = OLED_096_PAGES;
	OLED->Width = OLED_096_SEGS;
	OLED->Cursor = NULL;

	Result = OLED_SetDisplayOnOff(OLED, SSD1306_DISPLAY_OFF);
	Result = OLED_Set_ChargePumpOnOff(OLED, SSD1306_CHARGEPUMP_OFF);
	Result = OLED_Set_Display_ClockDiv(OLED, SSD1306_CLOCKFREQ_MAX);
	Result = OLED_Set_MultiplexRatio(OLED, SSD1306_MULTIPLEXRATIO_DEFAULT);
	Result = OLED_Set_DisplayOffset(OLED, SSD1306_DISPLAYOFFSET_LOWCOLUMN);
	Result = OLED_Set_StartLine(OLED, SSD1306_STARTLINE_DEFAULT);
	Result = OLED_Set_MemoryMode(OLED, SSD1306_MEMORYMODE_HORISONTAL);
	Result = OLED_Set_SegRemap(OLED, SSD1306_SEGREMAP_127_SEG0);
	Result = OLED_Set_ComScanDir(OLED, SSD1306_COMSCANDEC);
	Result = OLED_Set_ComPins(OLED, SSD1306_COMPINS_DEFAULT);
	Result = OLED_SetContrast(OLED, OLED_SETCONTRAST_MAX);
	Result = OLED_Set_PreCharge(OLED);
	Result = OLED_Set_VComDetect(OLED, SSD1306_SETVCOMDETECT_MID);
	Result = OLED_Set_Display_Use_GDDRAM(OLED);
	Result = OLED_Set_Display_ColorMode(OLED, SSD1306_COLORS_NON_INV);
	Result = OLED_Set_ChargePumpOnOff(OLED, SSD1306_CHARGEPUMP_ON);
	Result = OLED_SetDisplayOnOff(OLED, SSD1306_DISPLAY_ON);

	// Memory allocation for frame buffer
	Result = OLED_FrameMem_Init (OLED);

	OLED->FrameSize = OLED_096_PAGES * OLED_096_SEGS;

	Result = OLED_GDDR_Clear(OLED);
	if(Result != OLED_OK){
		  OLED_ErrorHandler(OLED);
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
	uint8_t squares_count = 0;
	uint8_t tempBuf = 0;
	for (size_t i = 0; i < OLED->FrameSize; i++)
	{

		if(t == 8)
		{
			tempBuf = ~tempBuf;
			t = 0;
			squares_count++;
		}
		if(squares_count == 16)
		{
			squares_count = 0;
			tempBuf = ~tempBuf;
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
 *
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
 * @brief Function sets OLED clock division
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_display_clock_freq_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_Display_ClockDiv(OLED_HandleTypeDef* OLED, ssd1306_display_clock_freq_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETDISPLAYCLOCKDIV;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);
	return Result;
}

/**
 * @brief Function sets OLED color mode
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_display_clock_freq_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_Display_ColorMode(OLED_HandleTypeDef* OLED, ssd1306_display_color_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function forces OLED to use data in GDDRAM to display image
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_Set_Display_Use_GDDRAM(OLED_HandleTypeDef* OLED)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_DISPLAYALLON;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	tempBuf = OLED_DISPLAYALLON_RESUME;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	return Result;
}

/**
 * @brief Function sets OLED charge pump state
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_ssd1306_chargepump_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_ChargePumpOnOff(OLED_HandleTypeDef* OLED, ssd1306_chargepump_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_CHARGEPUMP;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED tv com detect state
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_sevcomdetect_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_VComDetect(OLED_HandleTypeDef* OLED, ssd1306_vcomdetect_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETVCOMDETECT;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED precharge state
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_Set_PreCharge(OLED_HandleTypeDef* OLED)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETPRECHARGE;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SETPRECHARGE_PHASE_1 | OLED_SETPRECHARGE_PHASE_2;
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	return Result;
}

/**
 * @brief Function sets OLED com scan dir
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_setcomscan_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_ComScanDir(OLED_HandleTypeDef* OLED, ssd1306_comscan_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED segment remap
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_setsegremap_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_SegRemap(OLED_HandleTypeDef* OLED, ssd1306_segremap_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED memory mode
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_memorymode_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_MemoryMode(OLED_HandleTypeDef* OLED, ssd1306_memorymode_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_MEMORYMODE;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED offset
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_displayoffset_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_DisplayOffset(OLED_HandleTypeDef* OLED, ssd1306_displayoffset_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETDISPLAYOFFSET;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED start line offset
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 * @[IN]data value - ssd1306_startline_state_t CMD
 *
 */
OLED_StatusTypeDef OLED_Set_StartLine(OLED_HandleTypeDef* OLED, ssd1306_startline_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED sets multiplex ratio
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_Set_MultiplexRatio(OLED_HandleTypeDef* OLED, ssd1306_multiplex_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETMULTIPLEX;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

	return Result;
}

/**
 * @brief Function sets OLED com pins
 *
 * @returns  Nothing
 *
 * @[IN]data &OLED - Display object
 *
 */
OLED_StatusTypeDef OLED_Set_ComPins(OLED_HandleTypeDef* OLED, ssd1306_compins_state_t value)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SETCOMPINS;

	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	Result = OLED->DataSend(COMMAND, OLED->AddressI2C, (uint8_t *) &value, 1);

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
OLED_StatusTypeDef OLED_Set_Cursor(OLED_HandleTypeDef* OLED, uint8_t Byte, uint8_t Page)
{
	OLED_StatusTypeDef Result = OLED_OK;
	uint8_t tempBuf = OLED_SET_ADDR_PAGE_MODE + Page;

	OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = Byte & OLED_SET_CLMN_PAGE_MODE_L;
	OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);
	tempBuf = OLED_SET_CLMN_PAGE_MODE_H | (Byte >> 4);
	OLED->DataSend(COMMAND, OLED->AddressI2C, &tempBuf, 1);

	return Result;
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
