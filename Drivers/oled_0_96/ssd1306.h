/*
 * ssd1306.h
 *
 *  Created on: 5 нояб. 2021 г.
 *      Author: Maximus
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#define OLED_SETCONTRAST 0x81
#define OLED_SETCONTRAST_MAX 0xFF
#define OLED_SETCONTRAST_MIN 0x00

#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5

//TODO: Изменить на COLORS (используем американское произношение вместо британского?)
#define OLED_COLORS_INV		0xA6
#define OLED_COLORS_INV_TRUE	0x01
#define OLED_COLORS_INV_FALSE	0x00

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


#endif /* SSD1306_H_ */