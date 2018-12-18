#ifndef TIMER_H
#define TIMER_H
#include "stm32f0xx.h"
#include "can.h"

#define TIM_CLK 48000000U

#define DEFAULT_BRI 100

void set_brihtness(uint16_t bri);
void pwm_timer_init(void);
void wd_timer_init(void);

#endif
