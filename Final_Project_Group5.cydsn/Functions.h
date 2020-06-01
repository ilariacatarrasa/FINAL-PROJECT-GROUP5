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

#ifndef __SHOW_MENU__
    #define __SHOW_MENU__
    #include "cytypes.h"
    
    /*Declaration of function to show the menù on UART*/
    void show_menu();    
    
#endif

#ifndef __STORE_EEPROM__
    #define __STORE_EEPROM__
    #include "cytypes.h" 
    
    /*
    * @brief Storing packets of 6 accelerometer bytes (X, Y and Z samples) + 2 temperature bytes
    *        in a 256 bytes array. This function rejects the first 6 Acc bytes because the
    *        first Acc. sample that is read using the Watermark is the last sample left by the  
    *        previous read. 
    *        (Check the LIS3DH Application Note (page 52) for details)
    *
    * @param[in]: uint8_t data array containing (accelerometer's) bytes to be splitted in packs of 6 bytes.
    * @param[in]: uint8_t data array containing (temperature's) bytes to be splitted in packs of 2 bytes.
    * @param[out]: uint8_t* pointer to the output array where bytes are ordered as 6 Acc. bytes + 2 Temp. Bytes.
    * @param[in]: Number of bytes of the output array
    *
    */
    void ACC_TEMP_8bytePacking(uint8_t* SixByte_Packs, uint8_t* TwoByte_Packs, uint8_t* EightByte_Packs, int lenght);


    /* @Declaration of function to store in EEPROM data of accelerometer and temperature   
     * @param[counter]: 6-bit memory address to read from.
     * @param[tobepacked]: data to be packed from 10-byte array (6 byte accelerometer and
        2 byte of temperature) to 6-byte ( 4 byte for accelerometer and 2 from temperature)      
     * @param[tobesentEEPROM]: 6-byte packed array ( 4 byte for accelerometer and 2 from
        temperature)      
    */  
    void Store_EEPROM(uint16_t counter, uint8_t* tobepacked, uint8_t* tobesentEEPROM);
#endif

#ifndef __SEND_BCP__
    #define __SEND_BCP__
    #include "cytypes.h"
    
    //Header and Tail for data buffer to sent to Bridge control panel
    #define HEADER 0xA0
    #define TAIL 0xC0
    
    /* @Declaration of function to send to Bridge control panel data of accelerometer and temperature
     * @param[counter]: 6-bit memory address to read from.
     * @param[tobedepacked]: data to be depacked from 6-byte ( 4 byte for accelerometer and 2 from
        temperature to 10-byte array (header, tail, 6 byte accelerometer and 2 byte of temperature
     * @param[data_BCP]: 6-bit memory address to read from.
     * @param[temperature_modality]: flag to distiguish Celsius or Farheneit modaliy chosen from UART
    */
    void Send_BCP(uint8_t counter, uint8_t* tobedepacked, uint8_t* data_BCP, uint8_t temperature_modality);    
#endif

//Set the configuration of accelerometer (FSR and SF)
#ifndef __CONFIG_SETTINGS__
    #define __CONFIG_SETTINGS__
    
    #include "cytypes.h" 
    
    void Config_acc( uint8_t counter, uint8_t config, uint16_t address);
    /*Declaration of function to store in EEPROM data of accelerometer and temperature*/
    

#endif


/* [] END OF FILE */
