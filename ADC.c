/*
 * ADC.c
 *
 *  Created on: 18 мая 2025 г.
 *      Author: mrbru
 */


#include "ADC.h"

uint16_t data;
uint8_t temp;

void init_ADC(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->SQR3 |= 16;
	ADC1->SMPR1 |= 1<<18|1<<19|1<<20; //max sampling rate
	ADC->CCR |= ADC_CCR_TSVREFE;
	ADC->CCR &= ~ADC_CCR_VBATE;
	ADC1->CR2 |= ADC_CR2_CONT;
}

uint8_t convert_temp(){
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	data = ADC1->DR;
	temp = (((double)data/4095*3300-760.0)/2.5) + 25;
	return temp;
}

void init_GPIO_ADC(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER0 | GPIO_MODER_MODER1;
}

uint16_t get_data_IN0(){
	ADC1->SQR3 &= ~16;
	ADC1->SQR3 |= 0;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	uint16_t data = ADC1->DR;
	return data;
}

uint16_t get_data_IN1(){
	ADC1->SQR3 &= ~16;
	ADC1->SQR3 |= 1;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	uint16_t data = ADC1->DR;
	return data;
}
