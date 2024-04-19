/*
 * Project:	nucleo_board_G474RE
 * File:	Process.cpp
 *
 * 	Created on: Nov 23, 2023 
 *				with STM32CubeIDE, Ubunut 23.04
 *	Author: Lars Pföhler
 */

#include <CPlusPlus/Process.hpp>
#include <CPlusPlus/SerialConnection.hpp>
#include <CPlusPlus/myCanOpen.hpp>
#include <CPlusPlus/CanConnection.hpp>
#include <CPlusPlus/DCMotor.hpp>
#include <CPlusPlus/OutputMatrixPWM.hpp>
//#include <CPlusPlus/CmdInterpreter.hpp>
#include <vector>
#include "cmsis_os.h"
#include "main.h"

//#define PWM_MAX 17000
//#define PWM_MAX1 16000
//#define PWM_MAX2 15000
//#define PWM_MAX3 8500
//#define PWM_MAX4 7500

//TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
//extern size_t uart_timeout;
extern bool runvar;
extern bool dma_ready;
extern uint8_t Rx_data[100];


DCMotor motor1;
DCMotor motor2;
SerialConnectionDMA serial_connection(&huart2, &hdma_usart2_rx, uart_timeout, &motor1);	//
OutputMatrixPWM output_matrix;
PWM pwm;
ADC adc;

int32_t uwValue= 0;
uint32_t uwDirection = 0;
int test_case = 0;

//extern "C" {
//#include "Initializer.h"
//}

void doMyCppProcessHigh(void) {
	//uint8_t receive_buffer_dma [bufsize];
	//SerialConnectionPoll test_uart(huart2, uart_timeout, uart_timeout);
	//SerialConnectionDMA dma_uart(huart2 ,uart_timeout, receive_buffer_dma);

	//HAL_UART_Receive_DMA(&huart2, Rx_data, 4);	//init rec

	while(1) {	//loop replacing main while(1), break/return possible-> gets recalled from rtos from start
		HAL_GPIO_TogglePin(extern_LED_2_GPIO_Port, extern_LED_2_Pin);
		osDelay(100);
	}

}

void doMyCanTask(void)
{
	canOpenTask();	//calls loop of can_open_controller
//	HAL_GPIO_TogglePin(extern_LED_GPIO_Port, extern_LED_Pin);
	osDelay(1000);
}

void doMyCppProcessLow(void) {
	//uint8_t receive_buffer_dma [bufsize];
	//SerialConnectionPoll test_uart(huart2, uart_timeout, uart_timeout);
	//SerialConnectionDMA dma_uart(huart2 ,uart_timeout, receive_buffer_dma);


//	std::string to_send = "Hello Task-LOW\r\n";
	while(1) {
		//HAL_GPIO_TogglePin(extern_LED_3_GPIO_Port, extern_LED_3_Pin);
				osDelay(1000);
	}
//		test_uart.transmit(to_send);
////		test_uart.transmit(received);
//		osDelay(10000);
//	}
//
//	std::string error = "loop ended, exiting\r\n";
//	test_uart.transmit(error);
}


EXTERNC void MotorInit(void) 
{
	paramPID PID_1, PID_2;
	paramPI PI_1, PI_2;
	paramTrajGen Trapez_1, Trapez_2;

	PID_1.Kp = 50000.0 ;	//50000.0
	PID_1.Tn = 2000.0 ;	//1000.0;
	PID_1.Tv = 3000.0 ;	//3000.0;
	Trapez_1.vel_max=600;
	Trapez_1.acc=5000;
	Trapez_1.dec=5000;
	Trapez_1.servo_time=0.0001;
	PID_1.PID_lim=1;
	PI_1.Kp=500000.0;
	PI_1.Tn = 30000 ;
	PI_1.PI_lim=1;

	PID_2.Kp=1;
	PID_2.Tn=2;
	PID_2.Tv=3;
	Trapez_2.vel_max=200;
	Trapez_2.acc=5000;
	Trapez_2.dec=5000;
	Trapez_2.servo_time=0.0001;
	PID_2.PID_lim=1;
	PI_2.Kp=1;
	PI_2.Tn=2;
	PI_2.PI_lim=1;

	motor1.setParam(PI_1, PID_1, Trapez_1);
	motor1.setADC(&adc);
	motor1.setPMW(&pwm);
	output_matrix.setMatrix(&motor1);
	//motor2.setParam(PI_2, PID_2, Trapez_2);
}
EXTERNC void Stromregelung(void) 
{
	output_matrix.calcADCAxisValue();

	motor1.CurrentRegulation();
	motor2.CurrentRegulation();
	// Output-Matrix
	output_matrix.calcPWMAxisValue();

	// PWM
	motor1.setPWMValue();
	//motor2.setPWMValue();
}
EXTERNC void Positionsregelung(void) 
{
	//Read Sensor
	switch (test_case) {
	case 1:
		uwValue = __HAL_TIM_GetCounter(&htim2);
		break;
	case 2:
		uwValue ++;
		if (uwValue > 100000) {
			uwValue = 0;
		}
		break;
	default:
		break;
	}
	if (test_case > 0) {
		double tmp = uwValue * 360.0;
		tmp=tmp/900000.0;
		motor1.setPos(tmp);
	}

	//motor2.setPos(tmp);
	motor1.TrajectoryGenerator();
	//motor2.TrajectoryGenerator();
	//PID control position
	motor1.PositionRegulation();
	//motor2.PositonsRegulation();
}
void handlePositionInfo(void)
{
	static int32_t position = 0;
//	static uint32_t direction = 0;	not needed yet
	static std::vector<int32_t> last_pos_values;

	position = __HAL_TIM_GetCounter(&htim2);
//	direction = __HAL_TIM_DIRECTION_STATUS(&htim2);
	double temp = position * 360.0;
	temp = temp / 900000.0;	//resoltution of singal/degreee
	motor1.setCurrentPos(temp);
	//last_pos_values.push_back(temp);
}

void doMyCMDInterpreterTask()
{
	osDelay(500);	//short delay to make sure other inits take place
	serial_connection.start_receiving();
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dma_data, bufsize);
//	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	while (runvar) {
		if (dma_ready) {	// received stuff -> process
			serial_connection.process_data();
			dma_ready = false;
			serial_connection.start_receiving();
		}
		osDelay(100);
	}
	//motor1.setPos(myCmdInterpreter.getPos());
	// oder Funktion direkt hier implementieren
}
