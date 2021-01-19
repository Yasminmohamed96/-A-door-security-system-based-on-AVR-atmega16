/******************************************************************************
 *
 * Module: interrupt
 *
 * Created on: Sep 22, 2020
 *
 * File Name: interrupt.h
 *
 * Description: Source file for the AVR interrupt driver
 *
 * Author:yasmin mohamed
 *
 *******************************************************************************/
#ifndef INTERRUPT_H_
#define INTERRUPT_H_


#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum  { INTERRUPT0  ,INTERRUPT1  , INTERRUPT2 }INT_NUM;
typedef enum clock_type {falling_edge ,rising_edge }clock_type;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required external  interrupt.
 * 	2. Set the required edge detection.
 */
void Interrupt_Init(INT_NUM interrupt_n,clock_type clk_type);


/* External INT1 Interrupt Service Routine */
ISR(INT1_vect);

/*
 * Description: Function to set the Call Back function address.
 */
void Interrupt_setCallBack(void(*a_ptr)(void));


#endif /* INTERRUPT_H_ */
