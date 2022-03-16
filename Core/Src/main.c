/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include "stm32f0xx.h"

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */

  //Turning on the GPIO clock for Port C
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  for(int i = 0; i < 4; ++i)
  {
	  GPIOC->MODER |= (0x1UL << (12 + i * 2));		//GPIO_MODER_MODERx_0 set the 1 in 01
	  GPIOC->MODER &= ~(0x2UL << (12 + i * 2)); 	//~(GPIO_MODER_MODERx_1) set the 0 in 01

	  GPIOC->OTYPER &= ~(0x40 << i);				//~(GPIO_OTYPER_OT_x)

	  GPIOC->OSPEEDR |= (0x3UL << (12 + i * 2));	//GPIO_OSPEEDER_OSPEEDRx

	  GPIOC->PUPDR  &= ~(0x3UL << (12 + i * 2));	//~(GPIO_PUPDR_PUPDRx)
  }




  int delayTime = 100;

  int numOfLEDsToLight = 4;

  int setIdx = 0;
  int resetIdx = 3;

  while (1)
  {

	  GPIOC->BSRR |= (0x40U << setIdx);		//GPIO_BSRR_BS_setIdx
	  GPIOC->BRR |= (0x40U << resetIdx);	//GPIO_BRR_BR_resetIdx;

	  ++setIdx;
	  if(setIdx == numOfLEDsToLight)
		  setIdx = 0;
	  ++resetIdx;
	  if(resetIdx == numOfLEDsToLight)
		  resetIdx = 0;

	  HAL_Delay(delayTime);

  }

}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif
