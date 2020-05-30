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
        
    volatile uint8_t counter=FIRST_DATA_ADDR;    
    
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
    
    //SAVE ACCELEROMETER AND TEMP DATA IN EEPROM, SEND DEPACKET DATA TO BRIDGE CONTROL PANEL/////////////
              
//    uint8_t data_prova[6] = {0b00000000, 0b11111111 , 0b00000000, 0b11111111 , 0b00000000, 0b11111111 };    
    uint8_t data_EE[6] = {0}; //Data to be stored in EEPROM
    uint8_t data_BCP[10] = {0}; // Data to send to Bridge Control Panel
    //Function for storing in EEPROM data in 4 byte
//    Store_EEPROM(data_prova, data_EE);
//    sprintf(bufferUART, " data EEPROM 0x%02X 0x%02X 0x%02X 0x%02X \r\n", data_EE[0], data_EE[1], data_EE[2], data_EE[3]);
//    UART_1_PutBuffer;
    //WRITE IN EEPROM
    //
    
    ///Function for send Data to be sent to BRIDGE CONTROL PANEL        
    Send_BCP(data_EE, data_BCP);
    sprintf(bufferUART, " data BCP 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X \r\n", data_BCP[0], data_BCP[1], data_BCP[2], data_BCP[3], data_BCP[4], data_BCP[5], data_BCP[6]);
    UART_1_PutBuffer;
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
    
    for(;;){
        
        fifo_src_reg = ACC_readRegister(LIS3DH_FIFO_SRC_REG);     
            
        
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
            
            StartFlag =  2;  //exit from flag of START/stop
            
            /* Storing data in EEPROM */
            //Data to store in EEPROM: 4 bytes accelerometer (transform from 6 to 4 bytes) + 2bytes TEMP 
            //Data operations--> obtain data_EEPROM
            
            data[6] = DataBuffer[5]; 
            data[7] = DataBuffer[6]; 
            
            Store_EEPROM(data, data_EE);
            sprintf(bufferUART, "** Data_EE Read = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \r\n", data_EE[0], data_EE[1], data_EE[2],  data_EE[3], data_EE[4], data_EE[5]);
            UART_1_PutBuffer;
            
            EEPROM_writePage(counter, (uint8_t*) data_EE, DATA_BYTES_EEPROM);
            EEPROM_waitForWriteComplete();

            EEPROM_readPage(counter, (uint8_t*) data_read_array, DATA_BYTES_EEPROM);
            sprintf(bufferUART, "** EEPROM Read = 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \r\n", data_read_array[0], data_read_array[1], data_read_array[2],  data_read_array[3], data_read_array[4], data_read_array[5]);
            UART_1_PutBuffer;
           
            //Storing new set of data in EEPROM
            //EPROM_writePage((FIRST_DATA_ADDR + counter), (uint8_t*) data_EEPROM, DATA_BYTES_EEPROM);            
            //EEPROM_waitForWriteComplete();
            
            
            //update the storage counter in EEPROM at the address of COUNTER_ADD
            counter= counter+DATA_BYTES_EEPROM;
            EEPROM_writeByte(COUNTER_AD, counter);
            EEPROM_waitForWriteComplete();
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
        }
    }               
    
}

/* [] END OF FILE */
