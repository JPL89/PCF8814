/*
	@File 		main.cpp
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		07/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

#define F_CPU 16000000UL

#include <avr/io.h>

#include "nokia1100.h"

PCF8814 pcf8814;

int main(void)
{
	pcf8814.Serial_Init();
	
	pcf8814.Clear();

	pcf8814.String("Nokia",0,0,Font_Arial_16x16);

    while(1)
    {
        
    }
}
