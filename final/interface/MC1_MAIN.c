/*
 * MC1_MAIN.c
 *
 *  Created on: Sep 30, 2020
 *      Author: yasmin mohamed
 */


#include "keypad.h"
#include "uart.h"
#include "lcd.h"
#include <stdio.h>
#include "timer.h"

#define M2_READY 0x10
#define true 1
#define false 0
uint8 g_state=false;
uint8 g_maxEntry=0;
#define save 3
#define opendoor 4
#define changepassword 5
#define pause 6



uint8 g_flag=0;

#include "string.h"


void set_password(void)
{
	uint8 i;
	uint8 password1[7];
	uint8 check=0;
	uint8 j,k ;
	uint8 password2[7];

	LCD_displayString(" new password : ");

	LCD_goToRowColumn(1,0);

	for (i=0;i<5;i++)
	{
		password1[i] = KeyPad_getPressedKey(); //get the pressed key
		_delay_ms(2000);
		//LCD_displayCharacter( KeyPad_getPressedKey());
		LCD_intgerToString( password1[i]);
	}

	LCD_clearScreen();
	//_delay_ms(100);

	LCD_displayString("repeat password : ");
	LCD_goToRowColumn(1,0);

	for (j=0;j<5;j++)
	{
		password2[j] = KeyPad_getPressedKey(); //get the pressed key
		_delay_ms(2000);				//LCD_displayCharacter( KeyPad_getPressedKey());
		LCD_intgerToString( password2[j]);
	}

	password1[5]='#';
	password1[6]='\0';
	password2[5]='#';
	password2[6]='\0';

	for(k=0;k<5;k++)
	{
		if(password1[k]==password2[k])
		{			check++;		}
	}

	if (check == 5)
		{
			//LCD_clearScreen();
			UART_sendByte (M2_READY);
			UART_sendString(password1); //send the pressed key to the second MC using uart
			UART_sendByte (true);
			g_state= true;
		}

	else if (check != 5){	UART_sendByte (false);g_state= false;}
}

//////////////////////////////////////
/*
void confirm_password_notmatched_matched(uint8 state)
{
   uint8 MaxTry =3;
   uint8  check_if_correct=UART_recieveByte();
	if (check_if_correct==true)
	{	
		LCD_displayCharacter('y');
		return;
	}
	else
	{
		while(MaxTry>0)
		{
			LCD_displayCharacter('w');

			set_password();
			//confirm_password();
			MaxTry--;
		}
	}
}*/
///////////////////////////////////////////////
uint8 log_in (void)
{
	uint8 i;
	uint8 password[7];
	uint8 check=0;

	LCD_clearScreen();
	LCD_displayString("your password is: ");

	LCD_goToRowColumn(1,0);

    for (i=0;i<5;i++)
		{
			password[i] = KeyPad_getPressedKey(); //get the pressed key
			_delay_ms(2000);
			//LCD_displayCharacter( KeyPad_getPressedKey());
			LCD_intgerToString( password[i]);
		}

	LCD_clearScreen();
	password[5]='#';
	password[6]='\0';


	UART_sendByte (M2_READY);
	UART_sendString(password); //send the pressed key to the second MC using uart

	UART_sendByte (M2_READY);
	//while ((UART_recieveByte ()) != M2_READY);
	return (UART_recieveByte());
}


//////////////////////////////////////////////////
uint8 user_target (void)
{
	uint8 operation;
	operation = KeyPad_getPressedKey ();
	_delay_ms (2000);
	if ((operation == '+') || (operation == '-'))
	{
		if (operation == '+')
		{
			UART_sendByte (opendoor);
			return opendoor;
		}
		else if (operation == '-')
		{
			UART_sendByte (changepassword);
			return changepassword;
		}
	}
		return -1;
}
////////////////////////////////////////////////////

void increment_flag()
{
g_flag++;

}
void system_pause (void)
{

	TIMER_data g_timer1A={timer1,compare,0,Prescaler_1024,0,65500,0} ;
	timer_init(&g_timer1A);
	TIMER1_setCallBack(increment_flag);
	 LCD_displayString("system is locked: ");
	while(g_flag<7)
	{
		;
	}
	if(g_flag==7)
	{

		TIMER1_stop();
		g_flag=0;
	    LCD_clearScreen();
	}
}

void sent_stateFlag (uint8 flag)
{
	UART_sendByte (flag);
}
int main(void)
{
	    uint8 operation;
		uint8 matching_result=false;
		uint8 Str[20];
		UART_init(); // initialize UART driver
		LCD_init(); // initialize LCD driver
		UART_sendByte(M2_READY); // MC2 is ready
		UART_receiveString(Str); // receive the string
		LCD_displayString(Str); // display the string on LCD

    while(1)
    {
    		while((g_state==false))
    			{
    			    LCD_clearScreen();
    				set_password();

    			}

////////////////////////////////////////////////////////////////
    		LCD_clearScreen();
    		LCD_displayString("open_door:+ ");
    		LCD_goToRowColumn(1,0);
    		LCD_displayString("change_pass:- ");
//////////////////////////////////////////////////////
    		operation=user_target();
    		if(operation==opendoor)
    		{
    			while((matching_result==false)&&(g_maxEntry!=3))
			    {
    				matching_result=log_in();

    				if (matching_result==false)
    				{
    					g_maxEntry++;
    					LCD_intgerToString(g_maxEntry);
    				}
		     	}
    			if (g_maxEntry!=3)
    			{

    				g_maxEntry=0;
    				//open door

    				LCD_displayString("door opens ");
    				_delay_ms(10000000);
    				LCD_goToRowColumn(1,0);
    				LCD_displayString("door CLOSE ");
    				 _delay_ms(10000000);
    				matching_result=false;
    			}
    			else if (g_maxEntry==3)
    			{
    				LCD_intgerToString(5);
    				system_pause();

    				g_maxEntry=0;
    				matching_result=false;

    			}
    		}
    		else if(operation==changepassword)
    		{
    			while((matching_result==false)&&(g_maxEntry!=3))
    				{
						matching_result=log_in();

						if (matching_result==false)
						{
							g_maxEntry++;
							LCD_intgerToString(g_maxEntry);
						}

    				}
    			if (g_maxEntry!=3)
    			    {
    			 //   LCD_clearScreen();
    			    set_password();
    			    g_maxEntry=0;
    			    matching_result=false;
    			    }
    			else if (g_maxEntry==3){
    				LCD_intgerToString(5);
    				system_pause();

    				g_maxEntry=0;
    			    matching_result=false;

    				}
    			}
    		else
    		{
    			g_maxEntry=0;
    			matching_result=false;
    		}
    		}
     }

