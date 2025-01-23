#include"st7735_driver.h"
//enable st7735 driver
#ifdef ST7735_DRIVER_H

//config
#define DC 21
#define RST 47
#define LED 14
#define HOR 160//horizontal pixel
#define HOR_OFF 1//horizontal pixel offset
#define VER 160//vertical pixel
#define VER_OFF 26//vertical pixel offset
#define DIR 0//rotation direction
#define BRG 1//use bgr:0rgb,1bgr
#define INV 1//use inv:0disable,1enable

//example config for specific oem landscape screen
// //config
// #define DC 21
// #define RST 47
// #define LED 14
// #define HOR 160//horizontal pixel
// #define HOR_OFF 0//horizontal pixel offset
// #define VER 160//vertical pixel
// #define VER_OFF 0//vertical pixel offset
// #define DIR 0//rotation direction
// #define BRG 1//use bgr:0rgb,1bgr
// #define INV 0//use inv:0disable,1enable

//example config for specific oem vertical screen
// //config
// #define DC 21
// #define RST 47
// #define LED 14
// #define HOR 160//horizontal pixel
// #define HOR_OFF 0//horizontal pixel offset
// #define VER 160//vertical pixel
// #define VER_OFF 0//vertical pixel offset
// #define DIR 2//rotation direction
// #define BRG 1//use bgr:0rgb,1bgr
// #define INV 0//use inv:0disable,1enable

//define
#define SWRESET 0x01
#define SLPOUT 0x11
#define INVON 0x21
#define INVOFF 0x20
#define COLMOD 0x3a
#define MADCTL 0x36
#define DISPON 0x29

//io set
static void io(uint16_t pin,uint8_t level);
//delay ms
static void delay(uint32_t ms);
//send cmd
static void send_cmd(uint8_t cmd);
//send data
static void send_data(uint8_t data);
//send color
static void send_color(uint16_t*data,uint32_t length);
//init gpio
static void init_gpio();
//init spi
static void init_spi();
//init software
static void init_soft();
//st7735 init all
void st7735_init();
//st7735 flash color
void st7735_flash(int16_t x1,int16_t y1,int16_t x2,int16_t y2,uint16_t*color);

//io set
static void io(uint16_t pin,uint8_t level)
{
    return;
}

//delay ms
static void delay(uint32_t ms)
{
    return;
}

//send cmd
static void send_cmd(uint8_t cmd)
{
    io(DC,0);
    //spi_send(cmd);
    return;
}

//send data
static void send_data(uint8_t data)
{
    io(DC,1);
    //spi_send(data);
    return;
}

//send color
static void send_color(uint16_t*data,uint32_t length)
{
    io(DC,1);
    if(data==NULL||length==0)
    {
        return;
    }
    while(length)
    {
        //spi_send((uint8_t)(*data>>8));
        //spi_send((uint8_t)*data);
        ++data;
        --length;
    }
    return;
}

//init gpio
static void init_gpio()
{
    //dc pin
    //dc_init();
    if(RST!=-1)
    {
        //rst pin
        //rst_init();
    }
    if(LED!=-1)
    {
        //led pin
        //led_init();
    }
    return;
}

//init spi
static void init_spi()
{
    //spi_init();
}

//init software
static void init_soft()
{
    //disable backlight
    if(LED!=-1)
    {
        io(LED,0);
    }
    //hardware reset
    if(RST!=-1)
    {
        io(RST,1);
        io(RST,0);
        io(RST,1);
    }
    delay(200);
    //send init code
    send_cmd(SWRESET);
    delay(200);
    send_cmd(SLPOUT);
    delay(200);
    if(INV)
    {
        send_cmd(INVON);
    }else
    {
        send_cmd(INVOFF);
    }
    send_cmd(COLMOD);
    send_data(0x55);
    send_cmd(MADCTL);
    uint8_t bgr=0;
    if(BRG)
    {
        bgr=0x08;
    }
    switch(DIR)
    {
        case 0:
            send_data(0x80|bgr);
        break;
        case 1:
            send_data(0x40|bgr);
        break;
        case 2:
            send_data(0xe0|bgr);
        break;
        case 3:
            send_data(0x20|bgr);
        break;
        default:
            send_data(0x80|bgr);
        break;
    }
    send_cmd(DISPON);
    delay(200);
    //enable backlight
    if(LED!=-1)
    {
        io(LED,1);
    }
}

//st7735 init all
void st7735_init()
{
    init_gpio();
    init_spi();
    init_soft();
    return;
}

//st7735 flash color
void st7735_flash(int16_t x1,int16_t y1,int16_t x2,int16_t y2,uint16_t*color)
{
    //check
    if(x2<x1||y2<y1||color==NULL)
    {
        return;
    }
    if(x1<0)
    {
        return;
    }
    if(y1<0)
    {
        return;
    }
    if(x2>=HOR)
    {
        return;
    }
    if(y2>=VER)
    {
        return;
    }
    //flash
    uint8_t data=0;
    //x
    send_cmd(0x2a);
    data=(uint8_t)(((uint16_t)x1+HOR_OFF)>>8);
    send_data(data);
    data=(uint8_t)((uint16_t)x1+HOR_OFF);
    send_data(data);
    data=(uint8_t)(((uint16_t)x2+HOR_OFF)>>8);
    send_data(data);
    data=(uint8_t)((uint16_t)x2+HOR_OFF);
    send_data(data);
    //y
    send_cmd(0x2b);
    data=(uint8_t)(((uint16_t)y1+VER_OFF)>>8);
    send_data(data);
    data=(uint8_t)((uint16_t)y1+VER_OFF);
    send_data(data);
    data=(uint8_t)(((uint16_t)y2+VER_OFF)>>8);
    send_data(data);
    data=(uint8_t)((uint16_t)y2+VER_OFF);
    send_data(data);
    //color
    send_cmd(0x2c);
    uint32_t length=(x2-x1+1)*(y2-y1+1);
    send_color(color,length);
}

#endif//#ifdef ST7735_DRIVER_H
