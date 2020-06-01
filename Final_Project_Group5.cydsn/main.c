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

/*Include the macros*/
#include "header.h"

//Counter for address where to start



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
    
    // Start sampling Temperature values every Timer overflow
    Timer_Start();
    
    /* Initialize PWM so that the Onboard LED is OFF */
    PWM_OnboardLED_Start();
    
    /* Start on-board push button ISR */
    ISR_Button_StartEx(Custom_ISR_Button);
    
    
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
    ACC_writeRegister(LIS3DH_CTRL_REG1, LIS3DH_CTRL_REG1_ODR_START_50HZ);
                
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
    //Enable FIFO mode and the Watermark on 9 samples
    //ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE | 0x0A);
    //ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE);
    ACC_writeRegister(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE | 0x0A);
                
    ctrl1_FIFO = ACC_readRegister(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, " Control register of FIFO has been set to = 0x%02X \r\n", ctrl1_FIFO);
    UART_1_PutBuffer;
    
    /* Setting Control Register 3 */

    uint8_t ctrl_reg3;
    
    //Enable FIFO Watermark interrupt on INT1.
    ACC_writeRegister(LIS3DH_CTRL_REG3, 0x04);
    
    ctrl_reg3 = ACC_readRegister(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, " Control register 3 has been set to = 0x%02X \r\n", ctrl_reg3);
    UART_1_PutBuffer;
    
    UART_1_PutString("********************************************\r\n");
    /**/
            
    
    uint8_t data_read; //data read from EEPROM
    uint8_t fifo_src_reg;
    uint8_t FIFO_data[192];
    
    //save counter in EEPROM at the address of COUNTER_ADD
    EEPROM_writeByte(COUNTER_AD, counter);
    EEPROM_waitForWriteComplete();
        
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
    

    
    ///Function for send Data to be sent to BRIDGE CONTROL PANEL        
  
    //SENT TO UART
    // 
    ///////////////////////////////////////////////////////////////////
    FSRFlag = 0;
    SamplingFreqFlag = 0;
    StartFlag = 0;
    uint8_t onoff_initial;
    uint8_t data_add;
    uint8_t fsr_init;
    uint8_t sf_init;
    uint8_t data[8] = {0,0,0,0,0,0,0,0};
    uint8_t data_read_array[DATA_BYTES_EEPROM]= {0,0,0,0,0,0};
    
    
    // Inizializzazioni START STOP 
//    onoff_initial = EEPROM_readByte(ONOFF_ADD);
//    if (onoff_initial & ON)
//    {
//        StartFlag = START;
//    }
//    else if(onoff_initial & OFF)
//    {
//        StartFlag = STOP;
//    }
//    else
//    {
//        EEPROM_writeByte(ONOFF_ADD, OFF);
//        EEPROM_waitForWriteComplete();
//    }  
//    
//    data_add = EEPROM_readByte(COUNTER_AD);
//    fsr_init = EEPROM_readByte(ACC_CONFIG_FSR_AD);
//    if (data_add < FIRST_DATA_ADDR)
//    {
//        EEPROM_writeByte(ACC_CONFIG_FSR_AD, LIS3DH_CTRL_REG4_FSR_2);
//        EEPROM_writeByte(ACC_CONFIG_SF_AD, LIS3DH_CTRL_REG1_ODR_START_1HZ);
//        EEPROM_writeByte(COUNTER_AD, FIRST_DATA_ADDR);
//        EEPROM_waitForWriteComplete();        
//    }
//    else
//    {
//        counter = data_add;
//        FSRFlag = fsr_init;
//        SamplingFreqFlag = sf_init;
//    }
//      
    
    //PER FEDE
    EEPROM_writeByte(ONOFF_ADD, OFF);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(COUNTER_AD, FIRST_DATA_ADDR);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(ACC_CONFIG_FSR_AD, LIS3DH_CTRL_REG4_FSR_2);
    EEPROM_waitForWriteComplete();
    EEPROM_writeByte(ACC_CONFIG_SF_AD, LIS3DH_CTRL_REG1_ODR_START_1HZ);
    EEPROM_waitForWriteComplete();    
    ///////////////////////////////////////////////////////////////////
//    
    uint8_t dataAccTemp[80];
    
    for(;;){
      
        
        /* EEPROM */
        // save new configuration in EEPROM memory:    
        
        // FSR configuration
        if((FSRFlag != 0 )&&(FSRFlag != 1 ))
        {
            Config_acc(counter, FSRFlag, ACC_CONFIG_FSR_AD);           //stores new config data if there is any change from UART
            FSRFlag = 0;
        }
        
        // Sampling Frequency configuration
        if ((SamplingFreqFlag != 0)&&(SamplingFreqFlag != 1 ))
        {
            Config_acc(counter, SamplingFreqFlag, ACC_CONFIG_SF_AD);  //stores new config data if there is any change from UART
            SamplingFreqFlag = 0;
        }

        //If acquisition is started from user
        if(StartFlag == START)
        { 
            EEPROM_writeByte(ONOFF_ADD, ON);
            EEPROM_waitForWriteComplete();
            UART_1_PutString("** EEPROM Status ON ** \r\n");
            
            counter= counter+DATA_BYTES_EEPROM;
            StartFlag =  2;  //exit from flag of START/stop

            UART_1_PutString(" start flag=2\r\n");
            
            //Start FIFO interrupt on watermatk to start data acquisition from the accelerometer
            isr_FIFO_StartEx(Custom_isr_FIFO);
            
            if (FIFO_Read_Flag == 1)
            {
                ACC_TEMP_8bytePacking((uint8_t*) dataAcc, (uint8_t*) dataTemp, (uint8_t*) dataAccTemp, 80);

                
                
//                UART_1_PutString(" flag read=1\r\n");
//                
//                for (uint i=0; i<80; i++)
//                {
//                    sprintf(bufferUART, " %d ", dataAccTemp[i]);
//                    UART_1_PutString(bufferUART);
//                }
//                UART_1_PutString(" *********\r\n");
                    
                FIFO_Read_Flag = 0;          
        }
    
        }
        
        //If acquisition is stopped from user
        if(StartFlag==STOP)
        {       
            EEPROM_writeByte(ONOFF_ADD, OFF);
            EEPROM_waitForWriteComplete();
            UART_1_PutString("** EEPROM Status OFF ** \r\n");

            StartFlag = 2; //exit from flag of START/stop

        }
            
        //Reset counter if the EEPROM storage is full
        
        if ((int)counter > (int)(HIGHEST_ADDRESS-DATA_BYTES_EEPROM+1))
        {
            counter = FIRST_DATA_ADDR;
            
            //PATTERN BOTTONE EEPROM PIENA
        }
    } 
    
    
}

/* [] END OF FILE */
