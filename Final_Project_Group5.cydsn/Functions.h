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
    
    /*Declaration of function to store in EEPROM data of accelerometer and temperature*/
    void Store_EEPROM(uint8_t* tobepacked, uint8_t* tobesentEEPROM);    
#endif

#ifndef __SEND_BCP__
    #define __STORE_EEPROM__
    #include "cytypes.h"
    
    //Header and Tail for data buffer to sent to Bridge control panel
    #define HEADER 0xA0
    #define TAIL 0xC0
    
    /*Declaration of function to send to Bridge control panel data of accelerometer and temperature*/
    void Send_BCP(uint8_t* tobedepacked, uint8_t* data_BCP);    
#endif

/* [] END OF FILE */
