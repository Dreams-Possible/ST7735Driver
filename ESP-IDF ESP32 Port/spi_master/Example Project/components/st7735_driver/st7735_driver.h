//enable st7735 driver
#if 1
#ifndef ST7735_DRIVER_H
#define ST7735_DRIVER_H

//include
#include<stdio.h>
#include<stdint.h>
#include"driver/gpio.h"
#include"driver/spi_master.h"
#include"freertos/FreeRTOS.h"

//st7735 init all
void st7735_init();
//st7735 flash color
void st7735_flash(int16_t x1,int16_t y1,int16_t x2,int16_t y2,uint16_t*color);

#endif//#ifndef ST7735_DRIVER_H
#endif//#if 1
