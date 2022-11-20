/*
 * tlv5618.c
 *
 *  Created on: 2022年8月11日
 *      Author: 14913
 */


#include "tlv5618.h"

#define TLV5618_MOSI_GPIO GPIO_0_GPIO0
#define TLV5618_MOSI_PIN 0
#define TLV5618_SCK_GPIO GPIO_1_GPIO1
#define TLV5618_SCK_PIN 1
#define TLV5618_CS_GPIO GPIO_6_GPIO6
#define TLV5618_CS_PIN 6

#define TLV5618_MOSI_H GPIO_writePin(TLV5618_MOSI_PIN, 1)
#define TLV5618_MOSI_L GPIO_writePin(TLV5618_MOSI_PIN, 0)
#define TLV5618_SCK_H GPIO_writePin(TLV5618_SCK_PIN, 1)
#define TLV5618_SCK_L GPIO_writePin(TLV5618_SCK_PIN, 0)
#define TLV5618_CS_H GPIO_writePin(TLV5618_CS_PIN, 1)
#define TLV5618_CS_L GPIO_writePin(TLV5618_CS_PIN, 0)







/* 16bit, MSB first */
void TLV5618_SPI_Send_Byte( uint16_t write_dat )
{
    uint16_t i;
    uint16_t mode = 0xC000;
    //高四位清零
    write_dat = write_dat&0x0FFF;
    //高四位置模式
    write_dat = write_dat|mode;

    TLV5618_CS_L;
    Delay_us(1);
    for( i = 0; i < 16; i++ )
    {
        if( write_dat & 0x8000 )
            TLV5618_MOSI_H;
        else
            TLV5618_MOSI_L;
        write_dat <<= 1;
        TLV5618_SCK_H;
        Delay_us(1);
        TLV5618_SCK_L;
        Delay_us(1);
    }
    Delay_us(1);
    TLV5618_CS_H;
}




void TLV5618_SPI_Init(void)
{
    GPIO_setPinConfig(TLV5618_MOSI_GPIO);
    GPIO_setDirectionMode(TLV5618_MOSI_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(TLV5618_MOSI_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(TLV5618_MOSI_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(TLV5618_MOSI_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(TLV5618_SCK_GPIO);
    GPIO_setDirectionMode(TLV5618_SCK_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(TLV5618_SCK_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(TLV5618_SCK_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(TLV5618_SCK_PIN, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(TLV5618_CS_GPIO);
    GPIO_setDirectionMode(TLV5618_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(TLV5618_CS_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setMasterCore(TLV5618_CS_PIN, GPIO_CORE_CPU1);
    GPIO_setQualificationMode(TLV5618_CS_PIN, GPIO_QUAL_SYNC);

}









