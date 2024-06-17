#include "concurrency.h"

int other_led = 2;

void p1 (void)
{
  /* process 1 here */
  while(1)
  {
    // blink the LED
    digitalWrite(other_led, HIGH);  // turn the LED on 
    delay(1000);                      // wait for a second
    digitalWrite(other_led, LOW);   // turn the LED off
    delay(1000);                      // wait for a second
  }

  return;
}

void p2 (void)
{
/* process 2 here */
  while(1)
  {
    // blink the LED
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on 
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
    delay(1000);                      // wait for a second
  }
 return;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(other_led, OUTPUT);

  if (process_create (p1, 64) < 0) {
    return;
  }
  if (process_create (p2, 64) < 0) {
  return;
  }
}

void loop()
{
  process_start();
 /* if you get here, then all processes are either finished or
    there is deadlock */
  while (1) ;
}
