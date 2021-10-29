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

/* selector type character
 * leave default if you want
 * */
#define SELECTOR_TYPE     ">"
/* maximum characters in path, initial selector position line
 * leave default if you want
 * */
#define SELECTOR_POS_1    2
/* selector x coordinate position
 * leave default if you want
 * */
#define SELECTOR_X        0
/* maximum items on display
 * this need to be calculated and set manually
 * maximum items on display is equal to: (DISP_X_SIZE or DISP_X_SIZE / pFont->Height) - SELECTOR_POS_1
   for ili9486 and font height = 16 it's: (320 / 16) - 2 = 18 */
#define ITEMS             14
/* default disc name, it does not impact on path
 * leave default if you want
 * */
#define DRIVE             "DISC"
/* delay macro
 * this need to be specified and set manually
 * */
#define DELAY_MS(x)       HAL_Delay(x)
/* maximum characters of item indicator
 * leave default if you want*/
#define MAX_ITEM_INDCHAR  7
/* free space between item name and indicator
 * lave default if you want*/
#define FREE_SPACE_CHAR   1

/*FileViewer configuration type
 *
 * */
typedef struct {
    FATFS *fs;
    DIR *dir;
    FILINFO *filinfo;
    char path[MAX_PATH_CHAR];
    LCD_DrawPropTypeDef path_display_properties;
    LCD_DrawPropTypeDef items_display_properties;
} FileViewer;

/* initialization function */
bool FileViewer_init(FileViewer *);
/* open directory and display it's path and items */
void FileViewer_enter_directory(FileViewer *);
/* close actual directory and display previous directory */
void FileViewer_leave_directory(FileViewer *);
/* scroll down inside directory */
void FileViewer_scroll_down(FileViewer *);
/* scroll up inside directory */
void FileViewer_scroll_up(FileViewer *);
/* show full name of item */
void FileViewer_unwrap_item_name(FileViewer *);
/* scroll one page forward inside directory */
void FileViewer_scroll_page_right(FileViewer *);
/* scroll one page backward inside directory*/
void FileViewer_scroll_page_left(FileViewer *);
/* get current item name */
void FileViewer_get_item_name(FileViewer *, char item_name[MAX_FILENAME_CHAR]);
/* refresh view in directory */
void FileViewer_refresh_screen(FileViewer *);

#endif
