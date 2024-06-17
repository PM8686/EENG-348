
volatile byte state = LOW;
int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte leds[5];
int currentLED = 0; //for font switching within each case
int currNum = 0; //for switch case for counting from 0-9
int delayCtr = 0;
const byte interruptPin = 2;
bool updateCurrNum = false;

void setup()
{
    Serial.begin(9600);
    
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT); 
    pinMode(clockPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
}

void blink() {
    static unsigned long lastDebounceTime = 0;  // Keep track of the last time the interrupt was triggered
    unsigned long debounceDelay = 50;           // Debounce time in milliseconds

    // Check if enough time has passed since the last interrupt
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Record the time of the last interrupt
        lastDebounceTime = millis();
        
        // Set the flag to true
        updateCurrNum = true;
    }
}

void loop()
{
    if (currentLED == 4)
    {
      currentLED = 0;
    }
    else
    {
      currentLED++;
    }

    if(updateCurrNum) {
      if (currNum == 9)
      {
        currNum = 0;
      }
      else
      {
        currNum++;
      }
      updateCurrNum = false; //reset flag
    }


    if (currNum == 0) {
        leds[0] = 0x3E;
        leds[1] = 0x51;
        leds[2] = 0x49;
        leds[3] = 0x45;
        leds[4] = 0x3E;
      }
      else if (currNum == 1) {
        leds[0] = 0x00;
        leds[1] = 0x42;
        leds[2] = 0x7F;
        leds[3] = 0x40;
        leds[4] = 0x00;
      }
      else if (currNum == 2) {
        leds[0] = 0x42;
        leds[1] = 0x61;
        leds[2] = 0x51;
        leds[3] = 0x49;
        leds[4] = 0x46;
      }
      else if (currNum == 3) {
        leds[0] = 0x21;
        leds[1] = 0x41;
        leds[2] = 0x45;
        leds[3] = 0x4B;
        leds[4] = 0x31;
      }
      else if (currNum == 4) {
        leds[0] = 0x18;
        leds[1] = 0x14;
        leds[2] = 0x12;
        leds[3] = 0x7F;
        leds[4] = 0x10;
      }
      else if (currNum == 5) {
        leds[0] = 0x27;
        leds[1] = 0x45;
        leds[2] = 0x45;
        leds[3] = 0x45;
        leds[4] = 0x39;
      }
      else if (currNum == 6) {
        leds[0] = 0x3C;
        leds[1] = 0x4A;
        leds[2] = 0x49;
        leds[3] = 0x49;
        leds[4] = 0x30;
      }
      else if (currNum == 7) {
        leds[0] = 0x01;
        leds[1] = 0x71;
        leds[2] = 0x09;
        leds[3] = 0x05;
        leds[4] = 0x03;
      }
      else if (currNum == 8) {
        leds[0] = 0x36;
        leds[1] = 0x49;
        leds[2] = 0x49;
        leds[3] = 0x49;
        leds[4] = 0x36;
      }
      else {
        leds[0] = 0x06;
        leds[1] = 0x49;
        leds[2] = 0x49;
        leds[3] = 0x29;
        leds[4] = 0x1E;
      } 

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds[currentLED]); // Shift out current LED byte
    digitalWrite(latchPin, HIGH);
}
