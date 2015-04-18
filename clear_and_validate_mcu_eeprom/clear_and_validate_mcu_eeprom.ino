/*
Authors:
  Hopkinsville Community College Balloon Sat Team
  Jacob Henson
Hopkinsville Community College Balloon Sat
Last Modified: 04/17/2015
Based on the EEPROM_CLEAR example
*/
#include <EEPROM.h>

void clear_eeprom(){

  Serial.println("Clearing MCU EEPROM cleared ");
  // write a 255 to all 1024 bytes of the MCU EEPROM
  for (int i = 0; i < 1024; i++){
    if(i%32 == 0){
      Serial.print('-');
    }
    EEPROM.write(i, 255);
  }
  Serial.println("MCU EEPROM cleared");

}
void validate_eeprom(){
  byte output_data;
  Serial.println("Validating MCU EEPROM");
  for (int i = 0; i < 1024; i++) {
    output_data = 0;
    output_data = EEPROM.read(i);
    if(output_data!=255)
    {
      Serial.print("uncleared memory  at byte\t");
      Serial.print(i);
      Serial.print("\t value");
      Serial.println(output_data);
    }
  }
  Serial.println("MCU EEPROM valiation complete");

}

void setup() {
  Serial.begin(9600);

  clear_eeprom();
  Serial.println();
  Serial.println();
  validate_eeprom();

  Serial.print("Complete");
  // turn the LED on when we're done
  digitalWrite(7, HIGH);
}

void loop() {
}
