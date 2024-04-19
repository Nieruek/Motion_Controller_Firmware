/*
 * ledWrapper.cpp
 *
 *  Created on: Sep 30, 2023
 *      Author: larsi
 */
#include <CPlusPlus/ledWrapper.hpp>
#include <stdio.h>

#define steptime 5
	ledWrapper::ledWrapper(GPIO_TypeDef *gpio_port,  uint16_t gpio_pin) :
		m_gpio_port(gpio_port),
		m_gpio_pin(gpio_pin),
		m_time_ms()
	{}
	ledWrapper::ledWrapper(GPIO_TypeDef *gpio_port, uint16_t gpio_pin, int time_ms) :
		m_gpio_port(gpio_port),
		m_gpio_pin(gpio_pin),
		m_time_ms(time_ms)
	{}
	void ledWrapper::led_toggle(int time_ms) {
		HAL_GPIO_TogglePin(m_gpio_port, m_gpio_pin);
		if (time_ms>0) {
			HAL_Delay(time_ms);
			return;
		}
		HAL_Delay(m_time_ms);
	}
	void ledWrapper::led_on(){
		HAL_GPIO_WritePin(m_gpio_port, m_gpio_pin, GPIO_PIN_SET);
	}
	void ledWrapper::led_off() {
		HAL_GPIO_WritePin(m_gpio_port, m_gpio_pin, GPIO_PIN_RESET);
	}
	void ledWrapper::dimmer_on(int time_ms) {
		int steps = time_ms/steptime;
		for (int n=1; n<steps; n++) {
			int on_time = (steptime*n)/steps;
			int off_time = steptime-on_time;
			printf("On_Time: %d, Off_Time: %d\n", on_time, off_time);
			led_on();
			HAL_Delay(on_time);
			led_off();
			HAL_Delay(off_time);
		}
		led_on();
	}
	void ledWrapper::dimmer_off(int time_ms) {
		int steps = time_ms/steptime;
		for (int n=1; n<steps; n++) {
			int off_time = (steptime*n)/steps;
			int on_time = steptime-off_time;
			led_on();
			HAL_Delay(on_time);
			led_off();
			HAL_Delay(off_time);
		}
		led_off();
	}
	void ledWrapper::dimmer(int time_ms) {
		dimmer_on(time_ms);
		dimmer_off(time_ms);
	}

