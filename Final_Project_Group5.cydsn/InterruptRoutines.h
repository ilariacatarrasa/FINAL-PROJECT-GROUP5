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
    
    /*Prototyping interrupt service routine on receiving UART*/
    CY_ISR_PROTO (Custom_ISR_RX);
    
    /*Prototyping the interrupt service routine for triggering when FIFO is full*/
    CY_ISR_PROTO (Custom_isr_FIFO);
    
#endif


/* [] END OF FILE */
