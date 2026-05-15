/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// !!!
#include "fonts.h"
#include "ssd1306.h"
#include "stdio.h"
#include "max30102.h"
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

/* USER CODE BEGIN PV */
// !!!
extern uint8_t spo2;
extern uint8_t heartRate;
int16_t diff =0;
char SPO2[8] =  {0};
char HR[8] = {0};
int16_t diff_past = 0;
uint8_t i = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // !!!
  SSD1306_Init();
  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("max30102", &Font_11x18, 1);
  SSD1306_GotoXY (99,20);
  SSD1306_Puts ("bpm", &Font_7x10, 1);
  SSD1306_GotoXY (99,45);
  SSD1306_Puts ("%", &Font_11x18, 1);
  SSD1306_UpdateScreen();

  HAL_TIM_Base_Start_IT(&htim2);

  max30102_init();
  uint8_t consecutive_valid = 0;
  uint8_t is_showing_pulse = 255; 
  uint8_t past_heartRate = 0;
  int16_t diff_past = 0;

  while (1)
  {
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) 
    {
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      
      max30102_cal();
      spo2 = max30102_getSpO2();
      heartRate = max30102_getHeartRate();
      int16_t current_diff = max30102_getDiff();

      int16_t graph_val = current_diff;
      if (graph_val > 15) graph_val = 15;
      if (graph_val < 0) graph_val = 0;

      if (i == 0) diff_past = graph_val;
      
      SSD1306_DrawLine(i, 15 - diff_past, i + 1, 15 - graph_val, 1);
      diff_past = graph_val;

      if (i > 124) {
        SSD1306_DrawFilledRectangle(0, 0, 127, 16, 0);
        i = 0;
      } else {
        i++;
      }

      uint8_t is_valid_range = (heartRate >= 30 && heartRate <= 220 && spo2 > 0);
      uint8_t is_stable = 0;

      if (is_valid_range) {
        if (past_heartRate == 0) {
          is_stable = 1; 
        } else {
          int16_t hr_delta = abs((int16_t)heartRate - (int16_t)past_heartRate);
          int16_t threshold = (past_heartRate * 20) / 100;
          
          if (hr_delta <= threshold) {
            is_stable = 1;
          }
        }
      }


      if (is_stable) {
        past_heartRate = heartRate; 
        if (consecutive_valid < 10) consecutive_valid++;
      } else {
        if (consecutive_valid > 0) consecutive_valid--;
        if (consecutive_valid == 0) past_heartRate = 0; 
      }

      uint8_t should_show_pulse = (consecutive_valid >= 3); 

      if (should_show_pulse != is_showing_pulse) 
      {
        is_showing_pulse = should_show_pulse;
        
        
        SSD1306_DrawFilledRectangle(0, 17, 128, 47, 0); 
        
        if (is_showing_pulse) 
        {

          SSD1306_GotoXY(99, 20);
          SSD1306_Puts("bpm", &Font_7x10, 1);
          SSD1306_GotoXY(99, 45);
          SSD1306_Puts("%", &Font_11x18, 1);
        }
        else 
        {
          SSD1306_GotoXY(0, 25);
          SSD1306_Puts("Scanning", &Font_7x10, 1);
          SSD1306_GotoXY(0, 45);
          SSD1306_Puts("pulse...", &Font_7x10, 1);
        }
      }

      if (is_showing_pulse) 
      {
        sprintf(HR, "%3d", heartRate > 999 ? 999 : heartRate);
        sprintf(SPO2, "%3d", spo2);
        
        SSD1306_GotoXY(60, 20);
        SSD1306_Puts(HR, &Font_11x18, 1);
        SSD1306_GotoXY(60, 45);
        SSD1306_Puts(SPO2, &Font_11x18, 1);
      }

      SSD1306_UpdateScreen();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
