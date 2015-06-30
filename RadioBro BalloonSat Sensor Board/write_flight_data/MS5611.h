 /*
 This header enables communication with the Measurement Specialtities MS5611 Digital Pressure and Temperature sensor operating in SPI mode.
 BalloonSat Experiment Sensor Board
 Flight Code Accessory File
 Author: RadioBro Team
 Last Modified: March 17th, 2015
 This was tested in Arduino 1.0.6 on March 17th, 2015 using BalloonSat Experiment Board Version P52520.
 This code is open source and able to modified and redistributed freely.
 */

#ifndef MS5611_H
#define MS5611_H

//#include <WProgram.h>
#include <Arduino.h>

class MS5611 {
  public:
  MS5611();
  void init(int ss);
  long _press();
  long temp();
  unsigned long _read(byte address, int bytes);
  void _write(byte value, int timedelay);
  
  private:
  int ms5611cs;
  unsigned long C[7];
};
#endif
