/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : STM32TouchController.cpp
 ******************************************************************************
 * This file was created by TouchGFX Generator 4.23.2. This file is only
 * generated once! Delete this file from your project and re-generate code
 * using STM32CubeMX or change this file manually to update it.
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

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "OLED.H"
#include "app_freertos.h"
#include "CST816T.h"

void STM32TouchController::init()
{
  /**
   * Initialize touch controller and driver
   *
   */
  CST816T_Init();
}

bool STM32TouchController::sampleTouch(int32_t &x, int32_t &y)
{
  /**
   * By default sampleTouch returns false,
   * return true if a touch has been detected, otherwise false.
   *
   * Coordinates are passed to the caller by reference by x and y.
   *
   * This function is called by the TouchGFX framework.
   * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
   *
   */
  //static int touch = 0;
  //static uint16_t X, Y;
  if (CST816_GetAction((uint16_t *)&x, (uint16_t *)&y))
  {
    //x = (int32_t)X;
    //y = (int32_t)Y;
    HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
    vTaskDelay(9);
    return true;
  }
  // if (touch == 1)
  // {
  //   OLED_Clear();
  //   touch = 0;
  // }
  // OLED_Printf(1, 1, "NO TOUCH");
  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
  vTaskDelay(9);
  return false;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
