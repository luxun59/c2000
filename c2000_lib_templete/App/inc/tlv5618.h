/*
 * tlv5618.h
 *
 *  Created on: 2022年8月11日
 *      Author: 14913
 */

#ifndef APP_INC_TLV5618_H_
#define APP_INC_TLV5618_H_

#include "include.h"

void TLV5618_SPI_Send_Byte( uint16_t write_dat );
void TLV5618_SPI_Init();

#endif /* APP_INC_TLV5618_H_ */
