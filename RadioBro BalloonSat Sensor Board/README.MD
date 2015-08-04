###Prerequisite:
- [Arduino Software](http://arduino.cc/en/Main/Software)
- [Radiobro BalloonSat Experiment Sensor Board](http://shop.radiobro.com/BalloonSat-Experiment-Sensor-Board-BSExpBrd.htm)



#Pre-Flight:

###Clear and validate MCU EEPROM
  
Upload clear\_and\_validate\_mcu\_eeprom.  This will clear and validate the MCU EEPROM. clear\_and\_validate\_mcu\_eeprom will output complete status for the MCU EEPROM.


###Configure write\_flight\_data.  

You will need to adjust your time delay, pressure delay, and sensor read frequency to fit your needs.

```C
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
int pressure_delay = 300;
//------------------------------------------------------------------------------
//Settings End------------------------------------------------------------------
//------------------------------------------------------------------------------
```

#Flight
###Load flight program
Upload write\_flight\_data
###Launch!


#Read flight data
Upload read\_flight\_data  This will output the flight data over serial in a CSV format.  Save output to as a csv file.  then open with excel

Modify the variable print_calculated to change the output.

Print calculated values.
```C
boolean print_calculated = true;
```

Print bytes.
```C
boolean print_calculated = false;
```
