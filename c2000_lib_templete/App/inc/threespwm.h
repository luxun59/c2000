/*
 * spwm.h
 *
 *  Created on: 2022年8月13日
 *      Author: 14913
 */

#ifndef APP_INC_THREESPWM_H_
#define APP_INC_THREESPWM_H_


#include "include.h"


void SPWM_init();
void configurePhase(uint32_t base, uint32_t masterBase, uint16_t phaseVal);
__interrupt void epwm1ISR(void);
__interrupt void epwm2ISR(void);
__interrupt void epwm4ISR(void);



#endif /* APP_INC_THREESPWM_H_ */
