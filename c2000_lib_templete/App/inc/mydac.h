/*
 * mydac.h
 *
 *  Created on: 2022年7月21日
 *      Author: 14913
 */

#ifndef APP_INC_MYDAC_H_
#define APP_INC_MYDAC_H_

#include "include.h"

#define MYDACBUFFSIZE 256

#define myDAC0_BASE DACA_BASE
#define INT_myDAC0epwm_1 INT_EPWM6
#define myDAC0_epwmbase EPWM6_BASE



void mydac_init();
void mydac_configureEPWM(uint32_t myepwm_base,uint32_t frequence ,uint16_t duty);
__interrupt void epwm6ISR(void);


void mydac_start();
void mydac_stop();


extern uint16_t g_mydacdatabuff[MYDACBUFFSIZE];
extern volatile uint16_t g_mydac_index;

#endif /* APP_INC_MYDAC_H_ */
