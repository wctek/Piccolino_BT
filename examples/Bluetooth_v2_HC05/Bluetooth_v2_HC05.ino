#include <Wire.h>
#include <Piccolino_BT.h>

Piccolino_BT BT;

//Connect TX and RX with a wire and run this sketch

void setup() 
{
  Serial.begin(9600); 

  Serial.print(F("Initializing ..."));
  BT.begin(0x9A,HC05);  //default i2c address is 0x9A(A1,A0 = low) init for HC05 (bt2.0)

  // see the HC05 datasheet for more information on AT commands.  
  BT.Cmd("AT+ROLE=0"); // Set as Slave
  BT.Cmd("AT+PSWD=0000"); // the 4 digit security pin
  BT.Cmd("AT+NAME=Piccolino"); // our BT name
  BT.Cmd("AT+CMODE=1"); // Accept connections from any address
  BT.Cmd("AT+DISC"); // disconnect any active connections (just in case)    
  
  Serial.println(F("Done!\nReady - waiting for connection"));
  
  while(strcmp(BT.getStatus(),"CONNECTED")) // sleep here until we get a connection
      delay(500);

  Serial.println(F("Connected!\n"));
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

