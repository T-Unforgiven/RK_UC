/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.3.0   2025-05-14

The MIT License (MIT)
Copyright (c) 2019 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "usart.h"
#include "ADC.h"
#include "timers.h"

struct RingedBuffer input_buf;
struct RingedBuffer output_buf;
uint8_t tx_complete = 0;
uint8_t func = 0;



void init_diodes(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;
}

void init_keys(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0;
}

void init_EXTI(){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	EXTI->FTSR |= EXTI_FTSR_TR3;
	EXTI->IMR |= EXTI_IMR_MR3;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PE;
	NVIC_SetPriority(EXTI3_IRQn, 3);
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI3_IRQHandler(){
	EXTI->PR |= EXTI_PR_PR3;
	if(func == 1){
		GPIOA->ODR ^= GPIO_ODR_ODR_6;
	}
	if(func == 2){
		GPIOA->ODR ^= GPIO_ODR_ODR_7;
	}
}

void USART1_IRQHandler(){
	if(USART1->SR & USART_SR_RXNE){
		write_to_end(&input_buf, read_data());
	}
//	if(USART1->SR & USART_SR_TXE){
//		write_data(read_from_begin(&output_buf));
//	}
}

void DMA2_Stream7_IRQHandler(){
	 if (DMA2->HISR & DMA_HISR_TCIF7) {
	     DMA2->HIFCR = DMA_HIFCR_CTCIF7;
	     tx_complete = 1;
	 }
}

void test(struct RingedBuffer* buf){
	for(uint8_t i = 0; i < 4; i++){
		write_to_end(buf, i);
	}
}

int main(void)
{
	init_diodes();
	init_keys();
	init_EXTI();
	init_Buffer(&input_buf, 0, 0);
	init_Buffer(&output_buf, 0, 0);
	test(&output_buf);
	init_all(&output_buf);
	init_ADC();
	//start_timers();
	//init_GPIO_ADC();
	while (1)
	{
		analyze_buffer(&input_buf, &func);
	}
}
