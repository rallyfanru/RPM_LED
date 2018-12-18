#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include "stm32f0xx.h"
#include "rcc.h"
#include "spi.h"
#include "timer.h"
#include "can.h"

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

const uint8_t red_bit[]   = {16, 19, 22, 29, 0, 3, 6, 9};
const uint8_t green_bit[] = {17, 20, 23, 28, 1, 4, 7, 10};
const uint8_t blue_bit[]  = {18, 21, 30, 27, 2, 5, 8, 11};
const uint8_t* color[] = {blue_bit,red_bit,green_bit};




#endif

