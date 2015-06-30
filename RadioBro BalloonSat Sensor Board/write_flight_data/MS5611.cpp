 /*
 This header enables communication with the Measurement Specialtities MS5611 Digital Pressure and Temperature sensor operating in SPI mode.
 BalloonSat Experiment Sensor Board
 Flight Code Accessory File
 Author: RadioBro Team
 Last Modified: March 17th, 2015
 This was tested in Arduino 1.0.6 on March 17th, 2015 using BalloonSat Experiment Board Version P52520.
 This code is open source and able to modified and redistributed freely.
 */

#include "MS5611.h"
#include "Arduino.h"
//#include "WProgram.h"
#include "SPI.h"

//All commands come and math come from the MS5611 data sheet

MS5611::MS5611()
{

}

//set slave select and read calibration data
void MS5611::init(int ss)
{
  ms5611cs = ss;
  pinMode(ms5611cs, OUTPUT); //set slave select to output
  digitalWrite(ms5611cs, HIGH);

  _write(0x1E, 4); //Reset

  //get calibration coefficients
  C[1] = _read(0xA2, 2); //read C1
  C[2] = _read(0xA4, 2); //read C2
  C[3] = _read(0xA6, 2); //read C3
  C[4] = _read(0xA8, 2); //read C4
  C[5] = _read(0xAA, 2); //read C5
  C[6] = _read(0xAC, 2); //read C6
}

//read the pressure
long MS5611::_press()
{
  _write(0x58, 10); //Convert temp OSR=4096
  unsigned long D2 = _read(0x00, 3); //read ADC on MS5611

  //Serial.println(D2);

  _write(0x48, 10); //Convert press OSR=4096
  unsigned long D1 = _read(0x00, 3); //read ADC on MS5611

  //Serial.println(D1);

  double dT = D2 - (C[5] << 8);

  long TEMP = 2000L + (dT * (((double)C[6]) / 8388608.0));

  double OFF = (double)(C[2] * 2) + (((double)C[4] / 128.0) * (dT / 32768.0));

  double SENS = ((double)C[1] / 64.0) + (((double)C[3] / 2048.0) * ((double)dT / 262144.0));

  /*if(TEMP<2000L) //Calculate offset values for pressure
   {
   long OFF2 = 5L*(((TEMP-2000L)*(TEMP-2000L))>>1);
   long SENS2 = 5L*(((TEMP-2000L)*(TEMP-2000L))>>2);
   
   if (TEMP<-1500L)
   {
   OFF2 = OFF2 + (7L*((TEMP+1500L)*(TEMP+1500L)));
   SENS2 = SENS2 + (11L*(((TEMP+1500L)*(TEMP+1500L))>>1));
   }
   
   OFF = OFF - OFF2;
   SENS = SENS - SENS2;
   }*/

  long pressure = (SENS * ((double)(D1 >> 6) / 512.0)) - OFF;

  return pressure;
}

//read the temperature
long MS5611::temp()
{
  _write(0x58, 10); // Convert temp OSR=4096
  unsigned long D2 = _read(0x00, 3);

  double dT = D2 - (C[5] << 8);

  long TEMP = 2000L + (dT * (((double)C[6]) / 8388608.0));

  /*if(TEMP<2000L)
   {
   long T2 = ((long)(dT*dT)>>31);
   long OFF2 = 5L*(((TEMP-2000)*(TEMP-2000))>>1);
   long SENS2 = 5L*(((TEMP-2000)*(TEMP-2000))>>2);
  /* //These are offset values for pressure
   if (TEMP<-1500)
   {
   OFF2 = OFF2 + (7*((TEMP+1500)*(TEMP+1500)));
   SENS2 = SENS2 + (11*(((TEMP+1500)*(TEMP+1500))>>1));
   }
   
   TEMP = TEMP-T2;
   }*/

  return TEMP;
}

//reads data from the sensor
unsigned long MS5611::_read(byte address, int bytes)
{
  unsigned long value = 0; //set value to a 32 bit integer

  digitalWrite(ms5611cs, LOW); //set slave select low

  SPI.transfer(address); //set read address

    for (int i = 0; i < bytes; i++)
  {
    value = value << 8; //shift variable 8 bits to the right to recieve next byte
    value = value + SPI.transfer(0x00); //add the next byte of data to value
  }

  digitalWrite(ms5611cs, HIGH);

  return value;
}

//writes data to the sensor
void MS5611::_write(byte value, int timedelay)
{
  //timedelay = how long after the last byte CS is pulled high
  digitalWrite(ms5611cs, LOW);
  SPI.transfer(value); //send command to sensor
  delay(timedelay);
  digitalWrite(ms5611cs, HIGH);
}


