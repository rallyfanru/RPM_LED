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
	}
}


