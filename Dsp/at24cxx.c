#include "stm32f0xx_hal.h"
#include "iic.h"

extern uint8_t flag;
extern uint8_t I2c_Write;
uint8_t I2c_Read = 0;
uint8_t flag_ec11 = 0; 
void at24c02_adc_write(void)
{
	if (flag_ec11 == 1)
	{
		EEPROM_WriteBytes(&I2c_Write, 0, 256);
		flag_ec11 = 0;
	}
	else
	{
	}
}

void at24c02_adc_read(void)
{
	if (flag == 1)
	{
		EEPROM_ReadBytes(&I2c_Read, 0, 256);
		printf("2c%d\r\n", I2c_Read);
	}
	else
	{
		printf("error\r\n");
	}
}
