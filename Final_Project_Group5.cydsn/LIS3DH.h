/* ========================================
 *
 * FILE header.h Project 5.
 *
 * @autors: Catarrasa Ilaria, Fasani Federica, Milani Ilaria
 * ========================================
*/

#pragma once 

#include "SPI_Interface_LIS3DH.h"

#define SLAVE_ACC_CS_Write CS_2_Write
#define SPIM2               SPIM_2

 /*** ========= MACROS ========= ***/   

/** Instruction Set **/
#define SPI_ACC_SINGLE_WRITE 0b00000000   //to write 1 byte
#define SPI_ACC_MULTI_WRITE  0b01000000   //to write multiple bytes
#define SPI_ACC_SINGLE_READ  0b10000000   //to read 1 byte
#define SPI_ACC_MULTI_READ   0b11000000   //to read multiple bytes

//Address of the WHO AM I register
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

// Address of the Status register
#define LIS3DH_STATUS_REG 0x27

//Address of the Control register 1
#define LIS3DH_CTRL_REG1 0x20
//Define ODR HR / Normal / Low-power mode (1 Hz) and 
//hex value to set resolution mode to the accelerator. x,y,z enabled
#define LIS3DH_CTRL_REG1_ODR_START_1HZ  0x17   /* 1 Hz  */
#define LIS3DH_CTRL_REG1_ODR_START_10HZ 0x27   /* 10 Hz */
#define LIS3DH_CTRL_REG1_ODR_START_25HZ 0x37   /* 25 Hz */
#define LIS3DH_CTRL_REG1_ODR_START_50HZ 0x47   /* 50 Hz */


//brief Address of the Control register 4 
#define LIS3DH_CTRL_REG4 0x23
//FSR BDU ACTIVE
#define LIS3DH_CTRL_REG4_FSR_2  0x80    /* +- 2g  */     
#define LIS3DH_CTRL_REG4_FSR_4  0x90    /* +- 4g  */
#define LIS3DH_CTRL_REG4_FSR_8  0xA0    /* +- 8g  */   
#define LIS3DH_CTRL_REG4_FSR_16 0xB0    /* +- 16g */

//brief Address of the Control register 5
#define LIS3DH_CTRL_REG5 0x24
//FIFO enable
#define LIS3DH_CTRL_REG5_FIFO_EN 0x40
//Latch interrupt request on INT1_SRC register
#define LIS3DH_CTRL_REG5_LIR_INT1 0x20


//brief Address of the Control register 
#define LIS3DH_FIFO_CTRL_REG 0x2E
//Enable FIFO MODE, trigger event allows triggering signal on INT1
#define LIS3DH_FIFO_CTRL_REG_FIFO_MODE 0x40
//Enable Stream FIFO MODE, trigger event allows triggering signal on INT1
#define LIS3DH_FIFO_CTRL_REG_STRM_MODE 0x80
//Enable STEAM-TO-FIFO mode (default)
#define LIS3DH_FIFO_CTRL_REG_STRM2FIFO_MODE 0xC0
//Enable Bypass mode (default)
#define LIS3DH_FIFO_CTRL_REG_BYPASS_MODE 0x00


//brief Address of the Control register 3
#define LIS3DH_CTRL_REG3 0x22
//Enable interrupt on Watermark = 32 samples 
#define LIS3DH_CTRL_REG3_FIFO_WTM 0x04

//brief Address of the Interrupt-1 source register
#define LIS3DH_SRC_REG 0x31


//brief Address FIFO source
#define LIS3DH_FIFO_SRC_REG 0x2F
//Watermark = new data available
#define LIS3DH_FIFO_SRC_REG_WTM_FIFO 0x80
//Overrun FIFO Occurred
#define LIS3DH_FIFO_SRC_REG_OVRN_FIFO 0x40


/**
*   \brief Address of the X, Y, Z output LSB and MSB registers
*/
#define LIS3DH_OUT_X_L 0x28    /*LSB X*/
#define LIS3DH_OUT_X_H 0x29    /*MSB X*/

#define LIS3DH_OUT_Y_L 0x2A    /*LSB Y*/
#define LIS3DH_OUT_Y_H 0x2B    /*MSB Y*/

#define LIS3DH_OUT_Z_L 0x2C    /*LSB Z*/
#define LIS3DH_OUT_Z_H 0x2D    /*MSB Z*/


//#define LENGTH_BYTE 10
//#define TRANSMIT_BUFFER_SIZE 1+LENGTH_BYTE+1
//#define DATA_SIZE  3
//#define DATA_BYTES 6

/*** ========= FUNCTION DECLARATIONS ========= ***/

/** ====== Helper Functions ====== **/

/*
* @brief Write a single byte to set a register with the value dataByte @ reg_addr.
*
* @param[in]: 6-bit memory address to write to.
* @param[out]: 8-bit (unsigned byte) value to write in the memory location.
*/
void ACC_writeRegister(uint8_t reg_addr, uint8_t dataByte);

/*
* @brief Read the register byte @ reg_addr.
*
* @param[in]: 6-bit memory address to read from.
* @return Read value (1 unsigned byte).
*/
uint8_t ACC_readRegister(uint8_t reg_addr);

/*
* @brief Read 1+ bytes from memory.
*
* @param[in]: 6-bit memory address to from read.
* @param[out]: uint8_t* pointer to the output data (cast operation required for different data type).
* @param[in]: Number of bytes to read.
*
*/
void ACC_Multi_Read(uint8_t addr, uint8_t* data, uint8_t nBytes);

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
void ACC_TEMP_8bytePacking(uint8_t* SixByte_Data, uint8_t* TwoByte_Data, uint8_t* EightByte_Data, int lenght);


/* [] END OF FILE */