#include "main.h"


int main(void)
{
	RCC_init();
	pwm_timer_init();
	spi_init();
	set_brihtness(DEFAULT_BRI);

	led_send((uint32_t)(1 << green_bit[1])|(1 << green_bit[2])|(1 << green_bit[3])|(1 << green_bit[4]));
	can_init();

	while(1){
		led_send(set_leds_by_rpm(rpm));
	}
}


uint32_t set_leds_by_rpm(uint16_t rpm){
	uint32_t leds=0;
	uint8_t color=0;

	color=RED;
	if(rpm < 3000) color=GREEN;
	if(rpm < 1500) color=BLUE;

	if(rpm > 4000) rpm=4000;

	for(uint8_t i=0; i< rpm/500; i++){
		leds |= (1 << led_color[color][i]);
	}

	return leds;

};
