/*
 * tpl0401.c
 *
 *  Created on: 2022年8月18日
 *      Author: 14913
 */


#include "tpl0401.h"


// i2c addr 0101110




#define tpl0401_address 0x5C//地址
#define tpl0401_command  0x00//选择寄存器

#define TRUE    1
#define FALSE   0

#define tpl0401_SCL_GPIO GPIO_59_GPIO59
#define tpl0401_SCL_PIN 59

#define tpl0401_SDA_GPIO GPIO_23_GPIO23
#define tpl0401_SDA_PIN 23
//#define tpl0401_SCL_GPIO GPIO_59_GPIO59
//#define tpl0401_SCL_PIN 59
//
//#define tpl0401_SDA_GPIO GPIO_25_GPIO25
//#define tpl0401_SDA_PIN 25


#define SCL_H GPIO_writePin(tpl0401_SCL_PIN, 1)
#define SCL_L GPIO_writePin(tpl0401_SCL_PIN, 0)
#define SDA_H GPIO_writePin(tpl0401_SDA_PIN, 1)
#define SDA_L GPIO_writePin(tpl0401_SDA_PIN, 0)

#define SDA_in  tpl0401_sda_setin() //SDA改成输入模式
#define SDA_out   tpl0401_sda_setout()//SDA变回输出模式
#define SDA_val GPIO_readPin(tpl0401_SDA_PIN)//SDA的位值


void delay_1115(void)
{
    Delay_us(2);
}
/*******************************************
函数名称start
功    能完成IIC的起始条件操作
参    数无
返回值  无
********************************************/
void tpl0401_start(void)
{
    SCL_H;
    SDA_H;
    delay_1115();
    SDA_L;
    delay_1115();
    SCL_L;
}
/*******************************************
函数名称stop
功    能完成IIC的终止条件操作
参    数无
返回值  无
********************************************/
void tpl0401_stop(void)
{
    SDA_L;
    delay_1115();
    SCL_H;;
    delay_1115();
}

/*******************************************
函数名称mack
功    能完成IIC的主机应答操作
参    数无
返回值  无
********************************************/

void tpl0401_mack(void)
{
    SDA_L;
    Delay_us(1);
    SCL_H;
    delay_1115();
    SCL_L;
    Delay_us(1);
    SDA_H;
    delay_1115();
}
/*******************************************
函数名称mnack
功    能完成IIC的主机无应答操作
参    数无
返回值  无
********************************************/
void tpl0401_mnack(void)
{
    SDA_H;
    Delay_us(1);
    SCL_H;
    delay_1115();
    SCL_L;
    Delay_us(1);
    SDA_L;
    delay_1115();
}
/**********检查应答信号函数******************/
/*如果返回值为1则证明有应答信号反之没有*/ /*******************************************
函数名称check
功    能检查从机的应答操作
参    数无
返回值  从机是否有应答1--有0--无
********************************************/
void tpl0401_ack()
{
    uint16_t i;
    i=0;
    SDA_in;
    SCL_H;
    delay_1115();
    while(SDA_val&&(i<250)) i++;
    SDA_out;
    SCL_L;
    delay_1115();
}

/*******************************************
函数名称write1
功    能向IIC总线发送一个1
参    数无
返回值  无
********************************************/
void tpl0401_write1(void)
{
    SDA_H;
    delay_1115();
    SCL_H;
    delay_1115();
    SCL_L;
    delay_1115();
}

/*******************************************
函数名称write0
功    能向IIC总线发送一个0
参    数无
返回值  无
********************************************/
void tpl0401_write0(void)
{
    SDA_L;
    delay_1115();
    SCL_H;
    delay_1115();
    SCL_L;
    delay_1115();
}

/*******************************************
函数名称write1byte
功    能向IIC总线发送一个字节的数据
参    数wdata--发送的数据
返回值  无
********************************************/
void tpl0401_write1byte(uint16_t wdata)
{
    uint16_t i;
    for(i = 8;i > 0;i--)
    {
        if(wdata & 0x0080)
            tpl0401_write1();
        else
            tpl0401_write0();
        wdata <<= 1;
    }
    SDA_H;
    Delay_us(1);
}

/*******************************************
函数名称read1byte
功    能从IIC总线读取一个字节
参    数无
返回值  读取的数据
********************************************/
uint16_t tpl0401_read1byte(void)
{
    uint16_t  rdata = 0x00,i;
    uint16_t flag;
     for(i = 0;i < 8;i++)
     {
        SDA_H;
        SCL_H;
        SDA_in;
        flag = SDA_val;
        rdata <<= 1;
        if(flag)
            rdata |= 0x01;
       SDA_out;
        SCL_L;
     }
    return rdata;
}



void tpl0401_set(uint16_t data)
{

    tpl0401_start();
    tpl0401_write1byte(tpl0401_address);
    tpl0401_ack();
    tpl0401_write1byte(tpl0401_command);
    tpl0401_ack();
    tpl0401_write1byte(data);
    tpl0401_ack();
    tpl0401_stop();
}

uint16_t tpl0401_read()
{
    uint16_t result_l,result_h;
    uint16_t result;
    tpl0401_start();
    tpl0401_write1byte(tpl0401_address);
    tpl0401_ack();
    tpl0401_write1byte(tpl0401_command);
    tpl0401_ack();

    tpl0401_start();
    tpl0401_write1byte(tpl0401_address|0x0001);
    tpl0401_ack();
    result_h=tpl0401_read1byte();
    tpl0401_stop();
    return result;
}




void tpl0401_init(void)
{
    GPIO_setPinConfig(tpl0401_SCL_GPIO);
    GPIO_setDirectionMode(tpl0401_SCL_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(tpl0401_SCL_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(tpl0401_SCL_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(tpl0401_SCL_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(tpl0401_SDA_GPIO);
    GPIO_setDirectionMode(tpl0401_SDA_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(tpl0401_SDA_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(tpl0401_SDA_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(tpl0401_SDA_PIN, GPIO_QUAL_SYNC);


      SCL_H;
      SDA_H;
//      SDA_out;
}



void tpl0401_sda_setin()
{
    GPIO_setPinConfig(tpl0401_SDA_GPIO);
    GPIO_setDirectionMode(tpl0401_SDA_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(tpl0401_SDA_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(tpl0401_SDA_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(tpl0401_SDA_PIN, GPIO_QUAL_SYNC);

}

void tpl0401_sda_setout()
{
    GPIO_setPinConfig(tpl0401_SDA_GPIO);
    GPIO_setDirectionMode(tpl0401_SDA_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(tpl0401_SDA_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(tpl0401_SDA_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(tpl0401_SDA_PIN, GPIO_QUAL_SYNC);

}









