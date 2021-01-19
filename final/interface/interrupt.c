/******************************************************************************
 *
 * Module: interrupt
 *
 * File Name: interrupt.h
 *
 * Created on: Sep 22, 2020
 *
 * Description: Source file for the AVR interrupt driver
 *
 * Author: yasmin mohamed
 *
 *******************************************************************************/
#include "interrupt.h"


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	if(g_callBackPtr != NULL)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required external  interrupt.
 * 	2. Set the required edge detection.
 */
void Interrupt_Init(INT_NUM interrupt_n,clock_type clk_type)
{
	/* TO initialize  interrupt 0 */
	if(interrupt_n==0)
		{
		/* Configure INT0/PD2 as input pin*/
		CLEAR_BIT(DDRD,PD2);
		/* Enable external interrupt pin INT0*/
		SET_BIT(GICR,INT0);
		if(clk_type==falling_edge)
				{
				/* Trigger INT0 with the falling edge*/
				SET_BIT(MCUCR,ISC01);
				CLEAR_BIT(MCUCR,ISC00);
				}

				else if (clk_type==rising_edge)
				{
					/* Trigger INT0 with the rising edge*/
					SET_BIT(MCUCR,ISC01);
					SET_BIT(MCUCR,ISC00);
				}
		}
/****************************************************************/
	/* TO initialize  interrupt 1 */
	else if(interrupt_n==1)
	{
		 /* Configure INT1/PD3 as input pin*/
		CLEAR_BIT(DDRD,PD3);
		 /* Enable external interrupt pin INT1*/
		SET_BIT(GICR,INT1);
		if(clk_type==falling_edge)
		{
		/*Trigger INT1 with the falling edge*/
		SET_BIT(MCUCR,ISC11);
		CLEAR_BIT(MCUCR,ISC10);
		}
		else if (clk_type==rising_edge)
		{
			/*Trigger INT1 with the rising edge*/
			SET_BIT(MCUCR,ISC11);
			SET_BIT(MCUCR,ISC10);
		}
	}
/****************************************************************/
	/* TO initialize  interrupt 2 */
	else if(interrupt_n==2)
		{
			/* Configure INT2/PB2 as input pin*/
			CLEAR_BIT(DDRB,PB2);
			/* Enable external interrupt pin INT2*/
			SET_BIT(GICR,INT2);

			if(clk_type==falling_edge)
				{
					/* Trigger INT2 with the falling edge*/
					CLEAR_BIT(MCUCR,ISC2);
				}

			else if (clk_type==rising_edge)
				{
					/* Trigger INT2 with the RISING edge*/
					SET_BIT(MCUCR,ISC2);
				}
		}
/****************************************************************/
}

/*
 * Description: Function to set the Call Back function address.
 */
void Interrupt_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
