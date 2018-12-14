#include "main.h"

volatile uint32_t lon=0;

int main(void)
{
	RCC_init();

	LATCH_H();
	for(uint32_t i=0; i<0x1000; i++){};
	LATCH_L();

	spi_init();

	while(1){

		lon=0;
		for(uint8_t i=0; i < 4; i++){
			lon |= (1 << green_bit[i]);
			led_send(lon);
			for(uint32_t j=0; j<0x100000; j++){};
		};

	}

}






/*
	lon=0;
	for(uint8_t i=0; i < 4; i++){
		lon |= (1 << green_bit[i]);
		led_send(lon);
		Delay(200);
	};

	lon=0;
	for(uint8_t i=0; i < 4; i++){
		lon |= (1 << red_bit[i]);
		led_send(lon);
		Delay(200);
	};

	lon=0;
	for(uint8_t i=0; i < 4; i++){
		lon |= (1 << blue_bit[i]);
		led_send(lon);
		Delay(200);
	};


	lon=0;
	for(uint8_t i=0; i < 4; i++){
		lon |= (1 << red_bit[i])|(1 << green_bit[i]);
		led_send(lon);
		Delay(200);
	};
*/














