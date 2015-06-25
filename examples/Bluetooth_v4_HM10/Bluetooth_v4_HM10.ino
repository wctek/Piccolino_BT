#include <Wire.h>
#include <Piccolino_BT.h>

Piccolino_BT BT;

//Connect TX and RX with a wire and run this sketch

void setup() 
{
  Serial.begin(9600); 

  Serial.print(F("Initializing ..."));
  BT.begin(0x9a,HM10);  //default i2c address is 0x9A(A1,A0 = low), HM10 (BLE) or HC05 (BT2.0)

  // see the HC05 datasheet for more information on AT commands.  
  BT.Cmd("AT+ROLES"); // Set as Slave 
  BT.Cmd("AT+TYPE2"); // Require Pin for connection  
  BT.Cmd("AT+POWE3"); // Maximun TX Power (0-3) 2 default
  // password (pin) must be 6 digits and can't be 000000
  BT.Cmd("AT+PASS000001"); // the 6 digit security pin  
  BT.Cmd("AT+NAMEPiccolino"); // our BT name
  BT.Cmd("AT+RESET"); // All done, RELOAD NEW SETTINGS (reset your device also since BT caches names)

  Serial.println(F("Done!\nReady - waiting for connection"));
    
};

void loop() 
{
   char ch;

  while(BT.available()) {
    ch=BT.read();
   Serial.print(ch);
  }
  
  while(Serial.available()) {
    ch=Serial.read();
    BT.write(ch);
  }
      
};

