const int buttonPin = 4; //read from button pin PD2
int buttonState = 1; //current state (on or off) of button
int printFlag = 0; //flag to determine if should print button state


void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  PORTD = 0b11111111;
  DDRD = 0b00000000;
}

int readButton() {
  if ((PIND & (1 << buttonPin))) {
    return 1;
  }
  else {
    return 0;
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  buttonState = readButton();
  if (buttonState == 1 && printFlag == 0) {
    Serial.println("The button has been pushed to on.");
    printFlag = 1;
    delay(50); //Delay to debounce
  }
  else if (buttonState == 0 && printFlag == 1) {
    Serial.println("The button has been pushed to off.");
    printFlag = 0;
    delay(50); //Delay to debounce
  }

}
