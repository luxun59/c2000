/*
 * myadc.c
 *
 *  Created on: 2022年7月19日
 *      Author: 14913
 */


#include "device.h"
#include "driverlib.h"
#include "myadc.h"
#include "include.h"

#pragma DATA_SECTION(myDMAADC0Results, "ramgs1");
uint16_t myDMAADC0Results[RESULTS_BUFFER_SIZE];   // Buffer for results

volatile uint16_t bufferFull;                // Flag to indicate buffer is full


__interrupt void adcA1ISR(void);

__interrupt void dmach1ISR(void);


//
// adcA1ISR - ADC A Interrupt 1 ISR
//
#pragma CODE_SECTION(adcA1ISR, ".TI.ramfunc");
__interrupt void adcA1ISR(void)
{

    //
    // Clear the interrupt flag
    //
    ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);
    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INT_myADC0_1_INTERRUPT_ACK_GROUP);
}




//
// dmach1ISR - This is called at the end of the DMA transfer, the conversions
//              are stopped by removing the trigger of the first SOC from
//              the last.
//
#pragma CODE_SECTION(dmach1ISR, ".TI.ramfunc");
__interrupt void dmach1ISR(void)
{
    //
    // Stop the ADC by removing the trigger for SOC0
    //
//    ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER0,
//                               ADC_INT_SOC_TRIGGER_NONE);

    bufferFull = 1;

    //
    // Acknowledge interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
}



//
// Function to configure ePWM1 to generate the SOC.
//
void configureADCEPWM(uint32_t myepwm_base,uint32_t frequence ,uint16_t duty)
{
    uint32_t Period = 100000000/frequence;
    //
    // Disable SOCA
    //
    EPWM_disableADCTrigger(myepwm_base, EPWM_SOC_A);

    //
    // Configure the SOC to occur on the first up-count event
    //
    EPWM_setADCTriggerSource(myepwm_base, EPWM_SOC_A, EPWM_SOC_TBCTR_U_CMPA);
    EPWM_setADCTriggerEventPrescale(myepwm_base, EPWM_SOC_A, 1);

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

    //EPWM1_BASE
    // Freeze the counter
    //
    EPWM_setTimeBaseCounterMode(myepwm_base, EPWM_COUNTER_MODE_STOP_FREEZE);
}






// using  A3 A4 A6
void myadcdma_config()
{
    // Set the analog voltage reference selection to external.
    ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
    ASysCtl_setAnalogReference1P65(ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC);

    /***********ADC_init***********************************************/
    // ADC Initialization: Write ADC configurations and power up the ADC
    // Configures the ADC module's offset trim
    ADC_setOffsetTrimAll(ADC_REFERENCE_INTERNAL,ADC_REFERENCE_3_3V);
    // Configures the analog-to-digital converter module prescaler.
    ADC_setPrescaler(myADC0_BASE, ADC_CLK_DIV_2_0);
    // Sets the timing of the end-of-conversion pulse
    ADC_setInterruptPulseMode(myADC0_BASE, ADC_PULSE_END_OF_CONV);
    // Powers up the analog-to-digital converter core.
    ADC_enableConverter(myADC0_BASE);
    // Delay for 1ms to allow ADC time to power up
    DEVICE_DELAY_US(5000);

    // SOC Configuration: Setup ADC EPWM channel and trigger settings
    // Disables SOC burst mode.
    ADC_disableBurstMode(myADC0_BASE);
    // Sets the priority mode of the SOCs.
    ADC_setSOCPriority(myADC0_BASE, ADC_PRI_ALL_ROUND_ROBIN);
    // Start of Conversion 0 Configuration
    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 8 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    ADC_setupSOC(myADC0_BASE, ADC_SOC_NUMBER0, myADC0_TRIGGER_SOCA, ADC_CH_ADCIN3, 8U);
    ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
//    // Start of Conversion 0 Configuration
//    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
//    //      SOC number      : 1
//    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
//    //      Channel         : ADC_CH_ADCIN4
//    //      Sample Window   : 8 SYSCLK cycles
//    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
//    ADC_setupSOC(myADC0_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN4, 8U);
//    ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
//    // Start of Conversion 0 Configuration
//    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
//    //      SOC number      : 2
//    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
//    //      Channel         : ADC_CH_ADCIN6
//    //      Sample Window   : 8 SYSCLK cycles
//    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
//    ADC_setupSOC(myADC0_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN6, 8U);
//    ADC_setInterruptSOCTrigger(myADC0_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);


    // ADC Interrupt 1 Configuration
    //      SOC/EOC number  : 0
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    ADC_setInterruptSource(myADC0_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_enableInterrupt(myADC0_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(myADC0_BASE, ADC_INT_NUMBER1);

   /***********INTERRUPT_init***********************************************/
        // Interrupt Setings for INT_myADC0_1
    Interrupt_register(INT_myADC0_1, &adcA1ISR);
    Interrupt_enable(INT_myADC0_1);

   /***********epwmset****************************************************/
     // Set up the ePWM
     configureADCEPWM(myADC0_epwmbase,1000000,50);

     //
     // Initialize results buffer
     //
     uint32_t index;
     for(index = 0; index < RESULTS_BUFFER_SIZE; index++)
     {
         myDMAADC0Results[index] = 0;
     }

     /***********dma set****************************************************/
         // Initialize the DMA & configure DMA channels 1 & 2
         // Perform a hard reset on DMA
         //
         DMA_initController();

         //
         // Allow DMA to run free on emulation suspend
         //
         DMA_setEmulationMode(DMA_EMULATION_FREE_RUN);


         // DMA channel 1 set up for ADCA
         //
         DMA_configAddresses(DMA_CH1_BASE, (uint16_t *)&myDMAADC0Results,
                             (uint16_t *)myADC0_RESULT_BASE);

         //
         // Perform enough 16-word bursts to fill the results buffer. Data will be
         // transferred 32 bits at a time hence the address steps below.
         //
//         DMA_configBurst(DMA_CH1_BASE, 3, 1, 1);  //三通道
//         DMA_configTransfer(DMA_CH1_BASE, RESULTS_BUFFER_SIZE/3, -2, 1);
         DMA_configBurst(DMA_CH1_BASE, 1, 0, 0);
         DMA_configTransfer(DMA_CH1_BASE, RESULTS_BUFFER_SIZE, 0, 1);

         DMA_configMode(DMA_CH1_BASE, DMA_TRIGGER_ADCA1,
                        (DMA_CFG_ONESHOT_DISABLE | DMA_CFG_CONTINUOUS_DISABLE |
                         DMA_CFG_SIZE_16BIT));

         DMA_enableTrigger(DMA_CH1_BASE);
         DMA_disableOverrunInterrupt(DMA_CH1_BASE);
         DMA_setInterruptMode(DMA_CH1_BASE, DMA_INT_AT_END);
         DMA_enableInterrupt(DMA_CH1_BASE);

         Interrupt_register(INT_DMA_CH1, &dmach1ISR);
         Interrupt_enable(INT_DMA_CH1);
         //
         // Clearing all pending interrupt flags
         //
         DMA_clearTriggerFlag(DMA_CH1_BASE);   // DMA channel 1
         HWREGH(myADC0_BASE + ADC_O_INTFLGCLR) = 0x3U; // ADCA
 //        EPWM_forceADCTriggerEventCountInit(myADC0_BASE, EPWM_SOC_A); // EPWM2 SOCA

         DMA_startChannel(DMA_CH1_BASE);
         EPWM_enableADCTrigger(myADC0_BASE, EPWM_SOC_A);
}



void myadc_Start()
{
    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    DMA_startChannel(DMA_CH1_BASE);
    EPWM_enableADCTrigger(myADC0_epwmbase, EPWM_SOC_A);
    EPWM_setTimeBaseCounterMode(myADC0_epwmbase, EPWM_COUNTER_MODE_UP);
}

void myadc_Stop()
{
    //
    // Stop ePWM1, disabling SOCA and freezing the counter
    //
    DMA_stopChannel(DMA_CH1_BASE);
    EPWM_disableADCTrigger(myADC0_epwmbase, EPWM_SOC_A);
    EPWM_setTimeBaseCounterMode(myADC0_epwmbase, EPWM_COUNTER_MODE_STOP_FREEZE);
}




