#include "stm32f0xx_hal.h"

#define AMPLITUDE_MAX 4700
#define AMPLITUDE_MIM -4700


extern uint16_t adc_cont;
extern uint16_t adc_target;
uint16_t I2c_last = 0;
uint16_t I2c_Write = 0;
uint16_t add = 0;
uint16_t add_t = 0;
//uint16_t t_pid = 1;

uint16_t ad[101] = {
					987,1026,1066,1107,1148,1190,1233,1276,1320,1365,
					1410,1455,1501,1547,1593,1640,1687,1734,1781,1828,
					1875,1923,1970,2017,2063,2110,2156,2202,2248,2293,
					2338,2382,2426,2469,2512,2554,2596,2637,2677,2716,
					2755,2794,2831,2868,2904,2939,2974,3008,3041,3074,
					3105,3136,3166,3196,3225,3253,3280,3307,3333,3358,
					3383,3406,3430,3452,3474,3496,3517,3537,3556,3576,
					3594,3612,3630,3647,3663,3679,3694,3709,3724,3738,
					3752,3765,3778,3791,3803,3815,3826,3837,3848,3858,
					3868,3878,3888,3897,3906,3914,3923,3931,3939,3947,
					3954
					};

float LinearValue(uint16_t arr[], uint16_t Value, uint8_t mid)
{
	float temper;
	
	temper = (float)(Value - arr[mid]) / (arr[mid + 1] - arr[mid]) + mid;
	
	return temper;
}

float Temperature(uint16_t arr[], uint16_t Value, uint8_t len)
{
	uint16_t max, mid, min;

	min = 0;
	max = len - 1;
	mid = (max + min) / 2;
	if (Value == mid)
	{
		return mid;
	}
	
	while (Value != arr[mid])
	{
		mid = (max + min) / 2;
		if (Value == arr[mid])
		{
			return mid;
		}
		else if (Value > arr[mid] && Value < arr[mid + 1])
		{
			return LinearValue(ad, Value, mid);
		}
		else if (Value > arr[mid])
		{
			min = mid + 1;
		}
		else if (Value < arr[mid])
		{
			max = mid - 1;
		}
		else
		{
		}

		if (max < min)
		{
			return 254;
		}
	}

	return 255;

}

/*
float adc_filtering(float dr)
{
	float t = 0, t1 = 0;
	t = dr * 0.7 + t1 * 0.3;
	t1 = dr;

	
	return t;
}

void adc_k(void)
{
	static uint8_t c = 0;
	at[c] = ADC1->DR;
	c++;
	if (c == 10)
	{
		c = 0;
	}
}

void insert_sort(void)
{
	uint8_t i, j, t;
	uint16_t temp = 0;
	uint32_t sum = 0;

	for (i = 1; i < 10; i++)
	{
		temp = at[i];
	
		for (j = i; j > 0 && temp < at[j - 1]; j--)
		{
			if (temp < at[j - 1])
			{
				at[j] = at[j - 1];
			}
		}
	
		at[j] = temp;
	}
	
	for (t = 1; t < 9; t++)
	{
		sum += at[t];
	}
	
	value = sum / 8;

}
*/



void adc_entry(void)
{
	static int16_t m1 = 0, cn = 0;
	static int16_t a = 0;
	static int16_t a_stat = 0;
	int16_t m = 0; 

	m = ADC1->DR * 0.8 + m1 * 0.2;
	m1 = ADC1->DR;
	adc_cont = Temperature(ad, m, 101) * 10;

	a = adc_target - adc_cont;
	cn++;

	switch (a_stat)
	{
		case 0:
			if (2 < cn)
			{
				a_stat = 1;
				cn = 0;
			}
			break;
		case 1:
			if (a < -10)
			{
				GPIOA->BSRR = 1 << 0;   //beep
			}
			break;
		default:
			break;
	}

}


void adc_pid(void)
{
	static int16_t iError = 0, sumError = 0, dError = 0, LastError = 0;
	static uint16_t  SetPoint = 0;
	//int16_t kp_c = 130, ki_c = 650;
	//static int16_t k = 0;
	static uint16_t kp = 0;
	static float ki = 0, kd = 0.136;
	static int16_t t1 = 0;
	int16_t t = 0; 
	int16_t	adc_value = 0, NextPoint = 0; 
	
	SetPoint = I2c_last + add + add_t; 

	if (1000 < SetPoint)
	{
		I2c_last = 1000;
		add = 0;
		add_t = 0;
	}
	if (100 > SetPoint)
	{
		I2c_last = 100;
		add = 0;
		add_t = 0;
	}
	adc_target = SetPoint;
	I2c_Write = SetPoint;
	
/*
	if (t_pid == 1)
	{
		kp = kp_c + add;
		adc_target = kp;
	}
	if (t_pid == 0)
	{
		k = ki_c + add_t;
		adc_target = k;
	}
	ki = k / 1000;
*/
	t = ADC1->DR * 0.8 + t1 * 0.2;
	t1 = ADC1->DR;
	NextPoint = Temperature(ad, t, 101) * 10;

	if (300 >= NextPoint && 150 < NextPoint)
	{
		kp = 27.5, ki = 0.130;
	}
	if (400 >= NextPoint && 300 < NextPoint)
	{
		kp = 25.0, ki = 0.132;
	}
	else if (500 >= NextPoint && 400 < NextPoint)
	{
		kp = 24.0, ki = 0.145;
	}
	else if (600 >= NextPoint && 500 < NextPoint)
	{
		kp = 20, ki = 0.145;
	}
	else if (700 >= NextPoint && 600 < NextPoint)
	{
		kp = 16.5, ki = 0.145;
	}
	else if (800 >= NextPoint && 700 < NextPoint)
	{
		kp = 13.5, ki = 0.155;
	}
	else if (900 >= NextPoint && 800 < NextPoint)
	{
		kp = 9.5, ki = 0.158;
	}
	else if (950 >= NextPoint && 900 < NextPoint)
	{
		kp = 8.5, ki = 0.170;
	}
	else if (1000 >= NextPoint && 950 < NextPoint)
	{
		kp = 8, ki = 0.175;
	}
	else
	{
	//	kp = 0, ki = 0;
	//	printf("re\r\n");
	}

	iError = SetPoint - NextPoint;
	sumError += iError;

	if (AMPLITUDE_MAX < sumError)
	{
		sumError = AMPLITUDE_MAX;
	}

	if (AMPLITUDE_MIM > sumError)
	{
		sumError = AMPLITUDE_MIM;
	}

	dError = iError - LastError;
	LastError = iError;
	
	adc_value = kp * iError + ki * sumError + kd * dError;
	if (4700 < adc_value)
	{
		adc_value = 4700;
	}
	
	if (0 >= adc_value)
	{
		adc_value = 0;
	} 

	TIM3->CCR4 = adc_value;
}













