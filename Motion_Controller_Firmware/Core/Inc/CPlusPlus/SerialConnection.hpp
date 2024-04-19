/*
 * Project:	nucleo_board_G474RE
 * File:	SerialConnection.hpp
 *
 * 	Created on: Nov 23, 2023 
 *				with STM32CubeIDE, Ubunut 23.04
 *	Author: Lars Pföhler
 */

#ifndef INC_CPLUSPLUS_SERIALCONNECTION_HPP_
#define INC_CPLUSPLUS_SERIALCONNECTION_HPP_

#define bufsize 32

#include <string>
#include <string.h>
#include <stm32g4xx_hal.h>

#include <CPlusPlus/DCMotor.hpp>

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

constexpr size_t uart_timeout = 100;



class SerialConnectionPoll {
	public:
		SerialConnectionPoll(UART_HandleTypeDef uart_handle,
				uint32_t timeout, IMotorBase* current_axis);
		SerialConnectionPoll(UART_HandleTypeDef uart_handle, uint32_t timeout,
			uint32_t rec_timeout, IMotorBase* current_axis);
		void transmit(std::string &trm);
		void receive(std::string &rec);
	private:
		UART_HandleTypeDef m_uart_handle;
		uint32_t m_timeout;
		uint32_t m_rec_timeout;
		uint8_t m_rx_buffer [bufsize];
		DCMotor* m_current_axis;
};

class SerialConnectionDMA {
	public:
		//SerialConnectionDMA(UART_HandleTypeDef *uart_handle, DMA_HandleTypeDef *dma_handle,
		//		uint32_t timeout, DCMotor* current_axis);
		SerialConnectionDMA(UART_HandleTypeDef *uart_handle, DMA_HandleTypeDef *dma_handle,
					uint32_t timeout, IMotorBase* current_axis);
		void transmit(uint8_t* to_send, uint16_t size = bufsize);
		void transmitVal(double &val, regulator_type type);
		void transmitVal(double &pid_val, double &pi_val);
		void transmitInvalidValueMsg();
		void start_receiving();
		void process_data();
	private:
		UART_HandleTypeDef* m_uart_handle;
		DMA_HandleTypeDef* m_dma_handle;
		uint32_t m_timeout;
		DCMotor* m_current_axis;
		bool handleSysCmd(int id);
		bool handleAxisCmd(int second_digit);
		bool handleControllerParam(int fourth_digit,uint8_t* cmd,regulator_type used_controller,bool setflag=false);
		bool handleThirdDigit(int fourth_digit,int third_digit,uint8_t*cmd,bool setflag=false);
		void printHelp();
};


#endif /* INC_CPLUSPLUS_SERIALCONNECTION_HPP_ */
