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
/* Accelerometer LIS3DH Library */
#include "LIS3DH.h"
#include "Functions.h"
/* EEPROM 25LC256 Library */
#include "25LC256.h"

// Variabile declaration
uint8 ch_received;
uint8 TemperatureMode=0;
uint8 FahrenheitFlag=0;
int32 value_digit;
int   value_mv; 
int j;
uint8_t data_start[4];

//Counter for the on-board button pression duration 
long int button_press_counter = 0;
//Last reading of push button
uint8_t ButtonLastState = BUTTON_PRESSED;  
//Current reading of push button
uint8_t ButtonReading = BUTTON_PRESSED;



CY_ISR(Custom_ISR_Button){
    
    //Bring high the line for the on-board button for reset
    ButtonPressed_flag = 1; 
}        

CY_ISR_PROTO (Custom_isr_FIFO)
{
    //read the Interrupt-1 source register to bring interrupt line low
    fifo_src_reg = ACC_readRegister(LIS3DH_SRC_REG);

    ACC_Multi_Read(LIS3DH_OUT_X_L, ( uint8_t*) dataAcc, 66);
    
    ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_BYPASS_MODE);  
    ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE | 0x0A);    

    //bring the multiread flag high 
    FIFO_Read_Flag = 1;    
}

CY_ISR(Custom_ISR_ADC)
{
    /*Read Timer status register to bring interrupt line low*/
    Timer_ReadStatusRegister();
    
    /*counting the duration of the on-board button pression exploiting the ADC isr frequency*/
    
    if (ButtonPressed_flag == 1)
    {
        // Update state and reading
        ButtonLastState = ButtonReading;
        ButtonReading = Onboard_Button_Read();
        // Increment counter
        button_press_counter ++;
        if (ButtonReading != ButtonLastState)
        {
            // Reset counter if something changed
            button_press_counter = 0;
            ButtonPressed_flag = 0;
        }
        
        // If the button has been pressed for 5 seconds
        if (button_press_counter == FIVE_SEC)
        {
            // If push button is still pressed
            if (ButtonReading == BUTTON_PRESSED)
            {
                //Restart storing data in the EEPROM from the first location
                counter=FIRST_DATA_ADDR;
                EEPROM_writeByte(COUNTER_AD, counter);
                EEPROM_waitForWriteComplete();

                //Onboard LED STOPS the "EEPROM full" blinking pattern. Now EEPROM has been reset 
                PWM_OnboardLED_WritePeriod(255);  //1s period
                PWM_OnboardLED_WriteCompare(0);   //Onboard LED OFF

                button_press_counter = 0;
                ButtonPressed_flag = 0;     
                
            }
        }
    }


    value_digit=ADC_DelSig_Read32(); // Read value in ADC

    if(value_digit<0)      value_digit=0;
    if(value_digit>65535)  value_digit=65535;
    
   if (j>=19)
    { 
        j = 0;
        Buffer_Temp_Full = 1;
    }
    dataTemp[j]=value_digit>>8; // shift a dx di 8 bit --> MSB
    j++;
    dataTemp[j]=value_digit & 0xFF; // & bit a bit --> LSB
    j++;

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
            
            StartFlag=START;


            //Onboard LED tells the user that data acquisition is ON
            PWM_OnboardLED_WritePeriod(255);  //1s period
            PWM_OnboardLED_WriteCompare(128); //50% DC 
            // Start data storage on the EEPROM                
            // Save the configuration on the EEPROM
          
        break;
            
        // OFF command on UART
        case 'S':
        case 's':
     
            StartFlag=STOP;
           
            //Stop FIFO watermark isr to stop data acquisition from the accelerometer
            isr_FIFO_Stop();

            //Onboard LED tells the user that data acquisition is OFF
            PWM_OnboardLED_WritePeriod(255);  //1s period
            PWM_OnboardLED_WriteCompare(0);   //Onboard LED OFF
            // Stop data storage on the EEPROM
            
            // Save the configuration on the EEPROM
           
            
        break;
            
        case 'F':
        case 'f':
            if (TemperatureMode==0)
            {
                // Selection full scale range command on UART
                /* SHOW FSR MENU' ON UART*/
                UART_1_PutString("Configuration of the accelerometer full scale range \r\n");
                UART_1_PutString("----------------------------------------------------\r\n");
                UART_1_PutString("Character   | Full scale range \r\n");
                UART_1_PutString("----------------------------------------------------\r\n");
                UART_1_PutString("1           | +- 2g \r\n");
                UART_1_PutString("----------------------------------------------------\r\n");
                UART_1_PutString("2           | +- 4g \r\n");
                UART_1_PutString("----------------------------------------------------\r\n");
                UART_1_PutString("3           | +- 8g \r\n");
                UART_1_PutString("----------------------------------------------------\r\n");
                UART_1_PutString("4           | +- 16g \r\n");
                UART_1_PutString("----------------------------------------------------\r\n");
                FSRFlag = 1;
            }
            // Temperature in Fahrenheit command on UART
            if (TemperatureMode == 1)
            {
                FahrenheitFlag = 1;
                UART_1_PutString("Temperature mode setted in Farheneit\n\r");
            }
            TemperatureMode = 0;
            
        break;
            
        // Selection sampling frequency command on UART
        case 'P':
        case 'p':
            /* SHOW SAMPLING FREQUENCY MENU' ON UART*/
            UART_1_PutString("Configuration of the accelerometer sampling frequency \r\n");
            UART_1_PutString("------------------------------------------------------\r\n");
            UART_1_PutString("Character       | Sampling Frequency \r\n");
            UART_1_PutString("------------------------------------------------------\r\n");
            UART_1_PutString("1               | 1Hz \r\n");
            UART_1_PutString("------------------------------------------------------\r\n");
            UART_1_PutString("2               | 10Hz \r\n");
            UART_1_PutString("------------------------------------------------------\r\n");
            UART_1_PutString("3               | 25Hz \r\n");
            UART_1_PutString("------------------------------------------------------\r\n");
            UART_1_PutString("4               | 50Hz \r\n");
            UART_1_PutString("------------------------------------------------------\r\n");
            SamplingFreqFlag = 1;
              
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
                
                FSRFlag = LIS3DH_CTRL_REG4_FSR_2; 
                UART_1_PutString("FSR set to +-2 g \r\n");
            }
            if (SamplingFreqFlag == 1) // 1Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_CTRL_REG1_ODR_START_1HZ )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_CTRL_REG1_ODR_START_1HZ);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag = LIS3DH_CTRL_REG1_ODR_START_1HZ;
                UART_1_PutString("SF set to 1 Hz \r\n");
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
                FSRFlag = LIS3DH_CTRL_REG4_FSR_4;
                UART_1_PutString("FSR set to +-4 g \r\n");
            }
            if (SamplingFreqFlag == 1) // 10Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_CTRL_REG1_ODR_START_10HZ )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_CTRL_REG1_ODR_START_10HZ);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag = LIS3DH_CTRL_REG1_ODR_START_10HZ;
                UART_1_PutString("SF set to 10 Hz \r\n");
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
                FSRFlag = LIS3DH_CTRL_REG4_FSR_8;
                UART_1_PutString("FSR set to +-8 g \r\n");
            }
            if (SamplingFreqFlag == 1) // 25Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_CTRL_REG1_ODR_START_25HZ )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_CTRL_REG1_ODR_START_25HZ);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag = LIS3DH_CTRL_REG1_ODR_START_25HZ;
                UART_1_PutString("SF set to 25 Hz \r\n");
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
                FSRFlag = LIS3DH_CTRL_REG4_FSR_16;
                UART_1_PutString("FSR set to +-16 g \r\n");
            }
            if (SamplingFreqFlag == 1) // 50Hz
            {
                // Read register
                // communicate on UART if this freq is already setted
                if ( ACC_readRegister(LIS3DH_CTRL_REG1) & LIS3DH_CTRL_REG1_ODR_START_50HZ )
                {
                   UART_1_PutString("The selected option was already been setted \r\n");
                }
                // if not write the new freq + communicate on UART the change
                else
                {
                    ACC_writeRegister(LIS3DH_CTRL_REG1,LIS3DH_CTRL_REG1_ODR_START_50HZ);
                    UART_1_PutString("The change was successful \r\n");
                }
                
                // delate previous data in EEPROM + reset counter stooring data
                // save new configuration in EEPROM memory
                SamplingFreqFlag = LIS3DH_CTRL_REG1_ODR_START_50HZ;
                UART_1_PutString("SF set to 50 Hz \r\n");
            }
        break;
            
        // Visualize data on Bridge Control Panel command on UART
        case 'V':
        case 'v':
            //data acquisition from Acc and Storage in EEPROM must be STOPPED
            //Onboard LED tells the user that data acquisition is OFF
            PWM_OnboardLED_WritePeriod(255);  //1s period
            PWM_OnboardLED_WriteCompare(0);   //Onboard LED OFF
            //External LED tells the user that data are being streamed through UART
            Ext_LED_Write(EXT_LED_ON);
            
            
        break;
            
        // Stop visualize data on Bridge Control Panel command on UART
        case 'U':
        case 'u':
            
            //External LED tells the user that streaming through UART has been stopped
            Ext_LED_Write(EXT_LED_OFF);
            //data acquisition from Acc and Storage in EEPROM must be RE-Started?
            //vedi punto 6 dell'assignment, ultime righe
            
            
        break;
            
        // Temperature mode command on UART
        case 'T':
        case 't':
            /* SHOW TEMPERATURE MENU' ON UART*/
            UART_1_PutString("Store the temperature data \r\n");
            UART_1_PutString("----------------------------------------------------\r\n");
            UART_1_PutString("Character      | Temperature format \r\n");
            UART_1_PutString("----------------------------------------------------\r\n");
            UART_1_PutString("c              | Store data in Celsius format \r\n");
            UART_1_PutString("----------------------------------------------------\r\n");
            UART_1_PutString("f              | Store data in Fahrenheit format  \r\n");
            UART_1_PutString("----------------------------------------------------\r\n");
            TemperatureMode=1;
            
        break;
            
        // Temperature in Celsius command on UART
        case 'C':
        case 'c':
            if (TemperatureMode==1)
            {
                FahrenheitFlag=0;
                UART_1_PutString("Temperature mode setted in Celsius\n\r");
            }
            TemperatureMode=0;
            
        break;
        
        default:
        break;
    }
}

/* [] END OF FILE */
