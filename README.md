# File viewer for sd cards

## Features:
- Multi-platform
- No dynamic memory allocation
- File system based on FatFS.
- Display based on STM32_ADAFRUIT_LCD BSP functions.
- Short and long file name support

## Functions offer:
- Displaying directory contents: files and subdirectories
- Displaying path
- Displaying size of file
- Directory scrolling: up and down
- Directory scrolling by page: forward and backward
- Unwrapping file names, if it's too long

## Photos:
- Hardware: Ili9486 TFT-LCD display, STM32F446RE Nucleo Board
    #### Short file name
  ![](img/IMG_20211029_172232.jpg)
    #### Long file name
  ![](img/IMG_20211029_172021.jpg)
  
  File Viewer also works on lower size ILI9341 display.
    #### Short file name
  ![](img/IMG_20211030_132259.jpg)
    #### Long file name
  ![](img/IMG_20211030_132929.jpg)