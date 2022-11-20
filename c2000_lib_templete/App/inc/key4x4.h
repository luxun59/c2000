/*
 * key4x4.h
 *
 *  Created on: 2022年8月8日
 *      Author: 14913
 */

#ifndef APP_INC_KEY4X4_H_
#define APP_INC_KEY4X4_H_


#include "include.h"

/****************************************************/
//C2000
//外接矩阵键盘驱动
//
//
/****************************************************/

void KEY4x4_Init(void);
uint16_t KEY4x4_Scan(bool mode);

extern uint16_t g_flag_keyen;







#endif /* APP_INC_KEY4X4_H_ */
