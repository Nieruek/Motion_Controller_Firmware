/*
 * ParameterList.hpp
 *
 *  Created on: Mar 15, 2024
 *      Author: nirut
 */

#ifndef INC_CPLUSPLUS_PARAMETERLIST_HPP_
#define INC_CPLUSPLUS_PARAMETERLIST_HPP_

#include <cstdint>



struct parameter
{
	uint16_t id;
	union
	{
		int intVal;
		double doubleVal;
		uint8_t uint8Val;

	};
	uint8_t datatype;
};


struct paramList
{
	uint8_t	set_comp [4] = "set";
	uint8_t	get_comp [4] = "get";
	uint8_t	new_line [2] = "\n";
	uint8_t	c_return [2] = "\r";
	uint8_t zero_term [2] = "\0";
	uint8_t separator [2] = ".";

	//	some info prints and stuff

	//x.Val[0]=4;
};

//uint8_t to_send [bufsize] = "Hallo, ich kann sprechen ^.^ \n";
uint8_t to_send [bufsize] = "Version 1.0\n";
uint8_t not_valid [bufsize] = "invalid command try again :(\n";
uint8_t finished_command [bufsize] = "Enter next command:\n";
uint8_t valid_command [bufsize] = "Command valid, good job! :)\r\n";
uint8_t invalid_value[bufsize] = "Value < 0 try again :(\n";

uint8_t for_help[bufsize] = "For help use: get 0006\n";
uint8_t example_help[bufsize] = "cmd-example:set/get xxxx Value\n";
uint8_t first_digit_help[bufsize] = "1. digit 0=Sys,1=Axis\r\n";

uint8_t sys_cmd[bufsize] = "Case sys: ";
uint8_t second_digit_help_sys[bufsize] = "2.digit 0\n";
uint8_t third_digit_help_sys[bufsize] = "3.digit 0\n";
uint8_t fourth_digit_help_sys_1[bufsize] = "4.digit 0=Version,1=Status,";
uint8_t fourth_digit_help_sys_2[bufsize] = "2=Ready,3=Operation,4=Status,";
uint8_t fourth_digit_help_sys_3[bufsize] = "5=Control,6=Cmd List\r\n";

uint8_t axis_cmd[bufsize] = "Case axis: ";
uint8_t second_digit_help_axis[bufsize] = "2.digit 1=Axis1, 2=Axis2\r\n";
uint8_t third_digit_help_axis_1[bufsize] = "3.digit 0=PI,1=PID,2=Pos,3=Vel,";
uint8_t third_digit_help_axis_2[bufsize] = "4=Acc,5=Dec,6=PWM-DutyCycle\r\n";
uint8_t controller_cmd[bufsize] = "\nController command:";
uint8_t other_cmd[bufsize] = "Pos,Vel,Acc,Dec,PWM:";
uint8_t fourth_digit_help_axis_controller[bufsize] = "4.digit 0=Kp,1=Tn,2=Tv\r\n";
uint8_t fourth_digit_help_axis_other[bufsize] = "4.digit 0=valid,else: invalid\r\n";




#endif /* INC_CPLUSPLUS_PARAMETERLIST_HPP_ */
