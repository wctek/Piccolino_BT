/* 

Piccolino_BT - Simple BT library to interface the I2CUART IC to the HC05/HM10 bluetooth modules.
Written by Alex Sardo - http://piccolino.rocks 

History
========================================================================
Version 0.1 - initial release June 2015 - lots of work still to be done.

Feel free to modify - please submit updates to admin@wctek.com

*/

#include <Piccolino_BT.h>
#include <Wire.h>


uint8_t Piccolino_BT::ReadRegister(uint8_t reg_addr)
{
    uint8_t result;
	Wire.beginTransmission(_addr);
	Wire.write((reg_addr<<3));
	Wire.endTransmission(0);
	Wire.requestFrom(_addr,(uint8_t)1);
	result = Wire.read();
	return result;
 	
}

void Piccolino_BT::WriteRegister(uint8_t reg_addr, uint8_t val)
{
		Wire.beginTransmission(_addr);
		Wire.write((reg_addr<<3));
		Wire.write(val);
		Wire.endTransmission(1);
}

uint8_t Piccolino_BT::available(void)
{
   return ReadRegister(Piccolino_BT_REG_RXLVL);
}

size_t Piccolino_BT::write(uint8_t val)
{
	uint8_t tmp_lsr;

	do { 
		tmp_lsr = ReadRegister(Piccolino_BT_REG_LSR);
	} while ((tmp_lsr&0x20) ==0);
	
	WriteRegister(Piccolino_BT_REG_THR,val);   
}

int Piccolino_BT::read(void)
{
	volatile uint8_t val;
	if (available()) {
	  val = ReadRegister(Piccolino_BT_REG_RHR);
	  return val;      
	} else
        return -1;
}

void Piccolino_BT::flush()
{
	uint8_t tmp_lsr;
	
	do { 
		tmp_lsr = ReadRegister(Piccolino_BT_REG_LSR);
	} while ((tmp_lsr&0x20) ==0);
}

void Piccolino_BT::pinMode(uint8_t pin_number, uint8_t i_o)
{
    uint8_t temp_iodir;
    
    temp_iodir = ReadRegister(Piccolino_BT_REG_IODIR);
    if ( i_o == OUTPUT ) {
      temp_iodir |= (0x01 << pin_number);
    } else {
      temp_iodir &= (uint8_t)~(0x01 << pin_number);
    } 
    
    WriteRegister(Piccolino_BT_REG_IODIR, temp_iodir);
}

void Piccolino_BT::digitalWrite(uint8_t pin_number, uint8_t pin_state)
{
    uint8_t temp_iostate;
    
    temp_iostate = ReadRegister(Piccolino_BT_REG_IOSTATE);
    if ( pin_state == 1 ) {
      temp_iostate |= (0x01 << pin_number);
    } else {
      temp_iostate &= (uint8_t)~(0x01 << pin_number);
    }     
    WriteRegister(Piccolino_BT_REG_IOSTATE, temp_iostate);
}

void Piccolino_BT::commandMode()
{
      Piccolino_BT::digitalWrite(1,HIGH); // Enter Command Mode, pull KEY up
}

void Piccolino_BT::commsMode()
{
      Piccolino_BT::digitalWrite(1,LOW); // Exit Command Mode, pull KEY down
}

uint8_t Piccolino_BT::digitalRead(uint8_t pin_number)
{
    uint8_t temp_iostate;
    
    temp_iostate = ReadRegister(Piccolino_BT_REG_IOSTATE);
    if ( temp_iostate & (0x01 << pin_number)== 0 ) {
      return 0;
    } 
    return 1;
}

void Piccolino_BT::reset()
{
  // hardware reset (reset line tied to GPIO2)
  Piccolino_BT::digitalWrite(2,LOW);
  delay(500);
  Piccolino_BT::digitalWrite(2,HIGH);
}

void Piccolino_BT::Cmd(const char *cmd) {
  if(_dev==HC05)
    Piccolino_BT::commandMode(); // Enter Command Mode

  Piccolino_BT::print(cmd); // send command

  if(_dev==HC05)
    Piccolino_BT::print("\r\n");

  delay(150);

  if(_dev==HC05)
    Piccolino_BT::commsMode(); // Exit command Mode

  while(Piccolino_BT::available())
        Piccolino_BT::read(); // flush uart buffer
}

char *Piccolino_BT::getStatus() {
    char f=0,ch;
    Piccolino_BT::commandMode(); // Enter Command Mode
    delay(50);
    Piccolino_BT::print("AT+STATE?\r\n");
    delay(300);
    while(Piccolino_BT::available()&&f<12) {
        ch=Piccolino_BT::read();
        if(ch>32)
            BTSTATUS[f++]=ch;        
        if(ch==':'||ch=='+')
            f=0;
        if(ch<33) {
            BTSTATUS[f]=0;
            f=12;
            delay(100);
           while(Piccolino_BT::available())
            Piccolino_BT::read(); // flush uart buffer
        }
    }
    Piccolino_BT::commsMode(); // Exit command Mode
    return (char *)BTSTATUS;
}

void Piccolino_BT::begin(uint8_t addr, bool dev) 
{
    uint16_t divisor;
    uint8_t prescaler;
    uint8_t temp_lcr;

    _addr = (addr>>1);
    _dev = dev;

    Wire.begin();

    // set uart speed (we're limited to 9600 for the BT2.0 module since it can't go any faster, so be it)

    if ( (ReadRegister(Piccolino_BT_REG_MCR)&0x80) == 0) //if prescaler==1
        prescaler = 1;
    else 
        prescaler = 4;
   
    divisor = (Piccolino_BT_CRYSTCAL_FREQ/prescaler)/(153600); // 9600 baud * 16

    temp_lcr = ReadRegister(Piccolino_BT_REG_LCR);
    temp_lcr |= 0x80;
    WriteRegister(Piccolino_BT_REG_LCR,temp_lcr); 
    WriteRegister(Piccolino_BT_REG_DLL,(uint8_t)divisor);
    WriteRegister(Piccolino_BT_REG_DLH,(uint8_t)(divisor>>8));
    temp_lcr &= 0x7F;
    WriteRegister(Piccolino_BT_REG_LCR,temp_lcr); 
    // set data bits
    temp_lcr = ReadRegister(Piccolino_BT_REG_LCR);
    temp_lcr &= 0xC0; // clear lower 6 bits (see datasheet)
    temp_lcr |= 0x03; // 8 data bits no parity, 1 stop bit
    WriteRegister(Piccolino_BT_REG_LCR,temp_lcr);
    WriteRegister(Piccolino_BT_REG_FCR,0x01); // Enable FIFO buffer
    // set pins directions
    Piccolino_BT::pinMode(0,INPUT); // BT module state
    Piccolino_BT::pinMode(1,OUTPUT); // KEY (cmd/comms mode)
    Piccolino_BT::pinMode(2,OUTPUT); // RESET (active low)
    Piccolino_BT::reset();
    if(_dev==HC05)
    {
        Piccolino_BT::digitalWrite(1,HIGH); // Enter Command Mode
        Piccolino_BT::print("AT+UART=9600,0,0\r\n");  // set the speed to 9600 for the command interface (important since the default is 38400)
        Piccolino_BT::digitalWrite(1,LOW); // Exit Command mode

        delay(1000); // must wait 1 sec to give time to the HC-05 to set the baudrate!
    }
}