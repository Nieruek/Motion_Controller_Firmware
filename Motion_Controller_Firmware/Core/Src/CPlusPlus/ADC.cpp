/*
 * ADC.cpp
 *
 *  Created on: Jan 12, 2024
 *      Author: nirut
 */

#include "CPlusPlus/ADC.hpp"
#include <cstdint>
#include "stm32g4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
uint16_t adcVal[2] = {0};
double convVal1 = 0;
double convVal2 = 0;
const double conv = 3.3/4095;
const double ADC_OFFSET = 1.5;	//offset of driver board shunt, a verage = 1.53V
bool convADCActive = 0;
ADC::ADC()
{

}

void ADC::getADC_Val(double &pADC_Value, double pPI_Y)
{
	pADC_Value = convVal1;
	if(pPI_Y < 0)
	{
		pADC_Value = -convVal2;
	}

	if(convADCActive == 0)
	{
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcVal, 2);
		convADCActive = 1;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	convVal1 = ADC_OFFSET - (double(adcVal[0]) * conv);
	convVal2 = (double(adcVal[1]) * conv) - ADC_OFFSET;	//gets above offset with current
	convADCActive = 0;
}
