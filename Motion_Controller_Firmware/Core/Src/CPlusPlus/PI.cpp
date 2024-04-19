/*
 * PI.cpp
 *
 *  Created on: Dec 24, 2023
 *      Author: nirut
 */

#include "CPlusPlus/PI.hpp"

#define BOOST_FKT_PI (0.000001)

PI::PI():mTn(0),mKp(0),PI_lim(0),PI_error(0),PI_Y(0),PI_I_MEM(0), PI_error_1(0),mP_fkt(0)
{

}
//PI::PI(paramPI param):mTn(param.Tn),mKp(param.Kp),mTa(0.00000001),mE_sum(0)
//{
//
//}

void PI::Regeln(double pPID_Y, double ADC_Val)
{
	double P;
	double I;

	//Regelfehler
	PI_error = pPID_Y - (ADC_Val/1.53);

	PI_I_MEM = PI_I_MEM + PI_error;

	// P-Glied
	//P = PI_error * PI_P * 0.000001;
	P = PI_error * mP_fkt;
	// I-Glied
	//if(PID_Tn > 0.0001)
	if(mTn > 0.0001)
	{
		//I = 1/PID_Tn * PID_I_MEM;
		I = mI_fkt * PI_I_MEM;
	}
	else
	{
		I = 0.0;
	}

	// Ausgang
	PI_Y = P + I;

	//Limiter
	if(PI_Y > PI_lim)
	{
		PI_Y = PI_lim;
	}
	else if(PI_Y <  -1 * PI_lim)
	{
		PI_Y = -1 * PI_lim;
	}


	//Limiter
	if(PI_Y > PI_lim * 0.3)
	{
		PI_I_MEM -= PI_error; 	// anti Windup
		PI_Y = PI_Y - I;
	}
	else if(PI_Y <  -0.3 * PI_lim)
	{
		PI_I_MEM -= PI_error; 	// anti Windup
		PI_Y = PI_Y - I;
	}

}

	//SETTER
void PI::setparam(paramPI pPI)
{
	mTn=pPI.Tn;
	mI_fkt = 1/mTn;
	mKp=pPI.Kp;
	mP_fkt = mKp * BOOST_FKT_PI;
	PI_lim=pPI.PI_lim;
}
void PI::setTN(double tn)
{
	mTn = tn;
	mI_fkt = 1/mTn;
}
void PI::setKP(double kp)
{
	mKp = kp;
	mP_fkt = mKp * BOOST_FKT_PI;
}
/*void PI::setTA(double ta)
{
	mTa = ta;
}*/

	//GETTER
void PI::getparam(double &pTn,double &pKp)
{
	pTn=mTn;
	pKp=mKp;
}

void PI::getPIValue(double &pPI_Y)
{
	pPI_Y = PI_Y;
}
void PI::getTN(double &tn)
{
	tn = mTn;
}
void PI::getKP(double &kp)
{
	kp = mKp;
}
/*void PI::getTA(double &ta)
{
	ta = mTa;
}*/
