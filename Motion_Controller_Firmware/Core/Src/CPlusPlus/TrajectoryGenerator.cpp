/*
 * TrajectoryGenerator.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: nirut
 */

#include "CPlusPlus/TrajectoryGenerator.hpp"
#include "math.h"


TrajGen::TrajGen()
{

}
TrajGen::TrajGen(paramTrajGen pParam)
{
	myParamTrajGen.setpoint = 0.0;
	myParamTrajGen.s = 0.0;
	myParamTrajGen.dd_s = 0.0;
	myParamTrajGen.d_s = 0.0;
	myParamTrajGen.s_br = 0.0;
	myParamTrajGen.s_delta = 0.0;
	myParamTrajGen.motion_on = false;
	myParamTrajGen.vel_max = pParam.vel_max;
	myParamTrajGen.acc = pParam.acc;
	myParamTrajGen.dec = pParam.dec;
	myParamTrajGen.servo_time = pParam.servo_time;
	myParamTrajGen.d_s_max = myParamTrajGen.vel_max * myParamTrajGen.servo_time;
	myParamTrajGen.dd_s_max_acc = myParamTrajGen.acc * myParamTrajGen.servo_time * myParamTrajGen.servo_time;
	myParamTrajGen.dd_s_max_dec = myParamTrajGen.dec * myParamTrajGen.servo_time * myParamTrajGen.servo_time;
}
void TrajGen::Trapez()
{
	// Umrechnung auf die Zykluszeit (1ms)
	/*double delta_t = 0.0001;
	double acc_max = axis->acc_max * delta_t * delta_t;
	double dec_max = axis->dec_max * delta_t * delta_t;
	double vel_max = axis->vel_max * delta_t;*/

				//  Bewegungsrichtung - Wegstrecke bis zum Ziel:
	myParamTrajGen.s_delta = myParamTrajGen.setpoint - myParamTrajGen.s;

	if(myParamTrajGen.s_delta == 0.0)
	{
		myParamTrajGen.motion_on = false;
	}
	else
	{
		myParamTrajGen.motion_on = true;
		//  aktueller Bremsweg
		myParamTrajGen.s_br = round(myParamTrajGen.d_s /myParamTrajGen.dd_s_max_dec)  * 0.5 * (myParamTrajGen.d_s);

		// Geschwindigkeitsberechnung
		if(myParamTrajGen.s_delta >= 0.0 )						// positive Bewegungsrichtung
		{
			if(fabs(myParamTrajGen.s_delta) > fabs(myParamTrajGen.s_br))		// Beschleunigung
			{
				myParamTrajGen.dd_s = myParamTrajGen.dd_s_max_acc;
			}
			else                            	// Verzögerung
			{
				myParamTrajGen.dd_s = -1.0 * myParamTrajGen.dd_s_max_dec;
			}
		}
		else                                	// negative Bewegungsrichtung
		{
			if(fabs(myParamTrajGen.s_delta) > fabs(myParamTrajGen.s_br))     	// Beschleunigung
			{
				myParamTrajGen.dd_s = -1.0 * myParamTrajGen.dd_s_max_acc;
			}
			else                            	// Verzögerung
			{
				myParamTrajGen.dd_s = myParamTrajGen.dd_s_max_dec;
			}
		}
		myParamTrajGen.d_s = myParamTrajGen.d_s + myParamTrajGen.dd_s;

		// Begrenzung der Geschwindigkeit
		if (myParamTrajGen.d_s > myParamTrajGen.d_s_max) 							// konstante Fahrt ohen weitere Beschleunigung (positiv)
		{
			myParamTrajGen.d_s = myParamTrajGen.d_s_max;
			myParamTrajGen.dd_s = 0.0;
		}

		if(myParamTrajGen.d_s < -1.0 * myParamTrajGen.d_s_max ) 						// konstante Fahrt ohen weitere Beschleunigung (negativ)
		{
			myParamTrajGen.d_s = -1.0 * myParamTrajGen.d_s_max;
			myParamTrajGen.dd_s = 0.0;
		}

		// letzte Iteration vor erreichen des Ziels...
		if(fabs(myParamTrajGen.s_delta) <= fabs(myParamTrajGen.d_s))
		{
			myParamTrajGen.s = myParamTrajGen.setpoint;
			myParamTrajGen.dd_s = 0.0;
		}
		else
		{
			// Streckenberechnung
			myParamTrajGen.s = myParamTrajGen.s + myParamTrajGen.d_s;
		}


		// Rückgabewerte umgerechnet auf Sekunden

		//axis->v = axis->v / delta_t;
		//axis->a = axis->a / (delta_t * delta_t);
	}
}

void TrajGen::S_Curve()
{}

void TrajGen::CANOpen()
{}

	//SETTER
void TrajGen::setparam(paramTrajGen pParam)
{
	myParamTrajGen.setpoint = 0.0;
	myParamTrajGen.s = 0.0;
	myParamTrajGen.dd_s = 0.0;
	myParamTrajGen.d_s = 0.0;
	myParamTrajGen.s_br = 0.0;
	myParamTrajGen.s_delta = 0.0;
	myParamTrajGen.motion_on = false;
	myParamTrajGen.vel_max = pParam.vel_max;
	myParamTrajGen.acc = pParam.acc;
	myParamTrajGen.dec = pParam.dec;
	myParamTrajGen.servo_time = pParam.servo_time;
	myParamTrajGen.d_s_max = myParamTrajGen.vel_max * myParamTrajGen.servo_time;
	myParamTrajGen.dd_s_max_acc = myParamTrajGen.acc * myParamTrajGen.servo_time * myParamTrajGen.servo_time;
	myParamTrajGen.dd_s_max_dec = myParamTrajGen.dec * myParamTrajGen.servo_time * myParamTrajGen.servo_time;
}
void TrajGen::setAcc(double acc)
{
	myParamTrajGen.acc = acc;
	myParamTrajGen.dd_s_max_acc = myParamTrajGen.acc * myParamTrajGen.servo_time * myParamTrajGen.servo_time;
}
void TrajGen::setDec(double dec)
{
	myParamTrajGen.dec = dec;
	myParamTrajGen.dd_s_max_dec = myParamTrajGen.dec * myParamTrajGen.servo_time * myParamTrajGen.servo_time;
}
void TrajGen::setVelmax(double vel_max)
{
	myParamTrajGen.vel_max = vel_max;
	myParamTrajGen.d_s_max = myParamTrajGen.vel_max * myParamTrajGen.servo_time;
}
void TrajGen::setSetpoint(double setpoint)
{
	myParamTrajGen.setpoint = setpoint;
}
	//GETTER
void TrajGen::getStrecke(double &ps)
{
	ps = myParamTrajGen.s;
}
void TrajGen::getparam(paramTrajGen &pParam)
{
	pParam = myParamTrajGen;
}
void TrajGen::getAcc(double &acc)
{
	acc = myParamTrajGen.acc;
}
void TrajGen::getDec(double &dec)
{
	dec = myParamTrajGen.dec;
}
void TrajGen::getVelmax(double &vel_max)
{
	vel_max = myParamTrajGen.vel_max;
}
void TrajGen::getSetpoint(double &setpoint)
{
	setpoint = myParamTrajGen.setpoint;
}


