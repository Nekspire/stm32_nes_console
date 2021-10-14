/* Orientation
   - 0: 240x320 portrait 0'
   - 1: 320x240 landscape 90'
   - 2: 240x320 portrait 180'
   - 3: 320x240 landscape 270'
*/
#define  ILI9486_ORIENTATION      3

/* Color mode
   - 0: RGB565 (R:bit15..11, G:bit10..5, B:bit4..0)
   - 1: BRG565 (B:bit15..11, G:bit10..5, R:bit4..0)
*/
#define  ILI9486_COLORMODE        0

/* Analog touchscreen
   - 0: touchscreen disabled
   - 1: touchscreen enabled
*/
#define  ILI9486_TOUCH            0

/* Touchscreen calibration data for 4 orientations */
#define  TS_CINDEX_0        {126292, -10985, -3832, 54344497, -99932, 228645, -478659969}
#define  TS_CINDEX_1        {126292, -99932, 228645, -478659969, 10985, 3832, -14057190}
#define  TS_CINDEX_2        {126292, 10985, 3832, -14057190, 99932, -228645, 539154076}
#define  TS_CINDEX_3        {126292, 99932, -228645, 539154076, -10985, -3832, 54344497}

/* For multi-threaded or interrupt use, Lcd and Touchscreen simultaneous use can cause confusion (since it uses common I/O resources)
   If enabled, the Lcd functions wait until the touchscreen functions are run. The touchscreen query is not executed when Lcd is busy.
   - 0: multi-threaded protection disabled
   - 1: multi-threaded protection enabled
*/
#define  ILI9486_MULTITASK_MUTEX   0

//-----------------------------------------------------------------------------
// ILI9486 physic resolution (in 0 orientation)
#define  ILI9486_LCD_PIXEL_WIDTH  320
#define  ILI9486_LCD_PIXEL_HEIGHT 480
