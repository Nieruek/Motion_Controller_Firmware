/*
 * PWM.hpp
 *
 *  Created on: Feb 26, 2024
 *      Author: nirut
 */

#ifndef INC_CPLUSPLUS_PWM_HPP_
#define INC_CPLUSPLUS_PWM_HPP_

#ifdef __cplusplus
  class PWM
  {
  public:

	PWM();
	//PI(paramPI param);
	void setPWMValue(double pPI_Y);
	void getPWMValue(int & pPWM_A_val, int & pPWM_B_val);


  private:

	int PWM_A_val;
	int PWM_B_val;
  };
#endif



#endif /* INC_CPLUSPLUS_PWM_HPP_ */
