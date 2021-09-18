/*
	@File 		
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		07/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/
#ifndef NOKIA1100_H_
#define NOKIA1100_H_

//	DISPLAY DRIVER PCF8814

// FONTS
#include "Fonts/Arial_10x10.h"
#include "Fonts/Arial_12x12.h"
#include "Fonts/Arial_13x13.h"
#include "Fonts/Arial_16x16.h"

#include "Fonts/Arial_Bold_10x11.h"
#include "Fonts/Arial_Bold_16x17.h"
#include "Fonts/Times_New_Roman_Bold_12x11.h"

#include "Fonts/Calibri_9x10.h"
#include "Fonts/Tahoma_10x11.h"
#include "Fonts/Times_New_Roman_11x12.h"

#include "Fonts/Ms_Serif.h"
#include "Fonts/Verdana_11x11.h"
#include "Fonts/Arial_Italic_11x12.h"

#define PCF8814_CS			PB0
#define PCF8814_DC			PB1
#define PCF8814_MOSI		PB5
#define PCF8814_SCL			PB7

#define PCF8814_CS_LOW		PORTB &=(unsigned char)~(1 << PCF8814_CS)
#define PCF8814_CS_HIGH		PORTB |=(1 << PCF8814_CS)

#define PCF8814_DC_LOW		PORTB &=(unsigned char)~(1 << PCF8814_DC)
#define PCF8814_DC_HIGH		PORTB |=(1 << PCF8814_DC)

#define PCF8814_MOSI_LOW	PORTB &=(unsigned char)~(1 << PCF8814_MOSI)
#define PCF8814_MOSI_HIGH	PORTB |=(1 << PCF8814_MOSI)

#define PCF8814_SCL_LOW		PORTB &=(unsigned char)~(1 << PCF8814_SCL)
#define PCF8814_SCL_HIGH	PORTB |= (1 << PCF8814_SCL)

class PCF8814
{
	public:
		void String(const char * data, unsigned char x, unsigned char y, const unsigned char Font[]);
		void Char(unsigned char data, const unsigned char Font[]);
		void Set_Cursor_8x(unsigned char x, unsigned char y);
		void Set_Cursor(unsigned char x, unsigned char y);
		
		void Clear();
		
		void Serial_Init();
		void Serial_Char(unsigned char data);
		void Serial_Command(unsigned char cmd);
		void Serial_Write(unsigned char data);
		
		void Rotate();
		void DrawImage(const unsigned char Buffer[],unsigned char x,unsigned char y,unsigned char Width,unsigned char Pages);
		
		unsigned char X;
		unsigned char Y;	
};

#endif /* NOKIA1100_H_ */