/*
 * MC2_MAIN.c
 *
 *  Created on: Sep 30, 2020
 *      Author: yasmin mohamed
 */

#include "lcd.h"
#include "uart.h"
#include "external_eeprom.h"
#include "string.h"
#include "timer.h"

#define M2_READY 0x10
#define opendoor 4
#define changepassword 5
#define pause 6
volatile uint8 g_maxEntry=0;

#define true 1
#define false 0
#define save 3

#include <avr/interrupt.h>

#define NUMBER_OF_OVERFLOWS_PER_SECOND 123

uint8 g_saveflag=false;


uint8 tick=0;

uint8 g_flag=0;
uint8 g_flag_motor=0;
void store_password (void)
{
	uint8 password1[7];

	uint8 i;
	while ((UART_recieveByte ()) != M2_READY);
	UART_receiveString (password1);
	g_saveflag=UART_recieveByte ();
	for ( i=0 ; i<7 ; i++)
		{
		   EEPROM_writeByte (i,password1[i]);
		    _delay_ms (80);
		}
	}
uint8 operation (void)
{
	return ((UART_recieveByte ()));
}

uint8 if_savedPassword_matched (void)
{

	uint8 i;
	uint8 password[7];
	uint8 savedPassword[7];
	while ((UART_recieveByte ()) != M2_READY);
	UART_receiveString (password);


	for(i=0 ; i<5 ; i++)
    {
     	EEPROM_readByte(i,&savedPassword[i]);
     	_delay_ms(80);
    }
	savedPassword[i] = '\0';

	if ((strcmp (password,savedPassword)) == 0)
		{

			while ((UART_recieveByte ()) != M2_READY);
			//UART_sendByte (M2_READY);
			UART_sendByte (true);
			return true;
		}
	else
	{
		LCD_intgerToString(10);
		while ((UART_recieveByte ()) != M2_READY);
		//UART_sendByte (M2_READY);
		UART_sendByte (false);
		return false;
	}
}


/*
ISR(TIMER0_OVF_vect)
{
	 tick++;
	if(tick==300)
 {
		 tick=0;
		 g_flag_motor++;
 }
}
*//*
void timer0_init_normal()
{
	SREG  |= (1<<7);
	TCNT0 = 0; //timer initial value
		TIMSK = (1<<TOIE0); //enable overflow interrupt

		TCCR0 = (1<<FOC0) | (1<<CS02);
}
ISR(TIMER0_OVF_vect)
{
	tick++;
	if(tick == NUMBER_OF_OVERFLOWS_PER_SECOND){
		tick=0;
		 g_flag_motor++;
	}
}


*/
/*
void flag_motor(void){ g_flag_motor++;}
void motorDOOR_on(void){
	LCD_intgerToString(g_flag_motor);
	CLEAR_BIT(PORTC,PC2);
	SET_BIT(PORTC,PC3);}
*/

void openDoor(void)
{

	/* configure pin PC0 and PC1 as output pins*/

	SET_BIT(DDRC,PC2);
	SET_BIT(DDRC,PC3);
	SET_BIT(DDRC,PC4);
	SET_BIT(PORTC,PC4);
	/* Rotate the motor --> clock wise*/
	CLEAR_BIT(PORTC,PC2);
	SET_BIT(PORTC,PC3);
	_delay_ms(10000000);
	TOGGLE_BIT(PORTC,PC2);
	TOGGLE_BIT(PORTC,PC3);
	_delay_ms(10000000);
	CLEAR_BIT(PORTC,PC4);//disable motor
}


void increment_flag()
{
g_flag++;

}
void motorDOOR_off(void){
	LCD_intgerToString(6);
	TOGGLE_BIT(PORTC,PC2);
	TOGGLE_BIT(PORTC,PC3);

	TIMER1_stop();
	g_flag_motor=10;
}
void stop_motor_timer(void)
{
	LCD_intgerToString(2);
	CLEAR_BIT(PORTC,PC4);
	TIMER1_stop();
	//g_flag=0;

}
void openDOOR(void)
{

	/* configure pin PC0 and PC1 as output pins */

	SET_BIT(DDRC,PC2);
	SET_BIT(DDRC,PC3);
	SET_BIT(DDRC,PC4);
	SET_BIT(PORTC,PC4);

		TIMER1_setCallBack(motorDOOR_off);
		TIMER_data g_timer1A={timer1,normal,0,Prescaler_1024,0,0,0} ;
		timer_init(&g_timer1A);

		CLEAR_BIT(PORTC,PC2);
		SET_BIT(PORTC,PC3);

		while(g_flag_motor!=10){;}
		if(g_flag_motor==10)
		{
			TIMER1_setCallBack(stop_motor_timer);
			TIMER_data g_timer1={timer1,normal,0,Prescaler_1024,0,0,0} ;
			timer_init(&g_timer1);

		}
		}
		/*LCD_intgerToString(3);
		CLEAR_BIT(PORTC,PC2);
		SET_BIT(PORTC,PC3);
		LCD_intgerToString(4);
		TIMER_data g_timer1={timer1,normal,0,Prescaler_1024,0,0,0} ;
		timer_init(&g_timer1);
		TIMER1_setCallBack(stop_motor_timer);
*/
//}

void system_pause (void)
{
	/*SYSTEM PAUSED FOR 1 MIN */
    /*BUZZER IS ON FOR 1 MIN */

	TIMER_data g_timer1A={timer1,compare,0,Prescaler_1024,0,65500,0} ;
	timer_init(&g_timer1A);
	TIMER1_setCallBack(increment_flag);

	while(g_flag<7)
	{
		 SET_BIT(DDRA,PA0);
		 SET_BIT(PORTA,PA0);
	 }
	if(g_flag==7)
	{
		TOGGLE_BIT(PORTA,PA0);
		TIMER1_stop();
		g_flag=0;
	    LCD_clearScreen();
	}
}



int main(void)
{
	uint8 user_choice=save;
	UART_init();
	while(UART_recieveByte() != M2_READY){} // wait until MC2 is ready
	EEPROM_init();
	LCD_init();
	UART_sendString("I am Micro1#"); // send the required string to MC2
	
	uint8 matchState=false;
	uint8 changePassword_matchState=false;
    while(1)
    {

    	while((g_saveflag==false)&&(user_choice==save))
    	{
    		store_password();
    	}
    	user_choice=operation();
    	if(user_choice==changepassword)
   	    	{
    	    	while ((g_maxEntry!=3)&&(changePassword_matchState==false))
    	    	    {
    	    			changePassword_matchState=if_savedPassword_matched();

    	    			if (changePassword_matchState==false)
    	    				{

    	    					g_maxEntry++;
    	    					LCD_intgerToString(g_maxEntry);
    	    				}
    	    		}
    	    	if ((g_maxEntry!=3)&&(changePassword_matchState==true))
    	    		{		

     		    		store_password();
     		    		g_maxEntry=0;
     		    		changePassword_matchState=false;
       	    	    }
    	    	else if (g_maxEntry==3)
    	    		{
    	    		LCD_intgerToString(4);
    	        		system_pause();

    	        		g_maxEntry=0;
    	        		changePassword_matchState=false;

    	    		}

    	    	}
    	/////////////////////////////////////////////
    	else if(user_choice==opendoor)
    	{
    	while ((g_maxEntry!=3)&&(matchState==false))
    	    {
    			matchState=if_savedPassword_matched();

    			if(matchState==false)
    			{

    				g_maxEntry++;
    				LCD_intgerToString(g_maxEntry);
    			}
    	    }
    	if ((g_maxEntry!=3)&&(matchState==true))
    	    {		
    	    	g_maxEntry=0;

    	    	openDoor();
    	    	matchState=false;
    	    }
    	else if (g_maxEntry==3)
    		{
    		LCD_intgerToString(4);
    		system_pause();

    		g_maxEntry=0;
    		matchState=false;

    		}
    	}

    	else
    	{
    		g_maxEntry=0;
    		matchState=false;
    		changePassword_matchState=false;
    	}
    }
}
