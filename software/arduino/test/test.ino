
// include the SPI library:
#include <SPI.h>

// Use Pin 13 to control LED
int led = 13;

// Use Pin 53 as SPI Slave Select
int slaveSelect = 53;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(slaveSelect, OUTPUT);
  Serial.begin(115200);
  SPI.begin();
  //SPI.setBitOrder(LSBFIRST);
  //SPI.setDataMode(SPI_MODE0);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void loop() {
  
  while(1)
  {
    digitalWrite(slaveSelect,LOW);
    delay(1000);
    digitalWrite(slaveSelect,HIGH);
    delay(1000);
  }
  
  digitalWrite(led, HIGH);
  digitalWrite(slaveSelect,LOW);
  SPI.transfer(0xAF);
  digitalWrite(slaveSelect,HIGH);
  delay(500);
  digitalWrite(led, LOW);
  digitalWrite(slaveSelect,LOW);
  SPI.transfer(0x5F);
  digitalWrite(slaveSelect,HIGH);
  delay(500);
}
