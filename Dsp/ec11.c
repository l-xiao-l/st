#include "stm32f0xx_hal.h"

#define S0 0x00
#define S1 0x01
#define S2 0x02
#define S3 0x03
#define S4 0x04
#define S5 0x05


extern uint16_t add;
extern uint16_t add_t;
//extern uint8_t t_pid;
extern uint8_t flag_ec11;
static uint8_t t = 1;


void ec11_irq_handler(void)
{
	if (EXTI->PR & (0x000000001 << 9))
	{
		if (GPIOA->IDR&GPIO_PIN_10)
		{

			if (t == 0)
			{
				add_t += 10;
			}
			else
			{
			    add++;
			}
		}
		else
		{

			if (t == 0)
			{
				add_t -= 10;
			}
			else
			{
				add--;
			}
		}

	}
	EXTI->PR |= (0x00000001 << 9);

}


void EXTI4_15_IRQHandler(void)
{
	ec11_irq_handler();	
}


void key_scanning(void)
{
	static uint8_t k_data = S0;
	static uint8_t press = 0;
	static uint8_t press_l = 0;
	
	switch (k_data)
	{
		case S0:
			if (GPIOA->IDR & GPIO_PIN_8)
			{
				k_data = S0;
			}
			else
			{
				k_data = S1;
			}
			break;
		case S1:
			if (GPIOA->IDR & GPIO_PIN_8)
			{
				k_data = S0;
			}
			else 
			{
				k_data = S2;
			}
			break;
		case S2:
			if (GPIOA->IDR & GPIO_PIN_8)
			{
				k_data = S3;
			}
			else
			{
				press++;
				t = 1;
				//t_pid = 1;
				if (20 < press)
				{
					k_data = S4;
				}
				GPIOA->BSRR = 1 << 11;
			}
			break;
		case S3:
			if (GPIOA->IDR & GPIO_PIN_8)
			{
				k_data = S0;
			}
			else
			{
				k_data = S1;
			}
			break;
		case S4:
			if (GPIOA->IDR & GPIO_PIN_8)
			{
				k_data = S3;
			}
			else
			{
				press = 0;
				t = 0;
				//t_pid = 0;
				GPIOA->BRR = 1 << 11;
				GPIOA->BSRR = 1 << 12;

				press_l++;
				if (20 < press_l)
				{
					k_data = S5;
				}

			}
			break;

		case S5:
			if (GPIOA->IDR & GPIO_PIN_8)
			{
				k_data = S3;
			}
			else
			{
				press_l = 0;
				flag_ec11 = 1;
				GPIOA->BRR = 1 << 12;
			}
		default:
			k_data = S0;
			press = 0;
			press_l = 0;
			break;
	}
}




