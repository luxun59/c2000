/*
 * sys.h
 *
 *  Created on: 2022年7月18日
 *      Author: 14913
 */

#ifndef APP_INC_SYS_H_
#define APP_INC_SYS_H_

#include "device.h"
#include "driverlib.h"


static inline void Delay_us(uint16_t us)
{
    DEVICE_DELAY_US(us);
}



static inline void Delay_ms(uint16_t ms)
{
    for(;ms>0;ms--)
        Delay_us(1000);
}










#endif /* APP_INC_SYS_H_ */
