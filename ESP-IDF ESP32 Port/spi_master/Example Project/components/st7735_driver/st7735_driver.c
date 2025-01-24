#include"st7735_driver.h"
//enable st7735 driver
#ifdef ST7735_DRIVER_H

//config
#define DC 21
#define RST 47//no use as -1
#define LED 14//no use as -1
#define HOR 160//horizontal pixel
#define HOR_OFF 1//horizontal pixel offset
#define VER 80//vertical pixel
#define VER_OFF 26//vertical pixel offset
#define DIR 3//rotation direction
#define BRG 1//use bgr:0rgb,1bgr
#define INV 1//use inv:0disable,1enable
#define SPI SPI2_HOST
#define RATE 40*1000*1000
#define CS 10

//example config for specific oem landscape screen
// //config
// #define DC 21
// #define RST 47//no use as -1
// #define LED 14//no use as -1
// #define HOR 80//horizontal pixel
// #define HOR_OFF 26//horizontal pixel offset
// #define VER 160//vertical pixel
// #define VER_OFF 1//vertical pixel offset
// #define DIR 1//rotation direction
// #define BRG 1//use bgr:0rgb,1bgr
// #define INV 1//use inv:0disable,1enable

//example config for specific oem vertical screen
// //config
// #define DC 21
// #define RST 47//no use as -1
// #define LED 14//no use as -1
// #define HOR 160//horizontal pixel
// #define HOR_OFF 1//horizontal pixel offset
// #define VER 80//vertical pixel
// #define VER_OFF 26//vertical pixel offset
// #define DIR 3//rotation direction
// #define BRG 1//use bgr:0rgb,1bgr
// #define INV 1//use inv:0disable,1enable

//define
#define SWRESET 0x01
#define SLPOUT 0x11
#define INVON 0x21
#define INVOFF 0x20
#define COLMOD 0x3a
#define MADCTL 0x36
#define DISPON 0x29
#define GMCTRP1 0xe0
#define GMCTRN1 0xe1

//static data
typedef struct static_data_t
{
    spi_device_handle_t device;
}static_data_t;
static static_data_t static_data={0};

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
    gpio_set_level(pin,level);
    return;
}

//delay ms
static void delay(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
    return;
}

//send cmd
static void send_cmd(uint8_t cmd)
{
    io(DC,0);
    spi_transaction_t trans={0};
    trans.length=8,
    trans.tx_buffer=&cmd,
    spi_device_transmit(static_data.device,&trans);
    return;
}

//send data
static void send_data(uint8_t data)
{
    io(DC,1);
    spi_transaction_t trans={0};
    trans.length=8,
    trans.tx_buffer=&data,
    spi_device_transmit(static_data.device,&trans);
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
    for(uint32_t f=0;f<length;++f)
    {
        data[f]=SPI_SWAP_DATA_TX(data[f],16);
    }
    spi_transaction_t transaction={0};
    while (length>0)
    {
        if(length>=256)
        {
            transaction.length=4096;
            transaction.tx_buffer=data;
            spi_device_transmit(static_data.device,&transaction);
            data+=256;
            length-=256;
        }
        else
        {
            transaction.length=length*16;
            transaction.tx_buffer=data;
            spi_device_transmit(static_data.device,&transaction);
            length=0;
        }
    }
}

//init gpio
static void init_gpio()
{
    //gpio init
    gpio_config_t config={0};
    //dc pin
    config.intr_type=GPIO_INTR_DISABLE;
    config.mode=GPIO_MODE_OUTPUT;
    config.pin_bit_mask=(1ULL<<DC);
    config.pull_down_en=0;
    config.pull_up_en=0;
    gpio_config(&config);
    #if RST!=-1
        //rst pin
        config.intr_type=GPIO_INTR_DISABLE,
        config.mode=GPIO_MODE_OUTPUT,
        config.pin_bit_mask=(1ULL<<RST),
        config.pull_down_en=0,
        config.pull_up_en=0,
        gpio_config(&config);
    #endif//#if RST!=-1
    #if LED!=-1
        //led pin
        config.intr_type=GPIO_INTR_DISABLE;
        config.mode=GPIO_MODE_OUTPUT;
        config.pin_bit_mask=(1ULL<<LED);
        config.pull_down_en=0;
        config.pull_up_en=0;
        gpio_config(&config);
    #endif//#if LED!=-1
    return;
}

//init spi
static void init_spi()
{
    //add spi device
    spi_device_interface_config_t config={0};
    config.mode=0;
    config.spics_io_num=CS;
    config.queue_size=1;
    config.clock_speed_hz=RATE;
    spi_bus_add_device(SPI,&config,&static_data.device);
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
        delay(200);
    }
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
            send_data(0xc0|bgr);
        break;
        case 1:
            send_data(0x00|bgr);
        break;
        case 2:
            send_data(0xa0|bgr);
        break;
        case 3:
            send_data(0x60|bgr);
        break;
        default:
            send_data(0xc0|bgr);
        break;
    }
    send_cmd(GMCTRP1);
    send_data(0x09);
    send_data(0x16);
    send_data(0x09);
    send_data(0x20);
    send_data(0x21);
    send_data(0x1B);
    send_data(0x13);
    send_data(0x19);
    send_data(0x17);
    send_data(0x15);
    send_data(0x1E);
    send_data(0x2B);
    send_data(0x04);
    send_data(0x05);
    send_data(0x02);
    send_data(0x0E);
    send_cmd(GMCTRN1);
    send_data(0x0B);
    send_data(0x14);
    send_data(0x08);
    send_data(0x1E);
    send_data(0x22);
    send_data(0x1D);
    send_data(0x18);
    send_data(0x1E);
    send_data(0x1B);
    send_data(0x1A);
    send_data(0x24);
    send_data(0x2B);
    send_data(0x06);
    send_data(0x06);
    send_data(0x02);
    send_data(0x0F);
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
