/* ========================================
 *
 * DEFINITIONS OF MACROS
 * .
 *  Authors:
 * ========================================
*/
//Define buffer for UART
#define UART_1_PutBuffer UART_1_PutString(bufferUART)
char bufferUART[100];

//Define data_acc dimension
#define DATA_BYTES 6
//Define data to store in EEPROM: 4bytes ACC + 2bytes TEMP
#define DATA_BYTES_EEPROM 6


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
#define FIRST_DATA_ADDR 0x0004
#define HIGHEST_ADDRESS 0x7FFF //32767
//Define max number of byte to write in a single operation
#define MAX_BYTE_PAGE 64

/* [] END OF FILE */
