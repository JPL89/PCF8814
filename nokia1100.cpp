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
#include "nokia1100.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

void PCF8814::Serial_Write(unsigned char data)
{
	for (int i=7; i>=0; --i)
	{
		PCF8814_SCL_LOW;
		
		if (data & (1 << i))
			PCF8814_MOSI_HIGH;
		else
			PCF8814_MOSI_LOW;
		
		PCF8814_SCL_HIGH;
	}
}

void PCF8814::Serial_Char(unsigned char data)
{
	PCF8814_CS_HIGH;				// CS: No Active
	
	PCF8814_MOSI_HIGH;				// SDI: 1 -> Data write
	
	PCF8814_SCL_LOW;				// At the falling edge of SCE: SCLK must be LOW
	
	PCF8814_CS_LOW;					// Chip Select: Active low

	PCF8814_SCL_HIGH;				// SDIN: is sampled at the rising edge of SCLK
	
	PCF8814::Serial_Write(data);	// Send command
	
	PCF8814_CS_HIGH;				// CS: No Active
}

void PCF8814::Serial_Command(unsigned char cmd)
{
	PCF8814_CS_HIGH;				// CS: No Active
	
	PCF8814_MOSI_LOW;				// SDI: 0 -> Command write
	
	PCF8814_SCL_LOW;				// At the falling edge of SCE: SCLK must be LOW
	
	PCF8814_CS_LOW;					// Chip Select: Active low

	PCF8814_SCL_HIGH;				// SDIN: is sampled at the rising edge of SCLK
	
	PCF8814::Serial_Write(cmd);		// Send command
	
	PCF8814_CS_HIGH;				// CS: No Active
}

void PCF8814::Serial_Init()
{
	DDRB |= (1 << PCF8814_CS);
	DDRB |= (1 << PCF8814_DC);
	DDRB |= (1 << PCF8814_MOSI);
	DDRB |= (1 << PCF8814_SCL);
	
	PCF8814_CS_HIGH;

	PCF8814::Serial_Command( 0x28|0x07 );
	PCF8814::Serial_Command( 0xAA | 0 );
	PCF8814::Serial_Command( 0xEA | 1 ); 
	
	PCF8814::Serial_Command( 0x7F ); 
	PCF8814::Serial_Command( 0 );   
	
	PCF8814::Serial_Command( 0X3A ); 
	
	PCF8814::Serial_Command( 0x3D );
	PCF8814::Serial_Command( 0 );
	
	PCF8814::Serial_Command( 0x30 );
	PCF8814::Serial_Command( 0x20|0x04 ); 
	PCF8814::Serial_Command( 0x80|0x10 );
	PCF8814::Serial_Command( 0xA0 | 1 );
	
	// Rotate display
	PCF8814::Serial_Command( 0xC0 | 0x08 ); // Y
	PCF8814::Serial_Command( 0xa0 | 0x01 ); // X
	
	// Display On
	PCF8814::Serial_Command( 0xAF ); 
}

void PCF8814::Char(unsigned char data, const unsigned char Font[])
{
	unsigned char wd				= pgm_read_byte( &Font[0] );	// font width
	unsigned char pg				= pgm_read_byte( &Font[2] );	// font pages
	unsigned char st				= pgm_read_byte( &Font[3] );	// font start
	unsigned char ch				= 0;							// character
	unsigned char fh				= 5;							// font header size
	unsigned char cl				= 0;							// col
	unsigned int i					= 0;							// width * width ...
	unsigned int rw					= 0;							// rows
	
	int id = ( ( ( data - st) * ( (wd+1) * 2) + fh ) );
	
	unsigned char character_width = pgm_read_byte( &Font[ id ] );
	
	Set_Cursor(X, Y);
	
	for (cl=0; cl<pg; cl++)
	{
		Set_Cursor(X, Y + cl);
		
		for (; rw < (wd + i); rw++)
		{
			ch = pgm_read_byte( &Font[ (id + rw) + 2 ] );
			
			PCF8814::Serial_Char( ch );
		}
		
		i += wd;
	}
	
	X += character_width + 1;
}

void PCF8814::String(const char * data, unsigned char x, unsigned char y, const unsigned char Font[])
{
	PCF8814::X = x;
	PCF8814::Y = y;
	
	while(*data)
	{
		PCF8814::Char(*data++, Font);

	}
}

void PCF8814::Clear()
{
	PCF8814::Set_Cursor(0,0);
	
	for(int c=0; c<96; c++)
	{
		for(int r=0; r<65; r++)
		{
			PCF8814::Serial_Char( 0 );
		}
	}
	
}

// Ex: Draw a Pic from 0 to 128 7x
void PCF8814::DrawImage(
const unsigned char Buffer[],
unsigned char x,
unsigned char y,
unsigned char Width,
unsigned char Pages
)
{
	unsigned int j = 0;
	
	unsigned int i = 0;
	
	for (unsigned char k=0; k<Pages; k++)
	{
		PCF8814::Serial_Command(0xB0 | (y + k));
		
		PCF8814::Serial_Command(0x10 | (x >> 4));
		PCF8814::Serial_Command(0x0F & x);
		
		for (; i<(Width + j); i++)
		{
			PCF8814::Serial_Char( pgm_read_byte( &Buffer[ i ] ) );
		}
		
		j += Width;
	}
}

void PCF8814::Set_Cursor_8x(unsigned char x, unsigned char y)
{
	x <<= 3;
	PCF8814::Serial_Command(0xB0 | y);
	PCF8814::Serial_Command(0x10 | (x >> 4));
	PCF8814::Serial_Command(0x0F & x);
}

void PCF8814::Set_Cursor(unsigned char x, unsigned char y)
{
	PCF8814::Serial_Command(0xB0 | y);
	PCF8814::Serial_Command(0x10 | (x >> 4));
	PCF8814::Serial_Command(0x0F & x);
}
