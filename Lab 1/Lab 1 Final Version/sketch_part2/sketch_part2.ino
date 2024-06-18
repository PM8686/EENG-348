#include "testasm.h"

int light = 13;

void setup()
{
  // put your setup code here, to run once
  Serial.begin(9600);

  Serial.print("The sum of 7 and 8 is: ");
  Serial.println(sumval(7, 8));
  Serial.print("The sum of 121 and 185 is: ");
  Serial.println(sumval(121, 185));
  Serial.print("The sum of 255 and 1 is: ");
  Serial.println(sumval(255, 1));
  Serial.print("The sum of 254 and 253 is: ");
  Serial.println(sumval(254, 253));

  Serial.print("The diff between 12 and 5 is: ");
  Serial.println(diffval(12, 5));
  Serial.print("The diff between 43 and 96 is: ");
  Serial.println(diffval(43, 96));
  Serial.print("The diff between 9 and 3 is: ");
  Serial.println(diffval(9, 3));
  Serial.print("The diff between 100 and 250 is: ");
  Serial.println(diffval(100, 250));

  Serial.print("sumval(16,7) + diffval(16,7) is: ");
  Serial.println(ourMain(16, 7));
  Serial.print("sumval(250,100) + diffval(250,100) is: ");
  Serial.println(ourMain(250, 100));
  Serial.print("sumval(138,59) + diffval(138,59) is: ");
  Serial.println(ourMain(138, 59));
  Serial.print("sumval(24,109) + diffval(24,109) is: ");
  Serial.println(ourMain(24, 109));

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
