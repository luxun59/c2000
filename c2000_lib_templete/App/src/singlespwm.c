/*
 * singlespwm.c
 *
 *  Created on: 2022年8月16日
 *      Author: 14913
 */

#include "singlespwm.h"




//uint32_t g_spwm_index=0;
//
//#pragma DATA_SECTION(spwmtable, "ramgs1");
//uint16_t spwmtable[MYSPWMSIZE]={32768,32973,33179,33385,33591,33797,34003,34208,
//                      34414,34619,34825,35030,35236,35441,35646,35851,
//                      36056,36261,36466,36670,36874,37079,37283,37486,
//                      37690,37894,38097,38300,38503,38705,38908,39110,
//                      39312,39513,39715,39916,40116,40317,40517,40717,
//                      40917,41116,41315,41513,41712,41909,42107,42304,
//                      42501,42697,42893,43089,43284,43479,43673,43867,
//                      44061,44254,44446,44639,44830,45021,45212,45402,
//                      45592,45781,45970,46158,46346,46533,46719,46905,
//                      47091,47276,47460,47644,47827,48010,48192,48373,
//                      48554,48734,48913,49092,49270,49448,49625,49801,
//                      49976,50151,50325,50499,50672,50844,51015,51186,
//                      51356,51525,51693,51861,52028,52194,52360,52524,
//                      52688,52851,53014,53175,53336,53496,53655,53813,
//                      53970,54127,54282,54437,54591,54745,54897,55048,
//                      55199,55348,55497,55645,55792,55938,56083,56227,
//                      56371,56513,56654,56795,56934,57073,57210,57347,
//                      57483,57617,57751,57884,58016,58146,58276,58405,
//                      58533,58659,58785,58910,59033,59156,59277,59398,
//                      59517,59636,59753,59869,59984,60099,60212,60324,
//                      60434,60544,60653,60760,60867,60972,61077,61180,
//                      61282,61383,61482,61581,61678,61775,61870,61964,
//                      62057,62149,62239,62329,62417,62504,62590,62675,
//                      62758,62840,62922,63002,63080,63158,63234,63310,
//                      63384,63456,63528,63598,63667,63735,63802,63867,
//                      63932,63995,64056,64117,64176,64234,64291,64347,
//                      64401,64454,64506,64557,64606,64654,64701,64746,
//                      64791,64834,64875,64916,64955,64993,65030,65065,
//                      65099,65132,65164,65194,65223,65251,65277,65302,
//                      65326,65349,65370,65390,65409,65426,65442,65457,
//                      65471,65483,65494,65504,65512,65519,65525,65530,
//                      65533,65535,65536,65535,65533,65530,65525,65519,
//                      65512,65504,65494,65483,65471,65457,65442,65426,
//                      65409,65390,65370,65349,65326,65302,65277,65251,
//                      65223,65194,65164,65132,65099,65065,65030,64993,
//                      64955,64916,64875,64834,64791,64746,64701,64654,
//                      64606,64557,64506,64454,64401,64347,64291,64234,
//                      64176,64117,64057,63995,63932,63868,63802,63735,
//                      63667,63598,63528,63456,63384,63310,63234,63158,
//                      63080,63002,62922,62841,62758,62675,62590,62504,
//                      62417,62329,62239,62149,62057,61964,61870,61775,
//                      61678,61581,61482,61383,61282,61180,61077,60972,
//                      60867,60761,60653,60544,60434,60324,60212,60099,
//                      59985,59869,59753,59636,59517,59398,59277,59156,
//                      59033,58910,58785,58659,58533,58405,58276,58146,
//                      58016,57884,57751,57618,57483,57347,57211,57073,
//                      56934,56795,56654,56513,56371,56227,56083,55938,
//                      55792,55645,55497,55348,55199,55048,54897,54744,
//                      54591,54437,54282,54127,53970,53813,53655,53496,
//                      53336,53175,53013,52851,52688,52524,52360,52194,
//                      52028,51861,51693,51525,51356,51186,51015,50844,
//                      50672,50499,50325,50151,49976,49801,49625,49448,
//                      49270,49092,48913,48734,48553,48373,48191,48009,
//                      47827,47644,47460,47276,47091,46905,46719,46533,
//                      46346,46158,45970,45781,45592,45402,45212,45021,
//                      44830,44638,44446,44254,44061,43867,43673,43479,
//                      43284,43089,42893,42697,42501,42304,42107,41909,
//                      41711,41513,41314,41116,40916,40717,40517,40317,
//                      40116,39915,39714,39513,39311,39109,38907,38705,
//                      38502,38300,38096,37893,37690,37486,37282,37078,
//                      36874,36670,36465,36261,36056,35851,35646,35441,
//                      35235,35030,34825,34619,34414,34208,34002,33796,
//                      33591,33385,33179,32973,32767,32561,32355,32149,
//                      31944,31738,31532,31326,31121,30915,30710,30504,
//                      30299,30093,29888,29683,29478,29274,29069,28864,
//                      28660,28456,28252,28048,27844,27641,27438,27235,
//                      27032,26829,26627,26425,26223,26021,25820,25619,
//                      25418,25218,25017,24817,24618,24419,24220,24021,
//                      23823,23625,23427,23230,23033,22837,22641,22445,
//                      22250,22055,21861,21667,21474,21281,21088,20896,
//                      20704,20513,20322,20132,19942,19753,19564,19376,
//                      19189,19002,18815,18629,18444,18259,18074,17891,
//                      17707,17525,17343,17162,16981,16801,16621,16442,
//                      16264,16087,15910,15734,15558,15383,15209,15035,
//                      14863,14691,14519,14349,14179,14009,13841,13673,
//                      13506,13340,13175,13010,12846,12683,12521,12359,
//                      12199,12039,11880,11722,11564,11408,11252,11097,
//                      10943,10790,10638,10486,10336,10186,10037,9889,
//                      9742,9596,9451,9307,9164,9021,8880,8740,
//                      8600,8462,8324,8187,8052,7917,7783,7651,
//                      7519,7388,7258,7130,7002,6875,6749,6625,
//                      6501,6379,6257,6137,6017,5899,5781,5665,
//                      5550,5436,5323,5211,5100,4990,4882,4774,
//                      4668,4562,4458,4355,4253,4152,4052,3954,
//                      3856,3760,3665,3571,3478,3386,3295,3206,
//                      3118,3031,2945,2860,2776,2694,2613,2533,
//                      2454,2377,2300,2225,2151,2078,2007,1936,
//                      1867,1799,1733,1667,1603,1540,1478,1418,
//                      1358,1300,1243,1188,1134,1081,1029,978,
//                      929,881,834,788,744,701,659,619,
//                      580,542,505,470,436,403,371,341,
//                      312,284,258,233,209,186,165,145,
//                      126,109,93,78,64,52,41,31,
//                      23,16,10,5,2,0,0,0,
//                      2,5,10,16,23,31,41,52,
//                      64,78,93,109,126,145,165,186,
//                      209,233,258,285,312,341,372,403,
//                      436,470,506,542,580,619,660,702,
//                      745,789,835,881,929,979,1029,1081,
//                      1134,1189,1244,1301,1359,1418,1479,1541,
//                      1604,1668,1733,1800,1868,1937,2008,2079,
//                      2152,2226,2301,2378,2455,2534,2614,2695,
//                      2777,2861,2946,3032,3119,3207,3296,3387,
//                      3479,3572,3666,3761,3857,3955,4053,4153,
//                      4254,4356,4459,4563,4669,4775,4883,4992,
//                      5101,5212,5324,5437,5551,5667,5783,5900,
//                      6019,6138,6259,6380,6503,6626,6751,6877,
//                      7003,7131,7260,7390,7520,7652,7785,7919,
//                      8053,8189,8326,8463,8602,8741,8882,9023,
//                      9166,9309,9453,9598,9744,9891,10039,10188,
//                      10337,10488,10639,10792,10945,11099,11254,11409,
//                      11566,11723,11882,12041,12201,12361,12523,12685,
//                      12848,13012,13177,13342,13508,13675,13843,14011,
//                      14181,14351,14521,14693,14865,15037,15211,15385,
//                      15560,15736,15912,16089,16266,16444,16623,16803,
//                      16983,17164,17345,17527,17710,17893,18076,18261,
//                      18446,18631,18817,19004,19191,19379,19567,19755,
//                      19945,20134,20325,20515,20706,20898,21090,21283,
//                      21476,21669,21863,22058,22253,22448,22643,22839,
//                      23036,23233,23430,23627,23825,24023,24222,24421,
//                      24620,24820,25020,25220,25420,25621,25822,26024,
//                      26225,26427,26629,26832,27034,27237,27440,27643,
//                      27847,28050,28254,28458,28663,28867,29071,29276,
//                      29481,29686,29891,30096,30301,30507,30712,30917,
//                      31123,31329,31534,31740,31946,32152,32358,32564
//                      };
//
//
//
//
////
//// Globals
////
//EPWM_SignalParams pwmSignal =
//            {50000, 0.5f, 0.5f, true, DEVICE_SYSCLK_FREQ,
//            EPWM_COUNTER_MODE_UP_DOWN, EPWM_CLOCK_DIVIDER_1,
//            EPWM_HSCLOCK_DIVIDER_1};
//
//
//
//#pragma CODE_SECTION(epwm1ISR, ".TI.ramfunc");
//__interrupt void epwm1ISR(void)
//{
//
//    g_spwm_index++;
//    if(g_spwm_index>=MYSPWMSIZE)
//    {
//        g_spwm_index=0;
//    }
//    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, (uint16_t)(((float32_t)MYSPWMSIZE -((float32_t)( spwmtable[g_spwm_index]/0xffff * MYSPWMAM*MYSPWMSIZE))) + 0.5f));
//    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, (uint16_t)((float32_t)( spwmtable[g_spwm_index]/0xffff * MYSPWMAM*MYSPWMSIZE) + 0.5f));
////    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, Period/100*duty);
////    EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_B, Period/100*duty);
////    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_A, Period/100*duty);
////    EPWM_setCounterCompareValue(myEPWM3_BASE, EPWM_COUNTER_COMPARE_B, Period/100*duty);
//
//
//
//
//
//    //
//    // Clear INT flag for this timer
//    //
//    EPWM_clearEventTriggerInterruptFlag(myEPWM1_BASE);
//
//    //
//    // Acknowledge this interrupt to receive more interrupts from group 3
//    //
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
//}
//
//
//
//
//
//
//
//void SingleSPWM_init()
//{
//    //
//    // EPWM1 -> myEPWM1 Pinmux
//    //
//    GPIO_setPinConfig(GPIO_0_EPWM1_A);
//    GPIO_setPinConfig(GPIO_1_EPWM1_B);
//
//
//
//
//    EPWM_setClockPrescaler(myEPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);
//    EPWM_setTimeBasePeriod(myEPWM1_BASE, 0);
//    EPWM_setTimeBaseCounter(myEPWM1_BASE, 0);
//    EPWM_setTimeBaseCounterMode(myEPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
//    EPWM_disablePhaseShiftLoad(myEPWM1_BASE);
//    EPWM_setPhaseShift(myEPWM1_BASE, 0);
//    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, 0);
//    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
//    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, 0);
//    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
//    EPWM_setActionQualifierAction(myEPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
//
//    EPWM_setDeadBandDelayPolarity(myEPWM1_BASE, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
//    EPWM_setDeadBandDelayPolarity(myEPWM1_BASE, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);
//    EPWM_setDeadBandDelayMode(myEPWM1_BASE, EPWM_DB_RED, true);
//    EPWM_setRisingEdgeDelayCount(myEPWM1_BASE, 20);
//    EPWM_setDeadBandDelayMode(myEPWM1_BASE, EPWM_DB_FED, true);
//    EPWM_setFallingEdgeDelayCount(myEPWM1_BASE, 20);
//
//
//
//
////
//    // Configuring ePWM module for desired frequency and duty
//    //
//    EPWM_configureSignal(myEPWM1_BASE, &pwmSignal);
//
//    //
//        // Configure phase between PWM1, PWM2 & PWM3.
//        // PWM1 is configured as master and ePWM2 & 3
//        // are configured as slaves.
//        //
//        EPWM_disablePhaseShiftLoad(myEPWM1_BASE);
//        EPWM_setPhaseShift(myEPWM1_BASE, 0U);
//
//        //
//        // ePWM1 SYNCO is generated on CTR=0
//        //
//        EPWM_setSyncOutPulseMode(myEPWM1_BASE, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);
//
//        //
//        // Enable sync and clock to PWM
//        //
//        SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
//
//        //
//        // Enable interrupts required for this example
//        //
//        Interrupt_enable(INT_EPWM1);
//
//        Interrupt_register(INT_EPWM1, &epwm1ISR);
//
//}
//
//
//
//
//
//
//
