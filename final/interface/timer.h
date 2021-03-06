/*
 * timer.h
 *
 *  Created on: Oct 16, 2020
 *      Author: yasmin mohamed
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum{
	normal,compare ,SquareWave
}TIMER_mode;

typedef enum{
	timer0,timer1 ,timer2
}TIMER_num;

typedef enum{
	Prescaler_1 = 1 , Prescaler_8 = 2 , Prescaler_64 = 3 , Prescaler_256 = 4 , Prescaler_1024 = 5
}TIMER_prescaler;


typedef struct{
	TIMER_num TIMER_num;
	TIMER_mode TIMER_mode;
	uint8 channel;
	TIMER_prescaler TIMER_prescaler;
	uint16 initial_value;
	uint16 TIMER_compareValue_a;
	uint16 TIMER_compareValue_b;
}TIMER_data;

void timer0_init_normal_mode(uint8 initial_value,uint8 prescaler);
void timer0_init_CTC_mode(uint8 compare_value,uint8 initial_value,uint8 prescaler);
void Timer0_CTC_Square_Wave_Init(uint8 initial_value,uint8 prescaler);
void Timer1_init_Normal_Mode(uint8 initial_value,uint8 prescaler);
void Timer1_init_compare_Mode(uint8 compare_value_a,uint8 compare_value_b,uint8 initial_value,uint8 channel,uint8 prescaler);

//ISR(TIMER0_OVF_vect);
ISR(TIMER0_COMP_vect);

ISR(TIMER1_OVF_vect);
ISR(TIMER1_COMPA_vect);
ISR (TIMER1_COMPB_vect);

void timer_init(const TIMER_data *TIMER_DATA_Ptr);
void TIMER0_stop (void);
void TIMER1_stop(void);

void TIMER1_callback (void(*a_ptr)(void));
void TIMER0_callback (void(*a_ptr)(void));

#endif /* TIMER_H_ */
