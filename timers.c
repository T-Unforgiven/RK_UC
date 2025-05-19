/*
 * timers.c
 *
 *  Created on: 19 мая 2025 г.
 *      Author: mrbru
 */

#include "stm32f4xx.h"
#include "timers.h"

void init_tim_GPIO(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//tim3 ch1
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	GPIOA->AFR[0] |= (2 << 4*6);
	//tim3 ch2
	GPIOA->MODER |= GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] |= (2 << 4*7);
	//tim2 ch2
	GPIOA->MODER |= GPIO_MODER_MODER1_1;
	GPIOA->AFR[0] |= (1 << 1*4);
	//tim2 ch3
	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	GPIOA->AFR[0] |= (1 << 2*4);
}

void init_tim3_pwm(){
	//init ch1
	TIM3->ARR = 13440-1;
	TIM3->PSC = 1-1;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->EGR |= TIM_EGR_UG;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCER |= TIM_CCER_CC1P;

	//init ch2
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
	TIM3->CCER |= TIM_CCER_CC2E;
	TIM3->CCER |= TIM_CCER_CC2P;

	//start timer
	TIM3->CR1 |= TIM_CR1_CEN;
}

void init_tim2_pwm(){
	//init ch3
	TIM2->ARR = 13440-1;
	TIM2->PSC = 1-1;
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE;
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CCER |= TIM_CCER_CC3E;
	TIM2->CCER |= TIM_CCER_CC3P;

	//init ch2
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	TIM2->CCER |= TIM_CCER_CC2E;
	TIM2->CCER |= TIM_CCER_CC2P;

	//start timer
	TIM2->CR1 |= TIM_CR1_CEN;
}

void start_timers(){
	init_tim_GPIO();
	init_tim3_pwm();
	init_tim2_pwm();
}
