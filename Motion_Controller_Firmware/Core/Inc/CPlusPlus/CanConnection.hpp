/*
 * CanConnection.hpp
 *
 *  Created on: Dec 18, 2023
 *      Author: larsi
 */

#ifndef INC_CPLUSPLUS_CANCONNECTION_HPP_
#define INC_CPLUSPLUS_CANCONNECTION_HPP_



#include "myIncludes.hpp"
#include <fdcan.h>
#include <stm32g4xx_hal.h>

class CanConnection {
	public:
	enum CAN_USED {CANFD1, CANFD2};
	CanConnection(FDCAN_HandleTypeDef &can_handle, TIM_HandleTypeDef &timer,
			uint8_t node_id, uint8_t baudrate, CAN_USED can_nr);
	void setStatusLEDRed(GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
	void setStatusLEDGreen(GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
	void setBothStatusLED(GPIO_TypeDef* port_red, uint16_t pin_red,
			GPIO_TypeDef* port_green, uint16_t pin_green);
	void canProcessNoDelay();
	void canProcessDelay(int delay_ms = 1);	//1ms default

	private:
	void callInitFunc(CAN_USED can_nr);
	void initCan(CAN_USED can_nr);

	FDCAN_HandleTypeDef* m_can_handle;
	TIM_HandleTypeDef* m_timer;
	uint8_t m_node_id;
	uint8_t m_baudrate;
	GPIO_TypeDef* m_led_ports [2] ;
	uint16_t m_led_pins [2];
	CANopenNodeSTM32 m_can_open_node;	//create and store node here
};








#endif /* INC_CPLUSPLUS_CANCONNECTION_HPP_ */
