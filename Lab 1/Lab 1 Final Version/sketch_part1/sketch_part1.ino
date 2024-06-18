#include "testasm.h"

int light = 13;

void setup()
{
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("The sum of 4 and 4 is: ");
  Serial.println(testasm(4, 4));
  Serial.print("The sum of 100 and 155 is: ");
  Serial.println(testasm(100, 155));
  Serial.print("The sum of 250 and 150 is: ");
  Serial.println(testasm(250, 150));
  Serial.print("The sum of 255 and 1 is: ");
  Serial.println(testasm(255, 1));
  Serial.print("The sum of 255 and 255 is: ");
  Serial.println(testasm(255, 255));

  pinMode(light, OUTPUT);
}

void loop()
{
  // light blinks
  digitalWrite(light, HIGH);
  delay(400);
  digitalWrite(light, LOW);
  delay(400);
}
