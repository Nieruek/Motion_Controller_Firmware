
#include "CPlusPlus/CmdInterpreter.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "string"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_it.h"
#include "stdio.h"



extern DMA_HandleTypeDef hdma_usart2_rx;

bool interrupt_flag=0;
//uint8_t pos=0;
uint8_t rx_data[20]={0};
uint8_t MainBuffer[20]={0};
uint16_t MainBuffer_SIZE=20;

CmdInterpreter::CmdInterpreter()
{

}

void CmdInterpreter::cmd()
{
	//
	  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_data, sizeof(rx_data));
	  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	  while(1)
	  {
		  if(interrupt_flag==1)
		 	  {
		 		  interrupt_flag=0;
		 		  std::string receive= reinterpret_cast<char*>(MainBuffer);
		 		  	  //
		 		  std::size_t npos= receive.find("A");
		 		  if(npos!=std::string::npos)	// true wenn nichts gefunden
		 		  {

		 			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
		 		  }

		 		  npos= receive.find("LED OFF");
		 		  if(npos!=std::string::npos)	// true wenn nichts gefunden
		 		  {

		 			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
		 		  }
		 		 memset(MainBuffer,0,MainBuffer_SIZE);
		 		 		  memset(rx_data,0,sizeof(rx_data));
		 		 		 // pos=0;

		 	}
		  osDelay(100);
	  }

//		  if(MainBuffer[0]==76 && MainBuffer[1]==69 && MainBuffer[2]==68)
//		  {
//			  if(MainBuffer[4]==79&& MainBuffer[5]==78)
//			  {
//				  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
//
//			  }
//			  else if(MainBuffer[4]==79&& MainBuffer[5]==70 && MainBuffer[6]==70)
//			  {
//				  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
//			  }
//		  }

	  //

}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	static uint16_t pos=0;

	for(int i=0;i<Size;i++)
	{
		if(rx_data[i]==13)
		{
			interrupt_flag=1;
			pos=0;
		}
	}
	if(interrupt_flag==0)
	{

		if(pos+Size>MainBuffer_SIZE)
		{
			pos=0;
		}



	 	memcpy(&MainBuffer[pos],rx_data,Size);


	 	 pos=pos+Size;
	}




	 	 memset(rx_data,0,sizeof(rx_data));
	 	 HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_data,sizeof(rx_data));
		 __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);


}
