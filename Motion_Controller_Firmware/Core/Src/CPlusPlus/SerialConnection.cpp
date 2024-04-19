/*
 * Project:	nucleo_board_G474RE
 * File:	SerialConnection.cpp
 *
 * 	Created on: Nov 23, 2023 
 *				with STM32CubeIDE, Ubunut 23.04
 *	Author: Lars Pföhler
 */

#include <CPlusPlus/SerialConnection.hpp>
#include <CPlusPlus/CommandStrings.hpp>		//simply definition of chars/messages

#include <string.h>

#include "cmsis_os.h"
#include "main.h"
//extern DCMotor motor1;
#include <CPlusPlus/ParameterList.hpp>



extern SerialConnectionDMA serial_connection;	// global to use in ISR-handlers
extern bool dma_ready;	//gets set here in ISR -> used in process.cpp to process data
bool send_complete = true;

extern DCMotor motor1;	// global DCMotor1 in process.cpp
extern DCMotor motor2;	// global DCMotor2 in process.cpp
extern bool runvar;

uint8_t dma_rx_data[bufsize] = {0};	//used to receive dma data, no class members so accessable in ISR
uint8_t dma_tx_data[bufsize] = {0};	// use for dma transmit,
uint8_t ready_rx_data [bufsize] = {0};	//used to process data

SerialConnectionPoll::SerialConnectionPoll(UART_HandleTypeDef uart_handle,
		uint32_t timeout, IMotorBase* current_axis) :
			m_uart_handle(uart_handle),
			m_timeout(timeout),
			m_rec_timeout(timeout),
			m_current_axis(nullptr)
{}
SerialConnectionPoll::SerialConnectionPoll(UART_HandleTypeDef uart_handle,
		uint32_t timeout, uint32_t rec_timeout, IMotorBase* current_axis) :
			m_uart_handle(uart_handle),
			m_timeout(timeout),
			m_rec_timeout(rec_timeout),
			m_current_axis(nullptr)
{}
void SerialConnectionPoll::transmit(std::string &trm)
{
	size_t size = trm.length();
	uint8_t to_trm [size+1];
	std::copy(trm.begin(), trm.end(), &to_trm[0]);
	HAL_UART_Transmit(&m_uart_handle, to_trm, size, m_timeout);
}
void SerialConnectionPoll::receive(std::string &rec)
{
	HAL_UART_Receive(&m_uart_handle, m_rx_buffer, bufsize, m_rec_timeout);
	rec = reinterpret_cast<char*>(m_rx_buffer);
}


SerialConnectionDMA::SerialConnectionDMA(UART_HandleTypeDef *uart_handle, DMA_HandleTypeDef *dma_handle,
		uint32_t timeout, IMotorBase* current_axis) :
			m_uart_handle(uart_handle),
			m_dma_handle(dma_handle),
			m_timeout(timeout),
			m_current_axis(nullptr)
{}

void SerialConnectionDMA::start_receiving()
{
	HAL_UARTEx_ReceiveToIdle_DMA(m_uart_handle, (uint8_t*)dma_rx_data, bufsize);
	__HAL_DMA_DISABLE_IT(m_dma_handle, DMA_IT_HT);
}

void SerialConnectionDMA::transmit(uint8_t* to_send, uint16_t size)
{
	while (!send_complete) {
		if (!runvar) {	// in case stop requested
			return;
		}
		osDelay(10);//delay in case last message didnt finish yet
	}
	memset((uint8_t*)dma_tx_data, 0, bufsize);
	memcpy((uint8_t*)dma_tx_data, to_send, size);
	send_complete = false;	//gets reset in ISR of DMA-TX
	HAL_UART_Transmit_DMA(m_uart_handle, (uint8_t*)dma_tx_data, size);
}

void SerialConnectionDMA::transmitInvalidValueMsg()
{
	transmit(invlaid_value);
}

void SerialConnectionDMA::process_data()
{
	// Commands for this interpreter are defined in CommandStrings.h :)
	int i = 0;
	auto check_cmd_3 = [](uint8_t* comp, uint8_t* cmd) {
		return (*cmd == *comp && *(cmd+1) == *(comp+1) && *(cmd+2) == *(comp+2)); };
	auto check_cmd_2 = [](uint8_t* comp, uint8_t* cmd) {
		return (*cmd == *comp && *(cmd+1) == *(comp+1)); };

	auto check_and_inc_3 = [check_cmd_3, &i](uint8_t* comp, uint8_t* &cmd){
		if (check_cmd_3(cmd, comp)) {
			i = i + 4;	//inc by 4 because separated by " " -> cmd = "XXX " [4]
			cmd = cmd + 4;
			return true;
		}
		return false;
	};
	auto check_and_inc_2 = [check_cmd_2, &i](uint8_t* comp, uint8_t* &cmd){
		if (check_cmd_2(cmd, comp)) {
			i = i + 3;	//inc by 3 because separated by " " -> > cmd = "XX " [3]
			cmd = cmd + 3;
			return true;
		}
		return false;
	};

	auto get_value_double = [&i](uint8_t* cmd){
			double ret_val = atof((char*)cmd);
			if (ret_val <= 0) {
				return -1.0;
			}
			std::string count_helper = std::to_string(ret_val);	//ugly but need the size of double...
			count_helper.erase(count_helper.find_last_not_of("0", std::string::npos));
			if (count_helper.back() == *separator) {	// in case of "int" input
				count_helper.pop_back();
			}
			i = i + count_helper.length();	// get length to know next position of command ;)
			return ret_val;
		};

	// PROCESS
	while( i < bufsize) {	// process command
		auto cmd = (uint8_t*)&ready_rx_data[i];
		double read_value = 0.0;
		uint8_t *requested_value = nullptr;
		if (*cmd == *new_line || *cmd == *c_return)
		{	//end of command, return/should be completed
			transmit(finished_command);
			return;
		}
		if (i > 0)
		{
			cmd = cmd +1;	// if not \n + 1-> " " between next command
		}
		if (check_cmd_3(ver_comp, cmd))
		{	//version requested
			transmit(to_send);
			i = i + 1;
			goto go_on;
		}
		else if(check_and_inc_3(ax1_comp, cmd))
		{
			m_current_axis = &motor1;
		}
		else if(check_and_inc_3(ax2_comp, cmd))
		{
			m_current_axis = &motor2;
		}
		if (check_and_inc_3(set_comp, cmd))
		{		//SETTER
			if (check_and_inc_3(pid_comp, cmd))
			{	//PID
				if (check_and_inc_2(tn_comp, cmd))
				{
					m_current_axis->setTN(get_value_double(cmd), PID_REG);//TODO return param of getvalue is worng...
				}
				else if (check_and_inc_2(kp_comp, cmd))
				{
					m_current_axis->setKP(get_value_double(cmd), PID_REG);
				}
				/*else if (check_and_inc_2(ta_comp, cmd))
				{
					m_current_axis->setTA(get_value_double(cmd), PID_REG);
				}*/
				else
				{
					goto invalid_argument;
				}
			}
			else if (check_and_inc_3(pos_comp, cmd))
			{	//Position
				m_current_axis->setPos(get_value_double(cmd));
			}
			else if (check_and_inc_3(vel_comp, cmd))
			{	//Velocity
				m_current_axis->setVel(get_value_double(vel_comp));
			}
			else if (check_and_inc_3(acc_comp, cmd))
			{	//acceleration
				m_current_axis->setAcc(get_value_double(cmd));
			}
			else if (check_and_inc_2(pi_comp, cmd))
			{	//pi
				if (check_and_inc_2(tn_comp, cmd))
				{
					m_current_axis->setTN(get_value_double(cmd), PI_REG);
				}
				else if (check_and_inc_2(kp_comp, cmd))
				{
					m_current_axis->setKP(get_value_double(cmd), PI_REG);
				}
//				else if (check_and_inc_2(ta_comp, cmd))
//				{
//					m_current_axis->setTA(get_value_double(cmd), PI_REG);
//				}
				else
				{
					goto invalid_argument;
				}
			}
			else
			{	//invalid command, try again
				goto invalid_argument;
			}
		}
		else if (check_and_inc_3(get_comp, cmd))
		{			//GETTER
			if (check_and_inc_3(pid_comp, cmd))
			{	//PID
				if (check_and_inc_2(tn_comp, cmd))
				{
					m_current_axis->getTN(read_value, PID_REG);
					requested_value = tn_comp;
				}
				else if (check_and_inc_2(kp_comp, cmd))
				{
					m_current_axis->getKP(read_value, PID_REG);
					requested_value = kp_comp;
				}
//				else if (check_and_inc_2(ta_comp, cmd))
//				{
//					m_current_axis->getTA(read_value, PID_REG);
//					requested_value = ta_comp;
//				}
				else
				{
					goto invalid_argument;
				}
			}
			else if (check_and_inc_3(pos_comp, cmd))
			{	//Position
				m_current_axis->getPos(read_value);
				requested_value = pos_comp;
			}
			else if (check_and_inc_3(vel_comp, cmd))
			{	//Velocity
				m_current_axis->getVel(read_value);
				requested_value = vel_comp;
			}
			else if (check_and_inc_3(acc_comp, cmd))
			{	//acceleration
				m_current_axis->getAcc(read_value);
				requested_value = acc_comp;
			}
			else if (check_and_inc_2(pi_comp, cmd))
			{	//invalid command, try again
				if (check_and_inc_2(tn_comp, cmd))
				{
					m_current_axis->getTN(read_value, PI_REG);
					requested_value = tn_comp;
				}
				else if (check_and_inc_2(kp_comp, cmd))
				{
					m_current_axis->getKP(read_value, PI_REG);
					requested_value = kp_comp;
				}
//				else if (check_and_inc_2(ta_comp, cmd))
//				{
//					m_current_axis->getTA(read_value, PI_REG);
//					requested_value = ta_comp;
//				}
				else
				{
					goto invalid_argument;
				}
			}
			else
			{	//invalid command, try again
				goto invalid_argument;
			}
		}
		else if (check_and_inc_3(swo_comp, cmd))
		{	//switch on

		}
		else if (check_and_inc_3(rdy_comp, cmd))
		{	//ready

		}
		else if (check_and_inc_3(ope_comp, cmd))
		{	//operational

		}
		else if (check_and_inc_3(sta_comp, cmd))
		{	//status

		}
		else if (check_and_inc_3(ctr_comp, cmd))
		{	//control

		}
		else
		{	// Invalid command, break, user should try again
			goto invalid_argument;
		}
		if (read_value != 0.0 || requested_value)
		{
			uint8_t answer [bufsize] = {0};
			memcpy((uint8_t*)answer, requested_value, 4);
			sprintf((char*)&answer[5], " = %7.3f \n", read_value);	//copy double to buffer, ugly but working
			transmit(answer, bufsize);	//send answer
			return;
		}
		go_on:
		i = i + 3;	// some case was valid, so inc i and continue with next command if there is one
		continue;
		invalid_argument:	//command wasnt valid
		transmit(not_valid);
		return;
	}
	transmit(valid_command);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	static auto old_pos = 0;
	if (dma_ready) {	// data not processed jet, wait for next command
		memset((uint8_t*)dma_rx_data, 0, bufsize);
//		serial_connection.start_receiving();
		return;
	}
	if (old_pos + Size > bufsize || Size > bufsize) {
		// too big command, start again, try again
		old_pos = 0;
		memset((uint8_t*)dma_rx_data, 0, bufsize);
	} else {
		memcpy((uint8_t*)ready_rx_data + old_pos, (uint8_t*)dma_rx_data, Size);
		old_pos = old_pos + Size;
	}

	if (ready_rx_data[old_pos-1] == '\n') {	//check if whole command received
		old_pos = 0;
		dma_ready = true;
	} // e {
	serial_connection.start_receiving();
//	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	send_complete = true;	// sending enabled again
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{}	// only here if needed later
