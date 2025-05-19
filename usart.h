/*
 * usart.h
 *
 *  Created on: 14 мая 2025 г.
 *      Author: mrbru
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"

#define ERROR -1
#define OK 0
#define COMMAND_UNKNOWN "invalid "
#define COMMAND_ACCEPTED "OK "
#define DONE "command done "

void init_USART();
void conf_GPIO();
uint8_t read_data_USART();
uint8_t write_data_USART(char data);
void update_buf(uint8_t* buf, uint8_t* pos);
void update_read_pos(uint8_t* pos);
void analyze_command(uint8_t* buf, uint8_t* pos1, uint8_t* pos2, uint8_t* func);



#endif /* USART_H_ */
