/*
 * ledWrapper.hpp
 *
 *  Created on: Sep 30, 2023
 *      Author: larsi
 */

#ifndef INC_LEDWRAPPER_HPP_
#define INC_LEDWRAPPER_HPP_

#include "stm32g4xx_hal.h"

class ledWrapper{

public:
	ledWrapper();
	ledWrapper(GPIO_TypeDef *gpio_port, uint16_t gpio_pin);
	ledWrapper(GPIO_TypeDef *gpio_port,  uint16_t gpio_pin, int time_ms);
	void led_toggle(int time_ms=0);
	void led_on();
	void led_off();
	void dimmer_on(int time_ms);
	void dimmer_off(int time_ms);
	void dimmer(int time_ms);
private:
	GPIO_TypeDef *m_gpio_port;
	uint16_t m_gpio_pin;
	int m_time_ms;
};




#endif /* INC_LEDWRAPPER_HPP_ */
