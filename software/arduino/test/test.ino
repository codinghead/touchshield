
/******************************************************************************/
/**
* \file test.ino
* \mainpage
* \section intro Introduction
* Arduino sketch to implemnent SPI MASTER side of loop-back communication.
*
* \section contactInfo Contact Information
* For more information and the latest release, please visit this projects home
* page at http://codinghead.github.com/touchshield/
* To participate in the project or for other enquiries, please contact Stuart
* Cording at codinghead@gmail.com
*
* \section license Licensing Information
* Copyright (c) 2012 Stuart Cording
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* \author Stuart Cording aka CODINGHEAD
*
********************************************************************************/

// Sends 8-bit data via SPI to UC3C, incrementing by one after
// each transfer. Expects to receive sent data - 2 back from 
// UC3C
// UC3C code operates as a simple loop-back function
// Written for Arduino MEGA
// Arduino output sent and received data via Serial.print()
// LED (pin 13) blinks if received data is correct. Stays on
// permanently if it is wrong.

// TODO: Need to find out why UC3C doesn't like the SS pin
//       changing between bytes. Need to check UC3C bus timing
//       and set up clock and peripheral bus properly.
//       UPDATE: Added short delay before raising the SS pin again
//       seems time between end of transfer and SS rising edge
//       was too short.



// include the SPI library:
#include <SPI.h>

// Use Pin 13 to control LED
int led = 13;

// Variable for data to send to UC3C
byte dataByte = 0x00;
byte dataRead = 0x00;

// Use Pin 53 as SPI Slave Select
int slaveSelect = 53;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(slaveSelect, OUTPUT);
  Serial.begin(115200);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  digitalWrite(slaveSelect,LOW);
  SPI.transfer(dataByte);
  delay(1);
  digitalWrite(slaveSelect,HIGH);
  ++dataByte;
  delay(25);
}

void loop() {
  digitalWrite(led, HIGH);
  digitalWrite(slaveSelect,LOW);
  dataRead = SPI.transfer(dataByte);
  delay(1);
  digitalWrite(slaveSelect,HIGH);
  Serial.print(dataByte, HEX);
  Serial.print(" ");
  Serial.print(dataRead, HEX);
  Serial.println(" ");
  delay(250);
  if (dataRead == (dataByte - 1))
  {
    digitalWrite(led, LOW);
  }
  ++dataByte;
  delay(250);
}
