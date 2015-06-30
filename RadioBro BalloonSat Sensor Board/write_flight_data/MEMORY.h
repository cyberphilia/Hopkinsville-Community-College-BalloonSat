 /*
 This header file enables communication with the Microchip 25AA256 SPI EEPROM Module. 
 BalloonSat Experiment Sensor Board
 Flight Code Accessory File
 Author: RadioBro Team
 Last Modified: March 17th, 2015
 This was tested in Arduino 1.0.6 on March 17th, 2015 using BalloonSat Experiment Board Version P52520.
 This code is open source and able to modified and redistributed freely.
 */



#ifndef MEMORY_H
#define MEMORY_H
//#include <WProgram.h>
//#include <Arudino.h>

class MEMORY
{
  public:
    MEMORY();
    void initE(int ss);
    void writeEnable();
    int readStatus();
    unsigned int writeData(int data[], unsigned int addr);
  private:
    int eepromss;
};
#endif
