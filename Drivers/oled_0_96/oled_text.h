/*
 * oled_text.h
 *
 *  Created on: 27 нояб. 2021 г.
 *      Author: Nulliik
 */

#ifndef OLED_0_96_OLED_TEXT_H_
#define OLED_0_96_OLED_TEXT_H_

#include "oled096.h"

void TextInit();
void String_To_Buffer(char *string, OLED_HandleTypeDef *oled);
void String_To_Buffer_At_Pos(char *string, OLED_HandleTypeDef *oled, int x, int y);

#endif /* OLED_0_96_OLED_TEXT_H_ */
