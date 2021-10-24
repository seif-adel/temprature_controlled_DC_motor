/*
 * timer.c
 *
 *  Created on: Aug 13, 2021
 *      Author: seif adel
 */
#include "timer.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_OVF_vect)
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
void Timer_init(const Timer_configtype * Config_Ptr , uint16 compare_value)// this implementation is specific for pwm only due to the argument duty
{
	TCNT0 =0;
	if(Config_Ptr -> WayOfWork == INT)
	{
		if(Config_Ptr ->mode ==OVERFLOW)
		{
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock); // set timer clock
			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
			TCCR0 |= (1<<FOC0); // non PWM mode
			TCCR0 = (TCCR0 & 0xcf) | ((Config_Ptr->OutputMode) << 4); // set output mode when matching occur

		}
		else if (Config_Ptr ->mode ==CTC)
		{
			OCR0 = compare_value;
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock); // set timer clock
			TIMSK |= (1<<OCIE0); // Enable Timer0 compare match Interrupt
			TCCR0 |= (1<<FOC0); // non PWM mode
			DDRB = DDRB | (1<<PB3); // Configure PB3/OC0 Pin as output pin
			TCCR0|= (1<<WGM01); //  CTC Mode WGM01=1 & WGM00=0
			TCCR0 = (TCCR0 & 0xcf) | ((Config_Ptr->OutputMode) << 4); // set output mode when matching occur
		}

		/*else if (Config_Ptr ->mode ==PWM)
		{
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock); // set timer clock
			TCNT0 = Config_Ptr-> initial_value ; //Set Timer initial value
		}
		there is no interrupt usage with PWM
		*/

	}

	else if(Config_Ptr -> WayOfWork == POLL)
	{
			if(Config_Ptr ->mode ==OVERFLOW)
			{
				TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock); // set timer clock
				TCCR0 |= (1<<FOC0); // non PWM mode
				TCCR0 = (TCCR0 & 0xcf) | ((Config_Ptr->OutputMode) << 4); // set output mode when matching occur
			}
			else if (Config_Ptr ->mode ==CTC)
			{
				OCR0 = compare_value;
				TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock); // set timer clock
				TCCR0 |= (1<<FOC0); // non PWM mode
				DDRB = DDRB | (1<<PB3); // Configure PB3/OC0 Pin as output pin
				TCCR0|= (1<<WGM01); //  CTC Mode WGM01=1 & WGM00=0
				TCCR0 = (TCCR0 & 0xcf) | ((Config_Ptr->OutputMode) << 4); // set output mode when matching occur
			}

			else if (Config_Ptr ->mode ==PWM)
			{
				OCR0 = compare_value;
				TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock); // set timer clock
				DDRB = DDRB | (1<<PB3); // Configure PB3/OC0 Pin as output pin
				TCCR0 |= (1<<WGM00) | (1<<WGM01); //Fast PWM Mode WGM01=1 & WGM00=1
				TCCR0 = (TCCR0 & 0xcf) | ((Config_Ptr->OutputMode) << 4); // set output mode when matching occur
			}
	}
}

void Set_initial_value(const int value)
{
TCNT0 = value;
}

void Set_compare_value(const int value)
{
	TCNT0 = 0;
	OCR0 = value;
}

void Set_duty_cycle(const int value)
{
OCR0 = value;
}

void Timer_setCallBack(void(*app_ptr)(void)) // call this fn->put address of app function to be executed when interrupt occur in g_callbackptr then call this fn.in ISR
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = app_ptr;
}

bool Indication_of_overflow(void)
{
	while (BIT_IS_CLEAR(TIFR,TOV0));
	return 1;
}

bool Indication_of_compare(void)
{
	while (BIT_IS_CLEAR(TIFR,OCF0));
	return 1;
}
void Stop_Timer(void)
{
TCCR0 = 0;
CLEAR_BIT(TIMSK,TOIE0);
CLEAR_BIT(TIMSK,OCIE0);
}
