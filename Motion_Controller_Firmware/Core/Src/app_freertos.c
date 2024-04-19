/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CPlusPlus/Process.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for highPrioTask */
osThreadId_t highPrioTaskHandle;
const osThreadAttr_t highPrioTask_attributes = {
  .name = "highPrioTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 256 * 4
};
/* Definitions for lowPrioTask */
osThreadId_t lowPrioTaskHandle;
const osThreadAttr_t lowPrioTask_attributes = {
  .name = "lowPrioTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 256 * 4
};
/* Definitions for myCANTask */
osThreadId_t myCANTaskHandle;
const osThreadAttr_t myCANTask_attributes = {
  .name = "myCANTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 256 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void startHighPrioTask(void *argument);
void startLowPrioTask(void *argument);
void startMyCANTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of highPrioTask */
  highPrioTaskHandle = osThreadNew(startHighPrioTask, NULL, &highPrioTask_attributes);

  /* creation of lowPrioTask */
  lowPrioTaskHandle = osThreadNew(startLowPrioTask, NULL, &lowPrioTask_attributes);

  /* creation of myCANTask */
  myCANTaskHandle = osThreadNew(startMyCANTask, NULL, &myCANTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_startHighPrioTask */
/**
  * @brief  Function implementing the highPrioTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_startHighPrioTask */
void startHighPrioTask(void *argument)
{
  /* USER CODE BEGIN startHighPrioTask */
  /* Infinite loop */
  for(;;)
  {
	doMyCppProcessHigh();
    osDelay(1);
  }
  /* USER CODE END startHighPrioTask */
}

/* USER CODE BEGIN Header_startLowPrioTask */
/**
* @brief Function implementing the lowPrioTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startLowPrioTask */
void startLowPrioTask(void *argument)
{
  /* USER CODE BEGIN startLowPrioTask */
  /* Infinite loop */
  for(;;)
  {
	doMyCMDInterpreterTask();
    osDelay(1);
  }
  /* USER CODE END startLowPrioTask */
}

/* USER CODE BEGIN Header_startMyCANTask */
/**
* @brief Function implementing the myCANTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startMyCANTask */
void startMyCANTask(void *argument)
{
  /* USER CODE BEGIN startMyCANTask */
  /* Infinite loop */
  for(;;)
  {
	doMyCanTask();
    osDelay(1);
  }
  /* USER CODE END startMyCANTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

