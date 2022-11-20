/*
 * myfft.h
 *
 *  Created on: 2022年8月5日
 *      Author: 14913
 */

#ifndef APP_INC_MYFFT_H_
#define APP_INC_MYFFT_H_


#include "include.h"


//*****************************************************************************
// the defines
//*****************************************************************************
#define TEST_SIZE       (256U) //must be same as dsp_examples_setup.c
#define FFT_STAGES      (8U)
#define FFT_SIZE        (1 << FFT_STAGES)

void FFT_runTest();
void testfftdata();


extern float32_t test_magnitude[FFT_SIZE/2+1 ];
extern float32_t RFFToutBuff[FFT_SIZE];

#endif /* APP_INC_MYFFT_H_ */
