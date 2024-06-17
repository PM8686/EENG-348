volatile byte state = LOW;
const byte latchPin = 12;
const byte clockPin = 11;
const byte dataPin = 13;
byte leds[5];
int currentLED = 0; //for font switching within each case
int currNum = 0; //for switch case for counting from 0-9
int delayCtr = 0;

// Function prototypes
void setPin(int pin, int val);
void shiftByte(byte leds);

void setup()
{
    Serial.begin(9600);
//    clock = DDB3
//    latch = DDB4
//    data = DDB5
      DDRB |= (1 << DDB3) | (1 << DDB4) | (1 << DDB5);//set pin directions
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

    if (delayCtr == 250) {
      delayCtr = 0;
      if (currNum == 9)
      {
          currNum = 0;
      }
      else
      {
          currNum++;
      }
    }
    else {
      delayCtr++;
      delay(1);
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

    setPin(DDB4, LOW); //let shift register update output pins after data has been shifted in
    myshiftOut(dataPin, clockPin, LSBFIRST, leds[currentLED]); // Shift out current LED byte
//    clock = DDB3
//    latch = DDB4
//    data = DDB5
    setPin(DDB4, HIGH);
    Serial.println(leds[currentLED]);
}


void setPin(int pin, int val) {
  if (val == 1) {
    PORTB |= (1 << (pin)); //high
  }
  else {
    PORTB &= ~(1 << (pin)); //low
  }
}


void myshiftOut(int dataPin, int clockPin, int bitOrder, byte val) {
    for (uint8_t i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) {
            setPin(DDB5, !!(val & (1 << i))); // Write the bit value to the data pin
        } else {
            setPin(DDB5, !!(val & (1 << (7 - i)))); // Write the bit value to the data pin
        }

        // Pulse the clock pin to signal that the bit is ready
        setPin(DDB3, HIGH);
        setPin(DDB3, LOW);
    }
}
