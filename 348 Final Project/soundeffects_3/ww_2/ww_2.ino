/* modified toneMelody example
*/

int button1 = 1; //remove this bc it's not necessary
int button2 = 1;
int button3 = 1;
int button4 = 1;
int button5 = 1;
int button6 = 1;

//put all of this outside of any loops for setup

#include "pitches.h"

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

//put this in setup
void setup() { 
   pinMode (button1, INPUT_PULLUP); //specify button mode
   pinMode (button2, INPUT_PULLUP);
   pinMode (button3, INPUT_PULLUP);
   pinMode (button4, INPUT_PULLUP);
}

//put the if statements where you need them
void loop() {
  if (digitalRead (button1) == LOW){ //good rep
    for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations1[thisNote];
    tone(8, melody1[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  } 
  } else if (digitalRead (button2) == LOW){ //bad rep
     for (int thisNote = 0; thisNote < 2; thisNote++) {
    int noteDuration = 1000 / noteDurations2[thisNote];
    tone(8, melody2[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
} else if (digitalRead (button3) == LOW){ //calibration start
     for (int thisNote = 0; thisNote < 4; thisNote++) {
    int noteDuration = 1000 / noteDurations3[thisNote];
    tone(8, melody3[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
 else if (digitalRead (button4) == LOW){ //calibration done so countdown
     for (int thisNote = 0; thisNote <4; thisNote++) {
    int noteDuration = 1000 / noteDurations4[thisNote];
    tone(8, melody4[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
else if (digitalRead (button5) == LOW){ //power on
     for (int thisNote = 0; thisNote <4; thisNote++) {
    int noteDuration = 1000 / noteDurations4[thisNote];
    tone(8, melody4[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
else if (digitalRead (button6) == LOW){ //power off
     for (int thisNote = 0; thisNote <2; thisNote++) {
    int noteDuration = 1000 / noteDurations4[thisNote];
    tone(8, melody4[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
}
