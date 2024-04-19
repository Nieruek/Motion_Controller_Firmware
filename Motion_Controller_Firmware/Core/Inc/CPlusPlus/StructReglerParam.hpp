/*
 * StructReglerParam.hpp
 *
 *  Created on: Dec 31, 2023
 *      Author: nirut
 */

#ifndef INC_STRUCTREGLERPARAM_HPP_
#define INC_STRUCTREGLERPARAM_HPP_

enum regulator_type {PID_REG, PI_REG, INVALID};	//for functions to choose

struct paramPID
{
	double Tv;
	double Tn;
	double Kp;
	double PID_lim;
	double pos;						// aktuelle Istposition?
};
struct paramPI
{
	double Tn;
	double Kp;
	double PI_lim;
};

struct paramTrajGen
{
	double vel_max;
	double acc;						// positive Beschleunigung
	double dec;						// negative Beschleunigung
	double servo_time;
	double d_s_max;					// max. Geschwindigkeit
	double dd_s_max_acc;			// max. Beschleunigung positiv
	double dd_s_max_dec;			// mx. Beschleunigung negativ
	double s_br;					// Bremsweg
	double d_s;						// Geschwindigkeit
	double dd_s;					// Beschleunigung
	double s;						// Strecke
	bool motion_on;

	double setpoint;				// Zielpunkt
	double s_delta;					// Strecke bis zum Zielpunkt
};


#endif /* INC_STRUCTREGLERPARAM_HPP_ */
