/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"

#define WAIT_TIME 9000000
#define V25 1.43
#define AVG_SLOPE 4.3
uint16_t temperature = 0;
float ADC_value = 0;

int main(void)
{

	//ENABLE THE ADC RCC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//SINCE THE ADC IS SLOW SET PRESCALER
	//SINCE ADC IS ON APB2(72MHz) AND ADC_CLOCK =< 14MHz
	//ADC CLOCK SET TO (12MHz)
	RCC->CFGR |= RCC_CFGR_ADCPRE_1;
	//IF AN ADC PIN IS USED SET IT TO ANALOG MODE
	//SET ADC SAMPLE RATE
	ADC1->SMPR1 |= ADC_SMPR1_SMP16;
	//NUMBER OF ADC CHANNELS TO CONVERT
	//SET FIRST ADC IN SEQUENCE
	ADC1->SQR3 |= ADC_SQR3_SQ1_4;
	//ENABLE TRIGGER SOURCE
	ADC1->CR2 |= ADC_CR2_EXTTRIG;
	//SELECT ACTUAL CONVERSION TRIGGER
	ADC1->CR2 |= ADC_CR2_EXTSEL;
	//ENABLE ADC AND SET IN CONTINUOUS MODE
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_CONT;
	//WAIT FOR TIME Tstab
	for(int i=0; i<WAIT_TIME;i++){};
	//ENABLE ADC
	//ENABLE THE TEMPERATURE SENSOR
	ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_TSVREFE;
	//WAIT FOR TIME Tstab
	for(int i=0; i<WAIT_TIME;i++){};
	//CALIBRATE ADC
	ADC1->CR2 |= ADC_CR2_CAL;
	//WAIT FOR CALIBRATION TO END
	while(ADC1->CR2 & ADC_CR2_CAL){}
	//TRIGGER CALIBRATION
	ADC1->CR2 |= ADC_CR2_SWSTART;

	while(1){
		if(ADC1->SR & ADC_SR_EOC){
			ADC1->SR &= ~ADC_SR_EOC;
			ADC_value = ADC1->DR;
			//convert to actual temperature
			float VSENSE = (ADC_value/4095)*3.3;
			temperature = ((V25-VSENSE)/AVG_SLOPE)+25;
			//Start next conversion
			ADC1->CR2 |= ADC_CR2_SWSTART;

		}
	}
}
