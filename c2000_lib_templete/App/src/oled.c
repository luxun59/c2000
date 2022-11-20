/*
 * oled.c
 *
 *  Created on: 2022年7月26日
 *      Author: 14913
 */

#include <stdio.h>

#include "oled.h"
#include "include.h"

#include "driverlib.h"




#define MSP432E 2
#define MSP432P 3








#define u8 uint8_t
#define u32 uint32_t

/*****************************************************************/

#if (TRANSFER_METHOD == HW_IIC)

//IIC_0: P1.7  -- SCL;  P1.6  -- SDA
//IIC_1: P6.5  -- SCL;  P6.4  -- SDA
//IIC_2: P3.7  -- SCL;  P3.6  -- SDA
//IIC_3: P10.3 -- SCL;  P10.2 -- SDA
#define USE_HW_IIC IIC_0

#elif (TRANSFER_METHOD == HW_SPI)
//#define USE_HW_SPI SPI_2
#endif


#if (TRANSFER_METHOD == HW_IIC)
#define OLED_ADDRESS 0x3C //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78  0x3C = 0x78 >> 1

#if (USE_HW_IIC == IIC_0) //已验证
#define IIC_GPIOX GPIO_PORT_P1
#define IIC_SCL_Pin GPIO_PIN7
#define IIC_SDA_Pin GPIO_PIN6
#define EUSCI_BX EUSCI_B0_BASE

#elif (USE_HW_IIC == IIC_1) //已验证
#define IIC_GPIOX GPIO_PORT_P6
#define IIC_SCL_Pin GPIO_PIN5
#define IIC_SDA_Pin GPIO_PIN4
#define EUSCI_BX EUSCI_B1_BASE

#elif (USE_HW_IIC == IIC_2) //已验证
#define IIC_GPIOX GPIO_PORT_P3
#define IIC_SCL_Pin GPIO_PIN7
#define IIC_SDA_Pin GPIO_PIN6
#define EUSCI_BX EUSCI_B2_BASE

#elif (USE_HW_IIC == IIC_3) //已验证
#define IIC_GPIOX GPIO_PORT_P10
#define IIC_SCL_Pin GPIO_PIN3
#define IIC_SDA_Pin GPIO_PIN2
#define EUSCI_BX EUSCI_B3_BASE
#endif

#elif TRANSFER_METHOD == SW_IIC


#if BOARD == MSP432P
#define Set_Bit(val, bitn) (val |= (/*1 <<*/ (bitn))) //置1
#define Clr_Bit(val, bitn) (val &= ~(/*1<<*/ (bitn))) //置0
#define Get_Bit(val, bitn) (val & (1 << (bitn)))      //获取电平

//OLED SSD1306 I2C SCL P1.7
#define OLED_SSD1306_SCL_PIN_NUM (BIT7)
#define OLED_SSD1306_SCL_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Set() (Set_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))

//OLED SSD1306 I2C SDA   P1.6
#define OLED_SSD1306_SDA_PIN_NUM (BIT6)
#define OLED_SSD1306_SDA_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Set() (Set_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))

#elif BOARD == C2000




//OLED SSD1306 I2C SCL P12
#define OLED_SSD1306_SCL_PIN 12
#define OLED_SSD1306_SCL_GPIO GPIO_12_GPIO12
#define OLED_SCL_Set() GPIO_writePin(OLED_SSD1306_SCL_PIN, 1)
#define OLED_SCL_Clr() GPIO_writePin(OLED_SSD1306_SCL_PIN, 0)

//OLED SSD1306 I2C SDA   P34
#define OLED_SSD1306_SDA_PIN 34
#define OLED_SSD1306_SDA_GPIO GPIO_34_GPIO34
#define OLED_SDA_Set() GPIO_writePin(OLED_SSD1306_SDA_PIN, 1)
#define OLED_SDA_Clr() GPIO_writePin(OLED_SSD1306_SDA_PIN, 0)






#endif





#elif (TRANSFER_METHOD == SW_SPI)



#if BOARD == MSP432P

#define OLED_SCLK_PORT GPIO_PORT_P3
#define OLED_SCLK_PIN GPIO_PIN7

#define OLED_SDIN_PORT GPIO_PORT_P3
#define OLED_SDIN_PIN GPIO_PIN6

#define OLED_RST_PORT GPIO_PORT_P3
#define OLED_RST_PIN GPIO_PIN5

#define OLED_DC_PORT GPIO_PORT_P5
#define OLED_DC_PIN GPIO_PIN1

#define OLED_CS_PORT GPIO_PORT_P2
#define OLED_CS_PIN GPIO_PIN3


#define OLED_SCLK_Clr() GPIO_setOutputLowOnPin(OLED_SCLK_PORT, OLED_SCLK_PIN)
#define OLED_SCLK_Set() GPIO_setOutputHighOnPin(OLED_SCLK_PORT, OLED_SCLK_PIN)

#define OLED_SDIN_Clr() GPIO_setOutputLowOnPin(OLED_SDIN_PORT, OLED_SDIN_PIN)//DIN
#define OLED_SDIN_Set() GPIO_setOutputHighOnPin(OLED_SDIN_PORT, OLED_SDIN_PIN)

#define OLED_RST_Clr() GPIO_setOutputLowOnPin(OLED_RST_PORT, OLED_RST_PIN)//RES
#define OLED_RST_Set() GPIO_setOutputHighOnPin(OLED_RST_PORT, OLED_RST_PIN)

#define OLED_DC_Clr() GPIO_setOutputLowOnPin(OLED_DC_PORT, OLED_DC_PIN)//DC
#define OLED_DC_Set() GPIO_setOutputHighOnPin(OLED_DC_PORT, OLED_DC_PIN)

#define OLED_CS_Clr()  GPIO_setOutputLowOnPin(OLED_CS_PORT, OLED_CS_PIN)//CS
#define OLED_CS_Set()  GPIO_setOutputHighOnPin(OLED_CS_PORT, OLED_CS_PIN)


#elif BOARD == C2000

#define OLED_SCLK_PIN 59
#define OLED_SDIN_PIN 23
#define OLED_RST_PIN 39
#define OLED_DC_PIN 17
#define OLED_CS_PIN 16

#define OLED_SCLK_GPIO GPIO_59_GPIO59
#define OLED_SDIN_GPIO GPIO_23_GPIO23
#define OLED_RST_GPIO GPIO_39_GPIO39
#define OLED_DC_GPIO GPIO_17_GPIO17
#define OLED_CS_GPIO GPIO_16_GPIO16


#define OLED_SCLK_Clr() GPIO_writePin(OLED_SCLK_PIN, 0)
#define OLED_SCLK_Set() GPIO_writePin(OLED_SCLK_PIN, 1)

#define OLED_SDIN_Clr() GPIO_writePin(OLED_SDIN_PIN, 0)//DIN
#define OLED_SDIN_Set() GPIO_writePin(OLED_SDIN_PIN, 1)

#define OLED_RST_Clr() GPIO_writePin(OLED_RST_PIN, 0)//RES
#define OLED_RST_Set() GPIO_writePin(OLED_RST_PIN, 1)

#define OLED_DC_Clr()  GPIO_writePin(OLED_DC_PIN, 0)//DC
#define OLED_DC_Set()  GPIO_writePin(OLED_DC_PIN, 1)

#define OLED_CS_Clr()  GPIO_writePin(OLED_CS_PIN, 0)//CS
#define OLED_CS_Set()  GPIO_writePin(OLED_CS_PIN, 1)



#endif




#endif
/**********************************************************/




#define OLED_DATA 1
#define OLED_CMD 0


uint8_t OLED_GRAM[144][8];

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

#if (TRANSFER_METHOD == HW_IIC)
//I2C_Configuration，初始化硬件IIC引脚
void I2C_Configuration(void)
{
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
      IIC_GPIOX, IIC_SCL_Pin | IIC_SDA_Pin, GPIO_PRIMARY_MODULE_FUNCTION);
  const eUSCI_I2C_MasterConfig i2cConfig =
      {
          EUSCI_B_I2C_CLOCKSOURCE_SMCLK,   // SMCLK Clock Source
          48000000,                        // SMCLK = 48MHz
          EUSCI_B_I2C_SET_DATA_RATE_1MBPS, // Desired I2C Clock of 1MHz（实际可以更高，根据I2C协议可以3.4MHz，注意上拉电阻配置
          0,                               // No byte counter threshold
          EUSCI_B_I2C_NO_AUTO_STOP         // No Autostop
      };
  MAP_I2C_initMaster(EUSCI_BX, &i2cConfig);
  MAP_I2C_setSlaveAddress(EUSCI_BX, OLED_ADDRESS);
  MAP_I2C_setMode(EUSCI_BX, EUSCI_B_I2C_TRANSMIT_MODE);
  MAP_I2C_enableModule(EUSCI_BX);

  // MAP_I2C_clearInterruptFlag(
  //    EUSCI_BX, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_NAK_INTERRUPT);
  // MAP_I2C_enableInterrupt(
  //    EUSCI_BX, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_NAK_INTERRUPT);
  // MAP_Interrupt_enableInterrupt(INT_EUSCIB0);
}
//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
  if (mode)
    MAP_I2C_masterSendMultiByteStart(EUSCI_BX, 0x40);
  else
    MAP_I2C_masterSendMultiByteStart(EUSCI_BX, 0x00);
  MAP_I2C_masterSendMultiByteFinish(EUSCI_BX, dat);
}

#elif (TRANSFER_METHOD == SW_IIC)

void I2C_SW_Configuration()
{

#if BOARD == MSP432P
  OLED_SSD1306_SCL_IO_INIT;
  OLED_SSD1306_SDA_IO_INIT;
  delay_ms(200);
#elif BOARD == C2000

  GPIO_setPinConfig(OLED_SSD1306_SCL_GPIO);
  GPIO_setDirectionMode(OLED_SSD1306_SCL_PIN, GPIO_DIR_MODE_OUT);
  GPIO_setPadConfig(OLED_SSD1306_SCL_PIN, GPIO_PIN_TYPE_STD);
  GPIO_setMasterCore(OLED_SSD1306_SCL_PIN, GPIO_CORE_CPU1);
  GPIO_setQualificationMode(OLED_SSD1306_SCL_PIN, GPIO_QUAL_SYNC);

  GPIO_setPinConfig(OLED_SSD1306_SDA_GPIO);
  GPIO_setDirectionMode(OLED_SSD1306_SDA_PIN, GPIO_DIR_MODE_OUT);
  GPIO_setPadConfig(OLED_SSD1306_SDA_PIN, GPIO_PIN_TYPE_STD);
  GPIO_setMasterCore(OLED_SSD1306_SDA_PIN, GPIO_CORE_CPU1);
  GPIO_setQualificationMode(OLED_SSD1306_SDA_PIN, GPIO_QUAL_SYNC);

  Delay_ms(200);


#endif


}

//起始信号
void I2C_Start(void)
{
  OLED_SDA_Set();
  OLED_SCL_Set();
  Delay_us(1);
  OLED_SDA_Clr();
  Delay_us(1);
  OLED_SCL_Clr();

}

//结束信号
void I2C_Stop(void)
{
  OLED_SCL_Clr();
  OLED_SDA_Clr();
  Delay_us(1);
  OLED_SCL_Set();
  OLED_SDA_Set();
  Delay_us(1);

}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
  OLED_SDA_Set();
  Delay_us(1);
  OLED_SCL_Set();
  Delay_us(4);
  OLED_SCL_Clr();
}

//写入一个字节
void Send_Byte(uint8_t dat)
{

  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    OLED_SCL_Clr(); //将时钟信号设置为低电平
    if (dat & 0x0080) //将dat的8位从最高位依次写入
    {
      OLED_SDA_Set();
    }
    else
    {
      OLED_SDA_Clr();
    }

    Delay_us(1);
    OLED_SCL_Set();
    Delay_us(1);
    OLED_SCL_Clr();
    Delay_us(1);
    dat <<= 1;
  }
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
  I2C_Start();
  Send_Byte(0x78);
  I2C_WaitAck();
  if (mode)
  {
    Send_Byte(0x40);
  }
  else
  {
    Send_Byte(0x00);
  }
  I2C_WaitAck();
  Send_Byte(dat);
  I2C_WaitAck();
  I2C_Stop();
}

#elif (TRANSFER_METHOD == SW_SPI)


void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i;
    if(cmd)
      OLED_DC_Set();
    else
      OLED_DC_Clr();
    OLED_CS_Clr();
    for(i=0;i<8;i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
           OLED_SDIN_Set();
        else
           OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
}

void SPI_SW_Configuration()
{

#if BOARD == MSP432P

    MAP_GPIO_setAsOutputPin(OLED_SCLK_PORT,OLED_SCLK_PIN);
    MAP_GPIO_setAsOutputPin(OLED_SDIN_PORT,OLED_SDIN_PIN);
    MAP_GPIO_setAsOutputPin(OLED_RST_PORT,OLED_RST_PIN);
    MAP_GPIO_setAsOutputPin(OLED_DC_PORT,OLED_DC_PIN);
    MAP_GPIO_setAsOutputPin(OLED_CS_PORT,OLED_CS_PIN);


    OLED_RST_Set();
    delay_ms(100);
    OLED_RST_Clr();//复位
    delay_ms(200);
    OLED_RST_Set();

#elif BOARD == C2000

//#define OLED_SCLK_PIN 59
//#define OLED_SDIN_PIN 23
//#define OLED_RST_PIN 39
//#define OLED_DC_PIN 17
//#define OLED_CS_PIN 16

    GPIO_setPinConfig(OLED_SCLK_GPIO);
    GPIO_setDirectionMode(OLED_SCLK_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(OLED_SCLK_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(OLED_SCLK_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(OLED_SCLK_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(OLED_SDIN_GPIO);
    GPIO_setDirectionMode(OLED_SDIN_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(OLED_SDIN_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(OLED_SDIN_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(OLED_SDIN_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(OLED_RST_GPIO);
    GPIO_setDirectionMode(OLED_RST_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(OLED_RST_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(OLED_RST_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(OLED_RST_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(OLED_DC_GPIO);
    GPIO_setDirectionMode(OLED_DC_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(OLED_DC_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(OLED_DC_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(OLED_DC_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(OLED_CS_GPIO);
    GPIO_setDirectionMode(OLED_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(OLED_CS_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(OLED_CS_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(OLED_CS_PIN, GPIO_QUAL_SYNC);

    OLED_RST_Set();
    Delay_ms(100);
    OLED_RST_Clr();//复位
    Delay_ms(200);
    OLED_RST_Set();


#endif



}


#endif





//初始化SSD1306
void OLED_Init(void)
{
#if (TRANSFER_METHOD == HW_IIC)
  I2C_Configuration();
#elif (TRANSFER_METHOD == SW_IIC)
  I2C_SW_Configuration();
#elif (TRANSFER_METHOD == SW_SPI)
  SPI_SW_Configuration();
#endif

  OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
  OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
  OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
  OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
  OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WR_Byte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
  OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
  OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00, OLED_CMD); //-not offset
  OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
  OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
  OLED_WR_Byte(0x12, OLED_CMD);
  OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
  OLED_WR_Byte(0x40, OLED_CMD); //Set VCOM Deselect Level
  OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02, OLED_CMD); //
  OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
  OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
  OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)

  OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
  OLED_Clear();
}



//反显函数
void OLED_ColorTurn(u8 i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
        }
    if(i==1)
        {
            OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
        }
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
            OLED_WR_Byte(0xA1,OLED_CMD);
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
            OLED_WR_Byte(0xA0,OLED_CMD);
        }
}



//开启OLED显示
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
       OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
       OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
       OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
       for(n=0;n<128;n++)
         OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
//清屏函数
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
       for(n=0;n<128;n++)
            {
             OLED_GRAM[n][i]=0;//清除所有数据
            }
  }
    OLED_Refresh();//更新显示
}

//画点
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
    u8 i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
    u8 i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    OLED_GRAM[x][i]|=n;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}


//画线
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
    u8 i,k,k1,k2,y0;
    if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))return;
    if(x1==x2)    //画竖线
    {
            for(i=0;i<(y2-y1);i++)
            {
                OLED_DrawPoint(x1,y1+i);
            }
  }
    else if(y1==y2)   //画横线
    {
            for(i=0;i<(x2-x1);i++)
            {
                OLED_DrawPoint(x1+i,y1);
            }
  }
    else      //画斜线
    {
        k1=y2-y1;
        k2=x2-x1;
        k=k1*10/k2;
        for(i=0;i<(x2-x1);i++)
            {
              OLED_DrawPoint(x1+i,y1+i*k/10);
            }
    }
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
    int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
    u8 i,m,temp,size2,chr1;
    u8 y0=y;
    size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0;i<size2;i++)
    {
//        if(size1==12)
//        {temp=asc2_1206[chr1][i];} //调用1206字体
        if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
//        if(size1==24)
//        {temp=asc2_2412[chr1][i];} //调用2412字体
        else return;
                for(m=0;m<8;m++)           //写入数据
                {
                    if(temp&0x80)OLED_DrawPoint(x,y);
                    else OLED_ClearPoint(x,y);
                    temp<<=1;
                    y++;
                    if((y-y0)==size1)
                    {
                        y=y0;
                        x++;
                        break;
          }
                }
  }
}


//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
    while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
    {
        OLED_ShowChar(x,y,*chr,size1);
        x+=size1/2;
        if(x>128-size1)  //换行
        {
            x=0;
            y+=2;
    }
        chr++;
  }
}

//显示浮点数
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
void OLED_ShowFloat(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2)
{
    u8 t,temp;
    u8 enshow;
    int z_temp,f_temp;
    z_temp=(int)num;
    //整数部分
    for(t=0;t<z_len;t++)
    {
        temp=(z_temp/OLED_Pow(10,z_len-t-1))%10;
        if(enshow==0 && t<(z_len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }
            else
                enshow=1;
        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
    //小数点
    OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2);

    f_temp=(int)((num-z_temp)*(OLED_Pow(10,f_len)));
  //小数部分
    for(t=0;t<f_len;t++)
    {
        temp=(f_temp/OLED_Pow(10,f_len-t-1))%10;
        OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2);
    }

}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)
    {
      result*=m;
    }
    return result;
}

////显示2个数字
////x,y :起点坐标
////len :数字的位数
////size:字体大小
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
    u8 t,temp;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
            if(temp==0)
            {
                OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
            else
            {
              OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
            }
  }
}



//配置写入数据的起始位置
void OLED_WR_BP(u8 x,u8 y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
    u32 j=0;
    u8 x=0,y=0;
    if(y%8==0)y=0;
    else y+=1;
    for(y=y0;y<y1;y++)
     {
         OLED_WR_BP(x0,y);
         for(x=x0;x<x1;x++)
         {
             OLED_WR_Byte(BMP[j],OLED_DATA);
             j++;
     }
     }
}







