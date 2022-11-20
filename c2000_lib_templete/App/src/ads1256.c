/*
 * ads1256.c
 *
 *  Created on: 2022年8月13日
 *      Author: 14913
 */







#include "ads1256.h"

//依次相连
//***************************
//      Pin assign
//          STM32           ADS1256

//      GPIOB_Pin_15        ---> SCK
//      GPIOB_Pin_14        ---> DIN
//      GPIOB_Pin_13        <--- DOUT
//      GPIOB_Pin_12        <--- DRDY
//      GPIOB_Pin_11        ---> CS

//***************************

    /*端口定义*/

 // #define RCC_SCK     RCC_APB2Periph_GPIOB //SCK
    #define ads1256_SCK_GPIO    GPIO_39_GPIO39
    #define ads1256_SCK_PIN     39

//  #define RCC_DOUT    RCC_APB2Periph_GPIOB //MOSI
    #define ads1256_DOUT_GPIO   GPIO_17_GPIO17
    #define ads1256_DOUT_PIN    17

//  #define RCC_DIN     RCC_APB2Periph_GPIOB //MISO
    #define ads1256_DIN_GPIO    GPIO_16_GPIO16
    #define ads1256_DIN_PIN     16

//  #define RCC_DRDY    RCC_APB2Periph_GPIOB //DRDY
    #define ads1256_DRDY_GPIO   GPIO_57_GPIO57
    #define ads1256_DRDY_PIN    57

//  #define RCC_CS      RCC_APB2Periph_GPIOB //CS
    #define ads1256_CS_GPIO     GPIO_25_GPIO25
    #define ads1256_CS_PIN      25


//  #define RCC_PWDN    RCC_APB2Periph_GPIOB //PWDN
//  #define PORT_PWDN   GPIOC
//  #define PIN_PWDN    GPIO_PIN_9

//****************************************************************


    #define ads1256_CS_L()      GPIO_writePin(ads1256_CS_PIN, 0)
    #define ads1256_CS_H()      GPIO_writePin(ads1256_CS_PIN, 1)

    #define ads1256_SCK_L()     GPIO_writePin(ads1256_SCK_PIN, 0)
    #define ads1256_SCK_H()     GPIO_writePin(ads1256_SCK_PIN, 1)

    #define ads1256_DIN_L()     GPIO_writePin(ads1256_DIN_PIN, 0)       //Set ADS1256_DIN = 0
    #define ads1256_DIN_H()     GPIO_writePin(ads1256_DIN_PIN, 1)       //Set ADS1256_DIN = 1

    #define READ_ADS1256_DRDY  GPIO_readPin(ads1256_DRDY_PIN)
    #define READ_ADS1256_DOUT  GPIO_readPin(ads1256_DOUT_PIN)





















//初始化ADS1256 GPIO
void ads1256_Init_GPIO(void)
{

    GPIO_setPinConfig(ads1256_SCK_GPIO);
    GPIO_setDirectionMode(ads1256_SCK_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(ads1256_SCK_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(ads1256_SCK_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(ads1256_SCK_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(ads1256_DIN_GPIO);
    GPIO_setDirectionMode(ads1256_DIN_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(ads1256_DIN_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(ads1256_DIN_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(ads1256_DIN_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(ads1256_CS_GPIO);
    GPIO_setDirectionMode(ads1256_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(ads1256_CS_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(ads1256_CS_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(ads1256_CS_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(ads1256_DRDY_GPIO);
    GPIO_setDirectionMode(ads1256_DRDY_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(ads1256_DRDY_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(ads1256_DRDY_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(ads1256_DRDY_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(ads1256_DOUT_GPIO);
    GPIO_setDirectionMode(ads1256_DOUT_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(ads1256_DOUT_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(ads1256_DOUT_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(ads1256_DOUT_PIN, GPIO_QUAL_SYNC);

}



static void ADS1256_DelaySCLK(void)
{
    Delay_us(1);
}
/*
*********************************************************************************************************
*   函 数 名: ads1256_SPI_WriteByte
*   功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*   形    参: _data : 数据
*   返 回 值: 无
*********************************************************************************************************
*/
void ads1256_SPI_WriteByte(uint16_t TxData)
{
    unsigned char i;
    TxData = TxData&0x00FF;
    /* 连续发送多个字节时，需要延迟一下 */
    ADS1256_DelaySCLK();
    /*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
    for(i = 0; i < 8; i++)
    {
        if (TxData & 0x0080)
            ads1256_DIN_H();
        else
            ads1256_DIN_L();

        ads1256_SCK_H();
        ADS1256_DelaySCLK();
        TxData <<= 1;
        ads1256_SCK_L();            /* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
        ADS1256_DelaySCLK();
    }

}

/*
*********************************************************************************************************
*   函 数 名: ads1256_SPI_ReadByte
*   功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
uint16_t ads1256_SPI_ReadByte(void)
{
    uint16_t i;
    uint16_t read = 0;
    ADS1256_DelaySCLK();
    /*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
    for (i = 0; i < 8; i++)
    {
        ads1256_SCK_H();
        ADS1256_DelaySCLK();
        read = read<<1;
        ads1256_SCK_L();
        if (READ_ADS1256_DOUT)
        {
            read++;
        }
        ADS1256_DelaySCLK();
    }
    return read&0x00ff;
}

//-----------------------------------------------------------------//
//  功    能：ADS1256 写数据
//  入口参数: /
//  出口参数: /
//  全局变量: /
//  备    注: 向ADS1256中地址为regaddr的寄存器写入一个字节databyte
//-----------------------------------------------------------------//
void ADS1256WREG(uint16_t regaddr,uint16_t databyte)
{

    ads1256_CS_L();
    uint16_t i =0;
    while(READ_ADS1256_DRDY) i++;//当ADS1256_DRDY为低时才能写寄存器
    //向寄存器写入数据地址
    ads1256_SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x000F));
    //写入数据的个数n-1
    ads1256_SPI_WriteByte(0x00);
    //向regaddr地址指向的寄存器写入数据databyte
    ads1256_SPI_WriteByte(databyte);
    ads1256_CS_H();


}
//初始化ADS1256
void ADS1256_Init(void)
{
    //*************自校准****************
    uint32_t i =0;
    while(READ_ADS1256_DRDY) i++;
    ads1256_CS_L();
    ads1256_SPI_WriteByte(ADS1256_CMD_SELFCAL);
    i =0;
    while(READ_ADS1256_DRDY) i++;
    ads1256_CS_H();
    //**********************************

    ADS1256WREG(ADS1256_STATUS,0x06);               // 高位在前、校准、使用缓冲
    //ADS1256WREG(ADS1256_STATUS,0x04);               // 高位在前、不使用缓冲
    //ADS1256WREG(ADS1256_MUX,0x08);                  // 初始化端口A0为‘+’，AINCOM位‘-’

    ADS1256WREG(ADS1256_ADCON,ADS1256_GAIN_1);      // 放大倍数1
    ADS1256WREG(ADS1256_DRATE,ADS1256_DRATE_10SPS); // 数据500sps
    ADS1256WREG(ADS1256_IO,0x00);

    //*************自校准****************
    i =0;
    while(READ_ADS1256_DRDY) i++;
    ads1256_CS_L();
    ads1256_SPI_WriteByte(ADS1256_CMD_SELFCAL);
    i =0;
    while(READ_ADS1256_DRDY) i++;
    ads1256_CS_H();
    //**********************************
}


//读取AD值
int32_t ADS1256ReadData(uint16_t channel)
{

    uint32_t sum=0;
    uint16_t i =0;
    while(READ_ADS1256_DRDY)
    {
        i++;//当ADS1256_DRDY为低时才能写寄存器
//        if(i>0xfff)
//            return 2;
    }

    ADS1256WREG(ADS1256_MUX,channel);       //设置通道
    ads1256_CS_L();
    ads1256_SPI_WriteByte(ADS1256_CMD_SYNC);
    ads1256_SPI_WriteByte(ADS1256_CMD_WAKEUP);
    ads1256_SPI_WriteByte(ADS1256_CMD_RDATA);
    sum += (ads1256_SPI_ReadByte() *1);
//    sum += (ads1256_SPI_ReadByte() *256);
//    sum += ads1256_SPI_ReadByte();
    ads1256_SPI_ReadByte();
    ads1256_SPI_ReadByte();
    ads1256_CS_H();

    if (sum>0x7FFFFF)           // if MSB=1,
    {
        sum -= 0x1000000;       // do 2's complement

    }
    return sum;
}










