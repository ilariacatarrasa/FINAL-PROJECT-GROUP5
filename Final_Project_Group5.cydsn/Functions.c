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

/*Include macros*/
#include "header.h"

/* EEPROM 25LC256 Library */
#include "25LC256.h"

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

void ACC_TEMP_8bytePacking(uint8_t* SixByte_Packs, uint8_t* TwoByte_Packs, uint8_t* EightByte_Packs, int lenght)
{
    uint8_t count = 0;
    uint8_t SixByte_Packs_reject[60];
    //reject the first sample
    
    for (int k=0; k<60; k++)
    {
         SixByte_Packs_reject [k] = SixByte_Packs[k+6];   
    }
 
    for (int i=0; i<lenght; i+=6)
    {      
        for (int j=0; j<10; j+=2)
        {
            EightByte_Packs[i   +2*count] = SixByte_Packs_reject[i];
            EightByte_Packs[i+1 +2*count] = SixByte_Packs_reject[i+1];
            EightByte_Packs[i+2 +2*count] = SixByte_Packs_reject[i+2];
            EightByte_Packs[i+3 +2*count] = SixByte_Packs_reject[i+3];
            EightByte_Packs[i+4 +2*count] = SixByte_Packs_reject[i+4];
            EightByte_Packs[i+5 +2*count] = SixByte_Packs_reject[i+5];
            EightByte_Packs[i+6 +2*count] = TwoByte_Packs[j];
            EightByte_Packs[i+7 +2*count] = TwoByte_Packs[j+1];         
        }     
    
        count++;             
//    sprintf(bufferUART, " %d \r\n", EightByte_Data[i]);
//    UART_1_PutBuffer;

    }
}

/*Send data of accelerometer and temperaure to EEPROM */
void Store_EEPROM(uint16_t counter, uint8_t* tobepacked, uint8_t* tobesentEEPROM)
{   
    int j = 0;
    
    for (uint i=0; i<80; i += 8)
    {
        // Store X, Y, Z accelerometer data
        tobesentEEPROM[j] = tobepacked[i];
        tobesentEEPROM[j + 1] = ( tobepacked[i] & 0b11000000 ) | ( tobepacked[i + 3]>>2 );
        tobesentEEPROM[j + 2] = ((( tobepacked[i + 3]<<6 ) | ( tobepacked[i + 2]>>2 ))& 0b11110000 )| ( tobepacked[i + 58]>>4 );  
        tobesentEEPROM[j + 3] = ( tobepacked[i + 5]<<4 ) | ( tobepacked[i + 4]>>6 );
        tobesentEEPROM[j + 4] = tobepacked[i + 6]; // Store MSB of temperature data
        tobesentEEPROM[j + 5] = tobepacked[i + 7]; // Store LSB of temperature data
        EEPROM_writePage(counter, (uint8_t*) tobesentEEPROM, DATA_BYTES_EEPROM);
        EEPROM_waitForWriteComplete();
        
        j += DATA_BYTES_EEPROM ;
        counter= counter+DATA_BYTES_EEPROM;
       
        EEPROM_readPage(counter, (uint8_t*) tobesentEEPROM, DATA_BYTES_EEPROM);
        sprintf(bufferUART, "** EEPROM Read = %d %d %d %d %d %d \r\n", tobesentEEPROM[j], tobesentEEPROM[j + 1], tobesentEEPROM[j + 2],  tobesentEEPROM[j + 3], tobesentEEPROM[j + 4], tobesentEEPROM[j + 5]);
        UART_1_PutBuffer;
    }
}

/*Send data to Bridge control panel*/

void Send_BCP(uint8_t counter, uint8_t* tobedepacked, uint8_t* data_BCP, uint8_t temperature_modality)
{         
    int value_temp;
    uint16_t temperature;
    
    //start the EEPROM read from the counter address
    EEPROM_readPage(counter, (uint8_t*) tobedepacked, DATA_BYTES_EEPROM);
    
    data_BCP[0] = HEADER; // Headerof buffer to send to Bridge Control Panel
        
    data_BCP[1] = tobedepacked[0]; 
    data_BCP[2] = tobedepacked[1] & 0b11000000;
    data_BCP[3] = ( tobedepacked[1] << 2 ) | ( tobedepacked[2] >> 6 );  
    data_BCP[4] = ( tobedepacked[2] << 2 ) & 0b11000000; 
    data_BCP[5] = ( tobedepacked[2] << 4 ) | ((tobedepacked[3] >> 4 ) & (0b00001111));
    data_BCP[6] = ( tobedepacked[3] << 4 ) & 11110000;
    
    temperature = (uint16_t) tobedepacked[4] << 8 |  ( (uint16_t) tobedepacked[5] ) ;
 
    value_temp = ADC_DelSig_CountsTo_mVolts(temperature);
    sprintf(bufferUART, " value temperature MV: %d  \n\r", value_temp);
    UART_1_PutBuffer;
    value_temp = (value_temp-500)/10; // Temperature in Celsius
    if(temperature_modality==1)
    {
        value_temp= value_temp * 9/5 + 32; // Temperature in Fahrenheit
    }
    sprintf(bufferUART, " value temperature: %d  \n\r", value_temp);
    UART_1_PutBuffer;
    
    data_BCP[7] = (uint16_t)value_temp >> 8;  //MSB of temperature data
    data_BCP[8] = (uint16_t)value_temp & 0xFF ; //LSB of temperature data
    
    data_BCP[9] = TAIL; // Tail of buffer to send to Bridge Control Panel
    
    //send the pack to the BCP
    UART_1_PutArray(data_BCP, TRANSMIT_BUFFER_SIZE);

}

/*Set ACCELEROMETER*/

void Config_acc( uint8_t counter, uint8_t config, uint16_t address)
{
    int i = 0;
    uint8_t zero_array[64] = {0};
    
    counter=FIRST_DATA_ADDR;
    EEPROM_writeByte(COUNTER_AD, counter);
    EEPROM_waitForWriteComplete();   
    EEPROM_writeByte(address, config);
    EEPROM_waitForWriteComplete();
    
    UART_1_PutString("New configuration set: 0x02X \r\n");
    
}

/* [] END OF FILE */
