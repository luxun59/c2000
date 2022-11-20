///*
// * inventer.c
// *
// *  Created on: 2022年8月17日
// *      Author: 14913
// */
//
//
//
//#include "include.h"
//
//
////   开关频率：20kHz，采样和控制频率：40kHz
////###########################################################################
//#include "math.h"
///*---------------------------------函数声明---------------------------------------------*/
//interrupt void adc_isr(void);         // ADC中断子函数
//void Setup_Adc(void);                 // ADC模块配置子函数
//void Setup_ePWM1(void);               // ePWM1模块配置子函数
//void Setup_ePWM2(void);               // ePWM2模块配置子函数
//
///*----------------------------------宏定义----------------------------------------------*/
//#define  CPU_CLK  100e6               // 系统时钟 100MHz
//#define  PWM_CLK  20e3                // 开关频率 20kHz
//#define  Tsam     1/(2*PWM_CLK)       // 采样周期 Tsam=1/40k=25us
//#define  SP       CPU_CLK/(2*PWM_CLK) // ePWM周期寄存器的值 2500
//#define  NUM      800                 // 一个工频周期内控制周期数 800
//#define  pi       3.1415927           // π
//
////调节器参数
//#define  Kp_I    1                    // 电流环比例系数
//#define  kp     0.3                   // 电压环比例系数
//#define  kr     100                   // 电压环谐振系数
//#define  w0     50*2*pi               // 输出电压频率
//#define  wc     1*2*pi                // PR调节器截止频率
//
////限幅值
//#define  Iref_Max    6                // 电压环输出限幅
//#define  Iref_Min   -6
//#define  Vc_Max     0.98              // 电流环输出限幅
//#define  Vc_Min    -0.98
//#define  Umax       22.5              // 过压保护阈值
//#define  Imax       7.5               // 过流保护阈值
//
///*-------------------------------变量定义及初始化----------------------------------------*/
//float  Uo=0;                          // 输出电压
//float  IL=0;                          // 电感电流
//float  Uin=0;                         // 输入电压
//int    Uo_AD=0;
//int    IL_AD=0;
//int    Uin_AD=0;
//float  Urefm = 12;                    // 输出电压幅值给定 Uom*=12V
//float  Uref=0;                        // 输出电压指令值 Uo*
//float  Uref_Real[NUM];                // 存放20ms的输出电压指令
//float  Uo_Real[NUM];                  // 存放20ms的输出电压
//int    Phase_index = 0;               // 相位指针
//float  ek=0, ek_1=0, ek_2=0;          // 误差电压 e(k),e(k-1),e(k-2)
//float  uk=0, uk_1=0, uk_2=0;          // 指令值 u(k),u(k-1),u(k-2)
//float  Iref=0;                        // 电感电流给定 IL*
//float  ek_IL=0;                       // 误差电流 e(k)
//float  Vc=0;                          // 调制波幅值 Vc(t)
//int    start_flag = 0;                // 启动标志，为1表示装置正常运行
//int    protect_flag = 0;              // 保护标志，为1表示已启动保护
//
//
///*--------------------------------自定义子函数-----------------------------------------*/
//interrupt void adc_isr(void)   // ADC中断子函数
//{
//    //数据读取与处理
//    Uo_AD = AdcRegs.ADCRESULT0 >> 4;         // 读取AD转换结果
//    IL_AD = AdcRegs.ADCRESULT1 >> 4;
//    Uin_AD = AdcRegs.ADCRESULT2 >> 4;
//    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
//
//    Uo = Uo_AD * 0.010671 - 21.843774;      // 计算电压电流实际值
//    IL = IL_AD * 0.00751752 - 23.697444;
//    Uin = Uin_AD * 0.0107976 - 0.089283;
//
//    //过压过流检测
//    if ((Uo>Umax)||(Uo<-Umax)||(IL>Imax)||(IL<-Imax))
//    {
//        EALLOW;
//                   // 封锁ePWM1的驱动信号
//        EDIS;
//        protect_flag = 1;
//            // LED1灭
//            // LED2灭
//    }
//
//    //欠压检测
//    if ((protect_flag==0)&&(Uin>=20)&&(Uin<=40))
//        {
//        EALLOW;
//        EPwm1Regs.TZCLR.bit.OST = 1;           // PWM驱动复原
//        EDIS;
//        start_flag = 1;
//        GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;  // LED1亮
//        }
//    else
//        {
//        EALLOW;
//        EPwm1Regs.TZFRC.bit.OST = 1;           // 封锁ePWM1的驱动信号
//        EDIS;
//        start_flag = 0;
//        GpioDataRegs.GPBSET.bit.GPIO60 = 1;    // LED1灭
//        Vc = 0;
//        ek = 0, ek_1 = 0, ek_2 = 0;
//        uk = 0, uk_1 = 0, uk_2 = 0;
//        Iref = 0;
//        ek_IL = 0;
//        Phase_index = 0;
//        }
//
//    //闭环运算
//    if (start_flag == 1)
//    {
//    Uref = Urefm*sin(2*pi*Phase_index/NUM + pi/2); // 生成指令电压
//    Uref_Real[Phase_index] = Uref;                 // 存储指令电压
//    Uo_Real[Phase_index] = Uo;                     // 存储实际电压
//    Phase_index = Phase_index + 1;
//    if (Phase_index >= NUM)  Phase_index = 0;
//
//    //电压外环计算
//    ek = Uref - Uo;                       // 计算误差电压
//    uk = 1.99962422052943*uk_1 - 0.999685894918047*uk_2 +
//      0.315705254097646*ek - 0.599887266158828*ek_1 + 0.284200514377768*ek_2;
//    if(uk > Iref_Max)  uk = Iref_Max;     // 对电压调节器的输出进行限幅
//    else if(uk < Iref_Min)  uk = Iref_Min;
//    Iref = uk;                            // 计算电流指令值
//    ek_2 = ek_1;
//    ek_1 = ek;
//    uk_2 = uk_1;
//    uk_1 = uk;
////  Iref = 3*sin(2*pi*Phase_index/NUM + pi/2); // 调内环时用
//
//    //电流内环计算
//    ek_IL = Iref - IL;                       // 计算误差电流
//    Vc = Kp_I * ek_IL;                       // 计算控制电压
//    if(Vc > Vc_Max)  Vc = Vc_Max;            // 对电流调节器的输出进行限幅
//    else if(Vc < Vc_Min)  Vc = Vc_Min;
//    }
//
////  Vc = 0.8*sin(2*pi*Phase_index/NUM);;// 调开环时用
//    EPwm1Regs.CMPA.half.CMPA = (int)(SP/2 + Vc*SP/2); // 加载比较寄存器的值
//
//    //清除中断标志
//    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1; // 清除ADC的中断标志位
//    PieCtrlRegs.PIEACK.bit.ACK1 = 1;    // 清除第1组的中断响应标志位，使CPU可以响应第1组更多中断
//
//    return;
//}
//
//void Setup_Adc(void)  // ADC模块配置子函数
//{
//    //设置ADC时钟
//    AdcRegs.ADCTRL3.bit.ADCCLKPS = 3;     // FCLK = HSPCLK / (2 * ADCCLKPS) = 12.5MHz
//    AdcRegs.ADCTRL1.bit.CPS = 0;          // ADCCLK = FCLK / (CPS + 1) = 12.5MHz
//    AdcRegs.ADCTRL1.bit.ACQ_PS = 5;       // 采样窗，每个通道采样时间 =(2+ACQ_PS)*ADCCLK
//
//    //设置转换模式
//    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;     // 启动/停止模式
//    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;     // 禁用排序器覆盖
//    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;     // 级联排序器模式
//    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;    // 顺序采样模式
//
//    //设置采样通道
//    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 2; // 最大采样通道数为 3
//    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 2;  // 设置ADCINA2作为第1个采样通道
//    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 3;  // 设置ADCINA3作为第2个采样通道
//    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 1;  // 设置ADCINA1作为第3个采样通道
//
//    //设置触发方式
//    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// 允许ePWM的触发信号启动SEQ1
//    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // 使能SEQ1中断
//}
//
//void Setup_ePWM1(void)  // ePWM1模块配置子函数
//{
//    //时基模块TB
//    EPwm1Regs.TBPRD = SP;               // 周期值 SP = CPU_CLK/(2*PWM_CLK)
//    EPwm1Regs.TBPHS.half.TBPHS = 0;     // 相位为0
//    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;  // 时基时钟 TBCLK=SYSCLKOUT=1/150M
//    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
//    EPwm1Regs.TBCTL.bit.PHSEN = 0;      // 禁用相位寄存器
//    EPwm1Regs.TBCTL.bit.CTRMODE = 2;    // 增减计数
//
//    //计数比较模块CC
//    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0; // 影子装载模式
//    EPwm1Regs.CMPCTL.bit.LOADAMODE = 2; // CTR = 0 或 CTR = PRD时装载
//    EPwm1Regs.CMPA.half.CMPA =0;        // 设置初始占空比为0
//
//    //动作模块AQ
//    EPwm1Regs.AQCTLA.bit.CAU = 1;       // 向上计数且CTR=CMPA时，ePWM1A置低
//    EPwm1Regs.AQCTLA.bit.CAD = 2;       // 向下计数且CTR=CMPA时，ePWM1A置高
//
//    //死区产生模块DB
//    EPwm1Regs.DBCTL.bit.IN_MODE = 0;    // ePWM1A是双边沿输入
//    EPwm1Regs.DBCTL.bit.POLSEL = 2;     // ePWM1A不翻转，ePWM1B翻转
//    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;   // 使能双边沿延时
//    EPwm1Regs.DBRED = 30;               // 上升沿延时 DBRED*TBCLK = 200ns
//    EPwm1Regs.DBFED = 30;               // 下降沿延时 DBRED*TBCLK = 200ns
//
//    //错误联防模块TZ
//    EALLOW;
//    EPwm1Regs.TZCTL.bit.TZA = 2;        // 错误事件发生时，强制ePWM1A低状态
//    EPwm1Regs.TZCTL.bit.TZB = 2;        // 错误事件发生时，强制ePWM1B低状态
//    EDIS;
//}
//
//void Setup_ePWM2(void)  // ePWM2模块配置子函数
//{
//    //时基模块TB
//    EPwm2Regs.TBPRD = SP/2;             // 周期值设置为 ePWM1 的1/2
//    EPwm2Regs.TBPHS.half.TBPHS = 0;     // 相位为 0
//    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;  // 时基时钟 TBCLK=SYSCLKOUT=1/150M
//    EPwm2Regs.TBCTL.bit.CLKDIV = 0;
//    EPwm2Regs.TBCTL.bit.PHSEN = 0;      // 禁用相位寄存器
//    EPwm2Regs.TBCTL.bit.CTRMODE = 2;    // 增减计数
//
//    //事件触发模块ET
//    EPwm2Regs.ETSEL.bit.SOCAEN = 1;     // 使能 ePWM2SOCA 信号产生
//    EPwm2Regs.ETSEL.bit.SOCASEL = 1;    // 当TBCTR=0时产生 ePWM2SOCA信号
//    EPwm2Regs.ETPS.bit.SOCAPRD = 1;     // 在第1个事件时产生 ePWM2SOCA信号
//}
//
////===========================================================================
//// No more.
////===========================================================================
//
//
//
//
//
//
//
//
//
//
