#include <TimerOne.h>

// Output Properties
// Low-level register operations for speed
#define PIN 2 // Pin 2 on an Uno
#define OUT_PIN() DDRD |= ((1 << PIN)) // Initialize (pinmode OUT)
#define SET_PIN() PORTD |= ((1 << PIN)) // Set (digitalwrite HIGH)
#define CLR_PIN() PORTD &= ~((1 << PIN)) // Clear (digitalwrite LOW)

// Input Properties
#define SENSOR_PIN 3 // Pin 3 on an Uno
#define SYMBOL_PERIOD 1560 // Period of MAN_1200 signal from Manchester.h library
#define SAMPLE_PER_SYMBOL 8 // Number of times filter samples period

// ADC Setup
void ADC_setup(){
  ADCSRA =  bit (ADEN);                      // turn ADC on
  ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2);  // Prescaler of 128
  #ifdef INT_REF
  ADMUX  =  bit (REFS0) | bit (REFS1);    // internal 1.1v reference
  #else
  ADMUX  =  bit (REFS0) ;   // external 5v reference
  #endif
}

// Start ADC Conversion
void ADC_start_conversion(int adc_pin){
  ADMUX &= ~(0x07) ; //clearing enabled channels
  ADMUX  |= (adc_pin & 0x07) ;    // AVcc and select input port
  bitSet (ADCSRA, ADSC) ;
}

// Read ADC Conversion
int ADC_read_conversion(){
 while(bit_is_set(ADCSRA, ADSC));
 return ADC ;
}

float EDGE_THRESHOLD = 0; // 0 because the DC offset is removed
float oldValue = 0 ;
float filtered_value = 0;
float sensorValue;
//float shifted_filter = -10000;

void sample_signal_edge(){
  int temp = ADC_read_conversion(); // read result of previously triggered conversion
  float sensorValue = (float) temp; // typecast to float
  ADC_start_conversion(SENSOR_PIN); // start a conversion for next loop

  // Bitshift DC Offset Filter
  //long shiftedFCL = shifted_filter + (long)((sensorValue-oldValue)<<8);
  //shifted_filter = shiftedFCL - (shiftedFCL>>8);
  //long filtered_value = (shifted_filter+128)>>8;
  // 8us runtime, use for faster sampling frequency

  // DC Offset Filter
  filtered_value = (float) 0.990 * (filtered_value + sensorValue - oldValue);
  // 24us runtime, use bitshifting instead for faster signal speed

  // Filtered Output
  if (filtered_value > EDGE_THRESHOLD) {SET_PIN();}
  else if (filtered_value < EDGE_THRESHOLD) {CLR_PIN();}
  
  oldValue = sensorValue; // save old value
  }

void setup() {
  OUT_PIN(); // Initialize output pin
  ADC_setup(); // Initialize ADC
  analogReference(DEFAULT); // Use default reference for ADC
  // change DEFAULT to INTERNAL if voltage range <1.1V
  ADC_start_conversion(SENSOR_PIN); // Start ADC conversion
  Timer1.initialize(SYMBOL_PERIOD/SAMPLE_PER_SYMBOL); //1560/8
  Timer1.attachInterrupt(sample_signal_edge); // Attach ISR
}

void loop() {
}
