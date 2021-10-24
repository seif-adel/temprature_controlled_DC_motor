/*
 * adc.h
 *
 *  Created on: Aug 12, 2021
 *      Author: seif adel
 */

#ifndef ADC_H_
#define ADC_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern Public global variables to be used by other modules */
extern volatile uint16 g_adcResult;
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
ADC0 , ADC1 , ADC2, ADC3, ADC4, ADC5, ADC6 , ADC7
}ADC_channel;

typedef enum
{
 Div2, Div22 , Div4, Div8, Div16, Div32, Div64 , Div128
}ADC_prescaler;

typedef enum
{
INTERRUPT , POLLING
}ADC_mode;

typedef enum
{
AREF_EXTERNAL , AVCC , RESERVED, AREF_INTERNAL
}ADC_Voltage;

typedef struct
{
	ADC_channel channel;
	ADC_mode mode;
	ADC_Voltage voltage;
	ADC_prescaler prescaler;
}ADC_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the ADC driver
 * 	1. set the required mode if interrupt or polling .
 * 	2. Set the required channel number.
 * 	3. set the required prescaler.
 */
void ADC_init( ADC_ConfigType * Config_Ptr);

uint16 ADC_readChannel(void);

#endif /* ADC_H_ */
