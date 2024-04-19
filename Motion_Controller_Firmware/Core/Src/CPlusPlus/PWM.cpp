/*
 * PWM.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: nirut
 */

#include "CPlusPlus/PWM.hpp"
#include "main.h"

#define PWM_MAX 17000
#define PWM_MAX1 16000
#define PWM_MAX2 15000
#define PWM_MAX3 8500
#define PWM_MAX4 7500

PWM::PWM()
{

}

void PWM::setPWMValue(double pPI_Y)
{
	double temp = pPI_Y * PWM_MAX4;
	if(pPI_Y>=0)
	{

		PWM_A_val = PWM_MAX3 - (int)temp;
		PWM_B_val = PWM_MAX3 + (int)temp;		// möglicherweise hier Fehler mit Minus und uint32_t
														// if PWM-Value Grenzen überschreitet: testwert
	}
	else
	{
		PWM_A_val = PWM_MAX3 - (int)temp;
		PWM_B_val = PWM_MAX3 + (int)temp;
	}

	TIM1->CCR1 = PWM_A_val;
	TIM1->CCR3 = PWM_A_val;
	TIM1->CCR2 = PWM_B_val;
}

void PWM::getPWMValue(int &pPWM_A_val,int &pPWM_B_val)
{
	pPWM_A_val=PWM_A_val;
	pPWM_B_val=PWM_B_val;
}
