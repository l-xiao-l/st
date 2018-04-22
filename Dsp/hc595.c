#include "stm32f0xx_hal.h"


uint16_t adc_target = 0;
uint16_t adc_cont = 0;

uint16_t num[8][10] = {{0x01c0, 0x01f9, 0x01a4, 0x01b0, 0x0199, 0x0192, 0x0182, 0x01f8, 0x0180, 0x0190},
					   {0x02c0, 0x02f9, 0x02a4, 0x02b0, 0x0299, 0x0292, 0x0282, 0x02f8, 0x0280, 0x0290},
					   {0x04c0, 0x04f9, 0x04a4, 0x04b0, 0x0499, 0x0492, 0x0482, 0x04f8, 0x0480, 0x0490},
					   {0x08c0, 0x08f9, 0x08a4, 0x08b0, 0x0899, 0x0892, 0x0882, 0x08f8, 0x0880, 0x0890},
					   {0x10c0, 0x10f9, 0x10a4, 0x10b0, 0x1099, 0x1092, 0x1082, 0x10f8, 0x1080, 0x1090},
					   {0x20c0, 0x20f9, 0x20a4, 0x20b0, 0x2099, 0x2092, 0x2082, 0x20f8, 0x2080, 0x2090},
					   {0x40c0, 0x40f9, 0x40a4, 0x40b0, 0x4099, 0x4092, 0x4082, 0x40f8, 0x4080, 0x4090},
					   {0x80c0, 0x80f9, 0x80a4, 0x80b0, 0x8099, 0x8092, 0x8082, 0x80f8, 0x8080, 0x8090}};


void hc595_control(uint16_t data)
{
	uint8_t i;
	
	GPIOB->BSRR = 1 << 5;
	GPIOB->BRR = 1 << 3;
	GPIOA->BRR = 1 << 15;
	
	for (i = 0; i < 16; i++)
	{
		GPIOB->BRR = 1 << 3;
		if (data & 0x8000)
		{
			GPIOB->BSRR = 1 << 5;
		}
		else
		{
			GPIOB->BRR = 1 << 5;
		}
		
		GPIOB->BSRR = 1 << 3;
		data <<= 1;
	}
	GPIOA->BSRR = 1 << 15;
}

void branches_bt(void)
{
	uint16_t store = 0, nu = 0, store_t = 0, nu_t = 0;
	uint8_t i;
	store = adc_cont;
	store_t = adc_target;
	
	for (i = 0; i < 4; i++)
	{
		nu = store % 10;
		
		if (i == 1)
		{
			hc595_control(num[i][nu] ^ 0x80);
		}
		else
		{
			hc595_control(num[i][nu]);
		}
			
				
		
	/*	switch (i)
		{
			case 1:
				hc595_control(num[i][nu] ^ 0x80);
				break;
			default:
				hc595_control(0x00ff);
				break;
		}
	*/

		store /= 10;	
	}

	for (i = 0; i < 4; i++)
	{
		nu_t = store_t % 10;
	
		hc595_control(num[i + 4][nu_t]);
		switch (i)
		{
			case 1:
				hc595_control(num[i + 4][nu_t] ^ 0x80);
				break;
			default:
				hc595_control(0x00ff);
				break;
		}

		/*if (i == 1)
		{
			hc595_control(num[i + 4][nu_t] ^ 0x80);
		}
		else
		{
			hc595_control(num[i + 4][nu_t]);
		}
		*/	
		
		store_t /= 10; 
	}


}




















