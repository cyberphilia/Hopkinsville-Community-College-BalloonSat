 /*
 Modified by:
  Hopkinsville Community College Balloon Sat Team
  Jacob Henson
Hopkinsville Community College Balloon Sat
Last Modified: 04/17/2015
Based on BalloonSat Experiment Sensor Board Flight Code with some bug fixes

-------------------------------
 BalloonSat Experiment Sensor Board
 Flight Code
 Author: RadioBro Team
 Last Modified: March 17th, 2015
 Purpose: Read and Save Data from sensors on the BalloonSat Experiment Sensor Board. Detect launch and save data afterwards.
 PREFLIGHT: To access all memory locations, the onboard EEPROM must be overwritten to 0xFF (255) in all addresses before launch.
 Accomplish this with the EEPROM_CLEAR program.
 POSTFLIGHT: To recover saved data, use the RECOVERY program. Each byte will be printed on a seprate line.

 This was tested in Arduino 1.0.6 on March 17th, 2015 using BalloonSat Experiment Board Version P52520. Files MEMORY.CPP, MEMORY.h, MS5611.CPP, MS5611.h must be included in the same directory as this file to work.
 This code is open source and able to modified and redistributed freely.
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
  byte flight = 0 ; //start in flight state 0

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

  //FLIGHT STATE 0- WAITING FOR LAUNCH
  while (flight == 0) {

    digitalWrite(5, HIGH); //blink red led while waiting for ascent
    delay(1000);
    digitalWrite(5, LOW);
    delay(1000);
    pressure = ms5611._press(); //read the pressure sensor
    //check if altitude has increased by ~25 meters (3 mbar*100 for sensor)
    if (pressure < (base - 300)) {
      digitalWrite(5, LOW);
      flight = 1; //if ascent has occured, start saving data
    }

    /*
    //TIME DELAY- remove block comment to use
    //set a certain time after launch to begin saving data. Can be used alone or with pressure launch detect
     time = millis();
     //wait 5 minutes before saving data. Adjust the value added to startTime to change the length of the delay.
     if (time > startTime+300000){
     flight=1;
     }
     */
  }

  /*
  //PRINT START ADDRESS
  //prints address to which data starts being saved, remove block comment to have this written to the serial port
   Serial.print("Starting Address:  ");
   Serial.println(addr);
   */

  //FLIGHT STATE 1 (AIRBORNE)
  while (flight == 1)
  {
    //Read Sensors
    long pressure = ms5611._press(); //read pressure in mbar*100

    long temperature = ms5611.temp(); //read digital temperature

    time = millis(); //get mission time in milliseconds

    float tempf = analogRead(temppin); //record temp in mVs
    tempf = (tempf * 5000) / 1024;
    int temp = tempf;

    float IRf = analogRead(IRpin); //record IR in mV
    IRf = (IRf * 5000) / 1024;
    int IR = IRf;

    float Humidf = analogRead(Humidpin); //record humidity in mVs
    Humidf = (Humidf * 5000) / 1024;
    int Humid = Humidf;

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

    /*
    //OUTPUT DATA TO SERIAL
    //outputs the sensor readings to the serial port. Take out block comment to use.
    Serial.print("Pressure: ");
    Serial.println(pressure);
    Serial.print("DTemp: ");
    Serial.println(temperature);
    Serial.print("Time: ");
    Serial.println(time);
    Serial.print("Temp: ");
    Serial.println(temp);
    Serial.print("IR: ");
    Serial.println(IR);
    Serial.print("Humid: ");
    Serial.println(Humid);
    */

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
       EEPROM.write(1, addr);
     /*
    *******bug correction above********
    The program checks the first two bytes for addr.
    */
    delay(6000); //Delay 6 seconds (0.167 Hz * 10800 sec (3 hr) flight = 1800 packets saved < 2048 (# pakcets able to be saved)
  }
  //blink LEDs when memory is full
  while (flight == 2) {
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
