#ifndef FILE_VIEWER_H
#define FILE_VIEWER_H

#include <fatfs.h>
#include <fonts.h>
#include <stm32_adafruit_lcd.h>

// maximum characters in path
#define PATHSIZE        30
// initial selector position line
#define SELECTOR_POS_1  2
// selector x coordinate position
#define SELECTOR_X      0
// item x coordinate x position
#define RECORD_X        (Font16.Width + 1)
// maximum items on display
#define ITEMS           18
// default disc name, it does not impact on path
#define DRIVE           "DISC:"

typedef struct {
    FATFS *fs;
    DIR *dir;
    FILINFO *filinfo;
    char path[PATHSIZE];
    LCD_DrawPropTypeDef display_properties;
} FileViewer;

bool FileViewer_init(FileViewer *);
void FileViewer_enter_directory(FileViewer *);
void FileViewer_leave_directory(FileViewer *);
void FileViewer_scroll_down(FileViewer *);
void FileViewer_scroll_up(FileViewer *);

#endif
