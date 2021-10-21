#ifndef FILE_VIEWER_H
#define FILE_VIEWER_H

#include <fatfs.h>
#include <ffconf.h>
#include <fonts.h>
#include <stm32_adafruit_lcd.h>


#define SELECTOR_TYPE ">\0"
// maximum characters in path

#if _USE_LFN
#define MAX_PATH_CHAR     (5 * (_MAX_LFN + 1))
#define MAX_FILENAME_CHAR (_MAX_LFN + 1)
#else
#define MAX_PATH_CHAR     100
#define MAX_FILENAME_CHAR 13
#endif


// initial selector position line
#define SELECTOR_POS_1  2
// selector x coordinate position
#define SELECTOR_X      0
// maximum items on display
// maximum items on display is equal to: (DISP_X_SIZE or DISP_X_SIZE / pFont->Height) - SELECTOR_POS_1
// for ili9486 and font height = 16 it's: (320 / 16) - 2 = 18
#define ITEMS           14
// maximum characters of ITEM name in line
// maximum characters of ITEM name in line is equal to: (DISP_X_SIZE or DISP_X_SIZE / pFont->Width) - 1 (selector char)
// for ili9486 and font width = 14 it's: (480 / 14)  - 1 = 33
#define MAX_ITEM_CHAR  33
// default disc name, it does not impact on path
#define DRIVE           "DISC:"
// delay macro
#define DELAY_MS(x) HAL_Delay(x)

typedef struct {
    FATFS *fs;
    DIR *dir;
    FILINFO *filinfo;
    char path[MAX_PATH_CHAR];
    LCD_DrawPropTypeDef display_properties;
} FileViewer;

bool FileViewer_init(FileViewer *);
void FileViewer_enter_directory(FileViewer *);
void FileViewer_leave_directory(FileViewer *);
void FileViewer_scroll_down(FileViewer *);
void FileViewer_scroll_up(FileViewer *);
void FileViewer_scroll_item_horizontally(FileViewer *);

#endif
