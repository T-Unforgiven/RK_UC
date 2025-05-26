/*
 * usart.c
 *
 *  Created on: 22 мая 2025 г.
 *      Author: mrbru
 */

#include "usart.h"

uint8_t command_started = NO;
uint8_t command_length = 0;

void init_Buffer(struct RingedBuffer* buf, uint8_t b, uint8_t e){
	buf->begin = b;
	buf->end = e;
	buf->len = SIZE;
}

void init_usart(){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR = (26880000/115200); // set 115200 baudrate
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	//USART1->CR1 |= USART_CR1_TXEIE;
	USART1->CR1 |= USART_CR1_RXNEIE;
	USART1->CR3 |= USART_CR3_DMAT;
	USART1->CR1 |= USART_CR1_UE;
}

void conf_GPIO(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->AFR[1] |= (7<<(1*4));
	GPIOA->AFR[1] |= (7<<(2*4));
}

void setup_NVIC(){
	NVIC_SetPriority(USART1_IRQn, 1);
	NVIC_EnableIRQ(USART1_IRQn);
}

uint8_t read_data(){
	uint8_t data;
	data = USART1->DR;
	return data;
}

void write_data(uint8_t data){
	USART1->DR = data;
}

void write_command(uint8_t len, char* buf){
	uint8_t i = 0;
	while(i < len){
		write_data(buf[i]);
		i++;
	}
}

uint8_t write_data_USART(char data){
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = data;
	return 1;
}

void analyze_buffer(struct RingedBuffer* buf, uint8_t* func){
	uint8_t data = read_from_begin(buf);
	uint8_t ch_num;
	if((data == start) && !command_started){
		command_started = YES;
		buf->begin++;
	}
	if(command_started && (buf->begin != buf->end) && (command_length == 0)){
		command_length = data;
		buf->begin++;
	}
	if(command_started && (buf->end != ((buf->begin + (command_length - 1))%buf->len)) && (command_length != 0)){
			return;
		}
	if(command_started && (buf->end == ((buf->begin + (command_length - 1))%buf->len)) && (command_length != 0)){
		switch(data){
		case 0xAA:
			buf->begin++;
			ch_num = read_from_begin(buf);
			*func = ch_num;
			buf->begin++;
			break;
		case 0x99:
//			buf->begin++;
//			ch_num = read_from_begin(buf);
//			disable_channel(ch_num);
//			buf->begin++;
			break;
		case 0x87:
			//func
			break;
		case 0xED:
			//func
			break;
		case 0xCC:
			//func
			break;
		default:
			return;
		}
	}
}

void write_to_end(struct RingedBuffer* buf, uint8_t data){
	buf->buffer[buf->end] = data;
	buf->end++;
	if(buf->end == buf->len){
		buf->end = 0;
	}
}

uint8_t read_from_begin(struct RingedBuffer* buf){
	uint8_t data;
	if(buf->begin == buf->len){
			buf->begin = 0;
		}
	data = buf->buffer[buf->begin];
	return data;
}

void init_DMA(struct RingedBuffer* buf){
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream7->CR &= ~DMA_SxCR_EN;
	while(DMA2_Stream7->CR & DMA_SxCR_EN);
	DMA2_Stream7->CR = DMA_SxCR_CHSEL_2 |
	                      DMA_SxCR_MINC |
	                      DMA_SxCR_DIR_0 |
	                      DMA_SxCR_TCIE;

	DMA2_Stream7->PAR = (uint32_t)&(USART1->DR);
	DMA2_Stream7->M0AR = (uint32_t)buf->buffer;
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	NVIC_SetPriority(DMA2_Stream7_IRQn, 1);
}

uint8_t transfer_data_DMA(uint8_t tx_c, uint16_t length){
	if(tx_c){
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;
		while(DMA2_Stream7->CR & DMA_SxCR_EN);
		DMA2_Stream7->NDTR = length;
		DMA2_Stream7->CR |= DMA_SxCR_EN;
		return 0;
	}
	return 1;
}

void init_all(struct RingedBuffer* buf){
	conf_GPIO();
	init_usart();
	setup_NVIC();
	init_DMA(buf);
}
