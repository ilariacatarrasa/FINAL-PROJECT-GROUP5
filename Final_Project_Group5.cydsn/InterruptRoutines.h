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
    
   
    /* brief High and Low level state for digital output pin */
    #define EXT_LED_ON  1
    #define EXT_LED_OFF 0
    
    /*Prototyping interrupt service routine on receiving UART*/
    CY_ISR_PROTO (Custom_ISR_RX);
    
    /*Prototyping the interrupt service routine for triggering when FIFO is full*/
    CY_ISR_PROTO (Custom_isr_FIFO);
    
    /*Prototyping interrupt service routine on Timer count*/
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    /*save buffer array*/
    uint8 DataBuffer[TRANSMIT_BUFFER_SIZE];    
    
    
    volatile uint8 PacketReadyFlag; /*Flag for packet ready*/
    volatile uint8 StartFlag;
    volatile uint8 FSRFlag;
    volatile uint8 SamplingFreqFlag;
    uint8 DataBuffer[TRANSMIT_BUFFER_SIZE];
    
    
#endif


/* [] END OF FILE */
