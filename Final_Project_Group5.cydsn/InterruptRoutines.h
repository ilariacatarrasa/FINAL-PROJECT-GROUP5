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
    #include "header.h"
    #include "string.h"
    #include "project.h"
    
    /*variable definition*/
    #define BYTE_TO_SEND 8
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1
    #define BYTE_TEMP_TO_STORE 20    
    
    /* brief High and Low level state for digital output pin */
    #define EXT_LED_ON  1
    #define EXT_LED_OFF 0
    #define START 1
    #define STOP 0
    
    /* Time counter to count the 5s pressure of the on-board button (20ms*50=1s) */
    #define FIVE_SEC 50*5
    
    /*Prototyping interrupt service routine on receiving UART*/
    CY_ISR_PROTO (Custom_ISR_RX);
    
    /*Prototyping the interrupt service routine for triggering when FIFO is full*/
    CY_ISR_PROTO (Custom_isr_FIFO);
    
    /*Prototyping interrupt service routine on Timer count*/
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    /*Prototyping interrupt service routine on Button pression*/
    CY_ISR_PROTO(Custom_ISR_Button);  

    /* brief State of button pressed (logic low)*/
    #define BUTTON_PRESSED 0
    
    volatile uint8 PacketReadyFlag; /*Flag for packet ready*/
    volatile uint8 StartFlag;
    volatile uint8 FSRFlag;
    volatile uint8 SamplingFreqFlag;
    volatile uint8 dataTemp[BYTE_TEMP_TO_STORE];
    volatile int   value_temp;
    volatile uint8_t Buffer_Temp_Full;
       
    volatile int count_wtm;       
    volatile uint8_t datiAcc[8];
    volatile uint8_t data_EE[6];
    volatile uint8_t data_BCP[10];
    volatile uint16_t counter;
    uint8_t fifo_src_reg;
    

    volatile uint8_t FIFO_Read_Flag;
    
    volatile uint8_t dataAcc[66];
    //flag for the system reset
    volatile uint8_t ButtonPressed_flag;  

#endif


/* [] END OF FILE */
