/*
 * main.c
 *
 *  Created on: 2022年7月18日
 *      Author: 14913
 */

#include "device.h"
#include "driverlib.h"
#include "board.h"
#include "include.h"
#include <stdio.h>
#include <math.h>


uint32_t g_plotindex = 0;

uint16_t g_am = 1;
uint16_t g_fre = 1;

uint16_t ce_frequence = 2;


#define AM1DATA 32
#define AM2DATA 64
#define AM3DATA 127

uint16_t tpl0401date = 64;


void am_control();
void fre_control_init();
void fre_control();
void key_process(uint16_t keynum);
void am_peak();
void hmi_end();

uint16_t find_max(uint16_t max);


//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board Initialization
    // - Set up the ADC and initialize the SOC
    // - Enable ADC interrupt
    //
    Board_init();

//    mycmpss_init(3000,0);

    //uart init
    SCI_init(9600);

    unsigned char *msg;
//    msg = "\r\n\n\nHello World!\0";
//    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 17);
//    printf("hello world c%d\n",2000);

    //ADC init
    myadcdma_config();

    //key4x4 init
    KEY4x4_Init();
    //OLED init
    OLED_Init();
    OLED_ColorTurn(0);//
    OLED_DisplayTurn(0);//
    OLED_DrawPoint(0,0);
    OLED_DrawLine(20,0,50,60);
    OLED_DrawCircle(64,32,20);
    OLED_Refresh();
    OLED_DrawLine(20,0,20,80);
    OLED_Refresh();
    Delay_ms(100);
    OLED_ShowString(0,40,"FFT",16);
    OLED_Refresh();

//   index = 0;
    bufferFull = 0;

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    Interrupt_enableMaster();
    EINT;
    ERTM;

//    testfftdata();
//    FFT_runTest();


    OLED_Clear();
    OLED_ShowString(20,0,"FIN",16);
    OLED_Refresh();

    OLED_ShowString(0,0,"START",16);
    OLED_Refresh();

    OLED_Clear();
    ecap3IntCount = 0;
    ECAP_init();
    MYECAP3GPIO_init();

    uint16_t nowkeydata[4] = 0;

    //start dac
//    mydac_start();
    //start adc
//    myadc_Start();

    tpl0401_init();
    tpl0401_set(64);

    //
    // Loop indefinitely
    //
    while(1)
    {

        g_flag_keyen = KEY4x4_Scan(0);
        key_process(g_flag_keyen);

//        if(g_flag_keyen)
//        {
//            nowkeydata[0] = g_flag_keyen;
//            OLED_ShowNum(0, 0,nowkeydata[0],2,16);
//        }



        OLED_Refresh();
//        Delay_ms(50);

        //
        double x = 2*sin(g_plotindex*0.1*ce_frequence);
        double y = g_am*sin(g_plotindex*0.1*ce_frequence*g_fre);
        //OLED_DrawPoint((uint16_t)(32+x*7),(uint16_t)(32+y*7));
        OLED_Refresh();
        {
            g_plotindex=0;
        }

        x = 2*sin(g_plotindex*0.1*ce_frequence);
        y = g_am*sin(g_plotindex*0.1*ce_frequence*g_fre);
        //OLED_DrawPoint((uint16_t)(32+x*7),(uint16_t)(32+y*7));
        OLED_Refresh();
        if(g_plotindex++ >62)
        {
            g_plotindex=0;
        }
//
        if(ecap3IntCount >=  5)
        {
            /* frequence  = 100k/ecap3Cap1Count */
            OLED_ShowNum(0,20,ecap3Cap1Count[0],6,16);
            OLED_ShowNum(80,20,ecap3Cap1Count[1],6,16);
            OLED_ShowNum(0,40,ecap3Cap1Count[2],6,16);
            OLED_ShowNum(80,40,ecap3Cap1Count[3],6,16);
            OLED_Refresh();
            MYCAP3_restart();
            ecap3IntCount = 0;

        }
//        OLED_ShowNum(40,0,ecap3IntCount,2,16);


       /************adc****************/
        if(bufferFull == 1)
        {
            myadc_Stop();
            bufferFull = 2;     // Clear the buffer full flag

            OLED_Clear();
            uint32_t i ,showy= 0;



            myadc_Start();
        }





    }
}


//uin16_t find_max(uint16_t max)
//{
//
//}


//DMA_TRIGGER_ADCAEVT



void am_peak()
{

}





//按键处理函数
void key_process(uint16_t keynum)
{
    if(keynum == 16)
    {
        g_fre = 1;
        fre_control();

        OLED_Clear();
    }
    else if(keynum == 15)
    {
        g_fre = 2;
        fre_control();

        OLED_Clear();
    }
    else if(keynum == 14)
    {
        g_fre = 3;
        fre_control();

        OLED_Clear();
    }
    else if(keynum == 12)
    {
        g_fre = 4;
        fre_control();

        OLED_Clear();
    }
    else if(keynum == 11)
    {
        g_fre = 5;
        fre_control();

        OLED_Clear();
    }

    else if(keynum == 13)
    {
        g_am = 1;
        am_control();

        OLED_Clear();
    }
    else if(keynum == 9)
    {
        g_am = 2;
        am_control();

        OLED_Clear();
    }
    else if(keynum == 5)
    {
        g_am = 3;
        am_control();

        OLED_Clear();
    }




}


//频率控制函数 控制gpio
void fre_control()
{
    if(g_fre == 1)
    {

    }
    else if(g_fre == 2)
    {

    }
    else if(g_fre == 3)
    {

    }
    else if(g_fre == 4)
    {

    }
    else if(g_fre == 5)
    {

    }

}

//频率控制gpio初始化
void fre_control_init()
{



}




void am_control()
{
    if(g_am == 1)
    {
        tpl0401_set(AM1DATA);
//        tpl0401date+=10;
//        if(tpl0401date >=128)
//            tpl0401date = 0;
//        tpl0401_set(tpl0401date);

    }
    else if(g_am == 2)
    {

        tpl0401_set(AM2DATA);
//        tpl0401date-=10;
//        if(tpl0401date <=10)
//            tpl0401date = 127;
//        tpl0401_set(tpl0401date);
    }
    else if(g_am == 3)
    {
        tpl0401_set(AM3DATA);
    }


}


