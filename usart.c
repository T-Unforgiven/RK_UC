/*
 * usart.c
 *
 *  Created on: 14 мая 2025 г.
 *      Author: mrbru
 */

#include "usart.h"
#include "ADC.h"

void init_USART(){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR = (26880000/115200); // set 115200 baudrate
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
}

void conf_GPIO(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->AFR[1] |= (7<<(1*4));
	GPIOA->AFR[1] |= (7<<(2*4));
}

uint8_t read_data_USART(){
	uint8_t data;
	while(!(USART1->SR & USART_SR_RXNE));
	data = USART1->DR;
	return data;
}

uint8_t write_data_USART(char data){
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = data;
	return OK;
}

uint8_t write_string_USART(char* data){
	while(*data){
		write_data_USART(*data++);
	}
	return OK;
}

void update_buf(uint8_t* buf, uint8_t* pos){
	buf[*pos++] = read_data_USART();
	if(*pos == 32)
		*pos = 0;
}

void update_read_pos(uint8_t* pos){
	*pos++;
	if(*pos == 32)
		*pos = 0;
}

void analyze_command(uint8_t* buf, uint8_t* pos1, uint8_t* pos2, uint8_t* func){
	if(buf[*pos2] == 'k'){
		write_string_USART(COMMAND_ACCEPTED);
		update_buf(buf, pos1);
		update_read_pos(pos2);
		if(buf[*pos2] == '1'){
			write_string_USART(DONE);
			*func = 1;
			return;
		}
		else if(buf[*pos2] == '2'){
			write_string_USART(DONE);
			*func = 2;
			return;
		}
		else{
			write_string_USART(COMMAND_UNKNOWN);
			return;
		}
	}
	if(buf[*pos2] == 't'){
		write_data_USART(convert_temp());
		return;
	}
	if(buf[*pos2] == 'a'){
		write_string_USART(COMMAND_ACCEPTED);
		update_buf(buf, pos1);
		update_read_pos(pos2);
		if(buf[*pos2] == '0'){
			write_data_USART(get_data_IN0());
			return;
		}
		else if(buf[*pos2] == '1'){
			write_data_USART(get_data_IN1());
			return;
		}
	}
	write_string_USART(COMMAND_UNKNOWN);
	update_read_pos(pos2);
	return;
}




