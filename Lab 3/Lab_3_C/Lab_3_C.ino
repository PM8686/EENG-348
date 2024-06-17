#include <Adafruit_SSD1306.h>
// #include "display_library.h"
/* 
  #define statements for OLED_CS, OLED_DC, OLED_RST, OLED_SI, OLED_CLK 
  go here.
*/
int OLED_CS = 13;
int OLED_DC = 12;
int OLED_RST = 11;
int OLED_SI = 10;
int OLED_CLK = 9; 


Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);
// Our_Display disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);
// int velocity = 5;
int x = disp.width()/2;
int y = disp.height()/2;
int dx=0;
int dy=0;
int sign_varx = 1;
int sign_vary = -1;
// int dx = velocity/sqrt(2); // 45 degree angle
// int dy = velocity/sqrt(2); // 45 degree angle

// void setup() {
//   Serial.begin(9600);
//  disp.begin(SSD1306_SWITCHCAPVCC);
// //  disp.begin(0x2)
//  disp.clearDisplay();
//  disp.display();
//  disp.drawLine(0, 0, 127, 63, WHITE);
// }

// void loop() {
//   // Serial.println("hi");
//   // delay(10);
//   x = x + dx;
//   y = y + dy;
//   if (x >= disp.width()-4 || x <= 0 + 4)
//   {
//     dx = -1*dx;
//   }
//   if (y >= disp.height()-4 || y <= 0 + 4)
//   {
//     dy = -1*dy;
//   }
//   disp.clearDisplay();

//   disp.fillCircle(x, y, 4, WHITE);
//   disp.display();
// }


// This is just a code snippet, provided in case it helps you.
// My setup: This encoder has pin C (middle pin of the three on one side) and 
// one of the pair on the other side connected to ground.  The other pins are 
// connected as follows to my Arduino Mega:
//      'A': digital pin 6
//      'B': digital pin 7
//      'press': digital pin 5 ******************************************* (I think this is a button)
// In "real" code, you'd want a state machine to keep track of the rotation,
// and only take note of the "forward" or "back" when the encoder is reporting
// 0 again.

// Digital pin definitions
enum enDigitalPins
{
 // Rotary encoder input lines
 dpInEncoderA=7,
 dpInEncoderB=6,
 dpInEncoderPress=5,
};

int counter = 128;

// Define debounce interval (in milliseconds)
const unsigned long debounceDelay = 10;

// Variables to store previous state
int lastRotate = 0;
int lastPress = 0;
unsigned long lastDebounceTime = 0;

static void _ResetPins()
{
 // Rotary encoder input lines
 // Configure as input, turn on pullup resistors
 pinMode(dpInEncoderA, INPUT);
 digitalWrite(dpInEncoderA, HIGH);
 pinMode(dpInEncoderB, INPUT);
 digitalWrite(dpInEncoderB, HIGH);
 pinMode(dpInEncoderPress, INPUT);
 digitalWrite(dpInEncoderPress, HIGH);
}


//void _lowlevel_ReadEncoder(int &rotate, int& press)
//{
//  rotate = (digitalRead(dpInEncoderB) * 2) + digitalRead(dpInEncoderA);
//  press = digitalRead(dpInEncoderPress);
//}


void _lowlevel_ReadEncoder(int &rotate, int& press) {
 // Debounce for rotary encoder A and B pins
 int currentRotate = (digitalRead(dpInEncoderB) * 2) + digitalRead(dpInEncoderA);
 if (currentRotate != lastRotate && millis() - lastDebounceTime > debounceDelay) {
   lastRotate = currentRotate;
   rotate = currentRotate;
   lastDebounceTime = millis();
 }

 // Debounce for button press
 int currentPress = digitalRead(dpInEncoderPress);
 if (currentPress != lastPress && millis() - lastDebounceTime > debounceDelay) {
   lastPress = currentPress;
   press = currentPress;
   lastDebounceTime = millis();
 }
}

void ReadEncoder()
{
 int Position, Press;
 int isForward = 0;
 
 _ResetPins();
 Serial.println("Reading the encoder");
 _lowlevel_ReadEncoder(Position, Press);
 while (!Serial.available())
 {
   int Position2, Press2;
   do
   {
     _lowlevel_ReadEncoder(Position2, Press2);
      //  control the ball
      dx = sign_varx * (counter-128)/sqrt(2) * .1; // 45 degree angle
      dy = sign_vary * (counter-128)/sqrt(2) * .1; // 45 degree angle
      x = (x + dx);
      y = (y + dy);
      if (x >= disp.width()-4 || x <= 0 + 4)
      {
        sign_varx = -1*sign_varx;
      }
      if (y >= disp.height()-4 || y <= 0 + 4)
      {
        sign_vary = -1*sign_vary;
      }
      disp.clearDisplay();

      disp.fillCircle(x, y, 4, WHITE);
      disp.display();
    //  Serial.println("2 stuck in do while");
   } while ((Position2 == Position) && (Press2 == Press));
   if (Position2 != Position)
   {
     // "Forward" is shown by the position going from (0 to 1) or (1 to 3)
     // or (3 to 2) or (2 to 0).  Anything else indicates that the user is
     // turning the device the other way.  Remember: this is Gray code, not
     // binary.
     int isFwd = ((Position == 0) && (Position2 == 1)) ||
                 ((Position == 1) && (Position2 == 3)) ||
                 ((Position == 3) && (Position2 == 2)) ||
                 ((Position == 2) && (Position2 == 0));

     // increment counter
     isFwd ? counter = counter + 5 : counter = counter - 5;
     if (counter > 255) counter = 255;
     if (counter < 0) counter = 0;
     Serial.print(isFwd ? "FWD " : "BWD ");
     Serial.println(counter);
   }
   if (Press2 != Press)
   {
     Serial.println(Press ? "Press" : "Release");
   }
   Position = Position2;
   Press = Press2;
 }
}


void setup()
{
   disp.begin(SSD1306_SWITCHCAPVCC);
  //  disp.begin(0x2)
   disp.clearDisplay();
 disp.display();
 disp.drawLine(0, 0, 127, 63, WHITE);
 // configure the pins
 _ResetPins();

 // init serial communication
 Serial.begin(9600); 
 Serial.println("Ready to begin");
}


void loop()
{
 ReadEncoder();
}
