#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include "bitmap.h"


int OLED_CS = 13;
int OLED_DC = 12;
int OLED_RST = 8;
int OLED_SI = 10;
int OLED_CLK = 9; 
Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);


void setup() {
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
}

void loop() {
  // disp.clearDisplay();
  disp.drawBitmap(0, 0, robotBitmap, 128, 64, WHITE); //robotBitmap or emoticonBitmap
  disp.display();
}
