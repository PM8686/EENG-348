#include "testasm.h"

int light = 13;

void setup()
{
  // put your setup code here, to run once
  Serial.begin(9600);

  Serial.print("The fibonacci sequence up to n = 2 is: ");
  Serial.println(fib(2, 0));
  Serial.print("The fibonacci sequence up to n = 5 is: ");
  Serial.println(fib(5, 0));
  Serial.print("The fibonacci sequence up to n = 9 is: ");
  Serial.println(fib(9, 0));
  Serial.print("The fibonacci sequence up to n = 13 is: ");
  Serial.println(fib(13, 0));

  pinMode(light, OUTPUT);
}

void loop()
{
  // blink light
  digitalWrite(light, HIGH);
  delay(400);
  digitalWrite(light, LOW);
  delay(400);
}
