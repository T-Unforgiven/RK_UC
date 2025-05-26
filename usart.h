/*
 * usart.h
 *
 *  Created on: 22 мая 2025 г.
 *      Author: mrbru
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"
#include "adc.h"

#define start 0xCA
#define YES 1
#define NO 0
#define SIZE 128



struct RingedBuffer{
	uint8_t len;
	uint8_t buffer[SIZE];
	uint8_t begin;
	uint8_t end;
};

void init_Buffer(struct RingedBuffer* buf, uint8_t begin, uint8_t end);
void init_usart();
void init_DMA(struct RingedBuffer* buf);
void conf_GPIO();
void setup_NVIC();
uint8_t read_data();
void write_data(uint8_t data);
void write_command(uint8_t len, char* buf);
void analyze_buffer(struct RingedBuffer* buf, uint8_t* func);
void write_to_end(struct RingedBuffer* buf, uint8_t data);
uint8_t read_from_begin(struct RingedBuffer* buf);
uint8_t transfer_data_DMA(uint8_t tx_c, uint16_t length);

void init_all(struct RingedBuffer* buf);

uint8_t write_data_USART(char data);


#endif /* USART_H_ */
