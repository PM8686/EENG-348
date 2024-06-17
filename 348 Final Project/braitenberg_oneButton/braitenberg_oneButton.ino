/*
 * Simple braitenberg vehicle
 * http://tinkerlog.com
 */
 
#include <Arduino.h>
#include <hp_BH1750.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "pitches.h"
#include "melodies.h"
#include "bitmap.h"

hp_BH1750 leftSensor;
hp_BH1750 rightSensor;   
 
#define leftMotor 0
#define rightMotor 1
#define DIRA 2 //direction left motor
#define PWMA 3 //speed left motor
#define DIRB 4 //direction right motor
#define PWMB 11 //speed right motor
#define FORWARD 0
#define REVERSE 1

int leftSpeed = 0;
int rightSpeed = 0;

int left_bias = 0;
int right_bias = 0;

// Define the pins for the push button
// ONLY ONE BUTTON THAT SWITCHES BETWEEN STATES
const int button = 7;

// Define the pin for piezo buzzer
const int buzzer = 5;

// Define the pin for the OLED
int OLED_CS = 13;
int OLED_DC = 12;
int OLED_RST = 8;
int OLED_SI = 10;
int OLED_CLK = 9; 
// Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLST,OLED_CS);


// current note for melodies
int note = 0;


// Define braitenberg states
enum State {
  FEAR,
  AGGRESSION,
  LOVE,
  EXPLORE,
  STOP
};
 
// Initialize the current state
State currentState = LOVE;


void Drive(byte motor, byte dir, byte spd) {
  if (motor == leftMotor) {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }
}

void Stop(byte motor) {
  Drive(motor, 0, 0);
}

void myTone(byte pin, uint16_t frequency, uint16_t duration)
{ // input parameters: Arduino pin number, frequency in Hz, duration in milliseconds
  unsigned long startTime=millis();
  unsigned long halfPeriod= 1000000L/frequency/2;
  pinMode(pin,OUTPUT);
  while (millis()-startTime< duration)
  {
    if (frequency == 0) { //if rest, do nothing
      digitalWrite(pin,LOW);
      delayMicroseconds(halfPeriod);
      digitalWrite(pin,LOW);
      delayMicroseconds(halfPeriod);
    }
    else {
      digitalWrite(pin,HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(pin,LOW);
      delayMicroseconds(halfPeriod);
    }
  }
  pinMode(pin,INPUT);
}

void customDelay(unsigned long ms) {
  unsigned long start = millis(); // Get the current time
  while (millis() - start < ms) {
    // Wait until the desired time has elapsed
    // The loop will keep running until the difference between the current time and start time is greater than ms
  }
}

void Fear() {
  //Drive
  leftSpeed = (leftSensor.getLux() - left_bias)/2;
  rightSpeed = (rightSensor.getLux() - right_bias)/2;

  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  if (leftSpeed < 5) leftSpeed = -60;
  if (rightSpeed < 5) rightSpeed = -60;
  Drive(leftMotor, FORWARD, 60 + leftSpeed);
  Drive(rightMotor, FORWARD, 60 + rightSpeed);

  //Melody
  int tempo = 360;
  int notes = sizeof(melodyFear) / sizeof(melodyFear[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  if (note < notes * 2) {
    divider = melodyFear[note + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }
    myTone(buzzer, melodyFear[note], noteDuration * 0.9);
    customDelay(noteDuration * 0.75);
    noTone(buzzer);
    note = note + 2;
  }
  else {
    note = 0;
  }

  //Display
  // disp.clearDisplay();
  // // disp.drawBitmap(0, 0, fearBitmap, 128, 64, WHITE);
  // disp.drawChar(30,30, 'F', WHITE, BLACK, 5);
  // disp.display();
}

void Aggression() {
  // Driving
  leftSpeed = (rightSensor.getLux() - right_bias)/2;
  rightSpeed = (leftSensor.getLux() - left_bias)/2;
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  if (leftSpeed < 5) leftSpeed = -60;
  if (rightSpeed < 5) rightSpeed = -60;
  Drive(leftMotor, FORWARD, 60 + leftSpeed);
  Drive(rightMotor, FORWARD, 60 + rightSpeed);

  //Melody
  int tempo = 280;
  int notes = sizeof(melodyAggression) / sizeof(melodyAggression[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  if (note < notes * 2) {
    divider = melodyAggression[note + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }
    myTone(buzzer, melodyAggression[note], noteDuration);
    customDelay(noteDuration);
    noTone(buzzer);
    note = note + 2;
  }
  else {
    note = 0;
  }

  // //Display
  // disp.clearDisplay();
  // disp.drawBitmap(0, 0, aggressionBitmap, 128, 64, WHITE);
  // disp.display();

}

void Love() {
  //Drive
  //left sensor corresponds to right motor and right sensor to left motor
  leftSpeed = (leftSensor.getLux() - left_bias)/2;
  rightSpeed = (rightSensor.getLux() - right_bias)/2;
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 255;
  if (rightSpeed > 195) rightSpeed = 255;
  if (leftSpeed < 0) leftSpeed = 0;
  if (rightSpeed < 0) rightSpeed = 0;
  Drive(leftMotor, FORWARD, 255 - leftSpeed);
  Drive(rightMotor, FORWARD, 255 - rightSpeed);

  // Serial.print("L ");
  // Serial.println(leftSpeed);
  // Serial.print("R ");
  // Serial.println(rightSpeed);

  //Melody
  int tempo = 488;
  int notes = sizeof(melodyLove) / sizeof(melodyLove[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  if (note < notes * 2) {
    divider = melodyLove[note + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }
    myTone(buzzer, melodyLove[note], noteDuration * 0.9);
    customDelay(noteDuration);
    noTone(buzzer);
    note = note + 2;
  }
  else {
    note = 0;
  }

  //Display
  // disp.clearDisplay();
  // disp.drawBitmap(0, 0, loveBitmap, 128, 64, WHITE);
  // disp.display();
}

void Explore(){
  //Drive
  leftSpeed = (rightSensor.getLux() - right_bias)/2;
  rightSpeed = (leftSensor.getLux() - left_bias)/2;
  
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 255;
  if (rightSpeed > 195) rightSpeed = 255;
  if (leftSpeed < 0) leftSpeed = 0;
  if (rightSpeed < 0) rightSpeed = 0;

  Drive(leftMotor, FORWARD, 255 - leftSpeed);
  Drive(rightMotor, FORWARD, 255 - rightSpeed);
  // Serial.print("L ");
  // Serial.println(leftSpeed);
  // Serial.print("R ");
  // Serial.println(rightSpeed);
  //Melody
  if (note < sizeExplore) {
    int duration = 1000 / durationsExplore[note];
    myTone(buzzer, melodyExplore[note], duration * 0.5);
    customDelay(duration / 6);
    noTone(buzzer);
    note++;
  }
  else {
    note = 0;
  }

  // //Display
  // disp.clearDisplay();
  // disp.drawBitmap(0, 0, exploreBitmap, 128, 64, WHITE);
  // disp.display();
}



void setup() {
  leftSensor.begin(BH1750_TO_GROUND); //initialize sensors
  rightSensor.begin(BH1750_TO_VCC);
  // get one reading for the initial bias
  for (int i = 0; i < 5; i++)
  {
    leftSensor.start();  //starts a measurement
    rightSensor.start();
    left_bias += leftSensor.getLux();
    right_bias += rightSensor.getLux();
  }
  left_bias = left_bias/5;
  right_bias = right_bias/5;

  Serial.begin(9600);
  // Set button pins as input
  pinMode(button, INPUT_PULLUP);

  // Set buzzer pin as input
  pinMode(buzzer, INPUT_PULLUP);

  // // setup display
  // disp.begin(SSD1306_SWITCHCAPVCC);
  // disp.clearDisplay();

  //setup Ardumoto
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);

  Stop(rightMotor);
  Stop(leftMotor);
}
void Rest() {
    Stop(rightMotor);
    Stop(leftMotor);
    noTone(buzzer);
    delay(50);
}
unsigned long since_state_change = 0;
unsigned long button_delay = 500;
void loop() {
  // sensor values between MIN TO MAX
  leftSensor.start();  //starts a measurement
  rightSensor.start();

  // read state of button switches
  int buttonState = digitalRead(button);
  
  // Check which button is pressed and change the state accordingly
  if (buttonState == HIGH && millis() - since_state_change > button_delay) {
    if (currentState == FEAR) {
      currentState = AGGRESSION;
    }
    else if (currentState == AGGRESSION) {
      currentState = LOVE;
    }
    else if (currentState == LOVE) {
      currentState = EXPLORE;
    } 
    else if (currentState == EXPLORE){
      currentState = STOP;
      delay(100);
      int melody6[] = { //power off
          NOTE_CS5, NOTE_A4};
      int noteDurations6[] = {
          12, 12};
      for (int thisNote = 0; thisNote < 2; thisNote++) {
          int noteDuration = 1000 / noteDurations6[thisNote];
          tone(buzzer, melody6[thisNote], noteDuration);
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          noTone(buzzer);
        }
    } else {
      currentState = FEAR;
    }
    note = 0;
    button_delay = millis();
  }

  switch (currentState) {
    case FEAR:
      Fear();
      Serial.println("State FEAR");
      break;
    case AGGRESSION:
      Aggression();
      Serial.println("State AGGRESSION");
      break;
    case LOVE:
      Love();
      Serial.println("State LOVE");
      break;
    case EXPLORE:
      Explore();
      Serial.println("State EXPLORE");
      break;
    case STOP:
      Rest();
      Serial.println("State STOP");
      break;
  }

}