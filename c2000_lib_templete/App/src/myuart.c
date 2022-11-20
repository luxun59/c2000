/*
 * myuart.c
 *
 *  Created on: 2022年8月13日
 *      Author: 14913
 */




#include "myuart.h"
#include "stdio.h"


int fputc(int _c, register FILE *_fp)
{
        while (SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
                HWREGH(SCIA_BASE + SCI_O_TXBUF) = _c;
        return _c;
}

int putc(int _c, register FILE *_fp)
{
    while (SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
            HWREGH(SCIA_BASE + SCI_O_TXBUF) = _c;
    return _c;
}

int putchar(int data)
{
  while (SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
  HWREGH(SCIA_BASE + SCI_O_TXBUF) =data;
  return data;
}

int fputs(const char *_ptr, register FILE *_fp)
{
    unsigned int i, len;
    len = strlen(_ptr);
    for(i=0 ; i<len ; i++)
    {
        while (SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
        HWREGH(SCIA_BASE + SCI_O_TXBUF) = (uint8_t) _ptr[i];
    }
    return len;
}



//
// sciaTxISR - Disable the TXFF interrupt and print message asking
//             for two characters.
//
__interrupt void
sciaTxISR(void)
{

    //
    // Acknowledge the PIE interrupt.
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

//
// sciaRxISR - Read two characters from the RXBUF and echo them back.
//
__interrupt void
sciaRxISR(void)
{
    uint16_t receivedChar1, receivedChar2;

    //
    // Read two characters from the FIFO.
    //
    receivedChar1 = SCI_readCharBlockingFIFO(SCIA_BASE);
//    receivedChar2 = SCI_readCharBlockingFIFO(SCIA_BASE);

    //
    // Echo back the two characters.
    //
    unsigned char *msg;
    msg = "  You sent: \0";
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 13);
    SCI_writeCharBlockingFIFO(SCIA_BASE, receivedChar1);
//    SCI_writeCharBlockingFIFO(SCIA_BASE, receivedChar2);

    //
    // Clear the SCI RXFF interrupt and acknowledge the PIE interrupt.
    //
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXFF);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);


}











void SCI_init(uint32_t rate)
{

    //
    // GPIO28 is the SCI Rx pin.
    //
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    //
    // GPIO29 is the SCI Tx pin.
    //
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);

    //
    //
        // Map the ISR to the wake interrupt.
        //
        Interrupt_register(INT_SCIA_TX, sciaTxISR);
        Interrupt_register(INT_SCIA_RX, sciaRxISR);

        //
        // Initialize SCIA and its FIFO.
        //
        SCI_performSoftwareReset(SCIA_BASE);

        //
        // Configure SCIA for echoback.
        //
        SCI_setConfig(SCIA_BASE, 25000000, rate, (SCI_CONFIG_WLEN_8 |
                                                 SCI_CONFIG_STOP_ONE |
                                                 SCI_CONFIG_PAR_NONE));
        SCI_resetChannels(SCIA_BASE);
        SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
        SCI_enableFIFO(SCIA_BASE);
        SCI_enableModule(SCIA_BASE);
        SCI_performSoftwareReset(SCIA_BASE);

        //
        // Set the transmit FIFO level to 0 and the receive FIFO level to 2.
        // Enable the TXFF and RXFF interrupts.
        //
        SCI_setFIFOInterruptLevel(SCIA_BASE, SCI_FIFO_TX0, SCI_FIFO_RX1);
        SCI_enableInterrupt(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);


        //
        // Clear the SCI interrupts before enabling them.
        //
        SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);

        //
        // Enable the interrupts in the PIE: Group 9 interrupts 1 & 2.
        //
        Interrupt_enable(INT_SCIA_RX);
        Interrupt_enable(INT_SCIA_TX);
        Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);



}





//SCI_writeCharNonBlocking(mySCI0_BASE, sendChar);
//
//        // Wait for RRDY/RXFFST = 1 for 1 data available in FIFO
//        //
//        while(SCI_getRxFIFOStatus(mySCI0_BASE) == SCI_FIFO_RX0)
//        {
//            ;
//        }
//
//        //
//        // Check received character
//        //
//        receivedChar = SCI_readCharBlockingFIFO(mySCI0_BASE);










//#include "stdio.h"
//#define MAX_WRITE_LEN (528)
//void uart_printf(char * fmt, ...) //自定义变参函数
//{
//    char *  arg_ptr;
//    unsigned int  i ,  len;
//    static char write_buf[MAX_WRITE_LEN];
//
//    va_start(arg_ptr, fmt);
//    len = vsprintf((char *)write_buf, fmt, arg_ptr);
//    va_end(arg_ptr);
//
//    for (i = 0; i < len; i++)
//    {
//        if ( '\0' != write_buf[i] )
//        {
//            while (SciaRegs.SCIFFTX.bit.TXFFST != 0); //使用SCIA
//            SciaRegs.SCITXBUF=write_buf[i];
//        }
//    }
//}








