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

//Hex value to set resolution mode to the accelerator. x,y,z enabled
#define LIS3DH_HR_1_CTRL_REG1  0x23  /* 1 Hz  */
#define LIS3DH_HR_10_CTRL_REG1 0x39  /* 10 Hz */
#define LIS3DH_HR_25_CTRL_REG1 0x55  /* 25 Hz */
#define LIS3DH_HR_50_CTRL_REG1 0x71  /* 50 Hz */


//brief Address of the Control register 4 
#define LIS3DH_CTRL_REG4 0x23
//FSR BDU ACTIVE
#define LIS3DH_CTRL_REG4_FSR_2  0x80    /* +- 2g  */     
#define LIS3DH_CTRL_REG4_FSR_4  0x90    /* +- 4g  */
#define LIS3DH_CTRL_REG4_FSR_8  0xA0    /* +- 8g  */   
#define LIS3DH_CTRL_REG4_FSR_16 0xB0    /* +- 16g */

//brief Address of the Control register 5
#define LIS3DH_CTRL_REG5 0x24
//FIFO enabling stream mode
#define LIS3DH_CTRL_REG5_FIFO_EN 0x40

//brief Address of the Control register 
#define LIS3DH_FIFO_CTRL_REG 0x2E
//Enable FIFO MODE, trigger event allows triggering signal on INT1
#define LIS3DH_FIFO_CTRL_REG_FIFO_MODE 0x40
//Enable Bypass mode (default)
#define LIS3DH_FIFO_CTRL_REG_BYPASS_MODE 0x00

//brief Address of the Control register 3
#define LIS3DH_CTRL_REG3 0x22
//Enable FIFO data overrun (when FIFO is full interrupt enables and stops collecting data)
#define LIS3DH_CTRL_REG3_FIFO_OVERRUN 0x02

//brief Address
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


#define LENGTH_BYTE 10
#define TRANSMIT_BUFFER_SIZE 1+LENGTH_BYTE+1
#define DATA_SIZE  3
#define DATA_BYTES 6

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



/* [] END OF FILE */
