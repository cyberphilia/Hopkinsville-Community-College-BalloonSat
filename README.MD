This code was written to be used with the RadioBro BalloonSat Experiment Sensor Board

Most of code is based on examples from Arduino and RadioBro.

###Prerequisite
- [Arduino Software](http://arduino.cc/en/Main/Software)
- [BalloonSat Experiment Sensor Board](http://shop.radiobro.com/BalloonSat-Experiment-Sensor-Board-BSExpBrd.htm)

#Testing:
1. ###Clear and validate MCU EEPROM
  Run clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.

2. ###Write static data to SPI EEPROM
  Run write\_static\flight\_test.  This will write to MCU and SPI EEPROM a static test flight data. Run write\_static\flight\_test will run until is full.

3. ###Read flight data
  Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.

4. ###Clear and validate MCU EEPROM
  Run clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.

5. ###Test Sensors
  Run write\_sensor\_test\.  This will save sensor data with out waiting for a real flight.

6. ###Read flight data
  Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.  then open with excel

#Pre-Flight:
1. ###Clear and validate MCU EEPROM
  Run clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.

#Flight:
1. ###Load flight program
  Run write\_flight\_data
2. ###Launch

#Post-Flight
1. ###Read flight data
  Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.
