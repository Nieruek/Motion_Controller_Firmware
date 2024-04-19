/*
 * TestSerialConnection.cpp
 *
 *  Created on: Mar 10, 2024
 *      Author: nirut
 */


#include <CPlusPlus/SerialConnection.hpp>
//#include <CPlusPlus/CommandStrings.hpp>		//simply definition of chars/messages

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
paramList myParamList;

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
		osDelay(1);//delay in case last message didnt finish yet
	}
	memset((uint8_t*)dma_tx_data, 0, bufsize);
	memcpy((uint8_t*)dma_tx_data, to_send, size);
	send_complete = false;	//gets reset in ISR of DMA-TX
	HAL_UART_Transmit_DMA(m_uart_handle, (uint8_t*)dma_tx_data, size);
}

void SerialConnectionDMA::transmitVal(double &val, regulator_type type)
{
	uint8_t temp_tx_buffer[bufsize];
	switch (type) {
	case PID_REG:
		sprintf((char*)temp_tx_buffer,"PID_VAL: %7.3f\n", val);
		break;
	case PI_REG:
		sprintf((char*)temp_tx_buffer,"PI_VAL: %7.3f\n", val);
		break;
	default:
		sprintf((char*)temp_tx_buffer,"NO_VAL_SELECTED: %7.3f\n", val);
		break;
	}
	transmit(temp_tx_buffer, bufsize);
}

void SerialConnectionDMA::transmitVal(double &pid_val, double &pi_val)
{
	if (!send_complete || dma_ready) {
		return;
	}
	uint8_t temp_tx_buffer[bufsize];
	sprintf((char*)temp_tx_buffer,"PID: %7.3f -- PI: %7.3f\n", pid_val, pi_val);
	transmit(temp_tx_buffer, bufsize);
}

void SerialConnectionDMA::transmitInvalidValueMsg()
{
	transmit(invalid_value);
}

void SerialConnectionDMA::process_data()
{
	// Commands for this interpreter are defined in CommandStrings.h :)
	auto check_cmd_3 = [](uint8_t* cmd, uint8_t* comp) {
		return (*cmd == *comp && *(cmd+1) == *(comp+1) && *(cmd+2) == *(comp+2)); };

	auto check_and_inc_3 = [check_cmd_3](uint8_t* comp, uint8_t* &cmd){
		if (check_cmd_3(cmd, comp)) {
			cmd = cmd + 4;
			return true;
		}
		return false;
	};


	 //PROCESS
		auto cmd = (uint8_t*)&ready_rx_data[0];
		bool set=false;
		std::string param_id;
		std::string first_id;
		std::string second_id;
		std::string third_id;
		std::string fourth_id;
		int conv_id;
		int first_digit;
		int second_digit;
		int third_digit;
		int fourth_digit;


		if(check_and_inc_3(myParamList.set_comp,cmd))
		{
			set=true;
		}
		else if(check_and_inc_3(myParamList.get_comp,cmd))
		{
			set=false;
		}
		else
		{
			goto invalid;
		}


		//param_id=*cmd+*(cmd+1)+*(cmd+2)+*(cmd+3);
		first_id=*cmd;
		second_id=*(cmd+1);
		third_id=*(cmd+2);
		fourth_id=*(cmd+3);
		param_id.assign((char*)cmd,4);


		conv_id=std::stoi(param_id);
		first_digit=std::stoi(first_id);
		second_digit=std::stoi(second_id);
		third_digit=std::stoi(third_id);
		fourth_digit=std::stoi(fourth_id);
		cmd=cmd+5;



		switch(first_digit)
		{
		case 0:							// System command
			if(handleSysCmd(conv_id))
			{
				break;
			}
			goto invalid;
		case 1:								// axis command
			if(!handleAxisCmd(second_digit))
			{
				goto invalid;
			}

			if(!handleThirdDigit(fourth_digit,third_digit,cmd,set))
			{
				goto invalid;
			}
			//goto valid
		}
			transmit(valid_command);
			return;
			invalid:	//command wasnt valid
			transmit(not_valid);
			transmit(for_help);

			return;
	transmit(valid_command);
}
bool SerialConnectionDMA::handleSysCmd(int id)
{
	switch(id)
	{
	case 0000:
		transmit(to_send);
		break;
	case 0001:
		//swo
		break;
	case 0002:
		//rdy
		break;
	case 0003:
		//ope
		break;
	case 0004:
		//sta
		break;
	case 0005:
		//ctrl
		break;
	case 0006:
		// cmd list
		printHelp();
		break;
	default:
		//Error
		return false;
	}
	return true;
}

bool SerialConnectionDMA::handleControllerParam(int fourth_digit,uint8_t*cmd, regulator_type used_controller,bool setflag)
{
	double getVal=0;
	switch(fourth_digit)
	{
	case 0:
		if(setflag)
		{
			m_current_axis->setKP(atof((char*)cmd), used_controller);
		}
		else
		{
			m_current_axis->getKP(getVal, used_controller);
		}

		break;
	case 1:
		if(setflag)
		{
			m_current_axis->setTN(atof((char*)cmd), used_controller);
		}
		else
		{
			m_current_axis->getTN(getVal, used_controller);
		}

		break;
	case 2:
		if(setflag)
		{
			m_current_axis->setTV(atof((char*)cmd), used_controller);
		}
		else
		{
			m_current_axis->getTV(getVal, used_controller);
		}

		break;
	default:
		//Error
		return false;
	}
	if(!setflag)
	{
		uint8_t temp_tx_buffer[bufsize];
		sprintf((char*)temp_tx_buffer,"read: %7.3f\n",getVal);
		transmit(temp_tx_buffer,bufsize);
	}
	return true;
}
bool SerialConnectionDMA::handleAxisCmd(int second_digit)
{
	switch(second_digit)
	{
	case 1:							// axis 1
		m_current_axis = &motor1;
		break;
	case 2:							// axis 2
		m_current_axis = &motor2;
		break;
	default:
		//Error
		return false;
	}
	return true;
}

bool SerialConnectionDMA::handleThirdDigit(int fourth_digit, int third_digit, uint8_t*cmd, bool setflag)
{
	regulator_type used_controller;
	double getVal=0;
	switch(third_digit)
	{

	case 0:							// PI
		used_controller= PI_REG;
		return handleControllerParam(fourth_digit,cmd,used_controller,setflag);
		//goto checkRegId

	case 1:							// PID
		used_controller= PID_REG;
		return handleControllerParam(fourth_digit,cmd,used_controller,setflag);
		//goto checkRegId

	case 2:							// Position
		//double ret_val = atof((char*)cmd);
		if(fourth_digit!=0)
		{
			return false;
		}
		if(setflag)
		{
			m_current_axis->setPos(atof((char*)cmd));
		}
		else
		{
			m_current_axis->getPos(getVal);

		}

		break;
	case 3:							// Velocity
		if(fourth_digit!=0)
		{
			return false;
		}
		if(setflag)
		{
			m_current_axis->setVel(atof((char*)cmd));
		}
		else
		{
			m_current_axis->getVel(getVal);
		}
		break;
	case 4:
		if(fourth_digit!=0)
		{
			return false;
		}
		if(setflag)
		{
			m_current_axis->setAcc(atof((char*)cmd));
		}
		else
		{
			m_current_axis->getAcc(getVal);
		}

		break;
	case 5:
		//m_current_axis->setPWMValue(atof((char*)cmd));
		if(fourth_digit!=0)
		{
			return false;
		}
		if(setflag)
		{
			m_current_axis->setDec(atof((char*)cmd));
		}
		else
		{
			m_current_axis->getDec(getVal);
		}
		break;
	case 6:
		// TO-DO: setPwm duty cycle
		break;
	case 7: //set OFFSET
		if(fourth_digit!=0 || !setflag) {
			return false;
		}
		m_current_axis->setOffset(atof((char*)cmd));
		break;
	default:
		//error
		return false;

	}
	if(!setflag)
	{
		uint8_t temp_tx_buffer[bufsize];
		sprintf((char*)temp_tx_buffer,"read: %7.3f\n",getVal);
		transmit(temp_tx_buffer,bufsize);
	}
	return true;

}

void SerialConnectionDMA::printHelp()
{
	for(auto info:{example_help,first_digit_help,sys_cmd,second_digit_help_sys,axis_cmd,second_digit_help_axis,
		sys_cmd,third_digit_help_sys,axis_cmd,third_digit_help_axis_1,third_digit_help_axis_2,sys_cmd,
		fourth_digit_help_sys_1,fourth_digit_help_sys_2,fourth_digit_help_sys_3,axis_cmd,controller_cmd,
		fourth_digit_help_axis_controller,other_cmd,fourth_digit_help_axis_other})
	{
		transmit(info);
	}

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

	if (ready_rx_data[old_pos-1] == '\n'||ready_rx_data[old_pos-1] == '\r') {	//check if whole command received
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




