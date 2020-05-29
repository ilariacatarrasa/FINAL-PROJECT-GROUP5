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

/*Include headers*/
#include "InterruptRoutines.h"
#include "Functions.h"
#include "project.h"

/* FUNCTION FOR MENU' ON UART */
void show_menu()
{
    
    UART_1_PutString("\r\n");
    UART_1_PutString("*************************************** MENU' ********************************************\r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");    
    UART_1_PutString("Command  | Result                  \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("b        | Start data acquisition from the accelerometer and storage on the EEPROM memory \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("s        | Stop data acquisition from the accelerometer and storage on the EEPROM memory \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("f        | Configuration of the accelerometer full scale range \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("p        | Configuration of the accelerometer sampling frequency \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("v        | Start data visualization in the Bridge Control Panel \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("u        | Stop data visualization in the Bridge Control Panel \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("t        | Store the temperature data \r\n");
    UART_1_PutString("------------------------------------------------------------------------------------------\r\n");
    UART_1_PutString("\r\n");
    UART_1_PutString("*******************************************************************************************\r\n");
    
    return;
}


void Store_EEPROM(uint8_t* tobepacked, uint8_t* tobesentEEPROM)
{
    tobesentEEPROM[0] = tobepacked[1];
    tobesentEEPROM[1] = (tobepacked[0] & 0b1100000) | (tobepacked[3]>>2);
    tobesentEEPROM[2] = (((tobepacked[3]<<6) | (tobepacked[2]>>2))& 0b11110000)| (tobepacked[5]>>4);   
    tobesentEEPROM[3] = (tobepacked[5]<<4) | (tobepacked[4]>>6);
    tobesentEEPROM[4] = 0; // Store MSB of temperature data
    tobesentEEPROM[5] = 0;
}
void Send_BCP(uint8_t* tobedepacked, uint8_t* data_BCP)
{
    data_BCP[0] = HEADER; // Headerof buffer to send to Bridge Control Panel
        
    data_BCP[1] = tobedepacked[0]; 
    data_BCP[2] = tobedepacked[1] & 0b11000000;
    data_BCP[3] = ( tobedepacked[1] << 2 ) | ( tobedepacked[2] >> 6 );  
    data_BCP[4] = ( tobedepacked[2] << 2 ) & 0b11000000; 
    data_BCP[5] = (tobedepacked[2] << 4 ) | ((tobedepacked[3] >> 4 ) & (0b00001111));
    data_BCP[6] = ( tobedepacked[3] << 4 ) & 11110000;
    
    data_BCP[7] = 0x00; //MSB of temperature data
    data_BCP[8] = 0x00; //LSB of temperature data
    
    data_BCP[9] = TAIL; // Tail of buffer to send to Bridge Control Panel

}

/* [] END OF FILE */
