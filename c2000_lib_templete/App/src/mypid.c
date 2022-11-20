/*
 * mypid.c
 *
 *  Created on: 2022年8月15日
 *      Author: 14913
 */


#include "mypid.h"


DCL_PID pid1 = PID_DEFAULTS;

void myPID_Init()
{
/* initialise controller variables */
    pid1.Kp = 1.0f;
    pid1.Ki = 0.0f;
    pid1.Kd = 0.0f;
    pid1.Kr = 1.0f;
    pid1.c1 = 1.0f;
    pid1.c2 = 0.0f;
    pid1.d2 = 0.0f;
    pid1.d3 = 0.0f;
    pid1.i10 = 0.0f;
    pid1.i14 = 1.0f;
    pid1.Umax = 1.0f;
    pid1.Umin = -1.0f;

}

//    // run PID controller
//        uk = DCL_runPID_C4(&pid1, rk, yk, lk);

