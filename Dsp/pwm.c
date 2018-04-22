#include "stm32f0xx_hal.h"

extern uint16_t pwm_con;
uint16_t add = 0;
uint16_t add_t = 0;

uint16_t pwm_convert(uint16_t number)
{
	uint16_t a = 0;
	a = (number * 4800) / 1000 - 1;

	return a;
}

void pwm_entry(void)
{
	static uint16_t cont = 0;
	cont = 600 + add + add_t;
	if (1000 < cont)
	{
		cont = 1000;
		add = 0;
		add_t = 0;
	}
	else if (100 > cont)
	{
		cont = 100;
		add = 0;
		add_t = 0;
	}
	else
	{
	}

	pwm_con = cont;
	TIM3->CCR4 = pwm_convert(pwm_con);
}


