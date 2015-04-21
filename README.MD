This code was written to be used with the RadioBro BalloonSat Experiment Sensor Board

Most of code is based on examples from Arduino and RadioBro.

###Prerequisite:
- [Arduino Software](http://arduino.cc/en/Main/Software)
- [BalloonSat Experiment Sensor Board](http://shop.radiobro.com/BalloonSat-Experiment-Sensor-Board-BSExpBrd.htm)

#Testing:
###Clear and validate MCU EEPROM
Run clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.

###Write static data to SPI EEPROM
Run write\_static\_flight\_test.  This will write to MCU and SPI EEPROM a static test flight data. write\_static\_flight\_test will run until is full.

###Read flight data
Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.

###Clear and validate MCU EEPROM
Run clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.

###Test Sensors
Run write\_sensor\_test.  This will save sensor data with out waiting for a real flight.

###Read flight data
Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.  then open with excel

Modify the variable print_calculated to change the output.

Print calculated values.

	boolean print_calculated = true;

Print bytes.
	
	boolean print_calculated = false;

#Pre-Flight:
###Clear and validate MCU EEPROM
  
Run clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.

#Flight:
###Load flight program
Run write\_flight\_data
###Launch!

#Post-Flight

###Read flight data
Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.
