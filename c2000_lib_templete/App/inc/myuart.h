/*
 * myuart.h
 *
 *  Created on: 2022年8月13日
 *      Author: 14913
 */

#ifndef APP_INC_MYUART_H_
#define APP_INC_MYUART_H_


#include "include.h"

//
// Function Prototypes
//
__interrupt void sciaTxISR(void);
__interrupt void sciaRxISR(void);

void SCI_init(uint32_t rate);



#endif /* APP_INC_MYUART_H_ */
