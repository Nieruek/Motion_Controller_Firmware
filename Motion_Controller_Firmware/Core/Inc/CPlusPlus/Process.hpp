/*
 * Project:	nucleo_board_G474RE
 * File:	Process.hpp
 *
 * 	Created on: Nov 23, 2023 
 *				with STM32CubeIDE, Ubunut 23.04
 *	Author: Lars Pföhler
 */

#ifndef INC_CPLUSPLUS_PROCESS_HPP_
#define INC_CPLUSPLUS_PROCESS_HPP_

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include <stdbool.h>
#include "CPlusPlus/myIncludes.hpp"

/**
 * @brief wrapper for c++ fctns, so auto generarted main.c file can get used
 * 		and calls this function with c++ code. So no rename and stuff is required.
 * 		Should be called in main.c loop.
 */
EXTERNC void doMyCppProcessHigh(void);
EXTERNC void doMyCppProcessLow(void);
EXTERNC void handleMyCppProcessError(void);
EXTERNC void doMyCanTask(void);
EXTERNC void doMyCMDInterpreterTask(void);			// von Niru
EXTERNC void MotorInit(void);
EXTERNC void Stromregelung(void);
EXTERNC void Positionsregelung(void);
EXTERNC void handlePositionInfo(void);



#endif /* INC_CPLUSPLUS_PROCESS_HPP_ */
