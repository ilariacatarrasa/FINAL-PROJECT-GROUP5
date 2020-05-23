/**
 * @file main.c
 * @brief Main file of the EEPROM 25LC256 and Accelerometer LIS3DH project
 * 
 * Communication of accelerometer via SPI protocol
 * Usage of 25LC256 SPI EEPROM
 * interfaced using the library hereby implemented.
 *
 * @autors: Catarrasa Ilaria, Fasani Federica, Milani Ilaria
 * @date 13-May-2020
 */

// Include required header files
#include "project.h"
#include "stdio.h"
#include "string.h"
#include "InterruptRoutines.h"
#include "Functions.h"

//Define buffer for UART
#define UART_1_PutBuffer UART_1_PutString(bufferUART)
char bufferUART[100];

/* EEPROM 25LC256 Library */
#include "25LC256.h"
/* EEPROM LIS3DH Library */
#include "LIS3DH.h"


int main(void) {
    
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start UART */
    UART_1_Start();
    
    /* Start SPI Master 1*/
    SPIM_1_Start();
    
    /* Start SPI Master 2*/
    SPIM_2_Start();
    
    /* Start ISR_RX */
    isr_RX_StartEx(Custom_ISR_RX);
    
    /* Start ISR_FIFO */
    isr_FIFO_StartEx(Custom_isr_FIFO);
    
        
    
    CyDelay(10); //"The boot procedure is complete about 10 milliseconds after device power-up."
        
    
//    UART_1_PutString("*********    EEPROM TEST    *********\r\n");
//    
//    /* Definition of the extern from 25LC256.c */
//    //uint8_t eeprom_Status = 0;
//    
//    /* Data to Write */
//    int16_t data[DATA_SIZE] = {15, -32, 258};
//    
//    /**** PROJECT 3 *****/
//    
//    /* Write */
//    EEPROM_writePage(0x0001, (uint8_t*) data, DATA_BYTES);
//    EEPROM_waitForWriteComplete();
//    
//    /**** PROJECT 4 ****/
//    /* Read */
//    int16_t data_read[DATA_SIZE];
//    EEPROM_readPage(0x0001, (uint8_t*) data_read, DATA_BYTES);
//    
//    
//    
//    sprintf(bufferUART, "** EEPROM Read = %d %d %d \r\n", data_read[0], data_read[1], data_read[2]);
//    UART_1_PutBuffer;
//    
//    UART_1_PutString("*************************************\r\n");
//    
//    return 0;
    
    
    /* Read WHO AM I Accelerometer's register */

    uint8_t who_am_i;
    
    who_am_i = ACC_readRegister(LIS3DH_WHO_AM_I_REG_ADDR);
    sprintf(bufferUART, "** Who am I register = 0x%02X [Expected: 0x33]\r\n", who_am_i);
    UART_1_PutBuffer;
    
    
    /*******************************************************************/
    /*            SPI to set Accelerometer's registers                 */
    /*******************************************************************/
    
    /* Setting Control Register 5 */
    
    UART_1_PutString("**************Registers Values**************\r\n");
    
    uint8_t ctrl_reg5;    
    //Enable the FIFO 
    ACC_writeRegister(LIS3DH_CTRL_REG5, LIS3DH_CTRL_REG5_FIFO_EN);
                
    ctrl_reg5 = ACC_readRegister(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, " Control register 5 has been set to = 0x%02X \r\n", ctrl_reg5);
    UART_1_PutBuffer;
    
     /* Setting Control Register FIFO */
    uint8_t ctrl1_FIFO;    
    //Enable the FIFO mode
    ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE);
                
    ctrl_reg5 = ACC_readRegister(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, " Control register of FIFO has been set to = 0x%02X \r\n", ctrl1_FIFO);
    UART_1_PutBuffer;
    /* Setting Control Register 3 */

    uint8_t ctrl_reg3;
    
    ACC_writeRegister(LIS3DH_CTRL_REG3, LIS3DH_CTRL_REG3_FIFO_OVERRUN);
    
    ctrl_reg3 = ACC_readRegister(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, " Control register 3 has been set to = 0x%02X \r\n", ctrl_reg3);
    UART_1_PutBuffer;
    
    UART_1_PutString("********************************************\r\n");
    /**/
    
    // show the menù
    show_menu();

    for(;;){
    
    }
}

/* [] END OF FILE */
