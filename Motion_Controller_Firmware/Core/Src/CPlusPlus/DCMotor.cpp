/*
 * DCMotor.cpp
 *
 *  Created on: Dec 24, 2023
 *      Author: nirut
 */

#include "CPlusPlus/DCMotor.hpp"
#include "CPlusPlus/SerialConnection.hpp"
#include "CPlusPlus/OutputMatrixPWM.hpp"
#include "math.h"

extern SerialConnectionDMA serial_connection;
extern OutputMatrixPWM output_matrix;

DCMotor::DCMotor()
{}
DCMotor::DCMotor(paramPID pPID,paramPI pPI)//:myparamPID{pPID.Tv,pPID.Tn,pPID.Kp},myparamPI{pPI.Tn,pPI.Kp}
{
	//myparamPID.Kp=pPID.Kp;
//	myparamPID.Tv=pPID.Tv;
//	myparamPID.Tn=pPID.Tn;
//	myparamPI.Kp=pPI.Kp;
//	myparamPI.Tn=pPI.Tn;
}
void DCMotor::TrajectoryGenerator()
{
	myTrajectoryGenerator.Trapez();
}

void DCMotor::PositionRegulation()
{
	double s=0;
	myTrajectoryGenerator.getStrecke(s);
	myPID.Regeln(s);
}

void DCMotor::CurrentRegulation()
{
	double PID_Val=0;
	double ADC_Val=0;
	double PI_Val = 0;
	myPI.getPIValue(PI_Val);
	myPID.getPIDValue(PID_Val);
	ADC_Val = round(output_matrix.getADCAxisValue(this)*100) / 100;
	myPI.Regeln(PID_Val, ADC_Val);
}

void DCMotor::OutputMatrix()
{

}

bool DCMotor::checkIfValid(double toCheck)
{
	if (toCheck < 0) {
		serial_connection.transmitInvalidValueMsg();
		return false;
	}
	return true;
}
	//SETTER
void DCMotor::setParam(paramPI pPI,paramPID pPID, paramTrajGen pTrajGen)
{
	myPID.setparam(pPID);
	myPI.setparam(pPI);
	myTrajectoryGenerator.setparam(pTrajGen);
}
void DCMotor::setPWMValue()
{
//	myPI.getPIValue(PI_Y);
	static int counter = 0;
	double PI_Y = output_matrix.getPWMAxisValue(this);

	counter++;
//	if (counter > 10000) {	//DEBUG PRINT
//		double PID_Val = 0;
//		myPID.getPIDValue(PID_Val);
//		serial_connection.transmitVal(PID_Val, PI_Y);
//		counter = 0;
//	}

	if (myPWM) {
		myPWM->setPWMValue(PI_Y);
	}
}
void DCMotor::setPos(double pos)
{
//	if (checkIfValid(pos)) 	NO check, might want negativ value for testing/direction change
	myTrajectoryGenerator.setSetpoint(pos);
}
void DCMotor::setCurrentPos(double pos) 	//set position motor is measured in
{
	myPID.setPos(pos);
}
void DCMotor::setVel(double vel)
{
	if (checkIfValid(vel)) myTrajectoryGenerator.setVelmax(vel);
}
void DCMotor::setAcc(double acc)
{
	if (checkIfValid(acc)) myTrajectoryGenerator.setAcc(acc);
}
void DCMotor::setDec(double dec)
{
	if (checkIfValid(dec)) myTrajectoryGenerator.setDec(dec);
}

void DCMotor::setTN(double tn, regulator_type reg)
{
	switch (reg) {
		case PID_REG:
			if (checkIfValid(tn)) myPID.setTN(tn);
		case PI_REG:
		default:
			if (checkIfValid(tn)) myPI.setTN(tn);
	}
}
void DCMotor::setKP(double kp, regulator_type reg)
{
	switch (reg) {
		case PID_REG:
			if (checkIfValid(kp)) myPID.setKP(kp);
		case PI_REG:
		default:
			if (checkIfValid(kp)) myPI.setKP(kp);
	}
}
void DCMotor::setTV(double tv, regulator_type reg)
{
	switch (reg) {
		case PID_REG:
			if (checkIfValid(tv)) myPID.setTV(tv);
		case PI_REG:
		default:
			serial_connection.transmitInvalidValueMsg();
	}
}
void DCMotor::setOffset(double offset)
{
	myPID.setOffset(offset);
}
	//GETTER
void DCMotor::getPWMValue(int &pValueA, int &pValueB)
{
	myPWM->getPWMValue(pValueA, pValueB);
}
void DCMotor::getPIVal(double &pPI_Y)
{
	myPI.getPIValue(pPI_Y);
}
void DCMotor::getPos(double &pos)
{
	myTrajectoryGenerator.getSetpoint(pos);//myPID.getPos(pos);
}
void DCMotor::getVel(double &vel)
{
	myTrajectoryGenerator.getVelmax(vel);
}
void DCMotor::getAcc(double &acc)
{
	myTrajectoryGenerator.getAcc(acc);
}
void DCMotor::getDec(double &dec)
{
	myTrajectoryGenerator.getDec(dec);
}
void DCMotor::getTN(double &tn, regulator_type reg)
{
	switch (reg) {
		case PID_REG:
			myPID.getTN(tn);
		case PI_REG:
		default:
			myPI.setTN(tn);
	}
}
void DCMotor::getKP(double &kp, regulator_type reg)
{
	switch (reg) {
		case PID_REG:
			myPID.getKP(kp);
		case PI_REG:
		default:
			myPI.getKP(kp);
	}
}
void DCMotor::getTV(double &tv, regulator_type reg)
{
	switch (reg) {
		case PID_REG:
			myPID.getTV(tv);
		case PI_REG:
		default:
			serial_connection.transmitInvalidValueMsg();
	}
}
void DCMotor::setPMW(PWM* pwm)
{
	myPWM = pwm;
}
void DCMotor::setADC(ADC* adc)
{
	myADC = adc;
}
void DCMotor::getADCValue(double &adc_val)
{
	double pi_value;
	myPI.getPIValue(pi_value);
	myADC->getADC_Val(adc_val, pi_value);
}
