/*
 * oled_text.c
 *
 *  Created on: 27 нояб. 2021 г.
 *      Author: Nulliik
 */

#include "fonts.c"
#include "oled_text.h"

Font_TypeDef default_font;

static void Char_To_Buffer(uint8_t c, OLED_HandleTypeDef *oled);
static void Char_To_Buffer_At_Pos(uint8_t c, OLED_HandleTypeDef *oled, int x_p, int y_p);

void TextInit()
{
	default_font.font = (uint8_t*)Font_6x6_RuEng;
	default_font.font_height = 6;
	default_font.font_width = 6;
	default_font.offset_func = CharTo_Font_6x6_RuEng_Offset;
}

void String_To_Buffer(char *string, OLED_HandleTypeDef *oled)
{
	while(*string != '\0')
	{
		Char_To_Buffer(*string, oled);
		string++;
	}
}

static void Char_To_Buffer(uint8_t c, OLED_HandleTypeDef *oled)
{
	uint8_t x=0;
	uint8_t offset = default_font.offset_func(c);
	if(!offset) return; //If desired char is not presented in current font exit the function
	for (x=0; x < default_font.font_width - 1; x++)
	{
		*(oled->FrameMem + oled->Cursor->Segment + 128*oled->Cursor->Page + x) = default_font.font[c*5-5*offset+x];
	}
	oled->Cursor->Segment = oled->Cursor->Segment + default_font.font_width;
	if(oled->Cursor->Segment > (OLED_WIDTH+1))
	{
		oled->Cursor->Segment = 0;
		oled->Cursor->Page = oled->Cursor->Page+1;
		if(oled->Cursor->Page > OLED_HEIGHT-1) oled->Cursor->Page = 0;
	}
}

void String_To_Buffer_At_Pos(char *string, OLED_HandleTypeDef *oled, int x, int y)
{
	while(*string != '\0')
	{
		Char_To_Buffer_At_Pos(*string, oled,x,y);
		string++;
		x = x + default_font.font_width;
		if(x > (OLED_WIDTH+1))
		{
			x = 0;
			y = y+1;
			if(y > OLED_HEIGHT-1) y = 0;
		}
	}
}

static void Char_To_Buffer_At_Pos(uint8_t c, OLED_HandleTypeDef *oled, int x_p, int y_p)
{
	uint8_t x=0;
	uint8_t offset = default_font.offset_func(c);
	if(!offset) return; //If desired char is not presented in current font exit the function

	if(y_p % 8 != 0) //If user wants to put char between lines
	{
		int y_page_start = y_p/8;

		for (x=0; x < default_font.font_width - 1; x++)
		{
			//upper half
			*(oled->FrameMem + x_p + 128*y_page_start + x) = default_font.font[c*5-5*offset+x]<<(1+y_p-(y_page_start*8));
			//lower half
			*(oled->FrameMem + x_p + 128*(y_page_start+1) + x) = default_font.font[c*5-5*offset+x]>>(7-(y_p-(y_page_start*8)));
		}
	}
	else
	{
		for (x=0; x < default_font.font_width - 1; x++)
		{
			*(oled->FrameMem + x_p + 128*y_p + x) = default_font.font[c*5-5*offset+x];
		}
	}
}
