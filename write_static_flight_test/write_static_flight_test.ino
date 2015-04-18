/*
Authors:
  Hopkinsville Community College Balloon Sat Team
  Jacob Henson
Hopkinsville Community College Balloon Sat
Last Modified: 04/17/2015
Based on BalloonSat Experiment Sensor Board Flight Code
*/


#include "MS5611.h" //import functions for interacting with the MS5611 Digital Pressure and Temperature Sensor
#include "MEMORY.h" //import functions for interacting with the 25AA256 SPI External EEPROM
#include <SPI.h> //allows for SPI functionality
#include <EEPROM.h> //allows access to onboard EEPROM

MS5611 ms5611; //set instance of class MS5611
MEMORY aa256; //set instance of class MEMORY

//set arrays for data writing and variables for analog sensors
int data[16]; //array to hold sensor data
int temppin = A2; //lmt87 on ADC2
int IRpin = A4; //TSL262R on ADC4
int Humidpin = A3; //HIH-5030 on ADC3
long base; //intial pressure reading (used for detecting launch)
byte ahigh; //high byte of address
byte alow; //low byte of address


void setup()
{
 Serial.begin(9600); // start serial with baud rate 9600
 SPI.begin(); //start SPI functionality
 aa256.initE(10); //intialize 25AA256 on slave select pin 10
 ms5611.init(9); //initialize ms5611 (get calibration coefficients) on slave select pin 9

 //Serial.println("Initialized"); //uncomment to recieve serial confirmation of intialization

 //blink green LED after intialization
 digitalWrite(7, HIGH);
 delay(500);
 digitalWrite(7, LOW);
 base = ms5611._press(); //set an initial pressure
 //blink blue LED after base pressure is set
 digitalWrite(6, HIGH);
 delay(500);
 digitalWrite(6, LOW);
 //read last address saved to (brownout recovery)
 ahigh = EEPROM.read(0);
 alow = EEPROM.read(1);
print_header();
}
void print_header()
{
  Serial.print("addr");
  Serial.print(",temp >> 8");
  Serial.print(",temp");
  Serial.print(",IR >> 8");
  Serial.print(",IR");
  Serial.print(",Humid >> 8");
  Serial.print(",Humid");
  Serial.print(",pressure >> 24");
  Serial.print(",pressure >> 16");
  Serial.print(",pressure >> 8");
  Serial.print(",pressure");
  Serial.print(",temperature >> 8");
  Serial.print(",temperature");
  Serial.print(",time >> 24");
  Serial.print(",time >> 16");
  Serial.print(",time >> 8");
  Serial.println(",time");
}
void loop()
{
 //set up variables for reading data from sensors
 unsigned int addr = 0; //start saving data to address 0
 long pressure = 0;
 int temp = 0;
 int Humid = 0;
 int IR = 0;
 unsigned long time;
 unsigned long startTime = millis(); //record time after initialization (for time launch detect)
 byte flight = 1 ; //start in flight state 0

 //check to see if memory is full, if it is, just blink LEDs
 if (EEPROM.read(2) == 0) {
   flight = 2;
 }
 //if power was interrupted during flight, start saving at last address
 //checks to see if an address had previously been saved
 if (ahigh != 255 || alow != 255) {
   addr = ahigh * 256 + alow;
   flight=1;
 }
 //FLIGHT STATE 1 (AIRBORNE)
 while (flight == 1)
 {

   temp = 2492;
   IR = 0;
   Humid = 2397;
   pressure = 99988;
   temperature = 2250;
   time = millis(); //get mission time in milliseconds

   //Build Data array to write to memory
   data[0] = temp >> 8;
   data[1] = temp;
   data[2] = IR >> 8;
   data[3] = IR;
   data[4] = Humid >> 8;
   data[5] = Humid;
   data[6] = pressure >> 24;
   data[7] = pressure >> 16;
   data[8] = pressure >> 8;
   data[9] = pressure;
   data[10] = temperature >> 8;
   data[11] = temperature;
   data[12] = time >> 24;
   data[13] = time >> 16;
   data[14] = time >> 8;
   data[15] = time;


   //CSV format output
   Serial.print(addr,DEC);
   Serial.print(",");
   Serial.print(data[0],DEC);
   Serial.print(",");
   Serial.print(data[1],DEC);
   Serial.print(",");
   Serial.print(data[2],DEC);
   Serial.print(",");
   Serial.print(data[3],DEC);
   Serial.print(",");
   Serial.print(data[4],DEC);
   Serial.print(",");
   Serial.print(data[5],DEC);
   Serial.print(",");
   Serial.print(data[6],DEC);
   Serial.print(",");
   Serial.print(data[7],DEC);
   Serial.print(",");
   Serial.print(data[8],DEC);
   Serial.print(",");
   Serial.print(data[9],DEC);
   Serial.print(",");
   Serial.print(data[10],DEC);
   Serial.print(",");
   Serial.print(data[11],DEC);
   Serial.print(",");
   Serial.print(data[12],DEC);
   Serial.print(",");
   Serial.print(data[13],DEC);
   Serial.print(",");
   Serial.print(data[14],DEC);
   Serial.print(",");
   Serial.println(data[15],DEC);


   //make sure data is not overwritten
   if (addr < 32752) //2048 total packets, will not save if there is not enough space left on chip
   {
     aa256.writeEnable(); //allows data to be written
     addr = aa256.writeData(data, addr); //writes the "data" array to the external memory
   }
   //if memory is full, stop saving data
   else {
     flight = 2;
    //  EEPROM.write(0, 2); //sets a flag in the MCU EEPROM that SPI EEPROM is full
     EEPROM.write(2, 0);  //sets a flag in the MCU EEPROM that SPI EEPROM is full
/*
*******bug correction above********
The program checks the third byte for a value 0.
*/
   }
   //save address to onboard eeprom
  //  EEPROM.write(addr >> 8, 0);
  //  EEPROM.write(addr, 1);
   EEPROM.write(0,addr >> 8);
   EEPROM.write(1, 1);
/*
*******bug correction above********
The program checks the first two bytes for addr.
*/
   //delay(6000); //Delay 6 seconds (0.167 Hz * 10800 sec (3 hr) flight = 1800 packets saved < 2048 (# pakcets able to be saved)
 }
 //blink LEDs when memory is full
 while (flight == 2) {
   Serial.println("Memory is full...");
   digitalWrite(5, HIGH);
   digitalWrite(6, HIGH);
   digitalWrite(7, HIGH);
   delay(1000);
   digitalWrite(5, LOW);
   digitalWrite(6, LOW);
   digitalWrite(7, LOW);
   delay(5000);
 }

}
