/*
 * fonts.h
 *
 *  Created on: 27 нояб. 2021 г.
 *      Author: Nulliik
 */
#include <stdint.h>

#ifndef OLED_0_96_FONTS_H_
#define OLED_0_96_FONTS_H_

typedef int (*Char_Font_Offset)(uint8_t chr);

typedef struct {
	uint8_t *font;
	uint8_t font_width;
	uint8_t font_height;
	Char_Font_Offset offset_func;
}Font_TypeDef;

#endif /* OLED_0_96_FONTS_H_ */
