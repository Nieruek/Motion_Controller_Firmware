/*
 * PID.cpp
 *
 *  Created on: Dec 24, 2023
 *      Author: nirut
 */

#include "CPlusPlus/PID.hpp"

#define BOOST_FKT_PID (0.000001)

PID::PID():mTv(0), mTn(0), mKp(0), PID_lim(0), PID_error(0), PID_Y(0), PID_I_MEM(0), PID_error_1(0), m_pos(0),
			mP_fkt(0), mI_fkt(0), mD_fkt(0), m_offset(0)
{


		//debug_val =0;
		//debug = 0;
		//debug_valA = 5000;
		//debug_valB = 5000;
}
//PID::PID(paramPID param):mTv(param.Tv),mTn(param.Tn),mKp(param.Kp),mTa(0.0000005),mE_sum(0),mE_alt(0)
//{
//
//}

void PID::Regeln(double s)
{
	double P;
	double I;
	double D;

	//Regelfehler
	PID_error = s - m_pos;	//when moving and set 0 after -> keeps spinning because m_pos == 0 and s == 0 -> PID_error == 0 -> PID_I_MEM not changing -> keeps moving

	PID_I_MEM = PID_I_MEM + PID_error;

	// P-Glied
	//P = PID_error * PID_P * 0.000001;
	P = PID_error * mP_fkt;
	// I-Glied
	//if(PID_Tn > 0.0001)
	if(mTn > 0.0001)
	{
		//I = 1/PID_Tn * PID_I_MEM;
		I = mI_fkt * PID_I_MEM;
	}
	else
	{
		I = 0.0;
	}

	//D-Glied
	//D = (PID_error - PID_error_1) * PID_Tv * 0.000001;
	D = (PID_error - PID_error_1) * mD_fkt;
	// speichere den Wert für nächste iteration...
	PID_error_1 =  PID_error;

	// Ausgang
	PID_Y = P + I + D;

	//Limiter
	if(PID_Y > PID_lim)
	{
		PID_Y = PID_lim;
	}
	else if(PID_Y <  -1 * PID_lim)
	{
		PID_Y = -1 * PID_lim;
	}


	//Limiter
	if(PID_Y > PID_lim * 0.3)
	{
		PID_I_MEM -= PID_error; 	// anti Windup
		PID_Y = PID_Y - I;
	}
	else if(PID_Y <  -0.3 * PID_lim)
	{
		PID_I_MEM -= PID_error; 	// anti Windup
		PID_Y = PID_Y - I;
	}

	//return PID_Y;
}

	//SETTER
void PID::setparam(paramPID pPID)
{
	mKp=pPID.Kp;
	mP_fkt = mKp * BOOST_FKT_PID;
	mTn=pPID.Tn;
	mI_fkt = 1/mTn;
	mTv=pPID.Tv;
	mD_fkt = mTv * BOOST_FKT_PID;
	//setpoint = 0.0;
	//s = 0.0;
	//dd_s = 0.0;
	//d_s = 0.0;
	//pos = 0.0;
	//vel_max=pPID.vel_max;
	//acc=pPID.acc;
	//dec=pPID.dec;
	//servo_time=pPID.servo_time;
	PID_lim=pPID.PID_lim;
	//PID_P = 500000;
	//PID_Tn = 1000.0;
	//PID_Tv = 30000.0;
	PID_error = 0.0;
	PID_Y = 0.0;			// aktueller Y-Wert
	PID_I_MEM = 0.0;		// Speicher für I-Term
	PID_error_1 = 0.0;		// letzter Y-Wert

//	dd_s_max_acc = acc * servo_time * servo_time;
//	dd_s_max_dec = dec * servo_time * servo_time;
//	d_s_max = vel_max * servo_time;
}

void PID::setPos(double pos)
{
	m_pos = pos + m_offset;
}
void PID::setTV(double tv)
{
	mTv = tv;
	mD_fkt = mTv * BOOST_FKT_PID;
}
void PID::setTN(double tn)
{
	mTn = tn;
	mI_fkt = 1/mTn;
}
void PID::setKP(double kp)
{
	mKp = kp;
	mP_fkt = mKp * BOOST_FKT_PID;
}
void PID::setOffset(double offset)
{
	m_offset = offset;
}

/*void PID::setTA(double ta)
{
	mTa = ta;
}*/

	//GETTER
void PID::getparam(double &pTv,double &pTn,double &pKp)
{
	pTv=mTv;
	pTn=mTn;
	pKp=mKp;
}
void PID::getPIDValue(double & pPID_Y)
{
	pPID_Y=PID_Y;
}
void PID::getPos(double &pos)
{
	pos = m_pos;
}
void PID::getTV(double &tv)
{
	tv = mTv;
}
void PID::getTN(double &tn)
{
	tn = mTn;
}
void PID::getKP(double &kp)
{
	kp = mKp;
}
/*void PID::getTA(double &ta)
{
	ta = mTa;
}*/
