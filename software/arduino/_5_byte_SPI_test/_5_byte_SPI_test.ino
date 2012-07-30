
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

// Sends 5 x 8-bit data via SPI to UC3C, incrementing by one after
// each transfer. Expects to receive sent data - 1 back from 
// UC3C. After 5 bytes, SS is raised again.
// UC3C code operates as a simple loop-back function
// Written for Arduino MEGA
// Arduino output sent and received data via Serial.print()
// LED (pin 13) blinks if received data is incorrect. Stays off
// permanently if it is correct.

// include the SPI library:
#include <SPI.h>

// Use Pin 13 to control LED
int led = 13;

// Variable for data to send to UC3C
byte dataSent = 0x00;
byte dataRead = 0x00;

// Use Pin 53 as SPI Slave Select
int slaveSelect = 53;

void setup() {
  /* Use Pin 13 as an output (LED) */
  pinMode(led, OUTPUT);
  /* Use Pin53 as SS slave select */
  pinMode(slaveSelect, OUTPUT);
  /* Enable serial output for debugging */
  Serial.begin(115200);
  /* Enable the SPI library */
  SPI.begin();
  /* Set SPI for MSB first, SPI Mode 0 and clock div 2 */
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void loop() {
  // Set slave select signal low
  digitalWrite(slaveSelect,LOW);

  // Send five bytes of data in one transfer
  while(dataSent < 5)
  {
    dataRead = SPI.transfer(dataSent);
    
    // If data read was same as previous sent byte, turn LED off
    dataRead += 1;
    if (dataRead == 5)
    {
      dataRead = 0; 
    }
    if (dataSent == dataRead)
    {
      digitalWrite(led, LOW);
    }
    // Otherwise turn LED on to signal error output to Serial interface
    else
    {
      digitalWrite(led, HIGH);
      Serial.print(dataSent, HEX);
      Serial.print(" ");
      Serial.print(dataRead, HEX);
      Serial.println(" ");
    }
    // Increment value to be sent
    ++dataSent;
  }
  // End this transfer by setting SS high again
  digitalWrite(slaveSelect,HIGH);
  // Reset data to be sent back to 0
  dataSent = 0;
}
