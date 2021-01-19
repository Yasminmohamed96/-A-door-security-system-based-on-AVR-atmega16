/*
 * timer.c
 *
 *  Created on: Oct 16, 2020
 *      Author: yasmin mohamed
 */





#include "timer.h"

/* Description :
 * For clock=1Mhz and prescaler F_CPU/1024 every count will take 1ms
 * so put initial timer counter = 0  0 --> 255 (256ms per overflow)
 * so we need timer to overflow 2 times to get a 0.5 second
 *
 */

static volatile void (*g_callBackPtr0)(void) = NULL;
static volatile void (*g_callBackPtr1)(void) = NULL;


 uint8 flag=0;
 uint8 flag2=0;
 //uint8 tick=0;
//////////////////////////////////////////////////////
 ////////////////////////////////////////

/*
 ISR(TIMER0_OVF_vect)
 {
	 tick++;
	 if(tick==305)
	 {
	 if(g_callBackPtr0 != NULL)
	  		 		{
		 	 	 	 	 tick=0;
	  		 			/* Call the Call Back function in the application after the edge is detected */
	  		 			//(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	  		 		//}
	 //}
 //}
 /////////////////////////////////////////////////////////////////
 ISR(TIMER0_COMP_vect)
 {
	 if(g_callBackPtr0 != NULL)
	  		 		{
	  		 			/* Call the Call Back function in the application after the edge is detected */
	  		 			(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	  		 		}
 }
/////////////////////////////////////////////////////////////////
 ISR(TIMER1_OVF_vect)
 {
	 if(g_callBackPtr1 != NULL)
	  		 		{
	  		 			/* Call the Call Back function in the application after the edge is detected */
	  		 			(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	  		 		}
 }

///////////////////////////////////////////
ISR(TIMER1_COMPA_vect)
 {
 	flag++;
 	 if(flag==7)
 	 {
 		 if(g_callBackPtr1 != NULL)
 		 		{
 		 			/* Call the Call Back function in the application after the edge is detected */
 		 			(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
 		 		}
 	 }
 }


 //////////////////////////////////////////
 ISR (TIMER1_COMPB_vect)
 {flag2++;
	 if(flag2==2)
	 {
	  		 if(g_callBackPtr1 != NULL)
	  		 		{
	  		 			/* Call the Call Back function in the application after the edge is detected */
	  		 			(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	  		 		}
	 }
 }

 ////////////////////////////////////////




 //////////////////////////////////////////////////
void timer_init (const TIMER_data *TIMER_DATA_Ptr)
{
	SREG  |= (1<<7);
if ( TIMER_DATA_Ptr->TIMER_num==timer0)
{
	if( TIMER_DATA_Ptr->TIMER_mode==compare)
	{
		timer0_init_CTC_mode( TIMER_DATA_Ptr->TIMER_compareValue_a, TIMER_DATA_Ptr->initial_value, TIMER_DATA_Ptr->TIMER_prescaler);
	}
	else if ( TIMER_DATA_Ptr->TIMER_mode==normal)
	{
		timer0_init_normal_mode( TIMER_DATA_Ptr->initial_value,TIMER_DATA_Ptr->TIMER_prescaler);
	}
	else if( TIMER_DATA_Ptr->TIMER_mode==SquareWave)
		{
		Timer0_CTC_Square_Wave_Init( TIMER_DATA_Ptr->initial_value,TIMER_DATA_Ptr->TIMER_prescaler);
		}
}
////////////////////////////////////////////////////////////
	else if ( TIMER_DATA_Ptr->TIMER_num==timer1)
	{
		if( TIMER_DATA_Ptr->TIMER_mode==compare)
		{
			Timer1_init_compare_Mode( TIMER_DATA_Ptr->TIMER_compareValue_a, TIMER_DATA_Ptr->TIMER_compareValue_b, TIMER_DATA_Ptr->initial_value, TIMER_DATA_Ptr->channel, TIMER_DATA_Ptr->TIMER_prescaler);

		}
		else if( TIMER_DATA_Ptr->TIMER_mode==normal)
		{
			Timer1_init_Normal_Mode( TIMER_DATA_Ptr->initial_value,TIMER_DATA_Ptr->TIMER_prescaler);
		}

	}

//prescaler clock
//oca ocb double compare buffer Two Independent Output Compare Units
// Double Buffered Output Compare Registers
//Four Independent Interrupt Sources (TOV1, OCF1A, OCF1B, and ICF1)
//to the value stored in the OCR1A or ICR1 register.

}

void timer0_init_normal_mode(uint8 initial_value,uint8 prescaler)
{
	TCNT0 = initial_value; //Set Timer initial value to 0
	TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
	TCCR0 = ((TCCR0 & 0xF8) | (prescaler & 0x7));
}



void timer0_init_CTC_mode(uint8 compare_value,uint8 initial_value,uint8 prescaler)
{
	TCNT0 = initial_value;    // Set Timer initial value to 0
	OCR0  = compare_value; // Set Compare Value
	TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
	/* Configure timer0 control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0
	 * 3. No need for OC0 in this example so COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	//TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS02) | (1<<CS00);
	TCCR0 = ((TCCR0 & 0xF8) | (prescaler & 0x7));
}


void Timer0_CTC_Square_Wave_Init(uint8 initial_value,uint8 prescaler)
{
	TCNT0 = initial_value;  // Set Initial Value to 0
	OCR0 = 250; // Set Compare Value
	DDRB = DDRB | (1<<PB3); // Configure PB3/OC0 Pin as output pin
	/* Configure timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0
	 * 3. Toggle OC0 on compare match COM00=1 & COM01=0
	 * 4. clock = CPU clock CS00=1 CS01=0 CS02=0
	 */
	//TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<COM00) | (1<<CS00);
	TCCR0 = ((TCCR0 & 0xF8) | (prescaler & 0x7));


}

void Timer1_init_Normal_Mode(uint8 initial_value,uint8 prescaler)
{
	TCNT0 = initial_value; // Set Timer initial value to 0
	TIMSK = (1<<TOIE0); // Enable Timer0 Overflow Interrupt
	/* configure the timer
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 * 4. clock = F_CPU/256 CS00=0 CS01=0 CS02=1
	 */
	//TCCR0 = (1<<FOC0) | (1<<CS02);
	TCCR0 = ((TCCR0 & 0xF8) | (prescaler & 0x7));
}
void Timer1_init_compare_Mode(uint8 compare_value_a,uint8 compare_value_b,uint8 initial_value,uint8 channel,uint8 prescaler)
{
	/*TCCR1A |=(1<<COM1A0)|(1<<COM1B0)|(1<<FOC1A)|(1<<FOC1B); //NOT PWM ,CHANNAL ONE
	TCCR1B |= (1<<CS10)|(1<<CS12)|(1<<WGM12) ;				//ENABLE CLK prescalLer 1024 ,CHOOSE CTC MODE
	TIMSK  |=(1<<OCIE1A);									//ENABLE TIMER 1
	TCNT1  =0;												//INITIAL VALUE
	//OCR1A  =976;											//TOP VALUE TO COMPARE WITH
	OCR1A=top_value;
	OCR1B=top_value2;*/
	///////////////////////////
	if ((channel) == 0)
			{

				SET_BIT (TCCR1A,FOC1A);


				SET_BIT (TCCR1B,WGM12);

				TCCR1B = ((TCCR1B & 0xF8) | (prescaler & 0x7));

				TCNT1 = initial_value;

				OCR1A =compare_value_a;//78125//10sec//

				SET_BIT (TIMSK,OCIE1A);
			}
			if ((channel) == 1)
			{
				SET_BIT (TCCR1A,FOC1B);

				SET_BIT (TCCR1B,WGM12);

				TCCR1B = ((TCCR1B & 0xF8) | (prescaler & 0x7));

				TCNT1 =initial_value;

				OCR1B =compare_value_b;//1 min//468750

				SET_BIT (TIMSK,OCIE1B);
			}
}

//Bit 3 � FOC1A: Force Output Compare for Channel A
// Bit 2 � FOC1B: Force Output Compare for Channel B
//WGM13 WGM12(CTC1) WGM11(PWM11) WGM10(PWM10)
//4 0 1 0 0 CTC OCR1A Immediate MAX
//12 1 1 0 0 CTC ICR1 Immediate MAX
//COM1A1/COM1B1 COM1A0/COM1B0  1





void TIMER0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr0 = a_ptr;
}


void TIMER1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr1 = a_ptr;
}







void TIMER0_stop (void)
{
	TCCR0=0;
	CLEAR_BIT (TIMSK,OCIE0);
	CLEAR_BIT (TIMSK,TOIE0);
}
/////////////////////////////////////
void TIMER1_stop (void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = 0;
	CLEAR_BIT (TIMSK,OCIE1A);
	CLEAR_BIT (TIMSK,TOIE1);
}
