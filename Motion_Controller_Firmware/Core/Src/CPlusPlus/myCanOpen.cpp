/*
 * Project:	nucleo_board_G474RE
 * File:	CanOpen.cpp
 *
 * 	Created on: Dec 12, 2023 
 *				with STM32CubeIDE, Ubunut 23.04
 *	Author: Lars Pföhler
 */

#include <CPlusPlus/myCanOpen.hpp>
#include <stm32g4xx_hal.h>
extern bool runvar;
#include <fdcan.h>
#include <tim.h>
#include "CPlusPlus/CanConnection.hpp"
#include <cmsis_os.h>

void canOpenTask(void) {
	// Create class object and start it
	CanConnection myCanNode (hfdcan1, htim17, 21, 500, CanConnection::CANFD1);
	myCanNode.setBothStatusLED(extern_LED_3_GPIO_Port, extern_LED_3_Pin,
			extern_LED_2_GPIO_Port, extern_LED_2_Pin);

	FDCAN_TxHeaderTypeDef   TxHeader;
	//FDCAN_RxHeaderTypeDef   RxHeader;
	uint8_t               TxData[12];
	//uint8_t               RxData[12];
	int indx = 0;

//	  if(HAL_FDCAN_Start(&hfdcan1)!= HAL_OK)
//	  {
//		  Error_Handler();
//	  }
//	  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
//	  {
//		/* Notification Error */
//		Error_Handler();
//	  }
//
//	  TxHeader.Identifier = 0x11;
//		TxHeader.IdType = FDCAN_STANDARD_ID;
//		TxHeader.TxFrameType = FDCAN_DATA_FRAME;
//		TxHeader.DataLength = FDCAN_DLC_BYTES_12;
//		TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
//		TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
//		TxHeader.FDFormat = FDCAN_FD_CAN;
//		TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
//		TxHeader.MessageMarker = 0;
//

	do {
		myCanNode.canProcessDelay(10);

//		 for (int i=0; i<12; i++)
//		 {
//		  TxData[i] = indx++;
//		 }
//
//		 if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)
//		 {
//		  Error_Handler();
//		 }

		 osDelay(100);

	} while(1);

	for (auto i=0; i<5; i++) {	//blink fancy in case of error/end
		HAL_GPIO_TogglePin(extern_LED_2_GPIO_Port, extern_LED_2_Pin);
		HAL_GPIO_TogglePin(extern_LED_3_GPIO_Port, extern_LED_3_Pin);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}

