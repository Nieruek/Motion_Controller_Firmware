/*
 * PI.hpp
 *
 *  Created on: Dec 24, 2023
 *      Author: nirut
 */

#ifndef INC_PI_HPP_
#define INC_PI_HPP_
#include "StructReglerParam.hpp"

#ifdef __cplusplus
  class PI
  {
  public:
//  struct paramPI
//	{
//		double Tn;
//		double Kp;
//	};
	PI();
	//PI(paramPI param);
	void Regeln(double pPID_Y, double ADC_Val);
		//SETTER
	void setparam(paramPI pPI);
	void setTN(double tn);
	void setKP(double kp);
	void setTA(double ta);
		//GETTER
	void getPIValue(double &pPI_Y);
	void getparam(double &pTn,double &pKp);
	void getTN(double &tn);
	void getKP(double &kp);
	void getTA(double &ta);

  private:
	double mTn;
	double mKp;
	//double mTa;		// 1/100kHz
	double PI_lim;
	double PI_error;				// Regelfehler
	double PI_Y;					// aktueller Y-Wert, Reglerausgang
	double PI_I_MEM;				// Speicher für I-Term
	double PI_error_1;				// letzter Y-Wert, letzter Wert für Regelabweichung
	double mP_fkt;
	double mI_fkt;
  };
#endif



#endif /* INC_PI_HPP_ */
