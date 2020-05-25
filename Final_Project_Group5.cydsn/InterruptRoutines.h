/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    /*variable definition*/
    #define BYTE_TO_SEND 8
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1
    
    /*Prototyping interrupt service routine on receiving UART*/
    CY_ISR_PROTO (Custom_ISR_RX);
    
    /*Prototyping the interrupt service routine for triggering when FIFO is full*/
    CY_ISR_PROTO (Custom_isr_FIFO);
    
    /*Prototyping interrupt service routine on Timer count*/
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    /*save buffer array*/
    uint8 DataBuffer[TRANSMIT_BUFFER_SIZE];    
    
    /*Flag for packet ready*/
    volatile uint8 PacketReadyFlag;
    
    /*prove per ISR FIFO*/
    uint8_t data[6];
    uint8_t new_data;
    #define UART_1_PutBuffer UART_1_PutString(bufferUART)
    char bufferUART[100];
    
    
#endif


/* [] END OF FILE */
