const int buttonPin = 4; // read from button pin PD2
int buttonState = 1;     // current state (on or off) of button
int printFlag = 0;       // flag to determine if should print button state

// for debouncing
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup()
{
  Serial.begin(9600);
  PORTD = 0b11111111; // Set all PORTD pins to high (enable internal pull-up resistors)
  DDRD = 0b00000000;  // Set all PORTD pins as inputs
}

// read the button's state
int readButton()
{
  if ((PIND & (1 << buttonPin)))
  {
    return 1; // button pressed
  }
  else
  {
    return 0; // button not pressed
  }
}

void loop()
{
  unsigned long currentTime = millis(); // Get the current time in milliseconds

  int currentButtonState = readButton(); // get button's state

  // check if still debouncing
  if (currentButtonState != buttonState && (currentTime - lastDebounceTime) > debounceDelay)
  {
    buttonState = currentButtonState;
    lastDebounceTime = currentTime;

    // If the button is pressed and the flag is not set
    if (buttonState == 1 && printFlag == 0)
    {
      Serial.println("The button has been pushed to on.");
      printFlag = 1; // Set the flag
    }
    // If the button is released and the flag is set
    else if (buttonState == 0 && printFlag == 1)
    {
      Serial.println("The button has been pushed to off.");
      printFlag = 0; // Clear the flag
    }
  }
}