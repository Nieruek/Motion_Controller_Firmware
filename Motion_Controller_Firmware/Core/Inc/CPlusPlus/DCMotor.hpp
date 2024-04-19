/*
 * DCMotor.hpp
 *
 *  Created on: Dec 24, 2023
 *      Author: nirut
 */

#ifndef INC_DCMOTOR_HPP_
#define INC_DCMOTOR_HPP_

#include "IMotorBase.hpp"
#include "PID.hpp"
#include "PI.hpp"
#include "StructReglerParam.hpp"
#include "ADC.hpp"
#include "main.h"
#include "TrajectoryGenerator.hpp"
#include "PWM.hpp"

#ifdef __cplusplus
  class DCMotor: public IMotorBase
  {
  public:
	DCMotor();
	DCMotor(paramPID pPID,paramPI pPI);
	void PositionRegulation();
	void CurrentRegulation();
	void TrajectoryGenerator();
	void OutputMatrix();
	bool checkIfValid(double toCheck);
  public:	//SETTER
	void setParam(paramPI pPI,paramPID pPID,paramTrajGen pTrajGen);
	void setPWMValue();
	void setPos(double pos);	//PID
	void setCurrentPos(double pos);
	void setVel(double vel);
	void setAcc(double acc);
	void setDec(double dec);
	void setTN(double tn, regulator_type reg);
	void setKP(double kp, regulator_type reg);
	void setTV(double tv, regulator_type reg);
	void setADC(ADC* adc);
	void setPMW(PWM* pwm);
	void setOffset(double offset);
  public:	//GETTER
	//void getPIDVal(double &pPID_Y);
	void getPIVal(double &pPI_Y);
	void getPWMValue(int &pValueA, int &pValueB);
	void getPos(double &pos);	//PID
	void getVel(double &vel);
	void getAcc(double &acc);
	void getDec(double &dec);
	void getTN(double &tn, regulator_type reg);
	void getKP(double &kp, regulator_type reg);
	void getTV(double &tv, regulator_type reg);
	void getADCValue(double &adc_val);
  public: //DEBUG
	uint32_t debug_val;
	uint32_t debug_valA;
	uint32_t debug_valB;
	uint32_t debug;


  private:
//	paramPID myparamPID;
//	paramPI myparamPI;
	PID myPID;
	PI myPI;
	ADC* myADC;
	TrajGen myTrajectoryGenerator;
	PWM* myPWM;
	//int	PWM_val_A;			// PWM value A
	//int	PWM_val_B;			// PWM value A
	//double PID_Y;

	//SPI mySPI;

  };
#endif




#endif /* INC_DCMOTOR_HPP_ */
