/*
 * mycmpss.c
 *
 *  Created on: 2022年7月20日
 *      Author: 14913
 */

#include "mycmpss.h"





void    CMPSS_init();
void    cmpssOUTPUTXBAR_init();
void    cmpss_gpioint_init();


volatile uint16_t g_flag_cmpss = 0;



//
// Interrupt Handler
//


#pragma CODE_SECTION(cmpssgpioInterruptHandler, ".TI.ramfunc");
__interrupt void cmpssgpioInterruptHandler(void)
{
    g_flag_cmpss = 1;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    GPIO_togglePin(MYLED4);
    Interrupt_disable(INT_XINT1);
}



void mycmpss_init(uint32_t highvoltage,uint32_t lowvoltage)
{

    CMPSS_init(highvoltage,lowvoltage);
    cmpssOUTPUTXBAR_init();
//    cmpss_gpioint_init();
}




void CMPSS_init(uint32_t highvoltage,uint32_t lowvoltage){
    // Select the value for CMPHNMXSEL.
    ASysCtl_selectCMPHNMux(ASYSCTL_CMPHNMUX_SELECT_1);
    // Select the value for CMPLNMXSEL.
    ASysCtl_selectCMPLNMux(ASYSCTL_CMPLNMUX_SELECT_1);
    // Select the value for CMPHPM[object Object]SEL.
    ASysCtl_selectCMPHPMux(ASYSCTL_CMPHPMUX_SELECT_1,0U);
    // Select the value for CMPLPM[object Object]SEL.
    ASysCtl_selectCMPLPMux(ASYSCTL_CMPLPMUX_SELECT_1,0U);

    //myCMPSS0 initialization
    // Sets the configuration for the high comparator.
    CMPSS_configHighComparator(myCMPSS0_BASE,(CMPSS_INSRC_DAC));
    // Sets the configuration for the high comparator.
    CMPSS_configLowComparator(myCMPSS0_BASE,(CMPSS_INSRC_DAC|CMPSS_INV_INVERTED));
    // Sets the configuration for the internal comparator DACs.
    CMPSS_configDAC(myCMPSS0_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    // Sets the value of the internal DAC of the high comparator.
    CMPSS_setDACValueHigh(myCMPSS0_BASE,highvoltage);
    // Sets the value of the internal DAC of the low comparator.
    CMPSS_setDACValueLow(myCMPSS0_BASE,lowvoltage);
    //  Configures the digital filter of the high comparator.
    CMPSS_configFilterHigh(myCMPSS0_BASE, 0U, 1U, 1U);
    // Configures the digital filter of the low comparator.
    CMPSS_configFilterLow(myCMPSS0_BASE, 0U, 1U, 1U);
    // Sets the output signal configuration for the high comparator.
    CMPSS_configOutputsHigh(myCMPSS0_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    // Sets the output signal configuration for the low comparator.
    CMPSS_configOutputsLow(myCMPSS0_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    // Sets the comparator hysteresis settings.
    CMPSS_setHysteresis(myCMPSS0_BASE,0U);
    // Configures the comparator subsystem's ramp generator.
    CMPSS_configRamp(myCMPSS0_BASE,0U,0U,0U,1U,true);
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    CMPSS_disableLatchResetOnPWMSYNCHigh(myCMPSS0_BASE);
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    CMPSS_disableLatchResetOnPWMSYNCLow(myCMPSS0_BASE);
    // Sets the ePWM module blanking signal that holds trip in reset.
    CMPSS_configBlanking(myCMPSS0_BASE,1U);
    // Disables an ePWM blanking signal from holding trip in reset.
    CMPSS_disableBlanking(myCMPSS0_BASE);
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    CMPSS_configLatchOnPWMSYNC(myCMPSS0_BASE,false,false);
    // Enables the CMPSS module.
    CMPSS_enableModule(myCMPSS0_BASE);
    // Delay for CMPSS DAC to power up.
    DEVICE_DELAY_US(500);


}


void cmpssOUTPUTXBAR_init(){

    //
    // OUTPUTXBAR3 -> myOUTPUTXBAR0 Pinmux
    //
    GPIO_setPinConfig(GPIO_31_OUTPUTXBAR8);

    //myOUTPUTXBAR0 initialization
    XBAR_setOutputLatchMode(XBAR_OUTPUT8, false);
    XBAR_invertOutputSignal(XBAR_OUTPUT8, false);

    //Mux configuration
    XBAR_setOutputMuxConfig(XBAR_OUTPUT8, XBAR_OUT_MUX00_CMPSS1_CTRIPOUTH);
    XBAR_enableOutputMux(XBAR_OUTPUT8, XBAR_MUX00);
}


void cmpss_gpioint_init()
{
    EALLOW;
    // GPIO28 -> myGPIOInputInterrupt0 Pinmux
    GPIO_setPinConfig(cmpssGPIOconfig);
    //myGPIOInputInterrupt0 initialization
    GPIO_setDirectionMode(cmpssGPIOInputInterrupt0, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(cmpssGPIOInputInterrupt0, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(cmpssGPIOInputInterrupt0, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(cmpssGPIOInputInterrupt0, GPIO_QUAL_SYNC);


    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_RISING_EDGE);
    GPIO_setInterruptPin(cmpssGPIOInputInterrupt0, GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT1);

    Interrupt_register(INT_XINT1, &cmpssgpioInterruptHandler);
    Interrupt_enable(INT_XINT1);

    EDIS;
}






