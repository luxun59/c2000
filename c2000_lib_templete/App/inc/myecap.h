/*
 * myecap.h
 *
 *  Created on: 2022年8月9日
 *      Author: 14913
 */

#ifndef APP_INC_MYECAP_H_
#define APP_INC_MYECAP_H_


#include "include.h"

#define ECAP3MAXNUM 4


extern __interrupt void INT_myECAP3_ISR(void);


void ECAP_init();
void MYECAP3GPIO_init();
void MYCAP3_restart();

extern volatile uint16_t ecap3Cap1Count[ECAP3MAXNUM];

extern uint32_t ecap3IntCount;



#endif /* APP_INC_MYECAP_H_ */
