This code was written to be used with the RadioBro BalloonSat Experiment Sensor Board

Most of code is based on examples from Arduino and RadioBro.

###Prerequisite
- [Arduino Software](http://arduino.cc/en/Main/Software)
- [BalloonSat Experiment Sensor Board](http://shop.radiobro.com/BalloonSat-Experiment-Sensor-Board-BSExpBrd.htm)

#Testing:
###Step 1 Clear memory
Run clear\_memory.  This will clear the addresses from smaller EEPROM. clear\_memory will output complete status for the smaller EEPROM.

###Step 2 Read memory

Run read\_cleared\_memory.  This will confirm the addresses of the smaller EEPROM has been cleared.

###Step 3 Test Write to memory
Run write\_flight\_test.  This will write to smaller and larger EEPROM a static test flight data.

###Step 4 Read test flight data

Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.  then open with excel

###Step 5 Clear memory
Repeat step 1.

###Step 6 Test Sensors
Run write\_sensor\_data.  This will save sensor data with out waiting for a real flight.

###Step 7 Read test sensor data

Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.  then open with excel

#Pre-Flight:
###Step 1 Clear memory
Run clear\_memory.  This will clear the addresses from smaller EEPROM. clear\_memory will output complete status for the smaller EEPROM.

###Step 2 Read memory
Run read\_cleared\_memory.  This will confirm the addresses of the smaller EEPROM has been cleared.

#Flight:
###Step 1
Run write\_flight\_data
###Step 2
Launch

#Post-Flight
###Step 1 
Run read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.  then open with excel