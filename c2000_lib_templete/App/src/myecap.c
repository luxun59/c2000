/*
 * myecap.c
 *
 *  Created on: 2022骞?鏈?鏃?
 *      Author: 14913
 */


#include "include.h"

#include "myecap.h"

#define myECAP3_BASE ECAP3_BASE
#define INT_myECAP3 INT_ECAP3
#define INT_myECAP3_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP4


#define MYECAP3_GPIO  GPIO_16_GPIO16
#define MYECAP3_PIN 16



void MYECAP3GPIO_init();
void MYECAP3INPUTXBAR_init();
void MYECAP3INTERRUPT_init();


//
// Globals
//
uint32_t ecap3IntCount;
uint32_t ecapPassCount;
uint32_t epwm3TimerDirection;
volatile uint16_t epwm3PeriodCount;
// TODO: New for example 4, remove comment later




volatile uint16_t ecap3Cap1Count[ECAP3MAXNUM];



//
// myECAP3 ISR
//
__interrupt void INT_myECAP3_ISR(void)
{
    //
    // Get the capture counts. Each capture should match for all the eCAPs.
    //
    if(ecap3IntCount)
        ecap3Cap1Count[ecap3IntCount-1] = ECAP_getEventTimeStamp(myECAP3_BASE, ECAP_EVENT_1);
//    ecap3Cap2Count = ECAP_getEventTimeStamp(myECAP3_BASE, ECAP_EVENT_2);
//    ecap3Cap3Count = ECAP_getEventTimeStamp(myECAP3_BASE, ECAP_EVENT_3);
//    ecap3Cap4Count = ECAP_getEventTimeStamp(myECAP3_BASE, ECAP_EVENT_4);

    //
    // Validate synchronization.
    //

    ecap3IntCount++;

    //
    // Count correct captures
    //
    ecapPassCount++;

    //
    // Clear interrupt flags for more interrupts.
    //
    ECAP_clearInterrupt(myECAP3_BASE,ECAP_ISR_SOURCE_CAPTURE_EVENT_1);
    ECAP_clearGlobalInterrupt(myECAP3_BASE);

    //
    // Acknowledge the group interrupt for more interrupts.
    //
    Interrupt_clearACKGroup(INT_myECAP3_INTERRUPT_ACK_GROUP);

    if(ecap3IntCount >= ECAP3MAXNUM+1)
    {
        ECAP_disableTimeStampCapture(myECAP3_BASE);
        ECAP_stopCounter(myECAP3_BASE);
    }
    else
    {
        //
        // Start eCAP
        //

        ECAP_reArm(myECAP3_BASE);
    }


}









void ECAP_init()
{

    MYECAP3GPIO_init();
    MYECAP3INPUTXBAR_init();

    //myECAP3 initialization
    // Disable ,clear all capture flags and interrupts
    ECAP_disableInterrupt(myECAP3_BASE,
        (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
         ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
         ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
         ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
         ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
         ECAP_ISR_SOURCE_COUNTER_PERIOD   |
         ECAP_ISR_SOURCE_COUNTER_COMPARE));
    ECAP_clearInterrupt(myECAP3_BASE,
        (ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
        ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
        ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
        ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
        ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
        ECAP_ISR_SOURCE_COUNTER_PERIOD   |
        ECAP_ISR_SOURCE_COUNTER_COMPARE));
    // Disables time stamp capture.
    ECAP_disableTimeStampCapture(myECAP3_BASE);
    // Stops Time stamp counter.
    ECAP_stopCounter(myECAP3_BASE);
    // Sets eCAP in Capture mode.
    ECAP_enableCaptureMode(myECAP3_BASE);
    // Sets the capture mode.
    ECAP_setCaptureMode(myECAP3_BASE,ECAP_ONE_SHOT_CAPTURE_MODE,ECAP_EVENT_1);
    // Sets the Capture event prescaler.
    ECAP_setEventPrescaler(myECAP3_BASE, 1U);
    // Sets the Capture event polarity.
    ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_1,ECAP_EVNT_FALLING_EDGE);
//    ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_2,ECAP_EVNT_RISING_EDGE);
//    ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_3,ECAP_EVNT_FALLING_EDGE);
//    ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_4,ECAP_EVNT_RISING_EDGE);
    // Configure counter reset on events
    ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_1);
//    ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_2);
//    ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_3);
//    ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_4);
    // Select eCAP input.
    ECAP_selectECAPInput(myECAP3_BASE,ECAP_INPUT_INPUTXBAR7);
    // Sets a phase shift value count.
    ECAP_setPhaseShiftCount(myECAP3_BASE,0U);
    // Enable counter loading with phase shift value.
    ECAP_enableLoadCounter(myECAP3_BASE);
    // Configures Sync out signal mode.
    ECAP_setSyncOutMode(myECAP3_BASE,ECAP_SYNC_OUT_SYNCI);
    // Configures emulation mode.d
    ECAP_setEmulationMode(myECAP3_BASE,ECAP_EMULATION_STOP);

    // Starts Time stamp counter for myECAP3.
    ECAP_startCounter(myECAP3_BASE);
    // Enables time stamp capture for myECAP3.
    ECAP_enableTimeStampCapture(myECAP3_BASE);
    // Re-arms the eCAP module for myECAP3.
    ECAP_reArm(myECAP3_BASE);

    // Enables interrupt source for myECAP3.
    ECAP_enableInterrupt(myECAP3_BASE,(ECAP_ISR_SOURCE_CAPTURE_EVENT_1));

    MYECAP3INTERRUPT_init();

}



void MYECAP3GPIO_init()
{
    // GPIO16 -> myGPIO0 Pinmux
    GPIO_setPinConfig(MYECAP3_GPIO);

    //myGPIO0 initialization
    GPIO_setDirectionMode(MYECAP3_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(MYECAP3_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(MYECAP3_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(MYECAP3_PIN, GPIO_QUAL_ASYNC);



}



void MYECAP3INPUTXBAR_init()
{

    //myINPUTXBAR0 initialization
    XBAR_setInputPin(XBAR_INPUT7, 16);
}


void MYECAP3INTERRUPT_init()
{

    // Interrupt Setings for INT_myECAP3
    Interrupt_register(INT_myECAP3, &INT_myECAP3_ISR);
    Interrupt_enable(INT_myECAP3);
}



void MYCAP3_restart()
{
    // Starts Time stamp counter for myECAP3.
        ECAP_startCounter(myECAP3_BASE);
        // Enables time stamp capture for myECAP3.
        ECAP_enableTimeStampCapture(myECAP3_BASE);
        // Re-arms the eCAP module for myECAP3.
        ECAP_reArm(myECAP3_BASE);
}



