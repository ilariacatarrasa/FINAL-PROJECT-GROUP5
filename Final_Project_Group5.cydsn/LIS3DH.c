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
#include "LIS3DH.h"

/** ====== User-level Functions ====== **/


void ACC_writeRegister(uint8_t reg_addr, uint8_t dataByte) {
  	
	/* Prepare the TX packet */
    uint8_t dataTX[2] = {(SPI_ACC_SINGLE_WRITE | reg_addr) , dataByte};
	/* Nothing to RX... */
	uint8_t temp = 0;
	
    /* Enable the Slave */
    SLAVE_ACC_CS_Write(1);
    
	/* Write 1 byte to addr */
	SPI_Interface_LIS3DH_Multi_RW(dataTX, 2, &temp, 0);
    
	/* Disable the Slave */  
    SLAVE_ACC_CS_Write(0);
}

uint8_t ACC_readRegister(uint8_t reg_addr) {

	/* Prepare the TX data packet: instruction + address */
	uint8_t dataTX[1] = {(SPI_ACC_SINGLE_READ | reg_addr)};
	
	/* Prepare the RX byte */
	uint8_t dataRX = 0;
	
	/* Read 1 byte from addr */
	SPI_Interface_LIS3DH_Multi_RW(dataTX, 1, &dataRX, 1);
        
	return dataRX;

}

void ACC_Multi_Read(uint8_t addr, uint8_t* dataRX, uint8_t nBytes) {
    
	/* Prepare the TX data packet: instruction + address */
	uint8_t dataTX[1] = {( SPI_ACC_MULTI_READ | addr )};
	
	/* Read the nBytes */
	SPI_Interface_LIS3DH_Multi_RW(dataTX, 1, dataRX, nBytes);
		
}


/* [] END OF FILE */
