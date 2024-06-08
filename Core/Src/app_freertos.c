/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : FreeRTOS applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_touchgfx.h"
#include "tim.h"
#include "CST816T.h"
#include "st7789v.h"
#include "OLED.h"
#include "rtc.h"
#include "usart.h"
#include "stdio.h"
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 1024 * 6};
RTC_TimeTypeDef SetTime;
RTC_DateTypeDef SetDate;
extern RTC_TimeTypeDef GetTime = {0};
extern RTC_DateTypeDef GetDate = {0};
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void OLED_ShowTouch(void *arg);
void RtcGetTime(void *arg);
uint8_t RX_Data[100] = {0};
/* USER CODE END FunctionPrototypes */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RX_Data, 100); // 打开串口中断
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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(RtcGetTime, "RtcGetTime", 256, NULL, osPriorityNormal, NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief Function implementing the defaultTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */
  OLED_Init();
  OLED_Printf(1, 1, "Hello!");
  LCD_init(0);
  CST816T_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  // setTime24Hour(3, 2, 10);
  for (;;)
  {
    MX_TouchGFX_Process();
  }
  /* USER CODE END defaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void OLED_ShowTouch(void *arg)
{
  int touch = 0;
  uint16_t x = 0, y = 0;
  for (;;)
  {
    OLED_Printf(1, 1, "x:%d y:%d  ", x, y);
  }
}

void RtcGetTime(void *arg)
{
  OLED_Init();
  for (;;)
  {
    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
    OLED_Printf(1, 2, "Time:%02d:%02d:%02d", GetTime.Hours, GetTime.Minutes, GetTime.Seconds);
    osDelay(980);
  }
}

/**
 * @brief 检查字符串str中是否包含字符串target
 * @param str 需要检查的字符串
 * @param target 目标字符串
 * @note str和target中包含的字符串可以包含通配符'*'，表示任意字符或任意长度字符串
 * @return 相同返回1，不相同返回0
 */
char StringDetect(char *str, uint8_t *target)
{
  for (int i = 0, j = 0; str[i] != '\0'; i++, j++)
  {
    if (str[i] == '*')
    {
      i++;
      if (str[i] == '\0')
        return 1;
      for (; target[j] != '\0'; j++)
      {
        if (str[i] == target[j])
          break;
      }
    }
    if (str[i] != target[j])
      return 0;
  }
  return 1;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart == &huart1)
  {
    if (StringDetect("SD:*-*-*", RX_Data))
    {
      int year = 0;
      sscanf((char *)RX_Data, "SD:%d-%d-%d", (int *)&year, (int *)&SetDate.Month, (int *)&SetDate.Date);
      SetDate.Year = year;
      HAL_RTC_SetDate(&hrtc, &SetDate, RTC_FORMAT_BIN);
      HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"The data has been set successfully\n", 35);
    }
    else if (StringDetect("ST:*:*:*", RX_Data))
    {
      sscanf((char *)RX_Data, "ST:%d:%d:%d", (int *)&SetTime.Hours, (int *)&SetTime.Minutes, (int *)&SetTime.Seconds);
      HAL_RTC_SetTime(&hrtc, &SetTime, RTC_FORMAT_BIN);
      HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"The time has been set successfully\n", 35);
    }
    else
    {
      HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"error:Command Not Found\n", 24);
    }
    // HAL_UART_Transmit(&huart1, RX_Data, Size, 100);
    HAL_UARTEx_ReceiveToIdle_DMA(huart, RX_Data, 100);
  }
}
/* USER CODE END Application */
