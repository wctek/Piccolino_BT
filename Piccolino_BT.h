/* 
Description:
This is a example code for Sandbox Electronics' I2C/SPI to UART bridge module.
You can get one of those products on 
http://sandboxelectronics.com

Version:
V0.1

Release Date:
2014-02-16

Author:
Tiequan Shao          info@sandboxelectronics.com

Lisence:
CC BY-NC-SA 3.0

Please keep the above information when you use this code in your project. 
*/


#ifndef _Piccolino_BT_H_
#define _Piccolino_BT_H_

#include "Arduino.h"

//General Registers
#define     Piccolino_BT_REG_RHR        (0x00)
#define     Piccolino_BT_REG_THR        (0X00)
#define     Piccolino_BT_REG_IER        (0X01)
#define     Piccolino_BT_REG_FCR        (0X02)
#define     Piccolino_BT_REG_IIR        (0X02)
#define     Piccolino_BT_REG_LCR        (0X03)
#define     Piccolino_BT_REG_MCR        (0X04)
#define     Piccolino_BT_REG_LSR        (0X05)
#define     Piccolino_BT_REG_MSR        (0X06)
#define     Piccolino_BT_REG_SPR        (0X07)
#define     Piccolino_BT_REG_TCR        (0X06)
#define     Piccolino_BT_REG_TLR        (0X07)
#define     Piccolino_BT_REG_TXLVL      (0X08)
#define     Piccolino_BT_REG_RXLVL      (0X09)
#define     Piccolino_BT_REG_IODIR      (0X0A)
#define     Piccolino_BT_REG_IOSTATE    (0X0B)
#define     Piccolino_BT_REG_IOINTENA   (0X0C)
#define     Piccolino_BT_REG_IOCONTROL  (0X0E)
#define     Piccolino_BT_REG_EFCR       (0X0F)

//Special Registers
#define     Piccolino_BT_REG_DLL        (0x00)
#define     Piccolino_BT_REG_DLH        (0X01)

//Enhanced Registers
#define     Piccolino_BT_REG_EFR        (0X02)
#define     Piccolino_BT_REG_XON1       (0X04)
#define     Piccolino_BT_REG_XON2       (0X05)
#define     Piccolino_BT_REG_XOFF1      (0X06)
#define     Piccolino_BT_REG_XOFF2      (0X07)

//
#define     Piccolino_BT_INT_CTS        (0X80)
#define     Piccolino_BT_INT_RTS        (0X40)
#define     Piccolino_BT_INT_XOFF       (0X20)
#define     Piccolino_BT_INT_SLEEP      (0X10)
#define     Piccolino_BT_INT_MODEM      (0X08)
#define     Piccolino_BT_INT_LINE       (0X04)
#define     Piccolino_BT_INT_THR        (0X02)
#define     Piccolino_BT_INT_RHR        (0X01)

//Application Related 

#define 	Piccolino_BT_CRYSTCAL_FREQ   (18432000UL)	  

#define HM10 0
#define HC05 1

class Piccolino_BT : public Print
{ 
    public:
        void begin(uint8_t addr=0x9A, bool dev=HC05);
        int read();
        size_t write(uint8_t val);
        uint8_t available();
        void pinMode(uint8_t pin, uint8_t io);
        void digitalWrite(uint8_t pin, uint8_t value);
        uint8_t digitalRead(uint8_t pin);
		void flush();
        void reset();
        void commandMode();
        void commsMode();
        void Cmd(const char *cmd);
        char *getStatus();
    
    private:
        uint8_t _addr;
        bool _dev;
        char BTSTATUS[12];
        void    SetBaudrate(uint32_t baudrate);
        uint8_t ReadRegister(uint8_t reg_addr);
        void    WriteRegister(uint8_t reg_addr, uint8_t val);
        void    ResetDevice(void);
};

#endif





    
    
