#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx_hal.h"
#include "iic.h"

extern uint8_t flag;
extern uint16_t I2c_last;
extern uint16_t I2c_Write;
uint8_t flag_ec11 = 0; 
uint8_t I2c_Read[5];
uint8_t last = 1;

void at24c02_adc_write(void)
{
	char str[5];

	if (flag_ec11 == 1)
	{
		itoa(I2c_Write, str, 10);
		AT24CXX_Write(0, (uint8_t*)str, 5);
		flag_ec11 = 0;
	}
	else
	{
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
}



