/*
 * Simple braitenberg vehicle
 * http://tinkerlog.com
 */
 
#include <Arduino.h>
#include <hp_BH1750.h>
#include <Adafruit_SSD1306.h>
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

// Define the pins for the push button switches
const int button1 = 7;
const int button2 = 6;
const int button3 = 1;
const int button4 = 0;

// Define the pin for piezo buzzer
const int buzzer = 5;

// Define the pin for the OLED
int OLED_CS = 13;
int OLED_DC = 12;
int OLED_RST = 8;
int OLED_SI = 10;
int OLED_CLK = 9; 
Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);


// current note for melodies
int note = 0;


// Define braitenberg states
enum State {
  FEAR,
  AGGRESSION,
  LOVE,
  EXPLORE
};
 
// Initialize the current state
State currentState = LOVE;

void setup() {
  leftSensor.begin(BH1750_TO_GROUND); //initialize sensors
  rightSensor.begin(BH1750_TO_VCC);
  Serial.begin(9600);
  // Set button pins as input
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  // Set buzzer pin as input
  pinMode(buzzer, INPUT_PULLUP);

  // setup display
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();

  //setup Ardumoto
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

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

void Fear() {
  //Drive
  leftSpeed = leftSensor.getLux();
  rightSpeed = rightSensor.getLux();

  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 60 + leftSpeed);
  Drive(rightMotor, FORWARD, 60 + rightSpeed);

  Serial.print("L ");
  Serial.println(leftSpeed);
  Serial.print("R ");
  Serial.println(rightSpeed);

  //Melody
  // int tempo = 180;
  // int notes = sizeof(melodyFear) / sizeof(melodyFear[0]) / 2;
  // int wholenote = (60000 * 4) / tempo;
  // int divider = 0, noteDuration = 0;
  // if (note < notes * 2) {
  //   divider = melodyFear[note + 1];
  //   if (divider > 0) {
  //     noteDuration = (wholenote) / divider;
  //   } else if (divider < 0) {
  //     noteDuration = (wholenote) / abs(divider);
  //     noteDuration *= 1.5; 
  //   }
  //   tone(buzzer, melodyFear[note], noteDuration * 0.9);
  //   delay(noteDuration);
  //   noTone(buzzer);
  //   note = note + 2;
  // }
  // else {
  //   note = 0;
  // }

  // //Display
  // disp.clearDisplay();
  // disp.drawBitmap(0, 0, fearBitmap, 128, 64, WHITE);
  // disp.display();
}
unsigned long noteStartTime = 0;
void Aggression() {
  // Driving
  leftSpeed = rightSensor.getLux();
  rightSpeed = leftSensor.getLux();
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 60 + leftSpeed);
  Drive(rightMotor, FORWARD, 60 + rightSpeed);

  //Melody
  // if (note < sizeAggression) {
  //   int duration = 1000 / durationsAggression[note];
  //   tone(buzzer, melodyAggression[note], duration);
  //   int pauseBetweenNotes = duration * 1.30;
  //   delay(pauseBetweenNotes);
  //   noTone(buzzer);
  //   note++;
  // }
  // else {
  //   note = 0;
  // }

  // if (note < sizeAggression) {
  //   int duration = 1000 / durationsAggression[note];
    
  //   if (millis() - noteStartTime >= duration) {
  //     tone(buzzer, melodyAggression[note], duration);
  //     // noTone(buzzer);
  //     noteStartTime = millis();
  //     note++;
  //   }
  // }
  // else {
  //   note = 0;
  // }


  // Display
  disp.clearDisplay();
  disp.drawBitmap(0, 0, aggressionBitmap, 128, 64, WHITE);
  disp.display();

}

void Love() {
  //Drive
  //left sensor corresponds to right motor and right sensor to left motor
  leftSpeed = leftSensor.getLux();
  rightSpeed = rightSensor.getLux();
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 255 - leftSpeed);
  Drive(rightMotor, FORWARD, 255 - rightSpeed);



  //Melody
  // int tempo = 244;
  // int notes = sizeof(melodyLove) / sizeof(melodyLove[0]) / 2;
  // int wholenote = (60000 * 4) / tempo;
  // int divider = 0, noteDuration = 0;
  // if (note < notes * 2) {
  //   divider = melodyLove[note + 1];
  //   if (divider > 0) {
  //     noteDuration = (wholenote) / divider;
  //   } else if (divider < 0) {
  //     noteDuration = (wholenote) / abs(divider);
  //     noteDuration *= 1.5; 
  //   }
  //   tone(buzzer, melodyLove[note], noteDuration * 0.9);
  //   delay(noteDuration);
  //   noTone(buzzer);
  //   note = note + 2;
  // }
  // else {
  //   note = 0;
  // }

  // // Display
  // disp.clearDisplay();
  // disp.drawBitmap(0, 0, loveBitmap, 128, 64, WHITE);
  // disp.display();
}

void Explore(){
  //Drive
  leftSpeed = rightSensor.getLux();
  rightSpeed = leftSensor.getLux();
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 255 - leftSpeed);
  Drive(rightMotor, FORWARD, 255 - rightSpeed);

  // //Melody
  // if (note < sizeExplore) {
  //   int duration = 1000 / durationsExplore[note];
  //   tone(buzzer, melodyExplore[note], duration);
  //   int pauseBetweenNotes = duration * .90;
  //   delay(pauseBetweenNotes);
  //   noTone(buzzer);
  //   note++;
  // }
  // else {
  //   note = 0;
  // }

  // Display
  disp.clearDisplay();
  disp.drawBitmap(0, 0, exploreBitmap, 128, 64, WHITE);
  disp.display();
}

int count = 2;
 
void loop() {
  // sensor values between MIN TO MAX
  leftSensor.start();  //starts a measurement
  rightSensor.start();


  // read state of button switches
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);
  int buttonState3 = digitalRead(button3);
  int buttonState4 = digitalRead(button4);
  
  // Check which button is pressed and change the state accordingly
  if (buttonState1 == LOW) {
    if (currentState != FEAR) {
      note = 0;
    }
    currentState = FEAR;
  } else if (buttonState2 == LOW) {
    if (currentState != AGGRESSION) {
      note = 0;
    }
    currentState = AGGRESSION;
  } else if (buttonState3 == LOW) {
    if (currentState != LOVE) {
      note = 0;
    }
    currentState = LOVE;
  } else if (buttonState4 == LOW) {
    if (currentState != EXPLORE) {
      note = 0;
    }
    currentState = EXPLORE;
  }

  // Display
  if (count > 10) count = 2;
  count++;
    disp.clearDisplay();
    disp.fillCircle(disp.width()/2, disp.height()/2, count, WHITE);
    disp.display();

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
  }

  // delay(50);
}