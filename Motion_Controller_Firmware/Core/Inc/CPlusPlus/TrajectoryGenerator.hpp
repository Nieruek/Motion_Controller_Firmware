/*
 * TrajectoryGenerator.hpp
 *
 *  Created on: Feb 23, 2024
 *      Author: nirut
 */

#ifndef INC_CPLUSPLUS_TRAJECTORYGENERATOR_HPP_
#define INC_CPLUSPLUS_TRAJECTORYGENERATOR_HPP_

#include "StructReglerParam.hpp"
#ifdef __cplusplus
  class TrajGen
  {
  public:
	TrajGen();
	TrajGen(paramTrajGen pParam);
	void Trapez();
	void S_Curve();
	void CANOpen();
  public:	//SETTER
	void setparam(paramTrajGen pParam);
	void setAcc(double acc);
	void setDec(double dec);
	void setVelmax(double vel_max);
	void setSetpoint(double setpoint);
  public:	//GETTER
	void getStrecke(double &ps);
	void getparam(paramTrajGen &pParam);
	void getAcc(double &acc);
	void getDec(double &dec);
	void getVelmax(double &vel_max);
	void getSetpoint(double &setpoint);
  private:
	paramTrajGen myParamTrajGen;
  };
#endif



#endif /* INC_CPLUSPLUS_TRAJECTORYGENERATOR_HPP_ */
