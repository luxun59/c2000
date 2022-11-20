/*
 * oled.h
 *
 *  Created on: 2022年7月26日
 *      Author: 14913
 */

#ifndef APP_INC_OLED_H_
#define APP_INC_OLED_H_


#include "include.h"


#include "oledfront.h"

#define HW_IIC 0
#define SW_IIC 1
#define HW_SPI 2
#define SW_SPI 3


#define C2000 0

/*****************************************************************/
//配置驱动
//1.HW_IIC  硬件IIC
//2.SW_IIC  软件IIC
//3.HW_SPI  硬件SPI
//4.SW_SPI  软件IIC

//现在只支持  软件IIC和软件SPI
#define TRANSFER_METHOD SW_IIC




#define BOARD C2000



#if BOARD == C2000
// C2000 寄存器为16位
#define uint8_t uint16_t
#endif

//OLED控制用函数
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey);
void OLED_Init(void);
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_Refresh(void);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
//void OLED_ScrollDisplay(uint8_t num,uint8_t space);
void OLED_WR_BP(uint8_t x,uint8_t y);
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[]);
void OLED_ShowFloat(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2);


















#endif /* APP_INC_OLED_H_ */
