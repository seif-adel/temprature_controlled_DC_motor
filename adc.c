/*
 * adc.c
 *
 *  Created on: Aug 12, 2021
 *      Author: seif adel
 */

#include "adc.h"
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

volatile uint16 g_adcResult = 0;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
		g_adcResult = ADC;
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ADC_init(ADC_ConfigType*Config_Ptr)
{
	/* ADMUX Register Bits Description:
	 * ADLAR   = 0 right adjusted
	 */
	Config_Ptr->channel &= 0x07;
	Config_Ptr->voltage &=0x03;
	ADMUX = (ADMUX & 0x3f) | (Config_Ptr->voltage<<6);
	ADMUX = (ADMUX & 0xE0) | Config_Ptr->channel;
	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 */
	SET_BIT(ADCSRA,ADEN);
	if(Config_Ptr->mode==INTERRUPT)
	{
		SET_BIT(ADCSRA,ADIE); // required mode
	}
	ADCSRA = (ADCSRA & 0xf8) | Config_Ptr->prescaler; // required prescaler (required freq.)
}
////////////////////////////////////////////////////////////////////////////////////////////////
uint16 ADC_readChannel(void) // this implementation is always polling
{
	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
	//if(mode == POLLING)
	//{
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* wait for conversion to complete ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF); /* clear ADIF by write '1' to it :) */
	//g_adcResult = ADC; /* return the data register */
	return ADC;
	//}
}
