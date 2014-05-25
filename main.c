#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_i2c.h>
#include <stdio.h>
#include "ds1307.h"

void USART_init1(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    /* enable APB2 peripheral clock for USART1
     * note that only USART1 and USART6 are connected to APB2
     * the other USARTs are connected to APB1
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* enable the peripheral clock for the pins used by
     * USART1, PB6 for TX and PB7 for RX
     */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* This sequence sets up the TX and RX pins
     * so they work correctly with the USART1 peripheral
     */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins B6 (TX) and B7 (RX) are used
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // the pins are configured as alternate function so the USART peripheral has access to them
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // this defines the IO speed and has nothing to do with the baudrate!
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // this defines the output type as push pull mode (as opposed to open drain)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // this activates the pullup resistors on the IO pins
    GPIO_Init(GPIOB, &GPIO_InitStruct); // now all the values are passed to the GPIO_Init() function which sets the GPIO registers

    /* The RX and TX pins are now connected to their AF
     * so that the USART1 can take over control of the
     * pins
     */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

    /* Now the USART_InitStruct is used to define the
     * properties of USART1
     */
    USART_InitStruct.USART_BaudRate = baudrate; // the baudrate is set to the value we passed into this init function
    USART_InitStruct.USART_WordLength = USART_WordLength_8b; // we want the data frame size to be 8 bits (standard)
    USART_InitStruct.USART_StopBits = USART_StopBits_1; // we want 1 stop bit (standard)
    USART_InitStruct.USART_Parity = USART_Parity_No; // we don't want a parity bit (standard)
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
    USART_Init(USART1, &USART_InitStruct);

    // finally this enables the complete USART1 peripheral
    USART_Cmd(USART1, ENABLE);

    printf("\n\rUSART1 INITIALIZED!\n\r");
}

int main(void) {
    USART_init1(9600);
    I2C_init1();

    time t1 = getTime();
    printTime(&t1);

    // Enable external square wave output with frequency 1Hz
    t1.sqwe = 1;
    t1.rs0 = 0;
    t1.rs0 = 0;

    t1.date = 25;
    t1.month = 05;
    t1.year = 14;

    t1.hours = 13;
    t1.minutes = 9;
    t1.seconds = 20;

//    writeTime(&t1);


    u8 *ptr = read(0x00, RAM_SIZE, true);
    printRawData(ptr, RAM_SIZE);
    free(ptr);

    t1 = getTime();
    printTime(&t1);

    ptr = read(0x00, 8, false);
    printRawData(ptr, 8);

    while (1) {
    };

}
