/*
 * OutputMatrixPWM.cpp
 *
 *  Created on: Feb 26, 2024
 *      Author: nirut
 */

#include "CPlusPlus/OutputMatrixPWM.hpp"
#include "CPlusPlus/DCMotor.hpp"

OutputMatrixPWM::OutputMatrixPWM() :
	m_matrixes()
{}

void OutputMatrixPWM::setMatrix(DCMotor* motor)
{
	axisMatrix to_insert;
	setMatrix(to_insert, motor);
}

void OutputMatrixPWM::setMatrix(axisMatrix &matrix, DCMotor* motor)
{
	for (auto compare_matrix : m_matrixes) {
		if (compare_matrix.first == motor ) {
			compare_matrix.second.sensor_matrix_1[0] = matrix.sensor_matrix_1[0];
			compare_matrix.second.sensor_matrix_1[1] = matrix.sensor_matrix_1[1];
			compare_matrix.second.sensor_matrix_2[0] = matrix.sensor_matrix_2[0];
			compare_matrix.second.sensor_matrix_2[1] = matrix.sensor_matrix_2[1];
			return;
		}
	}
	m_matrixes.emplace(motor, matrix);
}

void OutputMatrixPWM::calcPWMAxisValue()
{
	int i = 0;
	double pi_val [2] = {0.0};
	for (auto &output_matrix : m_matrixes) {
		output_matrix.first->getPIVal(pi_val[i]);
		i++;
	}

	i = 0;
	for (auto &output_matrix : m_matrixes) {
		output_matrix.second.pwm_axis_value = pi_val[0] * output_matrix.second.sensor_matrix_1[i] +
				pi_val[1] * output_matrix.second.sensor_matrix_2[i];
		i++;
	}
}

void OutputMatrixPWM::calcADCAxisValue()
{
	int i = 0;
	double adc_value [2] = {0.0};
	for (auto &output_matrix : m_matrixes) {
		output_matrix.first->getADCValue(adc_value[i]);
		i ++;
	}

	i = 0;
	for (auto &output_matrix : m_matrixes) {
		output_matrix.second.adc_axis_value = adc_value[0] * output_matrix.second.sensor_matrix_1[i] +
				adc_value[1] * output_matrix.second.sensor_matrix_2[i];
		i++;
	}
}

double OutputMatrixPWM::getPWMAxisValue(DCMotor* motor_ptr)
{
	for (auto &matrix_it : m_matrixes) {
		if (matrix_it.first == motor_ptr) {
			return matrix_it.second.pwm_axis_value;
		}
	}
	return 0.0;	//not in list, sth went wrong
}

double OutputMatrixPWM::getADCAxisValue(DCMotor* motor_ptr)
{
	for (auto &matrix_it : m_matrixes) {
		if (matrix_it.first == motor_ptr) {
			return matrix_it.second.adc_axis_value;
		}
	}
	return 0.0;	//not in list, sth went wrong
}



