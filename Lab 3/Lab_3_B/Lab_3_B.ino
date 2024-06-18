/* Photocell simple testing sketch.

Connect one end of the photocell to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a resistor to ground
For more information see http://learn.adafruit.com/photocells */
#include <avr/io.h>
int photocellPin = 0; // the cell and 10K pulldown are connected to a0
int photocellReading; // the analog reading from the sensor divider
int Yellowpin = 11;   // connect Red LED to pin 11 (PWM pin)
int Greenpin = 10;
int LEDbrightness;

#define trigPin 2
#define echoPin 3
int Dist;

void setup(void)
{
  // send debugging information via the Serial monitor
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Yellowpin, OUTPUT);
  pinMode(Greenpin, OUTPUT);
}

void loop(void)
{
  analogWrite(Greenpin, 0);
  analogWrite(Yellowpin, 0);

  photocellReading = analogRead(photocellPin);

  Serial.print("Analog reading = ");
  Serial.println(photocellReading); // the raw analog reading

  // map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
  // analogWrite(Yellowpin, LEDbrightness);

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < 10)
  { // This is where the LED On/Off happens
    analogWrite(Yellowpin, LEDbrightness);
    analogWrite(Greenpin, 0);
  }
  else
  {
    analogWrite(Yellowpin, 0);
    analogWrite(Greenpin, LEDbrightness);
  }
  if (distance >= 200 || distance <= 0)
  {
    Serial.println("Out of range");
  }
  else
  {
    Dist = map(distance, 0, 200, 0, 255);
    Serial.println(Dist);
  }

  delay(1000);
}
