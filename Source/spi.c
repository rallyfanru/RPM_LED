#include "spi.h"

//Чтобы не дергать SPI одними и теми же значениями, сохранение предыдущего вывода
volatile uint32_t saved_num=0;


void led_send(uint32_t num){
	if(num != saved_num){
		LATCH_H();
		LATCH_L();
		spi_send16((uint16_t)(num & 0xFFFF));
		spi_send16((uint16_t)((num >> 16) & 0xFFFF));
		while((SPI1->SR & SPI_SR_BSY) > 0){};
		LATCH_H();
		LATCH_L();
		saved_num=num;
	}

};


void spi_send8(uint8_t data){
		 while((SPI1->SR & SPI_SR_TXE) == 0){};
		 SPI1_DR_8b = data;
};

void spi_send16(uint16_t data){
		 while((SPI1->SR & SPI_SR_TXE) == 0){};
		 SPI1->DR = data;
};


void spi_init(void){

	LATCH_H();
	LATCH_L();

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	//GPIO
	//MOSI PA7
	GPIOA->MODER |= GPIO_MODER_MODER7_1; //Alternate
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_0|GPIO_OSPEEDER_OSPEEDR7_1; //High
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL7;  //AF0

	//SCK  PA5
	GPIOA->MODER |= GPIO_MODER_MODER5_1; //Alternate
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0|GPIO_OSPEEDER_OSPEEDR5_1; //High
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5;  //AF0

	//Latch PA6
	GPIOA->MODER |= GPIO_MODER_MODER6_0; //Out
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6; //Push-pull
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_0|GPIO_OSPEEDER_OSPEEDR6_1; //High
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_1; //Pull-down

	//PWM PA3
	//Если нет таймера, инициализация с подтяжкой к земле, сигнал OE
	//GPIOA->MODER |= GPIO_MODER_MODER3_0; //Out
	//GPIOA->OTYPER &= ~GPIO_OTYPER_OT_3; //Push-pull
	//GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_0|GPIO_OSPEEDER_OSPEEDR3_1; //High
	//GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_1; //Pull-down

	//SPI
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
	SPI1->CR1 &= ~SPI_CR1_CRCEN;

	SPI1->CR1 |= SPI_CR1_SSI|SPI_CR1_SSM;
	SPI1->CR1 &= ~SPI_CR1_CPHA;
	SPI1->CR1 &= ~SPI_CR1_CPOL;
	SPI1->CR1 &= ~SPI_CR1_BR;
	SPI1->CR1 |= SPI_CR1_BR_0;  //111 - /32
	SPI1->CR1 |= SPI_CR1_MSTR;

	SPI1->CR2 |= (SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3);  //16bit

	SPI1->CR1 |= SPI_CR1_SPE;
}

