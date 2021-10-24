/*
 * timer.h
 *
 *  Created on: Aug 13, 2021
 *      Author: HP
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
INT , POLL
}Timer_way_of_work;

typedef enum
{
	NORMAL_OC0 , TOGGLE_OC0 , CLEAR_OC0 , SET_OC0
}OC0_mode;

typedef enum
{
OVERFLOW , CTC=2 , PWM=3
}Timer_Mode;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef struct
{
	Timer_way_of_work WayOfWork;
	Timer_Mode mode;
	Timer_Clock timer_clock;
	OC0_mode OutputMode;
}Timer_configtype;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the timer driver
 * 	1. Set the required clock Prescalar
 * 	2. Set the required mode. (PWM , CTC , overflow )
 *  3. Set the required way of work. ( interrupt , polling )
 */
void Timer_init(const Timer_configtype * Config_Ptr , uint16 duty);

void Set_initial_value(const int value);

void Set_compare_value(const int value);

void Set_duty_cycle(const int value);

void Timer_setCallBack(void(*a_ptr)(void));

bool Indication_of_overflow(void);

bool Indication_of_compare(void);

void Stop_Timer(void);



#endif /* TIMER_H_ */
