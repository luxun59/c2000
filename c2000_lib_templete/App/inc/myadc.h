/*
 * myadc.h
 *
 *  Created on: 2022年7月19日
 *      Author: 14913
 */

#ifndef APP_INC_MYADC_H_
#define APP_INC_MYADC_H_

#include "include.h"

#define myADC0_BASE ADCA_BASE
#define myADC0_RESULT_BASE ADCARESULT_BASE
#define myADC0_SOC0 ADC_SOC_NUMBER0
#define myADC0_FORCE_SOC0 ADC_FORCE_SOC0


#define myADC0_epwmbase EPWM5_BASE
#define myADC0_TRIGGER_SOCA ADC_TRIGGER_EPWM5_SOCA

// Interrupt Settings for INT_myADC0_1
#define INT_myADC0_1 INT_ADCA1
#define INT_myADC0_1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP1


//
// Defines
//
#define RESULTS_BUFFER_SIZE     256


__interrupt void adcA1ISR(void);

__interrupt void dmach1ISR(void);

// Function Prototypes
//
void configureADCEPWM(uint32_t myepwm_base,uint32_t frequence ,uint16_t duty);
void myadcdma_config();

void myadc_Start();
void myadc_Stop();


//
extern uint16_t myDMAADC0Results[RESULTS_BUFFER_SIZE];
extern volatile uint16_t bufferFull;

#endif /* APP_INC_MYADC_H_ */


