/*
 * OutputMatrixPWM.hpp
 *
 *  Created on: Feb 26, 2024
 *      Author: nirut
 */

#ifndef INC_CPLUSPLUS_OUTPUTMATRIXPWM_HPP_
#define INC_CPLUSPLUS_OUTPUTMATRIXPWM_HPP_

#include "map"

class DCMotor;

#ifdef __cplusplus
  class OutputMatrixPWM
  {
  public:
	struct axisMatrix {
		double sensor_matrix_1[2] = {1, 0};
		double sensor_matrix_2[2] = {0, 1};
		double pwm_axis_value;
		double adc_axis_value;

	};
	OutputMatrixPWM();
	//doMatrix(double pPI_Y);	//TODO
	//void doOutputMatrix(double pPI_Y, bool &pRU, bool &pRO,);	// TODO
	void setMatrix(axisMatrix &matrix, DCMotor* motor);
	bool getInputMatrix(DCMotor* motor);	//return sensor values of motor
	void setMatrix(DCMotor* motor);	//default matrix
	void calcPWMAxisValue();
	double getADCAxisValue(DCMotor* motor_ptr);
	double getPWMAxisValue(DCMotor* motor_ptr);
	void calcADCAxisValue();

  private:
	std::map<DCMotor* ,axisMatrix> m_matrixes;
  };
#endif




#endif /* INC_CPLUSPLUS_OUTPUTMATRIXPWM_HPP_ */
