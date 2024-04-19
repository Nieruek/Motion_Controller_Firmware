/*
 * CanConnection.cpp
 *
 *  Created on: Dec 18, 2023
 *      Author: larsi
 */

#include <CPlusPlus/CanConnection.hpp>
#include <cmsis_os.h>
#define RED_POSITION 0
#define GREEN_POSITION 1

CanConnection::CanConnection(FDCAN_HandleTypeDef &can_handle,
		TIM_HandleTypeDef &timer, uint8_t node_id, uint8_t baudrate, CAN_USED can_nr) :
				m_can_handle(&can_handle),
				m_timer(&timer),
				m_node_id(node_id),
				m_baudrate(baudrate)
{
	initCan(can_nr);
}

void CanConnection::initCan(CAN_USED can_nr)
{
	m_can_open_node.CANHandle = m_can_handle;
	callInitFunc(CANFD1);	//choose canfd nr here and set init
	m_can_open_node.timerHandle = m_timer;
	m_can_open_node.desiredNodeID = m_node_id;
	m_can_open_node.baudrate = m_baudrate;
	canopen_app_init(&m_can_open_node);
}

void CanConnection::callInitFunc(CAN_USED can_nr) {
	switch (can_nr) {
	case CANFD1:
		m_can_open_node.HWInitFunction = MX_FDCAN1_Init;
		return;
	case CANFD2:
		//m_can_open_node.HWInitFunction = MX_FDCAN2_Init;
		return;
	default:
		// Print error or stuff
		return;
	}
}

void CanConnection::setStatusLEDRed(GPIO_TypeDef* gpio_port, uint16_t gpio_pin)
{
	m_led_pins[RED_POSITION] = gpio_pin;
	m_led_ports[RED_POSITION] = gpio_port;
}
void CanConnection::setStatusLEDGreen(GPIO_TypeDef* gpio_port, uint16_t gpio_pin)
{
	m_led_pins[GREEN_POSITION] = gpio_pin;
	m_led_ports[GREEN_POSITION] = gpio_port;
}

void CanConnection::setBothStatusLED(GPIO_TypeDef* port_red, uint16_t pin_red,
			GPIO_TypeDef* port_green, uint16_t pin_green)
{
	setStatusLEDGreen(port_green, pin_green);
	setStatusLEDRed(port_red, pin_red);
}

void CanConnection::canProcessNoDelay()
{
	canProcessDelay(0);
}

void CanConnection::canProcessDelay(int delay_ms)
{
	if (m_led_ports[GREEN_POSITION] && m_led_ports[RED_POSITION]) {	//check if set
		HAL_GPIO_WritePin(m_led_ports[GREEN_POSITION], m_led_pins[GREEN_POSITION],
				m_can_open_node.outStatusLEDGreen == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		HAL_GPIO_WritePin(m_led_ports[RED_POSITION], m_led_pins[RED_POSITION],
				m_can_open_node.outStatusLEDGreen == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
	canopen_app_process();
	vTaskDelay(pdMS_TO_TICKS(delay_ms)); // sleep for ... ms
}
