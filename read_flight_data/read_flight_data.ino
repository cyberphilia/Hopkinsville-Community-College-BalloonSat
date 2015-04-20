/*
Authors:
  Hopkinsville Community College Balloon Sat Team
  Jacob Henson
Hopkinsville Community College Balloon Sat
Last Modified: 04/17/2015
Based on the SPI example
*/

#include "SPI.h"
#include <EEPROM.h>
#define DATAOUT 11//MOSI
#define DATAIN  12//MISO
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

//opcodes
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2


byte clr;
int address=0;
int array_index =0;
//data buffer
char buffer [128];

void fill_buffer()
{
  for (int I=0;I<128;I++)
  {
    buffer[I]=I;
  }
}


char spi_transfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1)))     // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}


byte read_spi_eeprom(int EEPROM_address)
{
  //READ EEPROM
  float data;
  digitalWrite(SLAVESELECT,LOW);
  SPI.transfer(READ); //transmit read opcode
  SPI.transfer((char)(EEPROM_address>>8));   //send MSByte address first
  SPI.transfer((char)(EEPROM_address));      //send LSByte address
  data = SPI.transfer(0xFF); //get data byte
  digitalWrite(SLAVESELECT,HIGH); //release chip, signal end transfer
  return data;
}
void print_header(boolean print_calculated){
  Serial.print("address");
  if(!print_calculated)
    Serial.print(",temp >> 8");
  Serial.print(",temp");
  if(!print_calculated)
    Serial.print(",IR >> 8");
  Serial.print(",IR");
  if(!print_calculated)
    Serial.print(",Humid >> 8");
  Serial.print(",Humid");
  if(!print_calculated){
    Serial.print(",pressure >> 24");
    Serial.print(",pressure >> 16");
    Serial.print(",pressure >> 8");
  }
  Serial.print(",pressure");
  if(!print_calculated)
    Serial.print(",dtemp >> 8");
  Serial.print(",dtemp");
  if(!print_calculated){
    Serial.print(",time >> 24");
    Serial.print(",time >> 16");
    Serial.print(",time >> 8");
  }
  Serial.println(",time");
}
int get_last_written_address(){
  byte address0 = EEPROM.read(0);
  byte address1 = EEPROM.read(1);
  byte full_check = EEPROM.read(2);

  //Check to see if SPI EEPROM has been marked as full
  if(full_check == 0)
    return 32768;
  //Check to see if SPI EEPROM is marked as cleared
  if (address0 == 255 && address1 == 255)
      return 0;

  return (address0 << 8) + address1;

}
void print_data(int last_address,boolean print_calculated){

  if(last_address <= 0){
    Serial.println("Invalid Address: the last address can not be zero or less");
    return;
  }
  if(last_address >32768){
    Serial.println("Invalid Address: the last address can not be greater than 32768");
    return;
  }

  byte array_data_0;
  byte array_data_1;
  byte array_data_2;
  byte array_data_3;
  byte array_data_4;
  byte array_data_5;
  byte array_data_6;
  byte array_data_7;
  byte array_data_8;
  byte array_data_9;
  byte array_data_10;
  byte array_data_11;
  byte array_data_12;
  byte array_data_13;
  byte array_data_14;
  byte array_data_15;

    for (int i = 0; i < last_address; i+=16) {
      array_data_0 =   read_spi_eeprom(i);
      array_data_1 =   read_spi_eeprom(i+1);
      array_data_2 =   read_spi_eeprom(i+2);
      array_data_3 =   read_spi_eeprom(i+3);
      array_data_4 =   read_spi_eeprom(i+4);
      array_data_5 =   read_spi_eeprom(i+5);
      array_data_6 =   read_spi_eeprom(i+6);
      array_data_7 =   read_spi_eeprom(i+7);
      array_data_8 =   read_spi_eeprom(i+8);
      array_data_9 =   read_spi_eeprom(i+9);
      array_data_10 =  read_spi_eeprom(i+10);
      array_data_11 =  read_spi_eeprom(i+11);
      array_data_12 =  read_spi_eeprom(i+12);
      array_data_13 =  read_spi_eeprom(i+13);
      array_data_14 =  read_spi_eeprom(i+14);
      array_data_15 =  read_spi_eeprom(i+15);


        Serial.print(i);//print address
        Serial.print(",");

        //temp
        if(print_calculated){
            Serial.print((array_data_0 << 8) + array_data_1, DEC);
            Serial.print(",");
        }
        else{
            Serial.print(array_data_0, DEC);
            Serial.print(",");
            Serial.print(array_data_1, DEC);
            Serial.print(",");
        }//end temp

        //IR
        if(print_calculated){
            Serial.print((array_data_2 << 8) + array_data_3, DEC);
            Serial.print(",");
        }
        else{
            Serial.print(array_data_2, DEC);
            Serial.print(",");
            Serial.print(array_data_3, DEC);
            Serial.print(",");
        }//end IR

        //Humid
        if(print_calculated){
            Serial.print((array_data_4 << 8) + array_data_5, DEC);
            Serial.print(",");
        }
        else{
            Serial.print(array_data_4, DEC);
            Serial.print(",");
            Serial.print(array_data_5, DEC);
            Serial.print(",");
        }//end Humid

        //pressure
        if(print_calculated){
            Serial.print((array_data_6 << 24) + (array_data_7 << 16) + (array_data_8 << 8) + array_data_9, DEC);
            Serial.print(",");
        }
        else{
            Serial.print(array_data_6, DEC);
            Serial.print(",");
            Serial.print(array_data_7, DEC);
            Serial.print(",");
            Serial.print(array_data_8, DEC);
            Serial.print(",");
            Serial.print(array_data_9, DEC);
            Serial.print(",");
        }//end pressure

        //temperature
        if(print_calculated){
            Serial.print((array_data_10 << 8) + array_data_11, DEC);
            Serial.print(",");
        }
        else{
            Serial.print(array_data_10, DEC);
            Serial.print(",");
            Serial.print(array_data_11, DEC);
            Serial.print(",");
        }//end temperature

        //pressure
        if(print_calculated){
            Serial.print((array_data_12 << 24) + (array_data_13 << 16) + (array_data_14 << 8) + array_data_15, DEC);
            Serial.print(",");
        }
        else{
            Serial.print(array_data_12, DEC);
            Serial.print(",");
            Serial.print(array_data_13, DEC);
            Serial.print(",");
            Serial.print(array_data_14, DEC);
            Serial.print(",");
            Serial.print(array_data_15, DEC);
            Serial.print(",");
        }//end pressure

      }// end for
}//end print_data


void setup() {
  Serial.begin(9600);

  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(SLAVESELECT,OUTPUT);
  digitalWrite(SLAVESELECT,HIGH); //disable device

  SPCR = SPCR = (1<<SPE)|(1<<MSTR);
  clr=SPSR;
  clr=SPDR;
  delay(10);
  Serial.println();
  Serial.println();
  delay(1000);


  boolean print_calculated = false;
  int last_address = get_last_written_address();


  print_header(print_calculated);
  print_data(last_address,print_calculated);


}

void loop() {

}
