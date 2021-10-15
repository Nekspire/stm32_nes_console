/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Lcd/stm32_adafruit_lcd.h"
#include "nes_controller/nes_controller.h"
#include "ili9486.h"
#include <stdio.h>
#include "fatfs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SELECTOR_X 0
#define RECORD_X (Font16.Width + 1)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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
  MX_FATFS_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  const char selector_type[] = ">";
  const int16_t selector_ylimit = (int16_t) (BSP_LCD_GetYSize() - Font16.Height);

  char path[30] = "";
  char message[30] = "";
  NES_Controller_Button button;
  Point selector_position;
  FATFS fs;
  FIL file;
  DIR dir;
  FILINFO filinfo;
  BYTE opt = 0;
  FRESULT fr_mount, fr_result = FR_OK;

  NES_Controller_Status controller_status = nes_controller_init(&hi2c1);

  BSP_LCD_Init();
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  fr_mount = f_mount(&fs, path, 0);

  if (fr_mount == FR_OK) {
    fr_result = f_getcwd(path, 30);
    f_close(&file);
    if (fr_result == FR_OK) {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) "DISC:", LEFT_MODE);
      BSP_LCD_DisplayStringAt((sizeof("DISC:") - 1) * Font16.Width, 0, (uint8_t *) path, LEFT_MODE);

      uint32_t index = 0;
      fr_result = f_findfirst(&dir, &filinfo, path, "*");

      if (fr_result == FR_OK) {
        BSP_LCD_DisplayStringAt(SELECTOR_X, 2 * Font16.Height, (uint8_t *) &selector_type, LEFT_MODE);
        selector_position.X = (int16_t) SELECTOR_X; selector_position.Y =  (int16_t) (2 * Font16.Height);
      }

      while (fr_result == FR_OK && filinfo.fname[0]) {
        // todo: lcd size limitation check
        BSP_LCD_DisplayStringAt(RECORD_X, (2 + index) * Font16.Height, (uint8_t *) filinfo.fname, LEFT_MODE);
        index += 1;
        fr_result = f_findnext(&dir, &filinfo);
      }

    } else {
      sprintf(message, "%s", "disc: open failed");
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) message, LEFT_MODE);
    }
  } else {
    sprintf(message, "%s", "disc: not mounted");
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) message, LEFT_MODE);
  }

  /*
  BSP_LCD_DisplayStringAt(RECORD_X, 2 * Font16.Height, (uint8_t *) "dir", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 3 * Font16.Height, (uint8_t *) "first_file.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 4 * Font16.Height, (uint8_t *) "file2.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 5 * Font16.Height, (uint8_t *) "file3.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 6 * Font16.Height, (uint8_t *) "file4.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 7 * Font16.Height, (uint8_t *) "file5.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 8 * Font16.Height, (uint8_t *) "file6.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 9 * Font16.Height, (uint8_t *) "file7.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 10 * Font16.Height, (uint8_t *) "file8.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 11 * Font16.Height, (uint8_t *) "file9.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 12 * Font16.Height, (uint8_t *) "file10.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 13 * Font16.Height, (uint8_t *) "file11.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 14 * Font16.Height, (uint8_t *) "file12.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 15 * Font16.Height, (uint8_t *) "file13.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 16 * Font16.Height, (uint8_t *) "file14.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 17 * Font16.Height, (uint8_t *) "file15.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, 18 * Font16.Height, (uint8_t *) "File16.nes", LEFT_MODE);
  BSP_LCD_DisplayStringAt(RECORD_X, BSP_LCD_GetYSize() - Font16.Height, (uint8_t *) "last_file.nes", LEFT_MODE); */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (controller_status == NES_CONTROLLER_OK) {
      HAL_Delay(100);
      bool state = nes_match_button(nes_controller_read_code(&hi2c1), &button);

      if (state == true) {
        switch (button) {
          case DOWN:
            if (selector_position.Y < selector_ylimit) {
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt(selector_position.X, selector_position.Y, (uint8_t *) selector_type, LEFT_MODE);
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              selector_position.Y += Font16.Height;
              BSP_LCD_DisplayStringAt(selector_position.X, selector_position.Y, (uint8_t *) selector_type, LEFT_MODE);
            }
            break;
          case UP:
            if (selector_position.Y > 2 * Font16.Height) {
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt(selector_position.X, selector_position.Y, (uint8_t *) selector_type, LEFT_MODE);
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              selector_position.Y -=  Font16.Height;
              BSP_LCD_DisplayStringAt(selector_position.X, selector_position.Y, (uint8_t *) selector_type, LEFT_MODE);
            }
            break;
          default:
            break;
        }
      }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
