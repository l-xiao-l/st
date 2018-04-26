#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx_hal.h"
#include "iic.h"

extern uint8_t flag;           //ec11.c
extern uint16_t I2c_last;      //table.c
extern uint16_t I2c_Write;     //table.c
uint8_t flag_exti0 = 0;        //ec11.c
uint8_t flag_ec11 = 0; 	       //ec11.c
uint8_t I2c_Read[5];
uint8_t flag_t = 0;

void at24c02_adc_write(void)
{
	char str[5];
	static uint16_t at_cn = 0;
	
	at_cn++;
	if (flag_ec11 == 1 || at_cn == 100 || (flag_exti0 == 1 && flag_t == 2))  //at_cn * 300
	{
		itoa(I2c_Write, str, 10);
		AT24CXX_Write(0, (uint8_t*)str, 5);
		flag_ec11 = 0;
		flag_exti0 = 0;
		flag_t = 0;
		at_cn = 0;
	}
}

void at24c02_adc_read(void)
{
	static uint8_t last = 1;

	if (flag == 1)
	{
		if (last)
		{
			AT24CXX_Read(0, I2c_Read, 5);
			I2c_last = atoi((char*)I2c_Read);
			last = 0;
		}
	}
	else
	{
	}

	GPIOA->BRR = 1 << 0;   //beep
	flag_t = 2;           //at24cxx.c   write
}



