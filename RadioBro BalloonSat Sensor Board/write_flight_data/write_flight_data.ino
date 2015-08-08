/*
Authors:
  Hopkinsville Community College Balloon Sat Team
  Jacob Henson
Hopkinsville Community College Balloon Sat
Last Modified: 06/12/2015
*/


//import functions for interacting with the MS5611 Digital Pressure and
// Temperature Sensor
#include "MS5611.h" 
//import functions for interacting with the 25AA256 SPI External EEPROM
#include "MEMORY.h" 
#include <SPI.h> //allows for SPI functionality
#include <EEPROM.h> //allows access to onboard EEPROM

MS5611 ms5611; //set instance of class MS5611
MEMORY aa256; //set instance of class MEMORY

//------------------------------------------------------------------------------
//Settings Start----------------------------------------------------------------
//------------------------------------------------------------------------------
unsigned int max_spi_eerom_memory = 32768;
int reading_frequency = 5000;//in miliseconds.  1000 miliseconds equals 1 second
int array_size = 16;
// Delays
// +++++++
// You can use time or pressure delay or both
int time_delay = 0; //in mileseconds
int pressure_delay = 0;
//------------------------------------------------------------------------------
//Settings End------------------------------------------------------------------
//------------------------------------------------------------------------------

//Variables
unsigned int next_address = 0;
int data[16]; //array to hold sensor data
int temppin = A2; //lmt87 on ADC2
int IRpin = A4; //TSL262R on ADC4
int Humidpin = A3; //HIH-5030 on ADC3
long pressure_base; 
unsigned long boot_timestamp = millis();
unsigned long timestamp;
// flight status:
// 0 - preflight
// 1 - in flight
// 2 - memory full
byte flight_status = 0 ; //start in flight state 0


long get_pressure()
{
    long pressure = 0;
    pressure = ms5611._press(); //read pressure in mbar*100
    return pressure;
}

long get_temperature()
{
    long temperature = ms5611.temp(); //read digital temperature
    return temperature;
}

int get_temperature_mvs()//record temp in mVs
{
    float tempf = analogRead(temppin); //record temp in mVs
    tempf = (tempf * 5000) / 1024;
    int temperature = tempf;
    return temperature;
}

int get_infrared()//record IR in mV
{
    float IRf = analogRead(IRpin); //record IR in mV
    IRf = (IRf * 5000) / 1024;
    int infrared = IRf;
    return infrared;
}

int get_humidity()//record IR in mV
{
    float Humidf = analogRead(Humidpin); //record humidity in mVs
    Humidf = (Humidf * 5000) / 1024;
    int humidity = Humidf;
    return humidity;
}



unsigned int get_next_address()
{
    unsigned int address = 0;
    byte ahigh = EEPROM.read(0);
    byte alow = EEPROM.read(1);     
 
    if (ahigh != 255 || alow != 255) {
        address = ahigh * 256 + alow;
    }
    return address;
}

void write_next_address(unsigned int address)
{
    EEPROM.write(0, address >> 8);
    EEPROM.write(1, address);
}

void setup() {

 
  Serial.begin(9600); // start serial with baud rate 9600
  SPI.begin(); //start SPI functionality
  aa256.initE(10); //intialize 25AA256 on slave select pin 10
  ms5611.init(9); //initialize ms5611 (get calibration coefficients) on slave select pin 9

  pressure_base = get_pressure();
  Serial.print("Maxium Memory:\t");
  Serial.println(max_spi_eerom_memory);
  
  Serial.print("Reading Frequency:\t");
  Serial.println(reading_frequency);
  
  Serial.print("Array Size:\t");
  Serial.println(array_size);

  Serial.print("Starting Address:\t");
  Serial.println(get_next_address());

  Serial.print("Number of saves with current array size:\t");
  Serial.println(max_spi_eerom_memory/array_size);

  Serial.print("Number of seconds until full:\t");
  Serial.println(((max_spi_eerom_memory/array_size)*reading_frequency)/1000);


}
void loop() {

  next_address = get_next_address();
  unsigned int max_address = max_spi_eerom_memory - array_size;
  int humidity = get_humidity();
  int infrared = get_infrared();
  int temperature_mvs = get_temperature_mvs();
  long temperature = get_temperature();
  long pressure = get_pressure();
  unsigned long timpstamp = millis();

// Pre flight 
if(flight_status == 0)
{
  //time check
  //pressure check

  if(time_delay < timpstamp && pressure >= pressure_base + pressure_delay)
  {
    flight_status = 1;
  } 
}

// In flight
if(flight_status == 1)
{
  if(next_address < max_address && next_address >= 0)
    {
      Serial.print("Address: "); 
      Serial.println(next_address);          

      int data[16] ;
      data[0] = temperature_mvs >> 8;
      data[1] = temperature_mvs;
      data[2] = infrared >> 8;
      data[3] = infrared;
      data[4] = humidity >> 8;
      data[5] = humidity;
      data[6] = pressure >> 24;
      data[7] = pressure >> 16;
      data[8] = pressure >> 8;
      data[9] = pressure;
      data[10] = temperature >> 8;
      data[11] = temperature;
      data[12] = timpstamp >> 24;
      data[13] = timpstamp >> 16;
      data[14] = timpstamp >> 8;
      data[15] = timpstamp; 


 
    //OUTPUT DATA TO SERIAL
    //outputs the sensor readings to the serial port. Take out block comment to use.
    Serial.print("Pressure: ");
    Serial.println(pressure);
    Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("timpstamp: ");
    Serial.println(timpstamp);
    Serial.print("temperature_mvs: ");
    Serial.println(temperature_mvs);
    Serial.print("infrared: ");
    Serial.println(infrared);
    Serial.print("humidity: ");
    Serial.println(humidity);
 



      // Write data to SPI EEPROM
      aa256.writeEnable(); //allows data to be written
      next_address = aa256.writeData(data, next_address); //writes the "data" array to the external memory

      // Write next addess to MCU EEPROM
      write_next_address(next_address); 
  }
  else
  {
      Serial.println("Memory Full");  
      flight_status = 2;
  }
}

// Memory Full 
// blink LEDs
while (flight_status == 2) 
{
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    delay(5000);
}




delay(reading_frequency);

}


