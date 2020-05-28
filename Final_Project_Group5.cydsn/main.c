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

/* EEPROM 25LC256 Library */
#include "25LC256.h"
/* EEPROM LIS3DH Library */
#include "LIS3DH.h"

//Define buffer for UART
#define UART_1_PutBuffer UART_1_PutString(bufferUART)
char bufferUART[100];

//Define data_acc dimension
#define DATA_BYTES 6
//Define data to store in EEPROM: 4bytes ACC + 2bytes TEMP
#define DATA_BYTES_EEPROM 6
//Define max number of byte to write in a single operation
#define MAX_BYTE_PAGE 64

/* Define configurations to save in EEPROM */
//Configuration ON-OFF
#define ON   0b10000000
#define OFF  0b01000000
//FSR config is saved in LIS3DH_CTRL_REG4
//SF config is saved in LIS3DH_CTRL_REG1

/* Define addresses in EEPROM */
//Configurations for accelerometer address
#define ACC_CONFIG_FSR_AD 0x0000
#define ACC_CONFIG_SF_AD  0x0001
//Configuration ON-OFF for accelerometer and temperature mode address
#define ONOFF_ADD 0x0002
//Counter for data address
#define COUNTER_AD 0x0003
//First data address
#define DATA_AD 0x0004
#define HIGHEST_ADDRESS 0x7FFF //32767


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
        
//    /* PROJECT 1 */    
//    /* Definition of the extern from 25LC256.c */
//    uint8_t eeprom_Status = 0;
//    
//    eeprom_Status = EEPROM_readStatus();
//    
//    sprintf(bufferUART, "** EEPROM Status = 0x%02x\r\n", eeprom_Status);
//    UART_1_PutBuffer;
//    
//    /* PROJECT 2 */
//    /* Value to Write */
//    uint8_t data = 94;
//    
//    /* Write */
//    EEPROM_writeByte(0x0000, data);
//    EEPROM_waitForWriteComplete();
//    
//    /* Read */
//    uint8_t data_read = EEPROM_readByte(0x0000);
//    
//    sprintf(bufferUART, "** EEPROM Status = %d (%d)\r\n", data_read, data);
//    UART_1_PutBuffer;
 
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
    ACC_writeRegister(LIS3DH_CTRL_REG5, LIS3DH_CTRL_REG5_FIFO_EN); //|LIS3DH_CTRL_REG5_LIR_INT1
                
    ctrl_reg5 = ACC_readRegister(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, " Control register 5 has been set to = 0x%02X \r\n", ctrl_reg5);
    UART_1_PutBuffer;
    
     /* Setting Control Register of FIFO */
    uint8_t ctrl1_FIFO;    
    //Enable the Stream mode for FIFO and latch int1
    ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_STRM_MODE);
                
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

    // Variables definitions
    uint8_t data[DATA_BYTES]; //
    uint8_t data_EEPROM[DATA_BYTES_EEPROM];
    uint8_t counter=0;
    char ZEROARRAY[64] = {0};
    int i=0;
    uint8_t data_read; //data read from EEPROM
    uint8_t fifo_src_reg;
    uint8_t FIFO_data[192];
    
    //save counter in EEPROM at the address of COUNTER_ADD
    EEPROM_writeByte(COUNTER_AD, counter);
    EEPROM_waitForWriteComplete();
    
    
    /* Variable initialization */
    PacketReadyFlag=0;
    StartFlag=0;                    //può creare problemi al reset??
    FSRFlag=0;
    SamplingFreqFlag=0;
    
    
    // show the menu
    show_menu();
    
    //Cleaning FIFO buffer
    uint8_t cleaning[6];
    
    while (ACC_readRegister(LIS3DH_FIFO_SRC_REG) == 0x20)
    {
        ACC_Multi_Read(LIS3DH_OUT_X_L, (uint8_t*) cleaning, 6);
    }
    sprintf(bufferUART, "\r\n FIFO clean \r\n");
    UART_1_PutBuffer;   

    for(;;){
        
            fifo_src_reg = ACC_readRegister(LIS3DH_FIFO_SRC_REG);
        
//        sprintf(bufferUART, " data 0x%02X  \r\n", fifo_src_reg);
//        UART_1_PutBuffer;
               
        
            if ( fifo_src_reg & LIS3DH_FIFO_SRC_REG_OVRN_FIFO )       
               {              
                ACC_Multi_Read(LIS3DH_OUT_X_L, (uint8_t*) FIFO_data, 192); 
                
                fifo_src_reg = ACC_readRegister(LIS3DH_FIFO_SRC_REG);
                sprintf(bufferUART, " fifo_src_reg dopo la multiread 0x%02X  \r\n", fifo_src_reg);
                UART_1_PutBuffer;
                
                sprintf(bufferUART, " Primi 63 valori del buffer: \r\n");
                UART_1_PutBuffer;   
                    
                for (uint i=0; i<192; i++)
                {
                    sprintf(bufferUART, " %d ", FIFO_data[i]);
                    UART_1_PutBuffer;
                }
                
                                    
                ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_BYPASS_MODE);  
                CyDelayUs(2);
                ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE);
                
                fifo_src_reg = ACC_readRegister(LIS3DH_FIFO_SRC_REG);
                sprintf(bufferUART, "\r\n fifo_src_reg dopo il reset 0x%02X  \r\n", fifo_src_reg);
                UART_1_PutBuffer;
        }
        
        /* EEPROM */
        
        //istituire un flag per ogni caso in isr_RX? perchè avendo i wait non può stare nella isr
        // if START
        if(StartFlag==1)
        {
            /* Storing data in EEPROM */
            //Data to store in EEPROM: 4bytes ACC (transform from 6 to 4 bytes) + 2bytes TEMP 
            //Data operations--> obtain data_EEPROM
            //update the storage counter in EEPROM at the address of COUNTER_ADD
            counter= counter+DATA_BYTES_EEPROM;
            EEPROM_writeByte(COUNTER_AD, counter);
            EEPROM_waitForWriteComplete();
            //Storing new set of data in EEPROM
            EEPROM_writePage((DATA_AD + counter), (uint8_t*) data_EEPROM, DATA_BYTES_EEPROM);
            EEPROM_waitForWriteComplete();
            data_read = EEPROM_readByte(ONOFF_ADD);
            if(data_read!=ON)
            {
                EEPROM_writeByte(ONOFF_ADD, ON);
                EEPROM_waitForWriteComplete();
                // Read: making sure the writing was sussesful
                data_read = EEPROM_readByte(ONOFF_ADD);   
                sprintf(bufferUART, "** EEPROM Status ON-OFF = %d \r\n", data_read);
                UART_1_PutBuffer;
            }
        }
        // if STOP
        else if(StartFlag==0)
        {
            if(data_read==ON)
            {
                EEPROM_writeByte(ONOFF_ADD, OFF);
                EEPROM_waitForWriteComplete();
                // Read: making sure the writing was sussesful
                data_read = EEPROM_readByte(ONOFF_ADD);   
                sprintf(bufferUART, "** EEPROM Status ON-OFF = %d \r\n", data_read);
                UART_1_PutBuffer;
            }
        }
        
        // if there is any change in accelerometer configuration of FSR or SF
        if(FSRFlag!=0 | SamplingFreqFlag!=0)
        {
            // delate previous data in EEPROM
            for (i=0;i<((counter-DATA_AD)/MAX_BYTE_PAGE);i++)
            {
                EEPROM_writePage((DATA_AD), (uint8_t*) ZEROARRAY, MAX_BYTE_PAGE);
                EEPROM_waitForWriteComplete();
            }
            // reset counter stooring data
            counter=0;
            EEPROM_writeByte(COUNTER_AD, counter);
            EEPROM_waitForWriteComplete();
            // save new configuration in EEPROM memory
            // FSR configuration
            if(FSRFlag==1)
            {
               EEPROM_writeByte(ACC_CONFIG_FSR_AD, LIS3DH_CTRL_REG4_FSR_2);
               FSRFlag=0;
            }
            else if(FSRFlag==2)
            {
               EEPROM_writeByte(ACC_CONFIG_FSR_AD, LIS3DH_CTRL_REG4_FSR_4); 
               FSRFlag=0;
            }
            else if(FSRFlag==3)
            {
               EEPROM_writeByte(ACC_CONFIG_FSR_AD, LIS3DH_CTRL_REG4_FSR_8);
               FSRFlag=0;
            }
            else if(FSRFlag==4)
            {
               EEPROM_writeByte(ACC_CONFIG_FSR_AD, LIS3DH_CTRL_REG4_FSR_16); 
               FSRFlag=0;
            }
            // SF configuration
            if(SamplingFreqFlag==1)
            {
               EEPROM_writeByte(ACC_CONFIG_SF_AD, LIS3DH_CTRL_REG1_ODR_START_1HZ);
               SamplingFreqFlag=0;
            }
            else if(SamplingFreqFlag==2)
            {
               EEPROM_writeByte(ACC_CONFIG_SF_AD, LIS3DH_CTRL_REG1_ODR_START_10HZ); 
               SamplingFreqFlag=0;
            }
            else if(SamplingFreqFlag==3)
            {
               EEPROM_writeByte(ACC_CONFIG_SF_AD, LIS3DH_CTRL_REG1_ODR_START_25HZ);
               SamplingFreqFlag=0;
            }
            else if(SamplingFreqFlag==4)
            {
               EEPROM_writeByte(ACC_CONFIG_SF_AD, LIS3DH_CTRL_REG1_ODR_START_50HZ); 
               SamplingFreqFlag=0;
            }
            
            EEPROM_waitForWriteComplete();
                    
            // Read: making sure the writing was sussesful
            data_read = EEPROM_readByte(ACC_CONFIG_FSR_AD);   
            sprintf(bufferUART, "** EEPROM FSR = %d \r\n", data_read);
            UART_1_PutBuffer;
            data_read = EEPROM_readByte(ACC_CONFIG_SF_AD);   
            sprintf(bufferUART, "** EEPROM SF = %d \r\n", data_read);
            UART_1_PutBuffer;
            
        }           
}
    
}

/* [] END OF FILE */
