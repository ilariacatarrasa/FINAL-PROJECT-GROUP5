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

/* [] END OF FILE */
