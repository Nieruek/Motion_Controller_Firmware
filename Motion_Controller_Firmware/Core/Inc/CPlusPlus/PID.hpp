/*
 * PID.hpp
 *
 *  Created on: Dec 24, 2023
 *      Author: nirut
 */

#ifndef INC_PID_HPP_
#define INC_PID_HPP_

#include "StructReglerParam.hpp"
#ifdef __cplusplus
  class PID
  {
  public:

	//PID(paramPID param);
	PID();
	void Regeln(double s);
  public:	//SETTER
	void setPos(double pos);
	void setparam(paramPID pPID);
	void setTV(double tv);
	void setTN(double tn);
	void setKP(double kp);
	void setTA(double ta);
	void setOffset(double offset);
  public:	//GETTER
	void getparam(double &pTv,double &pTn,double &pKp);
	void getPIDValue(double &pPID_Y);
	void getPos(double &pos);
	void getTV(double &tv);
	void getTN(double &tn);
	void getKP(double &kp);
	void getTA(double &ta);

  private:
	double mTv;
	double mTn;
	double mKp;
	//double mTa;		// 1/20kHz
//	double vel_max;
//	double acc;						// positive Beschleunigung
//	double dec;						// negative Beschleunigung
//	double servo_time;
//	double d_s_max;					// max. Geschwindigkeit
//	double dd_s_max_acc;			// max. Beschleunigung positiv
//	double dd_s_max_dec;			// mx. Beschleunigung negativ
//	double s_br;					// Bremsweg
//	double d_s;						// Geschwindigkeit
//	double dd_s;					// Beschleunigung
//	double s;						// Strecke
//	bool motion_on;
//	double pos;
//	double setpoint;				// Zielpunkt
//	double s_delta;					// Strecke bis zum Zielpunkt
	//double PID_P;
	//double PID_Tn;
	//double PID_Tv;
	double PID_lim;
	//int PWM_A_val;
	//int PWM_B_val;
	double PID_error;				// Regelfehler
	double PID_Y;					// aktueller Y-Wert, Reglerausgang
	double PID_I_MEM;				// Speicher für I-Term
	double PID_error_1;				// letzter Y-Wert, letzter Wert für Regelabweichung
	double m_pos;					// Istposition
	double mP_fkt;
	double mI_fkt;
	double mD_fkt;
	double m_offset;

  };
#endif

#endif /* INC_PID_HPP_ */
