int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte leds = 0;
int currentLED = 0;
void mydigitalWrite(int pin, byte val);
void myshiftOut(int dataPin, int clockPin, int bitOrder, byte val);
void mypinMode(int pin, int mode);
void setup()
{
    Serial.begin(9600);
    leds = 0;

//    clock = DDB3
//    latch = DDB4
//    data = DDB5
    DDRB |= (1 << DDB3) | (1 << DDB4) | (1 << DDB5);//set pin directions
}

void loop()
{
  Serial.println(digitalPinToBitMask(dataPin));
    leds = 0;

    if (currentLED == 8)
    {
        currentLED = 0;
    }
    else
    {
        currentLED++;
    }

    bitSet(leds, currentLED);
//    leds = B10100010;

    mydigitalWrite(latchPin, LOW);
    myshiftOut(dataPin, clockPin, LSBFIRST, leds);
    mydigitalWrite(latchPin, HIGH);

    delay(1000);
}

//void mypinMode(int pin, int mode) {
//    if (pin < NUM_DIGITAL_PINS) {
//        volatile uint8_t* portModeRegister = portModeRegister(digitalPinToPort(pin)); // Get pointer to the mode register for the pin's port
//        uint8_t bitMask = digitalPinToBitMask(pin); // Convert pin number to bitmask
//
//        if (mode == OUTPUT) {
//            DDRB |= bitMask; // Set the pin as an output by setting the corresponding bit in the mode register
//        } else {
//            DDRB &= ~bitMask; // Set the pin as an input by clearing the corresponding bit in the mode register
//        }
//    }
//}

void mydigitalWrite(int pin, byte val) {
    if (pin < NUM_DIGITAL_PINS) {
        byte bitMask = digitalPinToBitMask(pin); // Convert pin number to bitmask
        Serial.println(bitMask, BIN);
//        volatile uint8_t* port = portOutputRegister(digitalPinToPort(pin)); // Get pointer to output register for the pin's port

        if (val == HIGH) {
            PORTB |= bitMask; // Set the pin HIGH by setting the corresponding bit in the port register
        } else {
            PORTB &= ~bitMask; // Set the pin LOW by clearing the corresponding bit in the port register
        }
    }
}

void myshiftOut(int dataPin, int clockPin, int bitOrder, byte val) {
    for (uint8_t i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) {
            mydigitalWrite(dataPin, !!(val & (1 << i))); // Write the bit value to the data pin
        } else {
            mydigitalWrite(dataPin, !!(val & (1 << (7 - i)))); // Write the bit value to the data pin
        }

        // Pulse the clock pin to signal that the bit is ready
        mydigitalWrite(clockPin, HIGH);
        mydigitalWrite(clockPin, LOW);
    }
}
