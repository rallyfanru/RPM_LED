#include "main.h"

volatile uint32_t lon=0;
static uint8_t canmsg[]={1,2,3,4,5,6,7,8};

int main(void)
{

	RCC_init();
	timer2_init();
	timer3_init();

	can_init();

	LATCH_H();
	delay();
	LATCH_L();
	spi_init();

	while(1){

	for(uint8_t k=0; k<3; k++){
		lon=0;
		for(uint8_t i=0; i < 8; i++){
			lon |= (1 << color[k][i]);
			led_send(lon);
			delay();
			can_send(CAN,0x100,canmsg);
		};
		};
		};
}


void delay(void){
	for(uint32_t j=0; j<0x100000; j++){};
}

