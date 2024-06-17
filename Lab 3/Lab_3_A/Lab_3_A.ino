/* Photocell simple testing sketch. 
 
Connect one end of the photocell to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 
Connect LED from pin 11 through a resistor to ground 
For more information see http://learn.adafruit.com/photocells */
 #include <avr/io.h>
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDpin = 11;          // connect Red LED to pin 11 (PWM pin)
int LEDbrightness;        // 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600); 

  // FOR ANALOG_READ 
  // // Set ADC reference voltage to AVCC (5V)
  // ADMUX |= (1 << REFS0);
  // // Enable ADC and set prescaler to 128 (ADC clock = F_CPU / 128)
  // ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// // Function to read ADC value from channel 0
// uint16_t AnalogRead() {
//     // Select ADC channel 0
//     ADMUX &= ~(1 << MUX0);
//     // Start ADC conversion
//     ADCSRA |= (1 << ADSC);
//     // Wait for conversion to complete
//     while (ADCSRA & (1 << ADSC));
//     // Return ADC result (10-bit resolution)
//     return ADC;
// }
 
void loop(void) {
  
  // FOR ANALOG_READ 
  // photocellReading = AnalogRead();

  photocellReading = analogRead(photocellPin);  
  // explain how you can implement analogRead() using I/O mapped registers on the AVR (look at the appropriate section of the datasheet for the AVR)
  // PC0, ADC0
 
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading
 

  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
  analogWrite(LEDpin, LEDbrightness);
 
  delay(1000);
}