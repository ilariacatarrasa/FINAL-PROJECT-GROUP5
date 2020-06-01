****************************************************************
FINAL PROJECT GROUP 5.
****************************************************************
Program developed to to store accelerometer data in the external 
EEPROM, and stream them over UART for visualization.
****************************************************************
HEADER FILES:
* header.h *
All the macros tha are needed in different c files are written 
in this file.

* 25LC256.h *
All the macros and variables needed for the communication with
 external 
EEPROM are written here.

* InterruptRouines.h *
All the macros and variables needed for the interrupt routines 
are here.

* Functions.h *
All the macros and variables for functions implemented in the 
code are here.

* LIS3DH.h *
All the macros and declarations of functions for setting the 
accelerometer are put in this file.

* SPI_Interface_25LC256.h *
Functions declared for the read and write in EEPROM.

* SPI_Interface_LIS3DH.h *
Functions declared for the read and write from accelerometer.


****************************************************************
C FILES:
* 25LC256.c *
Functions defined for the read and write from accelerometer.

* InterruptRouines.c *
Functions for functioning the menu from UART, for data 
acquisition from interrupt on watermark level (for the 
accelerometer data) and from ADCDelSig (for the 
temperature data) and for the button of reset. 

* Functions.c *
Functions defined for storing data in EEPROM, to send 
to Bridge control panel, to show the menu in the UART and 
to configure the accelerometer at start.

* LIS3DH.c *
Functions defined for the read and write from accelerometer.

* SPI_Interface_25LC256.c *
Functions defined for the read and write in EEPROM.

* SPI_Interface_LIS3DH.c *
Functions defined for the read and write from accelerometer.
****************************************************************
WHAT TO DO:

From uart you can visualize the menu file that explains the 
possible actions. 
Menu:
b=Start data acquisition from the accelerometer and storage on 
the EEPROM memory;  
s=Stop data acquisition from the accelerometer and storage on 
the EEPROM memory;
f=Configuration of the accelerometer full scale range;
p=Configuration of the accelerometer sampling frequency ;
v=Start data visualization in the Bridge Control Panel;
u=Stop data visualization in the Bridge Control Panel;
t=Store the temperature data.

If you want to show the menu click the "?" case and it will 
appear again.

