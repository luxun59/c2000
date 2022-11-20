/*
 * mydac.c
 *
 *  Created on: 2022年7月21日
 *      Author: 14913
 */

#include "mydac.h"

volatile uint16_t g_mydac_index=0;
#pragma DATA_SECTION(g_mydacdatabuff, "ramgs1");
uint16_t g_mydacdatabuff[MYDACBUFFSIZE]={2048,2058,2068,2078,2088,2098,2107,2117,2126,2136,2144,2153,2162,2170,2178,2185,2193,2200,2206,2212,2218,2224,2229,2233,2237,2241,2244,2247,2249,2250,2252,2252,2253,2252,2252,2250,2249,2247,2244,2241,2237,2233,2229,2224,2218,2212,2206,2200,2193,2185,2178,2170,2162,2153,2144,2136,2126,2117,2107,2098,2088,2078,2068,2058,2048,2038,2028,2018,2008,1998,1989,1979,1970,1960,1952,1943,1934,1926,1918,1911,1903,1896,1890,1884,1878,1872,1867,1863,1859,1855,1852,1849,1847,1846,1844,1844,1843,1844,1844,1846,1847,1849,1852,1855,1859,1863,1867,1872,1878,1884,1890,1896,1903,1911,1918,1926,1934,1943,1952,1960,1970,1979,1989,1998,2008,2018,2028,2038,2048,2058,2068,2078,2088,2098,2107,2117,2126,2136,2144,2153,2162,2170,2178,2185,2193,2200,2206,2212,2218,2224,2229,2233,2237,2241,2244,2247,2249,2250,2252,2252,2253,2252,2252,2250,2249,2247,2244,2241,2237,2233,2229,2224,2218,2212,2206,2200,2193,2185,2178,2170,2162,2153,2144,2136,2126,2117,2107,2098,2088,2078,2068,2058,2048,2038,2028,2018,2008,1998,1989,1979,1970,1960,1952,1943,1934,1926,1918,1911,1903,1896,1890,1884,1878,1872,1867,1863,1859,1855,1852,1849,1847,1846,1844,1844,1843,1844,1844,1846,1847,1849,1852,1855,1859,1863,1867,1872,1878,1884,1890,1896,1903,1911,1918,1926,1934,1943,1952,1960,1970,1979,1989,1998,2008,2018,2028,2038};

#pragma CODE_SECTION(epwm6ISR, ".TI.ramfunc");
__interrupt void epwm6ISR(void)
{
    DAC_setShadowValue(myDAC0_BASE, g_mydacdatabuff[g_mydac_index]);
//    DAC_setShadowValue(myDAC0_BASE, 620);
//    DAC_setShadowValue(myDAC0_BASE, g_mydac_index*4);
    g_mydac_index++;
    if(g_mydac_index>=MYDACBUFFSIZE)
    {
        g_mydac_index=0;
    }
    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(myDAC0_epwmbase);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}










void mydac_init()
{
    EALLOW;
    // asysctl initialization
    // Disables the temperature sensor output to the ADC.
    ASysCtl_disableTemperatureSensor();
    // Set the analog voltage reference selection to external.
    ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
    ASysCtl_setAnalogReference1P65(ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC);

    // myDAC0 initialization
    // Set DAC reference voltage.
    DAC_setReferenceVoltage(myDAC0_BASE, DAC_REF_ADC_VREFHI);
    // Set DAC gain.
    DAC_setGainMode(myDAC0_BASE,DAC_GAIN_TWO);
    // Set DAC load mode.
    DAC_setLoadMode(myDAC0_BASE, DAC_LOAD_SYSCLK);
    // Enable the DAC output
    DAC_enableOutput(myDAC0_BASE);
    // Set the DAC shadow output
    DAC_setShadowValue(myDAC0_BASE, 0U);

    // Delay for buffered DAC to power up.
    DEVICE_DELAY_US(500);

    EDIS;
}



//
// Function to configure ePWM1 to generate the SOC.
//
void mydac_configureEPWM(uint32_t myepwm_base,uint32_t frequence ,uint16_t duty)
{
    uint32_t Period = 100000000/frequence;
    //
    // Set the compare A value to 1000 and the period to 1999
    // Assuming ePWM clock is 100MHz, this would give 50kHz sampling
    // 50MHz ePWM clock would give 25kHz sampling, etc.
    // The sample rate can also be modulated by changing the ePWM period
    // directly (ensure that the compare A value is less than the period).
    //
    EPWM_setCounterCompareValue(myepwm_base, EPWM_COUNTER_COMPARE_A, Period/100*duty);
    EPWM_setTimeBasePeriod(myepwm_base, Period-1);

    //
    // Set the local ePWM module clock divider to /1
    //
    EPWM_setClockPrescaler(myepwm_base,
                           EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);

    // EPWM_INT_TBCTR_PERIOD EPWM_INT_TBCTR_ZERO
    // Configure interrupt on event CTR=ZERO
    //
    EPWM_setInterruptSource(myepwm_base, EPWM_INT_TBCTR_PERIOD);
    EPWM_enableInterrupt(myepwm_base);
    EPWM_setInterruptEventCount(myepwm_base, 1);

    Interrupt_register(INT_myDAC0epwm_1, &epwm6ISR);
    Interrupt_enable(INT_myDAC0epwm_1);

    // Freeze the counter
    //
    EPWM_setTimeBaseCounterMode(myepwm_base, EPWM_COUNTER_MODE_STOP_FREEZE);
}


//epwm start
//EPWM_setTimeBaseCounterMode(EPWM2_BASE, EPWM_COUNTER_MODE_UP);





void mydac_start()
{
    g_mydac_index = 0;
    EPWM_setTimeBaseCounterMode(myDAC0_epwmbase, EPWM_COUNTER_MODE_UP);
}



void mydac_stop()
{
    EPWM_setTimeBaseCounterMode(myDAC0_epwmbase, EPWM_COUNTER_MODE_STOP_FREEZE);
}




