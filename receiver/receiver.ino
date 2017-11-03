#include "Manchester.h"

#include "TimerOne.h"

// Uno Pin 13 (Packet received)
#define OUT_LED() DDRB |= (1 << 5);
#define SET_LED() PORTB |= (1 << 5);
#define CLR_LED() PORTB &= ~(1 << 5);
#define TOG_LED() PORTB ^= (1 << 5);

// Uno Pin 8 (Red)
#define OUT_LED_R() DDRB |= (1 << 0);
#define SET_LED_R() PORTB |= (1 << 0);
#define CLR_LED_R() PORTB &= ~(1 << 0);
#define TOG_LED_R() PORTB ^= (1 << 0);

// Uno Pin 9 (Green)
#define OUT_LED_G() DDRB |= (1 << 1);
#define SET_LED_G() PORTB |= (1 << 1);
#define CLR_LED_G() PORTB &= ~(1 << 1);
#define TOG_LED_G() PORTB ^= (1 << 1);

// Uno Pin 10 (Yellow)
#define OUT_LED_Y() DDRB |= (1 << 2);
#define SET_LED_Y() PORTB |= (1 << 2);
#define CLR_LED_Y() PORTB &= ~(1 << 2);
#define TOG_LED_Y() PORTB ^= (1 << 2);

#define RX_PIN 3

// Packet Size
#define BUFFER_SIZE 8
uint8_t buffer[BUFFER_SIZE];

uint8_t lamp1[8] = {0x08, 0x6D, 0x67, 0x93, 0xDD, 0x79, 0x7A, 0x00}; // Lamp 1
uint8_t lamp2[8] = {0x08, 0x15, 0x45, 0x28, 0x3C, 0x7E, 0xEA, 0x00}; // Lamp 2

// Counters
int i = 0;
int j = 0;
int m = 0;

void(* softReset) (void) = 0; // Dirty reboot function

void setup()
{
  OUT_LED();
  OUT_LED_R();
  OUT_LED_G();
  OUT_LED_Y();

  Timer1.initialize(1000000); // callback does computation every second
  Timer1.attachInterrupt(callback);
  
  //Serial.begin(115200); // Print crashes program.
  //Serial.println("Program Started.");
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() 
{
  if (man.receiveComplete()){
    TOG_LED();

    //for (int k = 0; k<8; k++){
    //  Serial.print(buffer[k], HEX);
    //}
    //Serial.println();
    
    if (memcmp(lamp1, buffer, 8) == 0){
      //Serial.print("Lamp 1 Detected\n");
      i++;
    }
    else if (memcmp(lamp2, buffer, 8) == 0){
      //Serial.print("Lamp 2 Detected\n");
      j++;
    }
    else m++;

    //m++;
    man.beginReceiveArray(BUFFER_SIZE, buffer);
  }
}

void callback(){
  if (i >= 5){
	// If Lamp 1 is detected more than 5 times in 1 second
    //Serial.println("Under Lamp 1");
    CLR_LED_R();
    CLR_LED_Y();
    TOG_LED_G(); // Flash Green
  }
  else if (j >= 5){
	// If Lamp 2 is detected more than 5 times in 1 second
    CLR_LED_R();
    CLR_LED_G();
    TOG_LED_Y(); // Flash Yellow
    //Serial.println("Under Lamp 2");
  }
  else {
    CLR_LED_G();
    CLR_LED_Y();
    TOG_LED_R(); //Flash Red
    //Serial.println("No lamp detected.");
  }

  if (m >= 2 && m <= 5) {
	// if manchester.h assumes baudrate is slower
    softReset();
  }
  
  // Clear counters
  i = 0;
  j = 0;
  m = 0;
}


