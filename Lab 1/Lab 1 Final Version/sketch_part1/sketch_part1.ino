#include "testasm.h"

void setup() {
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
  
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
