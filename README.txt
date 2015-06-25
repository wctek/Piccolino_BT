Piccolino I2C Bluetooth 2.0/4.0 Library
Written by Alex Sardo - http://piccolino.rocks

Simple BT library for the Piccolino BT expansion board. The board uses the
SC16IS760 I2C UART to create a hardware serial channel to either the HM10 or HC05 BT
modules, leaving all other I/O pins available to the user.

The I2CUART IC has 8 user programmable I/O GPIOs. GPIO 0,1 and 2 are used by the
library, leaving GPIO3 to GPIO7 (5 gral purpose pins) free to be used for other
tasks.

To use the spare GPIO pins (3-7), you can just:

// create the BT object
Piccolino_BT BT;

//Init the library
BT.begin(0xa9,XXXX) // XXXX is either HC05 or HM10

//set gpio3 as output
BT.pinMode(3,OUTPUT);

//set gpio3 HIGH
BT.digitalWrite(0,HIGH);

See the included examples for more information.
