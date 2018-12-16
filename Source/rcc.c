#include "rcc.h"


void RCC_init(void){

	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	while(!(RCC->CR & RCC_CR_HSERDY)){};

	FLASH->ACR |= FLASH_ACR_PRFTBE;
	FLASH->ACR = (FLASH->ACR | 1);

	RCC->CFGR &= ~RCC_CFGR_HPRE;
 	RCC->CFGR &= ~RCC_CFGR_PPRE;

	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMUL));
 	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL6);


	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {};

	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {};

	SystemCoreClockUpdate();
}
