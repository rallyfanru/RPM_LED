#ifndef SPI_H
#define SPI_H
#include "stm32f0xx.h"

#define LATCH_L()        GPIOA->BSRR = GPIO_BSRR_BR_6
#define LATCH_H()        GPIOA->BSRR = GPIO_BSRR_BS_6

//Для доступа по 8бит
#define SPI1_DR_8b          (*(__IO uint8_t *)((uint32_t)SPI1+0x0C))


void spi_init(void);
void spi_send8(uint8_t data);
void spi_send16(uint16_t data);
void led_send(uint32_t num);


#endif
