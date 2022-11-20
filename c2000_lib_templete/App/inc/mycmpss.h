/*
 * mycmpss.h
 *
 *  Created on: 2022年7月20日
 *      Author: 14913
 */

#ifndef APP_INC_MYCMPSS_H_
#define APP_INC_MYCMPSS_H_


#include "include.h"


#define CMPSSOUTPUTGPIOPIN 14
#define CMPSSGPIOBAR GPIO_31_OUTPUTXBAR8

#define myCMPSS0_BASE CMPSS1_BASE


#define cmpssGPIOInputInterrupt0 24
#define cmpssGPIOconfig GPIO_24_GPIO24



void    mycmpss_init(uint32_t highvoltage,uint32_t lowvoltage);
void    cmpss_gpioint_init();


__interrupt void cmpssgpioInterruptHandler(void);





extern volatile uint16_t g_flag_cmpss;



#endif /* APP_INC_MYCMPSS_H_ */

