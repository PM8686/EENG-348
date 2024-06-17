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
int melodya[] = { //final sound
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5};
int noteDurationsa[] = {
  12, 12, 12, 12, 12};

int melodyb[] = { //mario kart countdown
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A5};
int noteDurationsb[] = {
  2, 2, 2, 1};
  

//put this in setup
void setup() { 
   pinMode (button1, INPUT_PULLUP); //specify button mode
   pinMode (button2, INPUT_PULLUP);
   pinMode (button3, INPUT_PULLUP);
   pinMode (button4, INPUT_PULLUP);
}

//put the if statements where you need them
void loop() {

//delay countdown
tone (8, 400, 100); //this is put in the for loop to countdown during the delay
  delay (1000);
  

//final sound
    for (int thisNote = 0; thisNote < 5; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationsa[thisNote];
    tone(8, melodya[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }


//mario kart countdown
    for (int thisNote = 0; thisNote <4; thisNote++) {
    float noteDuration = 1000.0 / noteDurationsb[thisNote];
    tone(8, melodyb[thisNote], noteDuration);
    float pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }


  














}
