/*
 * tpl0401.h
 *
 *  Created on: 2022年8月18日
 *      Author: 14913
 */

#ifndef APP_INC_TPL0401_H_
#define APP_INC_TPL0401_H_



#include "include.h"




void delay_1115(void);
void tpl0401_start(void);
void tpl0401_stop(void);
void tpl0401_mack(void);
void tpl0401_mnack(void);
void tpl0401_ack();
void tpl0401_write1(void);
void tpl0401_write0(void);
void tpl0401_write1byte(uint16_t wdata);
uint16_t tpl0401_read1byte(void);
void tpl0401_set(uint16_t data);
uint16_t tpl0401_read();
void tpl0401_init(void);
void tpl0401_sda_setin();
void tpl0401_sda_setout();




#endif /* APP_INC_TPL0401_H_ */




