/*
 * key4x4.c
 *
 *  Created on: 2022年8月8日
 *      Author: 14913
 */


#include "key4x4.h"
#include "include.h"



/****************************************************/
//c2000 280049
//外接矩阵键盘驱动
//
/****************************************************/

/****************************************************/
/************          接法如下          ************/
/*
 *         Rows and columns  行和列
 *
 * R4   R3   R2   R1     C1   C2   C3   C4
 * P08  P09  P04  P05    P58  P30  P18  P25
 *
 */
/*****************************************************/

#define C4_GPIO GPIO_9_GPIO9
#define C4_PIN 9
#define C3_GPIO GPIO_8_GPIO8
#define C3_PIN 8
#define C2_GPIO GPIO_11_GPIO11
#define C2_PIN 11
#define C1_GPIO GPIO_10_GPIO10
#define C1_PIN 10

#define R4_GPIO GPIO_30_GPIO30
#define R4_PIN 30
#define R3_GPIO GPIO_58_GPIO58
#define R3_PIN 58
#define R2_GPIO GPIO_5_GPIO5
#define R2_PIN 5
#define R1_GPIO GPIO_4_GPIO4
#define R1_PIN 4


#define Read1  GPIO_readPin( R1_PIN )
#define Read2  GPIO_readPin( R2_PIN )
#define Read3  GPIO_readPin( R3_PIN )
#define Read4  GPIO_readPin( R4_PIN )

#define High1 GPIO_writePin(C1_PIN, 1)
#define Low1  GPIO_writePin(C1_PIN, 0)
#define High2 GPIO_writePin(C2_PIN, 1)
#define Low2  GPIO_writePin(C2_PIN, 0)
#define High3 GPIO_writePin(C3_PIN, 1)
#define Low3  GPIO_writePin(C3_PIN, 0)
#define High4 GPIO_writePin(C4_PIN, 1)
#define Low4  GPIO_writePin(C4_PIN, 0)




uint16_t g_flag_keyen = 0;





//函数功能：延时
void key_delay(uint16_t t);

void KEY4x4_Init(void)
{
    GPIO_setPinConfig(R4_GPIO);
    GPIO_setDirectionMode(R4_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(R4_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setMasterCore(R4_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(R4_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(R3_GPIO);
    GPIO_setDirectionMode(R3_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(R3_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setMasterCore(R3_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(R3_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(R2_GPIO);
    GPIO_setDirectionMode(R2_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(R2_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setMasterCore(R2_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(R4_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(R1_GPIO);
    GPIO_setDirectionMode(R1_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(R1_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setMasterCore(R1_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(R1_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(C1_GPIO);
    GPIO_setDirectionMode(C1_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(C1_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(C1_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(C1_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(C2_GPIO);
    GPIO_setDirectionMode(C2_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(C2_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(C2_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(C2_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(C3_GPIO);
    GPIO_setDirectionMode(C3_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(C3_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(C3_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(C3_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(C4_GPIO);
    GPIO_setDirectionMode(C4_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(C4_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(C4_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(C4_PIN, GPIO_QUAL_SYNC);

    Low1;Low2;Low3;Low4;

//    Low1; High2; High3; High4;
//    High1; High2; Low3; High4;
//    High1; High2; High3; Low4;
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按; 1,支持连续按
uint16_t KEY4x4_Scan(bool mode)
{
    uint16_t i;
    static bool key_up = true; //按键按松开标志
    if (mode)
        key_up = true; //支持连按

//    High1;High2;High3;High4;
    Low1; Low2; Low3; Low4;

    uint16_t  R1 = Read1;
    uint16_t  R2 = Read2;
    uint16_t  R3 = Read3;
    uint16_t  R4 = Read4;

    uint16_t data_R = (R1 + R2 + R3 + R4);

//    if(1>0)
//    {
////        UARTprintf("R : %d %d %d %d \n",R1,R2,R3,R4);
//    }

    if ((R1 + R2 + R3 + R4) < 4 && key_up)
    {
        key_delay(25); //去抖动
        for (i = 0; i < 4; i++)
        {
            key_delay(5); //去抖动
            key_up = false;
            switch(i)
            {
            case 0:
                Low1; High2; High3; High4;
                break;
            case 1:
                High1; Low2; High3; High4;
                break;
            case 2:
                High1; High2; Low3; High4;
                break;
            case 3:
                High1; High2; High3; Low4;
                break;
            }

            if(Read1==0)
            {
                return 1+i;
            }
            else if(Read2==0)
            {
                return 5+i;
            }
            else if(Read3==0)
            {
                return 9+i;
            }
            else if(Read4==0)
            {
                return 13+i;
            }

        }

    }
    else if ((R1 + R2 + R3 + R4) == 4)
        key_up = true;
    return 0;
}

//函数功能：延时
static void key_delay(uint16_t t)
{
    Delay_ms(t);
}












