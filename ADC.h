/*
 * ADC.h
 *
 *  Created on: 18 мая 2025 г.
 *      Author: mrbru
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"

void init_ADC();
uint8_t convert_temp();
void init_GPIO_ADC();
uint16_t get_data_IN0();
uint16_t get_data_IN1();


#endif /* ADC_H_ */
