#include "timer.h"

volatile uint16_t brihtness=0;
volatile uint8_t led_blink=0;

void set_brihtness(uint16_t bri){
	if(bri > 1999) bri = 1999;

	brihtness=bri;
	TIM2->CCR4 = bri;
}



//TIM2 CH4
//PWM TIMER
//PWM PIN: PA3
//TIM2->CCR4 = 0   0%
//TIM2->CCR4 = 1999  100%
void pwm_timer_init(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_3;
	GPIOA->MODER |= GPIO_MODER_MODER3_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR3_1;

	//PA3 TIM2 CH4
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//AF2
	GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL3_Pos);

	TIM2->CCMR2|= TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;

	TIM2->CCER |= TIM_CCER_CC4E;
	TIM2->BDTR |=TIM_BDTR_MOE;

	TIM2->PSC = 48;
	TIM2->ARR = 1000;
	TIM2->CCR4 = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
}

/*
 * Раз в секунду проверяет, есть ли обмен по CAN
 */
void wd_timer_init(void){
       RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
       TIM3->PSC = TIM_CLK/1000 - 1;
       TIM3->ARR = 1000;
       TIM3->DIER |= TIM_DIER_UIE;
       TIM3->CR1 |= TIM_CR1_CEN;
       NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR & TIM_SR_UIF)       TIM3->SR &= ~TIM_SR_UIF;

    if(can_alive == 0) {
       can_send(CAN,(uint32_t)canreq_rpm.ID,(uint8_t *)canreq_rpm.msg);
       rpm=0;
       }
       can_alive=0;
};


void blink_timer_init(void){
       RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
       TIM14->PSC = TIM_CLK/1000 - 1;
       TIM14->ARR = 100;
       TIM14->DIER |= TIM_DIER_UIE;
       NVIC_EnableIRQ(TIM14_IRQn);
}

void TIM14_IRQHandler(void)
{
       if(TIM14->SR & TIM_SR_UIF) TIM14->SR &= ~TIM_SR_UIF;

       if(led_blink){
    	   TIM2->CCR4=brihtness;
    	   led_blink=0;
       }else{
    	   brihtness=TIM2->CCR4;
    	   TIM2->CCR4=0;
    	   led_blink=1;
       }

};

void blink(uint8_t enable){
	if(enable){
		TIM14->CR1 |= TIM_CR1_CEN;
	}else{
		TIM14->CR1 &= ~TIM_CR1_CEN;
		TIM2->CCR4=brihtness;
	}
}
