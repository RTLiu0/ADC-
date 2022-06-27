#pragma once
#include"stm32f10x.h"

void My_ADC_Init(void);
void My_ADC_Adjust(void);
void My_ADC_Nvic(void);
float My_ADC_voltage(uint16_t data);
