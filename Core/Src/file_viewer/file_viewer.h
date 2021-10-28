#ifndef FILE_VIEWER_H
#define FILE_VIEWER_H

#include <fatfs.h>
#include <ffconf.h>
#include <fonts.h>
#include <stm32_adafruit_lcd.h>

#if _USE_LFN
#define MAX_PATH_CHAR     (5 * (_MAX_LFN + 1))
#define MAX_FILENAME_CHAR (_MAX_LFN + 1)
#else
#define MAX_PATH_CHAR     100
#define MAX_FILENAME_CHAR 13
#endif

#define SELECTOR_TYPE     ">\0"
// maximum characters in path
// initial selector position line
#define SELECTOR_POS_1    2
// selector x coordinate position
#define SELECTOR_X        0
// maximum items on display
// maximum items on display is equal to: (DISP_X_SIZE or DISP_X_SIZE / pFont->Height) - SELECTOR_POS_1
// for ili9486 and font height = 16 it's: (320 / 16) - 2 = 18
#define ITEMS             14
// maximum characters of ITEM name in line
// maximum characters of ITEM name in line is equal to: (DISP_X_SIZE or DISP_X_SIZE / pFont->Width) - 1 (selector char)
// for ili9486 and font width = 14 it's: (480 / 14)  - 1 = 33
#define MAX_LINECHAR      33
// indicator correction, do not modify
#define MAX_ITEM_LINECHAR (MAX_LINECHAR - (MAX_ITEM_INDCHAR + FREE_SPACE_CHAR))
// default disc name, it does not impact on path
#define DRIVE             "DISC:"
// delay macro
#define DELAY_MS(x)       HAL_Delay(x)
// maximum characters of item indicator
#define MAX_ITEM_INDCHAR    7
// free space between item name and indicator
#define FREE_SPACE_CHAR     1

typedef struct {
    FATFS *fs;
    DIR *dir;
    FILINFO *filinfo;
    char path[MAX_PATH_CHAR];
    LCD_DrawPropTypeDef path_display_properties;
    LCD_DrawPropTypeDef items_display_properties;
} FileViewer;

bool FileViewer_init(FileViewer *);
void FileViewer_enter_directory(FileViewer *);
void FileViewer_leave_directory(FileViewer *);
void FileViewer_scroll_down(FileViewer *);
void FileViewer_scroll_up(FileViewer *);
void FileViewer_unwrap_item_name(FileViewer *);
void FileViewer_scroll_page_right(FileViewer *);
void FileViewer_scroll_page_left(FileViewer *);
void FileViewer_get_item_name(FileViewer *, char item_name[MAX_FILENAME_CHAR]);
void FileViewer_refresh_screen(FileViewer *);

#endif
