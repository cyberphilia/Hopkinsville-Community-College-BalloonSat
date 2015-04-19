 /*
 This file enables communication with the Microchip 25AA256 SPI EEPROM Module. 
 BalloonSat Experiment Sensor Board
 Flight Code Accessory File
 Author: RadioBro Team
 Last Modified: March 17th, 2015
 This was tested in Arduino 1.0.6 on March 17th, 2015 using BalloonSat Experiment Board Version P52520.
 This code is open source and able to modified and redistributed freely.
 */



#include "MEMORY.h"
#include "Arduino.h"
//#include "WProgram.h"
#include "SPI.h"

MEMORY::MEMORY()
{
}
//set slave select as an output
void MEMORY::initE(int ss)
{
  pinMode(ss, OUTPUT);
  digitalWrite(ss, HIGH);
  eepromss = ss;
}

//send write enable command to allow for the saving of data
void MEMORY::writeEnable()
{
  digitalWrite(eepromss, LOW);
  SPI.transfer(B00000110); //write enable command in binary
  digitalWrite(eepromss, HIGH);
}

int MEMORY::readStatus()
{
  digitalWrite(eepromss, LOW);
  SPI.transfer(B00000101); //read the STATUS register of the memory
  byte _status = SPI.transfer(0);
  digitalWrite(eepromss, HIGH);
  return _status; //returns the status
}

unsigned int MEMORY::writeData(int data[], unsigned int addr)
{
  digitalWrite(eepromss, LOW);
  SPI.transfer(B00000010); //send the write command. Write enable must be sent first
  SPI.transfer(addr >> 8);
  SPI.transfer(addr);

  for (byte i = 0; i < 16; i++) {
    SPI.transfer(data[i]); //send each array location to the memory
  }
  digitalWrite(eepromss, HIGH);
  addr = addr + 16; //advance the address pointer by 16
  delay(10);
  return addr; //return the next available address
}

