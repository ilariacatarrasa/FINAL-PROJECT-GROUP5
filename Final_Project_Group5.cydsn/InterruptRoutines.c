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


// ISR priorities are to be setted!!
// io direi di mettere priorità maggiore nel isr_RX, però ditemi voi





// Include header
#include "InterruptRoutines.h"
#include "project.h"
#include "LIS3DH.h"
#include "Functions.h"


// Variabile declaration
uint8 ch_received;
// uint8 StartFlag=0;
// uint8 SelectionFlag=0;
uint8 FSRFlag=0;
uint8 SamplingFreqFlag=0;
uint8 TemperatureMode=0;
uint8 FahrenheitFlag=0;
int   value_temp;
int32 value_digit;
int   value_mv; 

CY_ISR_PROTO (Custom_isr_FIFO)
{
    /*Send data to EEPROM*/
}

CY_ISR(Custom_ISR_ADC)
{
    /*Read Timer status register to bring interrupt line low*/
    Timer_ReadStatusRegister();  
    
    if(TemperatureMode==1) // if Temperature mode command has been received on UART
    {
        value_digit=ADC_DelSig_Read32(); // Read value in ADC
    
        if(value_digit<0)      value_digit=0;
        if(value_digit>65535)  value_digit=65535;
        
        DataBuffer[5]=value_digit>>8; // shift a dx di 8 bit --> MSB
        DataBuffer[6]=value_digit & 0xFF; // & bit a bit --> LSB
        
        value_mv=ADC_DelSig_CountsTo_mVolts(value_digit);
        value_temp= ; // Temperature in Celsius
        if(FahrenheitFlag==1)
        {
            value_temp= value_temp * 9/5 + 32; // Temperature in Fahrenheit
        }
        
        PacketReadyFlag=1;
    }
}

CY_ISR(Custom_ISR_RX)
{
    ch_received=UART_1_GetChar();
    switch (ch_received)
    {
        // ? command on UART: show the menù
        case '?':
            show_menu();
        break;
            
        // ON command on UART
        case 'B':
        case 'b':
            
            // StartFlag=1;
            // Start data acquisition from the accelerometer
            // Start data storage on the EEPROM
            // Save the configuration on the EEPROM
          
        break;
            
        // OFF command on UART
        case 'S':
        case 's':
            
            // StartFlag=0;
            // Stop data acquisition from the accelerometer
            // Stop data storage on the EEPROM
            // Save the configuration on the EEPROM
           
        break;
            
        case 'F':
        case 'f':
            // Selection full scale range command on UART
            /* SHOW FSR MENU' ON UART*/
            UART_1_PutString("Configuration of the accelerometer full scale range \r\n");
            UART_1_PutString("Character            Full scale range \r\n");
            UART_1_PutString("1                    +- 2g \r\n");
            UART_1_PutString("2                    +- 4g \r\n");
            UART_1_PutString("3                    +- 8g \r\n");
            UART_1_PutString("4                    +- 16g \r\n");
            FSRFlag=1;
         
            // Temperature in Fahrenheit command on UART
            if (TemperatureMode==1)
            {
                FahrenheitFlag=1;
            }
        break;
            
        // Selection sampling frequency command on UART
        case 'P':
        case 'p':
            /* SHOW SAMPLING FREQUENCY MENU' ON UART*/
            UART_1_PutString("Configuration of the accelerometer sampling frequency \r\n");
            UART_1_PutString("Character            Sampling Frequency \r\n");
            UART_1_PutString("1                    1Hz \r\n");
            UART_1_PutString("2                    10Hz \r\n");
            UART_1_PutString("3                    25Hz \r\n");
            UART_1_PutString("4                    50Hz \r\n");
            SamplingFreqFlag=1;
              
        break;
            
        // 1 command on UART
        case '1':
            if (FSRFlag==1) // +- 2g
            {
                // Read register
                // communicate on UART if this FSR is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG4) & LIS3DH_CTRL_REG4_FSR_2 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new FSR + communicate on UART the change
                else 
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_2);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 1Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_HR_1_CTRL_REG1 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_1_CTRL_REG1);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag=0;
            }
        break;
            
        // 2 command on UART
        case '2':
            if (FSRFlag==1) // +- 4g
            {
                // Read register
                // communicate on UART if this FSR is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG4) & LIS3DH_CTRL_REG4_FSR_4 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new FSR + communicate on UART the change
                else 
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_4);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 10Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_HR_10_CTRL_REG1 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_10_CTRL_REG1);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag=0;
            }
        break;
            
        // 3 command on UART
        case '3':
            if (FSRFlag==1) // +- 8g
            {
                // Read register
                // communicate on UART if this FSR is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG4) & LIS3DH_CTRL_REG4_FSR_8 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new FSR + communicate on UART the change
                else 
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_8);
                     UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 25Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_HR_25_CTRL_REG1 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_25_CTRL_REG1);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag=0;
            }
        break;
            
        // 4 command on UART
        case '4':
            if (FSRFlag==1) // +- 16g
            {
                // Read register
                // communicate on UART if this FSR is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG4) & LIS3DH_CTRL_REG4_FSR_16 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new FSR + communicate on UART the change
                else 
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG4,LIS3DH_CTRL_REG4_FSR_16);
                     UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                FSRFlag=0; 
            }
            if (SamplingFreqFlag==1) // 50Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_HR_50_CTRL_REG1 )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_HR_50_CTRL_REG1);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag=0;
            }
        break;
            
        // Visualize data on Bridge Control Panel command on UART
        case 'V':
        case 'v':
            
        break;
            
        // Stop visualize data on Bridge Control Panel command on UART
        case 'U':
        case 'u':
            
        break;
            
        // Temperature mode command on UART
        case 'T':
        case 't':
            if ( TemperatureMode == 0 )
            {
                Timer_Start(); // Start sampling Temperature values every Timer overflow
            }
            /* SHOW TEMPERATURE MENU' ON UART*/
            UART_1_PutString("Store the temperature data \r\n");
            UART_1_PutString("Character            Temperature format \r\n");
            UART_1_PutString("c                    Store data in Celsius format \r\n");
            UART_1_PutString("f                    Store data in Fahrenheit format  \r\n");
            TemperatureMode=1;
            
            // Timer_Stop(); in usvita dalla modalità temp?
        break;
            
        // Temperature in Celsius command on UART
        case 'C':
        case 'c':
            if (TemperatureMode==1)
            {
                FahrenheitFlag=0;
            }
            
        break;
        
        default:
        break;
    }
}

/* [] END OF FILE */
