#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

#include"spi_driver.h"
#include"st7735_driver.h"

void app_main(void)
{
    //spi init
    spi_init();

    //st7735 init all
    st7735_init();

    //simple test
    uint16_t*data=(uint16_t*)malloc(160*80*sizeof(uint16_t));
    uint16_t color=0;
    for(uint32_t i=0;i<160*80;++i)
    {
        data[i]=color;
        ++color;
    }

    //st7735 flash color
    st7735_flash(0,0,160-1,80-1,data);
}
