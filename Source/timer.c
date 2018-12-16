#include "timer.h"

volatile uint32_t pwm=0;
//TIM2 CH4
//PWM TIMER
//PWM PIN: PA3
//TIM2->CCR2 = 0   0%
//TIM2->CCR2 = 1999  100%
void timer2_init(void){
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
	TIM2->CCR4 = 100;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void timer3_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = (uint16_t)((TIM_CLK)/1000) - 1;
	TIM3->ARR = 1000 - 1;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void)
{

	if(TIM3->SR & TIM_SR_UIF)	TIM3->SR &= ~TIM_SR_UIF;
	pwm+=100;
	if(pwm > 1000) pwm=0;
	TIM2->CCR4 = pwm;
};


/*
void timer3_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = (uint16_t)((TIM6_CLK/2)/1000) - 1;
	TIM3->ARR = 200 - 1; 		//1*2000 1 sec
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM3_IRQn);
}


void TIM3_IRQHandler(void)
{
	uint32_t data=0;

	if(TIM3->SR & TIM_SR_UIF)	TIM3->SR &= ~TIM_SR_UIF;

	tim3_prescaler++;


	switch(mode){
		case 0:
			if((TIM4->CR1 & TIM_CR1_CEN) == 1){
				TIM4->CR1 &= ~TIM_CR1_CEN;
				TIM9->CCR2 = 1999;
			}

		if(tim3_prescaler > 10) tim3_prescaler =0;
			if((tim3_prescaler == 3) || (tim3_prescaler == 7)) {
							if(led_on < 4) {
								led_on++;
								for(uint8_t i=0; i < led_on; i++){
									data|= (1 << color[current_color][i]);
								};
							}else{
								data=0;
								current_color++;
								if(current_color > 2) current_color = 0;
								led_on = 0;
							};
						led_send(data);
				}
				break;

		case 1:
			if((TIM4->CR1 & TIM_CR1_CEN) == 0){
				TIM4->CR1 |= TIM_CR1_CEN;
			}
			data = (uint32_t)((1 << color[current_color][0]) | (1 << color[current_color][1]) | (1 << color[current_color][2]) | (1 << color[current_color][3]));
			led_send(data);
			if(tim3_prescaler == 40) {
				tim3_prescaler=0;
				current_color++;
				if(current_color > 2) current_color = 0;
			};
			break;

		case 2:
			if((TIM4->CR1 & TIM_CR1_CEN) == 1){
				TIM4->CR1 &= ~TIM_CR1_CEN;
				TIM9->CCR2 = 1999;
			}


				if(input_number == 0){
					data=0;
					led_send(data);
				}

				if(input_number == 1){
					data =(uint32_t)((1 << color[RED][0]));
					data|=(uint32_t)((1 << color[GREEN][0]));
					led_send(data);
				}

				if(input_number == 2){
					data=(uint32_t)((1 << color[GREEN][0]) | (1 << color[GREEN][1]));
					led_send(data);
				}

				if(input_number == 3){
					data=(uint32_t)((1 << color[GREEN][0]) | (1 << color[GREEN][1]) | (1 << color[GREEN][2]));
					led_send(data);
				}

				if(input_number == 4){
					data=(uint32_t)((1 << color[RED][0]) | (1 << color[RED][1]) | (1 << color[RED][2]) | (1 << color[RED][3]));
					led_send(data);
				}

				if(input_number > 4){
					if((TIM5->CR1 & TIM_CR1_CEN) == 0)TIM5->CR1 |= TIM_CR1_CEN;
				}else{
					if((TIM5->CR1 & TIM_CR1_CEN) == 1)TIM5->CR1 &= ~TIM_CR1_CEN;
				};



				break;

		default:
			mode=0;
			break;
	};


};



void timer4_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC = (uint16_t)((TIM6_CLK/2)/1000) - 1;
	TIM4->ARR = 5 - 1; 		//1*2000 1 sec
	TIM4->DIER |= TIM_DIER_UIE;
	//TIM4->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void)
{
	uint16_t ccr;

	if(TIM4->SR & TIM_SR_UIF)	TIM4->SR &= ~TIM_SR_UIF;
	ccr=TIM9->CCR2;
	if(up){
		if(ccr > 1990){
			up=0;
		};
		ccr+=3;
	}else{
		if(ccr < 11){
			up=1;
		};
		if(ccr > 10) ccr-=3;

	};

	TIM9->CCR2=ccr;
};



void timer5_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->PSC = (uint16_t)((TIM6_CLK/2)/1000) - 1;
	TIM5->ARR = 200;  								//1*2000 1 ???????.
	TIM5->DIER |= TIM_DIER_UIE;
	//TIM5->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM5_IRQn);
	NVIC_SetPriority (TIM5_IRQn, 5);
}

void TIM5_IRQHandler(void)
{
uint32_t data=0;

	if(TIM5->SR & TIM_SR_UIF)	TIM5->SR &= ~TIM_SR_UIF;
	if(blink == 0){
		blink=1;
		data=0;
	}else{
		data=(uint32_t)((1 << color[RED][0]) | (1 << color[RED][1]) | (1 << color[RED][2]) | (1 << color[RED][3]));
		blink=0;
	};
	led_send(data);

}


//PWM TIMER
//PWM PIN: PE6
//TIM9->CCR2 = 0   0%
//TIM9->CCR2 = 1999  100%
void timer9_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOE->MODER |= GPIO_MODER_MODE6_1;
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;

	//PE6 TIM9 CH2
	RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	GPIOE->AFR[0] |= (3 << GPIO_AFRL_AFSEL6_Pos);
	TIM9->CCER |= TIM_CCER_CC2E;
	TIM9->CCMR1|= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM9->PSC =  (uint16_t)((TIM6_CLK/2)/1000)/1000 - 1;
	TIM9->ARR = 2000 - 1;
	TIM9->CCR2 = 200;
	TIM9->CR1 |= TIM_CR1_CEN;
}
*/
