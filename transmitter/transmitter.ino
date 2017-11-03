#include <Manchester.h>

#define TX_PIN  11  //pin where your transmitter is connected

uint8_t moo = 1; //last led status
//uint8_t data[8] = {0xFF, 0x6D, 0x67, 0x93, 0xDD, 0x79, 0x7A}; // Lamp 1
uint8_t data[8] = {0xFF, 0x15, 0x45, 0x28, 0x3C, 0x7E, 0xEA}; // Lamp 2

void setup() 
{
  // Lamp specific initialization
  pinMode(4, OUTPUT); //  RED
  pinMode(5, OUTPUT); //  CYAN
  pinMode(6, OUTPUT); //  RED ORANGE
  pinMode(7, OUTPUT); //  GREEN
  pinMode(8, OUTPUT); //  BLUE
  pinMode(9, OUTPUT); //  AMBER
  pinMode(10, OUTPUT); // LIME
  //pinMode(11, OUTPUT); // NEUTRAL WHITE
  pinMode(12, OUTPUT); // COOL WHITE
  pinMode(13, OUTPUT); // WARM WHITE
  analogWrite(4, 255);
  analogWrite(5, 255);
  analogWrite(6, 255);
  analogWrite(7, 255);
  analogWrite(8, 255);
  analogWrite(9, 255);
  analogWrite(10, 255);
  //analogWrite(11, 255);
  analogWrite(12, 255);
  analogWrite(13, 0); // Turn on warm white
  // end lamp specific initialization
  
  Serial.begin(115200);
  man.setupTransmit(TX_PIN, MAN_1200);
}

uint8_t datalength=8;

void loop() 
{
  data[0] = datalength;
  Serial.println(datalength);
  man.transmitArray(datalength, data);

  //delay(100);
}
