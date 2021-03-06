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
/**
 * @file SPI_Interface.c
 * @brief Functions required to interface with the SPI MASTER (SPIM).
 * 
 * This source code file contains macros and functions to interface
 * with the SPI Master (SPIM) of the PSoC.
 *
 * @author Mattia Pesenti
 * @date April 9, 2020
*/

/*											 
 * NOTE: in the .h Lib of the SPI Slave     		   *
 *       you need to define SLAVE_CS_Write  		   *
 *       with the name of the Chip Select    		   * 
 *       pin (as defined in the Top Design)  		   *
 * E.g.: #define SLAVE_CS_Write CS_1_Write (default)   *
*/

#include "SPI_Interface_LIS3DH.h"

#ifndef SLAVE_ACC_CS_Write
    #define SLAVE_ACC_CS_Write CS_2_Write
#endif

/*
* @brief Full-Duplex, Single-Operation 1-Byte Trade
*/
uint8_t SPI_Interface_LIS3DH_tradeByte(uint8_t byte) {
	
	/* Enable the Slave */
	SLAVE_ACC_CS_Write(0);
	
	/* Load the TX Buffer */
	SPIM_2_WriteTxData(byte);
	
	/* Wait for TX */
    while( !(SPIM_2_ReadTxStatus() & SPIM_2_STS_BYTE_COMPLETE) );
	
	/* Read the RX Buffer */
	uint8_t data = SPIM_2_ReadRxData();
	
	/* Disable the Slave */
	SLAVE_ACC_CS_Write(1);
	
	return data;
    
}


/*
* @brief RX-only, Dual-Operation 1-Byte READ
*/
uint8_t SPI_Interface_LIS3DH_ReadByte(uint8_t byteTX) {
	
    /* Enable the Slave */
	SLAVE_ACC_CS_Write(0);
	/* Load the TX Buffer */
    SPIM_2_WriteTxData(byteTX); /*byte da inviare*/
    	
    /* SPI Dummy Byte */
    SPIM_2_WriteTxData(SPI_DUMMY_BYTE); /*dummy*/
    	
    /* Wait for TX/RX */
    while(!(SPIM_2_ReadTxStatus() & SPIM_2_STS_SPI_DONE)); /*Si verificano entrambi i byte, attende se scritti i 2 byte*/
    	                                                    /*attendo scambio di informazione tra slave e master*/
    /* Read the RX Buffer logica FIFO */
    SPIM_2_ReadRxData(); /*dummy read*/
    uint8_t byteRX = SPIM_2_ReadRxData(); /*Vera read*/
    	
    /* Disable the Slave */
    SLAVE_ACC_CS_Write(1);
    	
    return byteRX;	

		
	
}


/*
* @brief Full-Duplex, Multi-Byte Trade (RX/TX)
*/
void SPI_Interface_LIS3DH_Multi_Trade(uint8_t* dataTX, uint8_t* dataRX, uint8_t nBytes) {
	
    
	
}

/*
* @brief RX-only, Dual-Operation Multi-Byte READ/WRITE
*/
void SPI_Interface_LIS3DH_Multi_RW(uint8_t* dataTX, uint8_t bytesTX, uint8_t* dataRX, uint8_t bytesRX) {
	
    /* Enable the Slave */
    SLAVE_ACC_CS_Write(0);
        
    int8_t count = bytesTX, index = 0;
    	
    /* Transmit Data */
    while ( count > 0 ) {
            
    	/* Load the TX buffer with Data*/
            SPIM_2_PutArray(&dataTX[index*SPI_TxBufferSize], (count > SPI_TxBufferSize ? SPI_TxBufferSize : count));
            /* Wait for TX */
            while( !(SPIM_2_ReadTxStatus() & SPIM_2_STS_SPI_DONE) );
            
            /* Update count */
            count -= SPI_TxBufferSize;
            index++;
           
    }
        
    /* Clear the RX Buffer */
    SPIM_2_ClearFIFO();
    SPIM_2_ClearRxBuffer();
        
    /* Init the Dummy TX Buffer */
    uint8_t dummyTX[SPI_RxBufferSize];
    memset(dummyTX, SPI_DUMMY_BYTE, SPI_RxBufferSize);
        
    /* Update count for RX */
    count = bytesRX;
    index = 0;
        
    /* Get the RX Data */
    while ( count > 0 ) {
         
            /* Load the TX buffer with Dummy Bytes*/
            SPIM_2_PutArray(dummyTX, (count > SPI_TxBufferSize ? SPI_TxBufferSize : count));
            /* Wait for TX */
            while( !(SPIM_2_ReadTxStatus() & SPIM_2_STS_SPI_DONE) );
            /* Read the RX Buffer */
            for( uint8_t j = 0; j < (count > SPI_TxBufferSize ? SPI_TxBufferSize : count); j++ ) {
                dataRX[j + index*SPI_RxBufferSize] = SPIM_2_ReadRxData();   
            }
            
            count -= SPI_RxBufferSize;
            index++;
            
    }
        
    /* Disable the Slave */    
    SLAVE_ACC_CS_Write(1);	
    
    /* Clear */
    SPIM_2_ClearFIFO();
    SPIM_2_ClearRxBuffer();
    SPIM_2_ClearTxBuffer();
}

/* [] END OF FILE */
