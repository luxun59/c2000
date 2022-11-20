/*
 * myfft.c
 *
 *  Created on: 2022年8月5日
 *      Author: 14913
 */


#include <stdint.h>
//#include "fpu.h"
#include "dsp.h"
#include "fpu32/fpu_rfft.h"

#include "include.h"
//
#include "myfft.h"


//*****************************************************************************
// the globals
//*****************************************************************************


// Object of the structure RFFT_F32_STRUCT
RFFT_F32_STRUCT rfft;
// Handle to the RFFT_F32_STRUCT object
RFFT_F32_STRUCT_Handle hnd_rfft = &rfft;

// Object of the structure RFFT_ADC_F32_STRUCT
RFFT_ADC_F32_STRUCT rfft_adc;
// Handle to the RFFT_ADC_F32_STRUCT object
RFFT_ADC_F32_STRUCT_Handle hnd_rfft_adc = &rfft_adc;




//#pragma DATA_SECTION(RFFToutBuff,"CFFToutBuff");
//
//#pragma DATA_SECTION(test_input,"test_input");
//
//#pragma DATA_SECTION(test_magnitude,"test_magnitude");
//
//#pragma DATA_SECTION(test_phase,"test_phase");
//
//#pragma DATA_SECTION(twiddleFactors,"twiddleFactors");



#pragma DATA_SECTION(RFFToutBuff,"ramgs1");

#pragma DATA_SECTION(test_input,"ramgs1");

#pragma DATA_SECTION(test_magnitude,"ramgs1");

#pragma DATA_SECTION(test_phase,"ramgs1");

#pragma DATA_SECTION(twiddleFactors,"ramgs1");




float32_t RFFToutBuff[FFT_SIZE];              //定义FFT处理复数结果数据

float32_t test_input[FFT_SIZE];
//float test_magnitude_phase[(TEST_SIZE >> 1) + 1];

float32_t test_magnitude[FFT_SIZE/2+1 ];
float32_t test_phase[FFT_SIZE/2+1];
float32_t twiddleFactors[TEST_SIZE];




void FFT_runTest()
{

    testfftdata();
    float *p_temp;

    // Configure the object
    RFFT_f32_setInputPtr(hnd_rfft,(float *) test_input);
    RFFT_f32_setOutputPtr(hnd_rfft, RFFToutBuff);
    RFFT_f32_setMagnitudePtr(hnd_rfft, test_magnitude);
    RFFT_f32_setPhasePtr(hnd_rfft, test_phase);
    RFFT_f32_setStages(hnd_rfft,  FFT_STAGES);
    RFFT_f32_setFFTSize(hnd_rfft, FFT_SIZE);

    RFFT_f32_setTwiddlesPtr(hnd_rfft, twiddleFactors);
    // Twiddle factor pointer

    //Calculate twiddle factor
    RFFT_f32_sincostable(hnd_rfft);

    //Link the RFFT_ADC_F32_STRUCT to RFFT_F32_STRUCT. Tail pointer
    //of RFFT_ADC_F32_STRUCT must point to the OutBuf pointer of
    //RFFT_F32_STRUCT

//    RFFT_ADC_f32_setTailPtr(hnd_rfft_adc, &(hnd_rfft->OutBuf));
//    RFFT_ADC_f32_setInBufPtr(hnd_rfft_adc, test_input);

    RFFT_f32u(hnd_rfft);

//    RFFT_adc_f32u(hnd_rfft_adc);    // Calculate real FFT with 12-bit

    RFFT_f32_mag(hnd_rfft);


}




void testfftdata()
{
    uint32_t i;
    for(i=0;i<256;i++)
    {
        if( i%32 < 16)
        {
            test_input[i] = 0;
        }
        else
        {
            test_input[i] = 2000;
        }
    }
}






