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

// Include header
#include "InterruptRoutines.h"
#include "project.h"
#include "LIS3DH.h"


// Variabile declaration
uint8 ch_received;
// uint8 StartFlag=0;
uint8 SelectionFlag=0;
uint8 FSRFlag=0;
uint8 SamplingFreqFlag=0;
uint8 TemperatureFlag=0;

CY_ISR(Custom_ISR_RX)
{
    ch_received=UART_1_GetChar();
    switch (ch_received)
    {
        // ? command on UART: show the menù
        case '?':
            // show the menù
            // call function show menù
            /* MENU' on UART */
    
            UART_1_PutString("*************************************\r\n");
            UART_1_PutString("****************MENU'****************\r\n");
            UART_1_PutString("\r\n");
            UART_1_PutString("Command      Result                  \r\n");
            UART_1_PutString("b            Start data acquisition from the accelerometer and storage on the EEPROM memory \r\n");
            UART_1_PutString("s            Stop data acquisition from the accelerometer and storage on the EEPROM memory \r\n");
            UART_1_PutString("f            Configuration of the accelerometer full scale range \r\n");
            UART_1_PutString("p            Configuration of the accelerometer sampling frequency \r\n");
            UART_1_PutString("v            Start data visualization in the Bridge Control Panel \r\n");
            UART_1_PutString("u            Stop data visualization in the Bridge Control Panel \r\n");
            UART_1_PutString("t            Store the temperature data \r\n");
            UART_1_PutString("\r\n");
            UART_1_PutString("*************************************\r\n");


            SelectionFlag=1;
        break;
        // ON command on UART
        case 'B':
        case 'b':
            if (SelectionFlag==1)
            {
                // StartFlag=1;
                // Start data acquisition from the accelerometer
                // Start data storage on the EEPROM
                // Save the configuration on the EEPROM
                SelectionFlag=0;
            }
        break;
        // OFF command on UART
        case 'S':
        case 's':
            if (SelectionFlag==1)
            {
                // StartFlag=0;
                // Stop data acquisition from the accelerometer
                // Stop data storage on the EEPROM
                // Save the configuration on the EEPROM
                SelectionFlag=0;
            }
        break;
        case 'F':
        case 'f':
            // Selection full scale range command on UART
            if (SelectionFlag==1)
            {
                UART_1_PutString("Configuration of the accelerometer full scale range \r\n");
                UART_1_PutString("Character            Full scale range \r\n");
                UART_1_PutString("1                    +- 2g \r\n");
                UART_1_PutString("2                    +- 4g \r\n");
                UART_1_PutString("3                    +- 8g \r\n");
                UART_1_PutString("4                    +- 16g \r\n");
                FSRFlag=1;
                SelectionFlag=0;
            }
            // Temperature in Fahrenheit command on UART
            if (TemperatureFlag==1)
            {
                TemperatureFlag=0;
            }
        break;
        // Selection sampling frequency command on UART
        case 'P':
        case 'p':
            if (SelectionFlag==1)
            {
                UART_1_PutString("Configuration of the accelerometer sampling frequency \r\n");
                UART_1_PutString("Character            Sampling Frequency \r\n");
                UART_1_PutString("1                    1Hz \r\n");
                UART_1_PutString("2                    10Hz \r\n");
                UART_1_PutString("3                    25Hz \r\n");
                UART_1_PutString("4                    50Hz \r\n");
                SamplingFreqFlag=1;
                SelectionFlag=0;
            }
        break;
        // 1 command on UART
        case '1':
            if (FSRFlag==1) // +- 2g
            {
                // Read register
                // communicate on UART if this FSR is already setted
                // if not write the new FSR + communicate on UART the change
                ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_2);
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 1Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                // if not write the new freq + communicate on UART the change
                ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_1_CTRL_REG1);
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag=0;
            }
        break;
        // 2 command on UART
        case '2':
            if (FSRFlag==1) // +- 4g
            {
                ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_4);
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 10Hz
            {
                ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_10_CTRL_REG1);
                SamplingFreqFlag=0;
            }
        break;
        // 3 command on UART
        case '3':
            if (FSRFlag==1) // +- 8g
            {
                ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_8);
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 25Hz
            {
                ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_25_CTRL_REG1);
                SamplingFreqFlag=0;
            }
        break;
        // 4 command on UART
        case '4':
            if (FSRFlag==1) // +- 16g
            {
                ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_16);
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 50Hz
            {
                ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_50_CTRL_REG1);
                SamplingFreqFlag=0;
            }
        break;
        // Visualize data on Bridge Control Panel command on UART
        case 'V':
        case 'v':
            if (SelectionFlag==1)
            {
                SelectionFlag=0;
            }
        break;
        // Stop visualize data on Bridge Control Panel command on UART
        case 'U':
        case 'u':
            if (SelectionFlag==1)
            {
                SelectionFlag=0;
            }
        break;
        // Temperature mode command on UART
        case 'T':
        case 't':
            if (SelectionFlag==1)
            {
                UART_1_PutString("Store the temperature data \r\n");
                UART_1_PutString("Character            Temperature format \r\n");
                UART_1_PutString("c                    Store data in Celsius format \r\n");
                UART_1_PutString("f                    Store data in Fahrenheit format  \r\n");
                TemperatureFlag=1;
                SelectionFlag=0;
            }
        break;
        // Temperature in Celsius command on UART
        case 'C':
        case 'c':
            if (TemperatureFlag==1)
            {
                TemperatureFlag=0;
            }
            //SelectionFlag=0;
        break;
        
        default:
        break;
    }
}

/* [] END OF FILE */
