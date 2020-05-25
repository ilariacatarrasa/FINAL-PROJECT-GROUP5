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
    
    /* Start ADC */
    ADC_DelSig_Start();
    
    /* Start ISR_RX */
    isr_RX_StartEx(Custom_ISR_RX);
    
    /* Start ISR_FIFO */
    //isr_FIFO_StartEx(Custom_isr_FIFO);
    
    /* Start ISR_ADC */
    isr_ADC_StartEx(Custom_ISR_ADC);  
    
    /* Start ADC conversion */
    ADC_DelSig_StartConvert();
    
    /* Initialize PWM so that the Onboard LED is OFF */
    PWM_OnboardLED_Start();
    
    CyDelay(10); //"The boot procedure is complete about 10 milliseconds after device power-up."
        
  
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
    
    UART_1_PutString("**************Registers Values**************\r\n");
    
    /*Setting control register 1*/
     uint8_t ctrl_reg1;    
    //Enable the ODR
    ACC_writeRegister(LIS3DH_CTRL_REG1, LIS3DH_CTRL_REG1_ODR_START_1HZ);
                
    ctrl_reg1 = ACC_readRegister(LIS3DH_CTRL_REG1);
    sprintf(bufferUART, " Control register 1 has been set to = 0x%02X \r\n", ctrl_reg1);
    UART_1_PutBuffer;
    
    /* Setting Control Register 5 */
            
    uint8_t ctrl_reg5;    
    //Enable the FIFO 
    ACC_writeRegister(LIS3DH_CTRL_REG5, LIS3DH_CTRL_REG5_FIFO_EN|LIS3DH_CTRL_REG5_LIR_INT1);
                
    ctrl_reg5 = ACC_readRegister(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, " Control register 5 has been set to = 0x%02X \r\n", ctrl_reg5);
    UART_1_PutBuffer;
    
     /* Setting Control Register of FIFO */
    uint8_t ctrl1_FIFO;    
    //Enable the Stream mode for FIFO and latch int1
    ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE);
                
    ctrl1_FIFO = ACC_readRegister(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, " Control register of FIFO has been set to = 0x%02X \r\n", ctrl1_FIFO);
    UART_1_PutBuffer;
    
    /* Setting Control Register 3 */

    uint8_t ctrl_reg3;
    
    //Enable FIFO Overrun interrupt on INT1.
    ACC_writeRegister(LIS3DH_CTRL_REG3, LIS3DH_CTRL_REG3_FIFO_OVERRUN);
    
    ctrl_reg3 = ACC_readRegister(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, " Control register 3 has been set to = 0x%02X \r\n", ctrl_reg3);
    UART_1_PutBuffer;
    
    UART_1_PutString("********************************************\r\n");
    /**/

    uint8_t new_data;
    uint8_t data[6];        

    
    // show the menu
    show_menu();


    for(;;){
       


        
        new_data = ACC_readRegister(LIS3DH_FIFO_SRC_REG);
        //sprintf(bufferUART, " data 0x%02X  \r\n", INT1_Read());
        //UART_1_PutBuffer;
        if ( new_data & LIS3DH_FIFO_SRC_REG_OVRN_FIFO ) //      
        {              
            /* non funziona :
            for (i = 0; i<32; i++)
            {
                //ACC_Multi_Read(LIS3DH_OUT_X_L, &data[0], 6); 
            
            }
            //DA PROVARE COME LA EEPROM DOPO*/          
            
            UART_1_PutString(" Overrun occurred! \r\n");
                        
            ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_BYPASS_MODE);            
            ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE);
        }
        else
        {               
            data[0] = ACC_readRegister(LIS3DH_OUT_Z_L);
            data[1] = ACC_readRegister(LIS3DH_OUT_Z_H);
            if (data[0]||data[1] != 0)
            {
                sprintf(bufferUART, " data 0x%02X 0x%02X \r\n", data[0], data[1]);
                UART_1_PutBuffer;
            }
            /*sprintf(bufferUART, " OVerrun not occurred 0x%02X \r\n", new_data);
            UART_1_PutBuffer;  */          
        }
        
        
    }
}


/* [] END OF FILE */
