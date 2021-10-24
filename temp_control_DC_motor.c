/*
 * temp_control_DC_motor.c
 *
 *  Created on: Aug 12, 2021
 *      Author: HP
 */
#include "adc.h"
#include "lcd.h"
#include "timer.h"

#define low_level 10
#define mid_level 20
#define high_level 30
#define motor_PORT PORTB
#define motor_PORT_DIR DDRB
#define IN1 PB0
#define IN2 PB1
int main(void)
{
	ADC_ConfigType ADC_Config = {ADC2,Div128,POLLING,AREF_EXTERNAL};
	LCD_Configtype LCD_Config = {BIT_8,HIGHER};
	Timer_configtype Timer_config = {POLL , PWM , F_CPU_8 ,CLEAR_OC0};
	uint32 temp;
	LCD_init(&LCD_Config); /* initialize LCD driver */
	ADC_init(&ADC_Config); /* initialize ADC driver */
	motor_PORT_DIR |= (1<<IN1) | (1<<IN2);
	LCD_clearScreen(); /* clear LCD at the beginning */
	/* display this string "Temp =    " only once at LCD */
	LCD_displayString("Temp = ");
	LCD_goToRowColumn(0,10);
	/* display character 'C' on the screen "Temp =   C" */
	LCD_displayCharacter('C');
	Timer_init(&Timer_config,128);/* initialize timer driver */
    while(1)
    {
		LCD_goToRowColumn(0,7); /* display the number every time at this position */
		temp=ADC_readChannel(); /* read channel two where the temp sensor is connect */
		temp = (temp*150*5)/(1023*1.5); /* calculate the temp from the ADC value*/
		LCD_intgerToString(temp); /* display the temp on LCD screen */
		if(temp<low_level)
		{
			CLEAR_BIT(motor_PORT,IN1);
			CLEAR_BIT(motor_PORT,IN2);
			Timer_init(&Timer_config,0);
		}
		else if(temp>=low_level && temp<mid_level)
		{
			CLEAR_BIT(motor_PORT,IN1);
			SET_BIT(motor_PORT,IN2);
			Timer_init(&Timer_config,64);
		}
		else if(temp>=mid_level && temp< high_level)
		{
			CLEAR_BIT(motor_PORT,IN1);
			SET_BIT(motor_PORT,IN2);
			Timer_init(&Timer_config,128);
		}
		else
		{
			CLEAR_BIT(motor_PORT,IN1);
			SET_BIT(motor_PORT,IN2);
			Timer_init(&Timer_config,254);
		}
    }
}
