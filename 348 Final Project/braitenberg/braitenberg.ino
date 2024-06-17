/*
 * Simple braitenberg vehicle
 * http://tinkerlog.com
 */
 
#include <Arduino.h>
#include <hp_BH1750.h>
#include "pitches.h"
#include <Adafruit_SSD1306.h>

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
  // pinMode(PWMA, OUTPUT); 2
  // pinMode(PWMB, OUTPUT); 3
  // pinMode(DIRA, OUTPUT); 4
  // pinMode(DIRB, OUTPUT); 11
// Define the pins for the push button switches
const int button1 = 7;
const int button2 = 6;
const int button3 = 1;
const int button4 = 0;

// Define the pin for piezo buzzer
const int buzzer =5;

// Define the pin for the OLED
int OLED_CS = 13;
int OLED_DC = 12;
int OLED_RST = 8;
int OLED_SI = 10;
int OLED_CLK = 9; 
Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);


// notes in the melody:
int melodya[] = { //final sound
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5};
int noteDurationsa[] = {
  12, 12, 12, 12, 12};

int melodyb[] = { //mario kart countdown
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A5};
int noteDurationsb[] = {
  2, 2, 2, 1};

// notes in the melody:
int melody1[] = { //good rep
  NOTE_AS5, NOTE_DS6};
int noteDurations1[] = {
  16, 16};
  
int melody2[] = { //had rep
  NOTE_C3, NOTE_C3};
int noteDurations2[] = {
  10, 8};
  
int melody3[] = { //calibration start
  NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A6};
int noteDurations3[] = {
  12, 12, 12, 12};

int melody4[] = { //calibration done so countdown
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A5};
int noteDurations4[] = {
  2, 2, 2, 2};
  
int melody5[] = { //power on
 NOTE_A4, NOTE_CS5, NOTE_E5, NOTE_A5};
int noteDurations5[] = {
  12, 12, 12, 12, 1, 12, 12};

int melody6[] = { //power off
 NOTE_CS5, NOTE_A4};
int noteDurations6[] = {
  12, 12};

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
  pinMode (buzzer, INPUT_PULLUP);

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

//   //power on
//   for (int thisNote = 0; thisNote <4; thisNote++) {
//     int noteDuration = 1000 / noteDurations4[thisNote];
//     tone(buzzer, melody4[thisNote], noteDuration);
//     int pauseBetweenNotes = noteDuration * 1.30;
//     delay(pauseBetweenNotes);
//     noTone(buzzer);
//   }
// delay(1000);
  //calibration start
  for (int thisNote = 0; thisNote < 4; thisNote++) {
    int noteDuration = 1000 / noteDurations3[thisNote];
    tone(buzzer, melody3[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }

  //calibration



  
delay(1000);
  //calibration done so countdown
  for (int thisNote = 0; thisNote <4; thisNote++) {
    int noteDuration = 1000 / noteDurations4[thisNote];
    tone(buzzer, melody4[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
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
  leftSpeed = leftSensor.getLux();
  rightSpeed = rightSensor.getLux();
  Serial.print("L1: ");
  Serial.println(leftSpeed);
  Serial.print("L2: ");
  Serial.println(rightSpeed);
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 60 + leftSpeed);
  Drive(rightMotor, FORWARD, 60 + rightSpeed);
}

void Aggression() {
  leftSpeed = rightSensor.getLux();
  rightSpeed = leftSensor.getLux();
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 60 + leftSpeed);
  Drive(rightMotor, FORWARD, 60 + rightSpeed);
}

void Love() {
  //left sensor corresponds to right motor and right sensor to left motor
  leftSpeed = leftSensor.getLux();
  rightSpeed = rightSensor.getLux();
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 255 - leftSpeed);
  Drive(rightMotor, FORWARD, 255 - rightSpeed);
}

void Explore(){
  leftSpeed = rightSensor.getLux();
  rightSpeed = leftSensor.getLux();
  //speed values range betweeen 60 to 200
  if (leftSpeed > 195) leftSpeed = 195;
  if (rightSpeed > 195) rightSpeed = 195;
  Drive(leftMotor, FORWARD, 255 - leftSpeed);
  Drive(rightMotor, FORWARD, 255 - rightSpeed);
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
    currentState = FEAR;
  } else if (buttonState2 == LOW) {
    currentState = AGGRESSION;
  } else if (buttonState3 == LOW) {
    currentState = LOVE;
  } else if (buttonState4 == LOW) {
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


  delay(50);
}