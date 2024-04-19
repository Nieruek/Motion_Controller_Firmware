/*
 * Commands.hpp
 *
 *  Created on: Mar 5, 2024
 *      Author: root
 */

#ifndef INC_CPLUSPLUS_COMMANDSTRINGS_HPP_
#define INC_CPLUSPLUS_COMMANDSTRINGS_HPP_

	// Strings for user-interface definedd here
	// different commands and info messages

	// length 4
	uint8_t ver_comp [4] = "ver";
	uint8_t swo_comp [4] = "swo";
	uint8_t sta_comp [4] = "sta";
	uint8_t rdy_comp [4] = "rdy";
	uint8_t ope_comp [4] = "ope";
	uint8_t ctr_comp [4] = "ctr";
	uint8_t	set_comp [4] = "set";
	uint8_t	get_comp [4] = "get";
	uint8_t	vel_comp [4] = "vel";
	uint8_t	pos_comp [4] = "pos";
	uint8_t	acc_comp [4] = "acc";
	uint8_t	pid_comp [4] = "pid";
	uint8_t ax1_comp [4] = "ax1";
	uint8_t ax2_comp [4] = "ax2";

	// length 3
	uint8_t	pi_comp [3] = "pi";
	uint8_t	kp_comp [3] = "kp";
	uint8_t	tv_comp [3] = "tv";
	uint8_t	tn_comp [3] = "tn";
	uint8_t	ta_comp [3] = "ta";

	//	length 2
	uint8_t	new_line [2] = "\n";
	uint8_t	c_return [2] = "\r";
	uint8_t zero_term [2] = "\0";
	uint8_t separator [2] = ".";

	//	some info prints and stuff
	uint8_t to_send [bufsize] = "Hallo, ich kann sprechen ^.^ \n";
	uint8_t not_valid [bufsize] = "invalid command try again :(\n";
	uint8_t finished_command [bufsize] = "Enter next command:\n";
	uint8_t valid_command [bufsize] = "Command valid, good job! :)\n";
	uint8_t invlaid_value[bufsize] = "Value < 0 try again :(\n";

#endif /* INC_CPLUSPLUS_COMMANDSTRINGS_HPP_ */
