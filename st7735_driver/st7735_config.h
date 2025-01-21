#include"st7735_driver.h"
//enable st7735 driver
#ifdef ST7735_DRIVER_H

#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT 0x11 
#define ST7735_FRMCTR1 0xb1
#define ST7735_FRMCTR2 0xb2
#define ST7735_FRMCTR3 0xb3
#define ST7735_INVCTR 0xb4
#define ST7735_PWCTR1 0xc0
#define ST7735_PWCTR2 0xc1
#define ST7735_PWCTR3 0xc2
#define ST7735_PWCTR4 0xc3
#define ST7735_PWCTR5 0xc4
#define ST7735_VMCTR1 0xc5
#define ST7735_INVON 0x21
#define ST7735_INVOFF 0x20
#define ST7735_COLMOD 0x3a
#define ST7735_MADCTL 0x36
#define ST7735_GMCTRP1 0xe0
#define ST7735_GMCTRN1 0xe1
#define ST7735_NORON 0x13
#define ST7735_DISPON 0x29

#endif//#ifdef SIMULATE_SPI_H

//example config for specific oem landscape screen
// //config
// #define SPI HSPI_HOST
// #define RATE 20*1000*1000
// #define CS 15
// #define DC 32
// #define RST 33//no use as -1
// #define LED 16//no use as -1
// #define HOR 160//horizontal pixel
// #define HOR_OFF 1//horizontal pixel offset
// #define VER 80//vertical pixel
// #define VER_OFF 26//vertical pixel offset
// #define DIR 3//rotation direction
// #define BRG 1//use bgr:0rgb,1bgr
// #define INV 1//use inv:0disable,1enable


//example config for specific oem vertical screen
// //config
// #define SPI HSPI_HOST
// #define RATE 20*1000*1000
// #define CS 15
// #define DC 32
// #define RST 33//no use as -1
// #define LED 16//no use as -1
// #define HOR 80//horizontal pixel
// #define HOR_OFF 26//horizontal pixel offset
// #define VER 160//vertical pixel
// #define VER_OFF 1//vertical pixel offset
// #define DIR 1//rotation direction
// #define BRG 1//use bgr:0rgb,1bgr
// #define INV 1//use inv:0disable,1enable



