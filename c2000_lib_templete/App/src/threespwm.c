/*
 * spwm.c
 *
 *  Created on: 2022年8月13日
 *      Author: 14913
 */

#include "threespwm.h"

#define MYSPWMSIZE 1000
#define MYSPWMAM  1

//! - ePWM1A is on GPIO0
//! - ePWM1B is on GPIO1
//! - ePWM2A is on GPIO2
//! - ePWM2B is on GPIO3
//! - ePWM4A is on GPIO6
//! - ePWM4B is on GPIO7


#define GPIO_PIN_EPWM1_A 0
#define GPIO_PIN_EPWM1_B 1
#define GPIO_PIN_EPWM2_A 2
#define GPIO_PIN_EPWM2_B 3
#define GPIO_PIN_EPWM4_A 6
#define GPIO_PIN_EPWM4_B 7

#define myEPWM1_BASE EPWM1_BASE
#define myEPWM2_BASE EPWM2_BASE
#define myEPWM3_BASE EPWM4_BASE


uint32_t g_spwm_index_1=0;
uint32_t g_spwm_index_2=0;
uint32_t g_spwm_index_3=0;

#pragma DATA_SECTION(spwmtable, "ramgs1");
uint16_t spwmtable[MYSPWMSIZE]={502,505,508,511,514,517,520,524,527,530,533,536,539,542,545,549,552,
                                555,558,561,564,567,570,574,577,580,583,586,589,592,595,598,601,
                                604,608,611,614,617,620,623,626,629,632,635,638,641,644,647,650,
                                653,656,659,662,665,668,671,674,677,680,683,686,688,691,694,697,
                                700,703,706,709,711,714,717,720,723,726,728,731,734,737,739,742,
                                745,748,750,753,756,759,761,764,767,769,772,775,777,780,782,785,
                                788,790,793,795,798,800,803,805,808,810,813,815,818,820,822,825,
                                827,829,832,834,837,839,841,843,846,848,850,852,855,857,859,861,
                                863,866,868,870,872,874,876,878,880,882,884,886,888,890,892,894,
                                896,898,900,902,904,905,907,909,911,913,914,916,918,920,921,923,
                                925,926,928,930,931,933,934,936,937,939,940,942,943,945,946,948,
                                949,950,952,953,954,956,957,958,959,961,962,963,964,965,967,968,
                                969,970,971,972,973,974,975,976,977,978,979,980,981,981,982,983,
                                984,985,985,986,987,988,988,989,990,990,991,991,992,993,993,994,
                                994,995,995,995,996,996,997,997,997,998,998,998,998,999,999,999,
                                999,999,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
                                999,999,999,999,999,998,998,998,998,997,997,997,996,996,995,995,
                                995,994,994,993,993,992,991,991,990,990,989,988,988,987,986,985,
                                985,984,983,982,981,981,980,979,978,977,976,975,974,973,972,971,
                                970,969,968,967,965,964,963,962,961,959,958,957,956,954,953,952,
                                950,949,948,946,945,943,942,940,939,937,936,934,933,931,930,928,
                                926,925,923,921,920,918,916,914,913,911,909,907,905,904,902,900,
                                898,896,894,892,890,888,886,884,882,880,878,876,874,872,870,868,
                                866,863,861,859,857,855,852,850,848,846,843,841,839,837,834,832,
                                829,827,825,822,820,818,815,813,810,808,805,803,800,798,795,793,
                                790,788,785,782,780,777,775,772,769,767,764,761,759,756,753,750,
                                748,745,742,739,737,734,731,728,726,723,720,717,714,711,709,706,
                                703,700,697,694,691,688,686,683,680,677,674,671,668,665,662,659,
                                656,653,650,647,644,641,638,635,632,629,626,623,620,617,614,611,
                                608,604,601,598,595,592,589,586,583,580,577,574,570,567,564,561,
                                558,555,552,549,545,542,539,536,533,530,527,524,520,517,514,511,
                                508,505,502,498,495,492,489,486,483,480,476,473,470,467,464,461,
                                458,455,451,448,445,442,439,436,433,430,426,423,420,417,414,411,
                                408,405,402,399,396,392,389,386,383,380,377,374,371,368,365,362,
                                359,356,353,350,347,344,341,338,335,332,329,326,323,320,317,314,
                                312,309,306,303,300,297,294,291,289,286,283,280,277,274,272,269,
                                266,263,261,258,255,252,250,247,244,241,239,236,233,231,228,225,
                                223,220,218,215,212,210,207,205,202,200,197,195,192,190,187,185,
                                182,180,178,175,173,171,168,166,163,161,159,157,154,152,150,148,
                                145,143,141,139,137,134,132,130,128,126,124,122,120,118,116,114,
                                112,110,108,106,104,102,100,98,96,95,93,91,89,87,86,84,
                                82,80,79,77,75,74,72,70,69,67,66,64,63,61,60,58,
                                57,55,54,52,51,50,48,47,46,44,43,42,41,39,38,37,
                                36,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,
                                19,19,18,17,16,15,15,14,13,12,12,11,10,10,9,9,
                                8,7,7,6,6,5,5,5,4,4,3,3,3,2,2,2,
                                2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,1,1,1,1,1,2,2,2,2,3,3,3,
                                4,4,5,5,5,6,6,7,7,8,9,9,10,10,11,12,
                                12,13,14,15,15,16,17,18,19,19,20,21,22,23,24,25,
                                26,27,28,29,30,31,32,33,35,36,37,38,39,41,42,43,
                                44,46,47,48,50,51,52,54,55,57,58,60,61,63,64,66,
                                67,69,70,72,74,75,77,79,80,82,84,86,87,89,91,93,
                                95,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,
                                126,128,130,132,134,137,139,141,143,145,148,150,152,154,157,159,
                                161,163,166,168,171,173,175,178,180,182,185,187,190,192,195,197,
                                200,202,205,207,210,212,215,218,220,223,225,228,231,233,236,239,
                                241,244,247,250,252,255,258,261,263,266,269,272,274,277,280,283,
                                286,289,291,294,297,300,303,306,309,312,314,317,320,323,326,329,
                                332,335,338,341,344,347,350,353,356,359,362,365,368,371,374,377,
                                380,383,386,389,392,396,399,402,405,408,411,414,417,420,423,426,
                                430,433,436,439,442,445,448,451,455,458,461,464,467,470,473,476,
                                480,483,486,489,492,495,498
                                };




//
// Globals
//
EPWM_SignalParams pwmSignal =
            {50000, 0.5f, 0.5f, true, DEVICE_SYSCLK_FREQ,
            EPWM_COUNTER_MODE_UP_DOWN, EPWM_CLOCK_DIVIDER_1,
            EPWM_HSCLOCK_DIVIDER_1};



#pragma CODE_SECTION(epwm1ISR, ".TI.ramfunc");
__interrupt void epwm1ISR(void)
{

    g_spwm_index_1++;
    if(g_spwm_index_1>=MYSPWMSIZE)
    {
        g_spwm_index_1=0;
    }
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A,  spwmtable[g_spwm_index_1] );
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, MYSPWMAM*MYSPWMSIZE-spwmtable[g_spwm_index_1]);


    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(myEPWM1_BASE);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}


#pragma CODE_SECTION(epwm2ISR, ".TI.ramfunc");
__interrupt void epwm2ISR(void)
{

    g_spwm_index_2++;
    if(g_spwm_index_2>=MYSPWMSIZE)
    {
        g_spwm_index_2=0;
    }
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A,  spwmtable[g_spwm_index_2] );
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, MYSPWMAM*MYSPWMSIZE-spwmtable[g_spwm_index_2]);


    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(myEPWM2_BASE);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}



#pragma CODE_SECTION(epwm4ISR, ".TI.ramfunc");
__interrupt void epwm4ISR(void)
{

    g_spwm_index_3++;
    if(g_spwm_index_3>=MYSPWMSIZE)
    {
        g_spwm_index_3=0;
    }
    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A,  spwmtable[g_spwm_index_3] );
    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, MYSPWMAM*MYSPWMSIZE-spwmtable[g_spwm_index_3]);


    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(myEPWM3_BASE);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}







void ThreeSPWM_init()
{
    //
    // EPWM1 -> myEPWM1 Pinmux
    //
    GPIO_setPinConfig(GPIO_0_EPWM1_A);
    GPIO_setPinConfig(GPIO_1_EPWM1_B);
    //
    // EPWM2 -> myEPWM2 Pinmux
    //
    GPIO_setPinConfig(GPIO_2_EPWM2_A);
    GPIO_setPinConfig(GPIO_3_EPWM2_B);
    //
    // EPWM3 -> myEPWM4 Pinmux
    //
    GPIO_setPinConfig(GPIO_6_EPWM4_A);
    GPIO_setPinConfig(GPIO_7_EPWM4_B);



    EPWM_setClockPrescaler(myEPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
    EPWM_setTimeBasePeriod(myEPWM1_BASE, 0);
    EPWM_setTimeBaseCounter(myEPWM1_BASE, 0);
    EPWM_setTimeBaseCounterMode(myEPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(myEPWM1_BASE);
    EPWM_setPhaseShift(myEPWM1_BASE, 0);
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    EPWM_setDeadBandDelayPolarity(myEPWM1_BASE, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
    EPWM_setDeadBandDelayPolarity(myEPWM1_BASE, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
    EPWM_setDeadBandDelayMode(myEPWM1_BASE, EPWM_DB_RED, true);
    EPWM_setRisingEdgeDelayCount(myEPWM1_BASE, 20);
    EPWM_setDeadBandDelayMode(myEPWM1_BASE, EPWM_DB_FED, true);
    EPWM_setFallingEdgeDelayCount(myEPWM1_BASE, 20);


    EPWM_setClockPrescaler(myEPWM2_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
    EPWM_setTimeBasePeriod(myEPWM2_BASE, 0);
    EPWM_setTimeBaseCounter(myEPWM2_BASE, 0);
    EPWM_setTimeBaseCounterMode(myEPWM2_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(myEPWM2_BASE);
    EPWM_setPhaseShift(myEPWM2_BASE, 0);
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM2_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    EPWM_setDeadBandDelayPolarity(myEPWM2_BASE, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
    EPWM_setDeadBandDelayPolarity(myEPWM2_BASE, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
    EPWM_setDeadBandDelayMode(myEPWM2_BASE, EPWM_DB_RED, true);
    EPWM_setRisingEdgeDelayCount(myEPWM2_BASE, 20);
    EPWM_setDeadBandDelayMode(myEPWM2_BASE, EPWM_DB_FED, true);
    EPWM_setFallingEdgeDelayCount(myEPWM2_BASE, 20);


    EPWM_setClockPrescaler(myEPWM3_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
    EPWM_setTimeBasePeriod(myEPWM3_BASE, 0);
    EPWM_setTimeBaseCounter(myEPWM3_BASE, 0);
    EPWM_setTimeBaseCounterMode(myEPWM3_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(myEPWM3_BASE);
    EPWM_setPhaseShift(myEPWM3_BASE, 0);
    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, 0);
    EPWM_setCounterCompareShadowLoadMode(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(myEPWM3_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    EPWM_setDeadBandDelayPolarity(myEPWM3_BASE, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
    EPWM_setDeadBandDelayPolarity(myEPWM3_BASE, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
    EPWM_setDeadBandDelayMode(myEPWM3_BASE, EPWM_DB_RED, true);
    EPWM_setRisingEdgeDelayCount(myEPWM3_BASE, 20);
    EPWM_setDeadBandDelayMode(myEPWM3_BASE, EPWM_DB_FED, true);
    EPWM_setFallingEdgeDelayCount(myEPWM3_BASE, 20);

//
    // Configuring ePWM module for desired frequency and duty
    //
    EPWM_configureSignal(myEPWM1_BASE, &pwmSignal);
    EPWM_configureSignal(myEPWM2_BASE, &pwmSignal);
    EPWM_configureSignal(myEPWM3_BASE, &pwmSignal);

    //
        // Configure phase between PWM1, PWM2 & PWM3.
        // PWM1 is configured as master and ePWM2 & 3
        // are configured as slaves.
        //
        EPWM_disablePhaseShiftLoad(myEPWM1_BASE);
        EPWM_setPhaseShift(myEPWM1_BASE, 0U);

        //
        // ePWM1 SYNCO is generated on CTR=0
        //
        EPWM_setSyncOutPulseMode(myEPWM1_BASE, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);

        //
        // Configure phase shift for EPWM2 & 3
        //
        configurePhase(myEPWM2_BASE, myEPWM1_BASE, 120);
        configurePhase(myEPWM3_BASE, myEPWM1_BASE, 240);


        EPWM_enablePhaseShiftLoad(myEPWM2_BASE);
        EPWM_enablePhaseShiftLoad(myEPWM3_BASE);

        //
        // Enable sync and clock to PWM
        //
        SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

        //
        // Enable interrupts required for this example
        //
        EPWM_setInterruptSource(myEPWM1_BASE, EPWM_INT_TBCTR_ZERO);
        EPWM_enableInterrupt(myEPWM1_BASE);
        EPWM_setInterruptEventCount(myEPWM1_BASE, 1);

        EPWM_setInterruptSource(myEPWM2_BASE, EPWM_INT_TBCTR_ZERO);
        EPWM_enableInterrupt(myEPWM2_BASE);
        EPWM_setInterruptEventCount(myEPWM2_BASE, 1);

        EPWM_setInterruptSource(myEPWM3_BASE, EPWM_INT_TBCTR_ZERO);
        EPWM_enableInterrupt(myEPWM3_BASE);
        EPWM_setInterruptEventCount(myEPWM3_BASE, 1);

        Interrupt_enable(INT_EPWM1);

        Interrupt_register(INT_EPWM1, &epwm1ISR);

        Interrupt_enable(INT_EPWM2);

        Interrupt_register(INT_EPWM2, &epwm2ISR);

        Interrupt_enable(INT_EPWM4);

        Interrupt_register(INT_EPWM4, &epwm4ISR);

}



//
// configurePhase - Configure ePWMx Phase
//
void configurePhase(uint32_t base, uint32_t masterBase, uint16_t phaseVal)
{
    uint32_t readPrdVal, phaseRegVal;

    //
    // Read Period value to calculate value for Phase Register
    //
    readPrdVal = EPWM_getTimeBasePeriod(masterBase);

    //
    // Caluclate phase register values based on Time Base counter mode
    //
    if((HWREGH(base + EPWM_O_TBCTL) & 0x3U) == EPWM_COUNTER_MODE_UP_DOWN)
    {
        phaseRegVal = (2U * readPrdVal * phaseVal) / 360U;
    }
    else if((HWREGH(base + EPWM_O_TBCTL) & 0x3U) < EPWM_COUNTER_MODE_UP_DOWN)
    {
        phaseRegVal = (readPrdVal * phaseVal) / 360U;
    }

    EPWM_selectPeriodLoadEvent(base, EPWM_SHADOW_LOAD_MODE_SYNC);
    EPWM_setPhaseShift(base, phaseRegVal);
    EPWM_setTimeBaseCounter(base, phaseRegVal);
}

