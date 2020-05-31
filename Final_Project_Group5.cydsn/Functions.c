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

void ACC_TEMP_8bytePacking(uint8_t* SixByte_Data, uint8_t* TwoByte_Data, uint8_t* EightByte_Data, int lenght)
{
    uint8_t count = 1;
    //start packing from the 2nd accelerometer sample 
    for (int i=6; i<lenght; i+=6)
    {
        for (int j=0; j<64; j+=2)
        {
            EightByte_Data[i   +2*count] = SixByte_Data[i];
            EightByte_Data[i+1 +2*count] = SixByte_Data[i+1];
            EightByte_Data[i+2 +2*count] = SixByte_Data[i+2];
            EightByte_Data[i+3 +2*count] = SixByte_Data[i+3];
            EightByte_Data[i+4 +2*count] = SixByte_Data[i+4];
            EightByte_Data[i+5 +2*count] = SixByte_Data[i+5];
            EightByte_Data[i+6 +2*count] = TwoByte_Data[j];
            EightByte_Data[i+7 +2*count] = TwoByte_Data[j+1];
            
            count++;
        }        
                    
//    sprintf(bufferUART, " %d \r\n", EightByte_Data[i]);
//    UART_1_PutBuffer;

    }
}

/*Send data of accelerometer and temperaure to EEPROM */
void Store_EEPROM(uint8_t counter, uint8_t* tobepacked, uint8_t* tobesentEEPROM)
{
    // Store X, Y, Z accelerometer data
    tobesentEEPROM[0] = tobepacked[1];
    tobesentEEPROM[1] = ( tobepacked[0] & 0b1100000 ) | ( tobepacked[3]>>2 );
    tobesentEEPROM[2] = ((( tobepacked[3]<<6 ) | ( tobepacked[2]>>2 ))& 0b11110000 )| ( tobepacked[5]>>4 );   
    tobesentEEPROM[3] = ( tobepacked[5]<<4 ) | ( tobepacked[4]>>6 );
    tobesentEEPROM[4] = tobepacked[6]; // Store MSB of temperature data
    tobesentEEPROM[5] = tobepacked[7]; // Store LSB of temperature data
    EEPROM_writePage(counter, (uint8_t*) tobesentEEPROM, DATA_BYTES_EEPROM);
    EEPROM_waitForWriteComplete();
    
    
//    EEPROM_readPage(counter, (uint8_t*) tobesentEEPROM, DATA_BYTES_EEPROM);
//    sprintf(bufferUART, "** EEPROM Read = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \r\n", tobesentEEPROM[0], tobesentEEPROM[1], tobesentEEPROM[2],  tobesentEEPROM[3], tobesentEEPROM[4], tobesentEEPROM[5]);
//    UART_1_PutBuffer;
    
}

/*Sed to Bridge control panel*/
/*Sed to Bridge control panel*/
void Send_BCP(uint8_t counter, uint8_t* tobedepacked, uint8_t* data_BCP, uint8_t temperature_modality)
{       
    
    int value_temp;
    uint16_t temperature;
    
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
    
//    UART_1_PutArray(data_BCP, TRANSMIT_BUFFER_SIZE);

}



/*Set ACCELEROMETER*/
void Config_acc( uint8_t counter, uint8_t config, uint16_t address)
{
    int i = 0;
    uint8_t zero_array[64] = {0};
    
//    for (i=0 ; i<counter - FIRST_DATA_ADDR; i+=64)
//    {
//        EEPROM_writePage((FIRST_DATA_ADDR), (uint8_t*) zero_array, MAX_BYTE_PAGE);
//        EEPROM_waitForWriteComplete();
//        UART_1_PutString("ERASE COMPLETED \r\n");
//    }
    // reset counter stooring data
    
    counter=FIRST_DATA_ADDR;
    EEPROM_writeByte(COUNTER_AD, counter);
    EEPROM_waitForWriteComplete();   
    EEPROM_writeByte(address, config);
    EEPROM_waitForWriteComplete();
    
    UART_1_PutString("New configuration set: 0x02X \r\n");
    
}

/* [] END OF FILE */
