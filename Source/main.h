#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include "stm32f0xx.h"
#include "rcc.h"
#include "spi.h"

/*
 * SPI1
 * PWM: PA3
 * SCK:PA5
 * LATCH:PA6
 * MOSI:PA7
 *
 * CAN1:
 * RX: PA11
 * TX: PA12
 *
 * UART1
 * PA9:TX
 * PA10:RX
 */


#define BLUE 0
#define GREEN 2
#define RED 1

const uint8_t blue_bit[]  = {0, 5, 0x12, 0x13};
const uint8_t red_bit[]   = {2, 3, 0x10, 0x15};
const uint8_t green_bit[] = {1, 4, 0x11, 0x14};

const uint8_t* color[] = {blue_bit,red_bit,green_bit};



#endif

