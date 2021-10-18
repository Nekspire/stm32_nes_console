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
#include <memory.h>
#include <ctype.h>
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

#define SELECTOR_POS_1 2
#define SELECTOR_X 0
#define RECORD_X (Font16.Width + 1)
#define ITEMS 18
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int item = 0;
char items_fname[ITEMS][13];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
size_t strlprecat( char* dst, const char * src, size_t size) {
  size_t dstlen = strnlen( dst, size);
  size_t srclen = strlen( src);
  size_t srcChars = srclen;
  size_t dstChars = dstlen;

  if (0 == size) {
    /* we can't write anything into the dst buffer */
    /*  -- bail out                                */

    return( srclen + dstlen);
  }

  /* determine how much space we need to add to front of dst */

  if (srcChars >= size) {
    /* we can't even fit all of src into dst */
    srcChars = size - 1;
  }

  /* now figure out how many of dst's characters will fit in the remaining buffer */
  if ((srcChars + dstChars) >= size) {
    dstChars = size - srcChars - 1;
  }

  /* move dst to new location, truncating if necessary */
  memmove( dst+srcChars, dst, dstChars);

  /* copy src into the spot we just made for it */
  memcpy( dst, src, srcChars);

  /* make sure the whole thing is terminated properly */
  dst[srcChars + dstChars] = '\0';

  return( srclen + dstlen);
}
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
  Point selector_pixel_position;
  int selector_position = SELECTOR_POS_1;
  int slash = 1;
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
    fr_result = f_getcwd(path, sizeof(path));

    if (fr_result == FR_OK) {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) DRIVE, LEFT_MODE);
      BSP_LCD_DisplayStringAt((sizeof(DRIVE) - 1) * Font16.Width, 0, (uint8_t *) path, LEFT_MODE);

      fr_result = f_findfirst(&dir, &filinfo, path, "*");

      if (fr_result == FR_OK && filinfo.fname[0]) {
        BSP_LCD_DisplayStringAt(SELECTOR_X, 2 * Font16.Height, (uint8_t *) &selector_type, LEFT_MODE);
        selector_pixel_position.X = (int16_t) SELECTOR_X; selector_pixel_position.Y =  (int16_t) (2 * Font16.Height);
      }

      while (fr_result == FR_OK && filinfo.fname[0]) {
        if (item < ITEMS) {
          BSP_LCD_DisplayStringAt(RECORD_X, (2 + item) * Font16.Height, (uint8_t *) filinfo.fname, LEFT_MODE);
          memcpy(items_fname[item], filinfo.fname, sizeof(filinfo.fname));
          item += 1;
          fr_result = f_findnext(&dir, &filinfo);
        } else break;
      }

    } else {
      sprintf(message, "%s open failed", DRIVE);
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) message, LEFT_MODE);
    }
  } else {
    sprintf(message, "%s not mounted", DRIVE);
    BSP_LCD_DisplayStringAt(0, 0, (uint8_t *) message, LEFT_MODE);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (controller_status == NES_CONTROLLER_OK) {
      HAL_Delay(75);
      bool state = nes_match_button(nes_controller_read_code(&hi2c1), &button);

      if (state == true) {
        switch (button) {
          case DOWN:
            if (selector_pixel_position.Y < (SELECTOR_POS_1 + item - 1) * Font16.Height) {
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt(SELECTOR_X, selector_pixel_position.Y, (uint8_t *) selector_type,
                                      LEFT_MODE);

              selector_pixel_position.Y += Font16.Height; selector_position += 1;

              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_DisplayStringAt(SELECTOR_X, selector_pixel_position.Y, (uint8_t *) selector_type,
                                      LEFT_MODE);
            } else {
              // search for next item
              fr_result = f_findnext(&dir, &filinfo);
              if (fr_result == FR_OK && filinfo.fname[0]) {
                // shift down items_fname[]
                for (int i = 0; i < ITEMS - 1; i++) {
                  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                  BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + i) * Font16.Height, (uint8_t *) items_fname[i],
                                          LEFT_MODE);
                  memcpy(items_fname[i], items_fname[i + 1], sizeof(filinfo.fname));
                  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                  BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + i) * Font16.Height, (uint8_t *) items_fname[i],
                                          LEFT_MODE);
                }
                // copy last item
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + ITEMS - 1) * Font16.Height,
                                        (uint8_t *) items_fname[ITEMS - 1],
                                        LEFT_MODE);
                memcpy(items_fname[ITEMS - 1], filinfo.fname, sizeof(filinfo.fname));
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + ITEMS-1) *Font16.Height,
                                        (uint8_t *) items_fname[ITEMS - 1],
                                        LEFT_MODE);
              }
            }
            break;
          case UP:
            if (selector_pixel_position.Y > SELECTOR_POS_1 * Font16.Height) {
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt(SELECTOR_X, selector_pixel_position.Y, (uint8_t *) selector_type,
                                      LEFT_MODE);

              selector_pixel_position.Y -=  Font16.Height; selector_position -= 1;

              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_DisplayStringAt(SELECTOR_X, selector_pixel_position.Y, (uint8_t *) selector_type,
                                      LEFT_MODE);
            }
            break;
          case A:
            if (strchr(items_fname[selector_position - SELECTOR_POS_1], '.') != NULL) {
              // file
            } else {
              // dir
              // clear old path on display
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt((sizeof(DRIVE) - 1) * Font16.Width, 0, (uint8_t *) path, LEFT_MODE);
              // add to path new directory
              if (path[strlen(path) - 1] != '/') {
                strcat(path, "/");
                slash += 1;
              }
              strcat(path, items_fname[selector_position - SELECTOR_POS_1]);
              // display new path
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_DisplayStringAt((sizeof(DRIVE) - 1) * Font16.Width, 0, (uint8_t *) path, LEFT_MODE);
              // clear last item selector
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt(SELECTOR_X, selector_position * Font16.Height, (uint8_t *) &selector_type,
                                      LEFT_MODE);
              // clear items from last path
              for (int i = 0; i < item; i++) {
                BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + i) * Font16.Height, (uint8_t *) items_fname[i],
                                        LEFT_MODE);
              }
              // find first item in path
              fr_result = f_findfirst(&dir, &filinfo, path, "*");
              // display item selector if there are items in directory
              if (fr_result == FR_OK && filinfo.fname[0]) {
                // set initial selector position
                selector_position = SELECTOR_POS_1;
                // display selector on screen
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(SELECTOR_X, SELECTOR_POS_1 * Font16.Height, (uint8_t *) &selector_type,
                                        LEFT_MODE);
                selector_pixel_position.X = (int16_t) SELECTOR_X;
                selector_pixel_position.Y = (int16_t) (SELECTOR_POS_1 * Font16.Height);
              }
              // reset items name buffer
              memset(items_fname, 0, sizeof items_fname);
              // reset item counter
              item = 0;
              while (fr_result == FR_OK && filinfo.fname[0]) {
                if (item < ITEMS) {
                  BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + item) * Font16.Height, (uint8_t *) filinfo.fname,
                                          LEFT_MODE);
                  memcpy(items_fname[item], filinfo.fname, sizeof(filinfo.fname));
                  item += 1;
                  fr_result = f_findnext(&dir, &filinfo);
                } else break;
              }
            }
            break;
          case B:
            // path is not root == "/"
            if (strlen(path) > 1) {
              int path_len = strlen(path);
              int chars = 0, i = path_len;
              // clear old path on display
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt((sizeof(DRIVE) - 1) * Font16.Width, 0, (uint8_t *) path, LEFT_MODE);
              // check '/' separator in path
              while (path[i] != '/') {
                i--;
                chars++;
              }
              if (slash >= 2) {
                path_len = path_len - chars;
                slash -= 1;
              } else {
                path_len = path_len - (chars - 1);
              }
              // trim path
              strlprecat(path, path, path_len + 1);
              // display new path
              BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
              BSP_LCD_DisplayStringAt((sizeof(DRIVE) - 1) * Font16.Width, 0, (uint8_t *) path, LEFT_MODE);
              // clear last item selector
              BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
              BSP_LCD_DisplayStringAt(SELECTOR_X, selector_position * Font16.Height, (uint8_t *) &selector_type,
                                      LEFT_MODE);
              // set initial selector position
              selector_position = SELECTOR_POS_1;
              // clear items from last path
              for (int i = 0; i < item; i++) {
                BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + i) * Font16.Height, (uint8_t *) items_fname[i],
                                        LEFT_MODE);
              }
              // find first item in path
              f_closedir(&dir);
              fr_result = f_findfirst(&dir, &filinfo, path, "*");
              // display item selector if there are items in directory
              if (fr_result == FR_OK && filinfo.fname[0]) {
                // set initial selector position
                selector_position = SELECTOR_POS_1;
                // display selector on screen
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DisplayStringAt(SELECTOR_X, SELECTOR_POS_1 * Font16.Height, (uint8_t *) &selector_type,
                                        LEFT_MODE);
                selector_pixel_position.X = (int16_t) SELECTOR_X;
                selector_pixel_position.Y = (int16_t) (SELECTOR_POS_1 * Font16.Height);
              }
              // reset items name buffer
              memset(items_fname, 0, sizeof items_fname);
              // reset item counter
              item = 0;
              while (fr_result == FR_OK && filinfo.fname[0]) {
                if (item < ITEMS) {
                  BSP_LCD_DisplayStringAt(RECORD_X, (SELECTOR_POS_1 + item) * Font16.Height, (uint8_t *) filinfo.fname,
                                          LEFT_MODE);
                  memcpy(items_fname[item], filinfo.fname, sizeof(filinfo.fname));
                  item += 1;
                  fr_result = f_findnext(&dir, &filinfo);
                } else break;
              }
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
