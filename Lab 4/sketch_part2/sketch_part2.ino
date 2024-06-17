#include "concurrency.h"
#include <Adafruit_SSD1306.h>

int OLED_CS = 13;
int OLED_DC = 12;
int OLED_RST = 11;
int OLED_SI = 10;
int OLED_CLK = 9; 


Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);

int height = disp.height();
int width = disp.width();

// ball 1
int x_1 = width/2 + 25;
int y_1 = height/2;
int dx_1=0;
int dy_1=0;
int sign_varx_1 = 1;
int sign_vary_1 = -1;

// ball 2
int x_2 = width/2 - 25;
int y_2 = height/2;
int dx_2=0;
int dy_2=0;
int sign_varx_2 = 1;
int sign_vary_2 = -1;

lock_t lock;

void p1 (void)
{
  /* process 1 here */
  while(1)
  {
    dx_1 = 0 * sign_varx_1;
    dy_1 = 3 * sign_vary_1;
    x_1 = (x_1 + dx_1);
    y_1 = (y_1 + dy_1);
    if (x_1 >= width-4 || x_1 <= 0 + 4)
    {
      sign_varx_1 = -1*sign_varx_1;
    }
    if (y_1 >= height-4 || y_1 <= 0 + 4)
    {
      sign_vary_1 = -1*sign_vary_1;
    }

    // use a lock to change the shared display
    lock_acquire(&lock);
    disp.clearDisplay();
    disp.fillCircle(x_1, y_1, 4, WHITE);
    disp.display();
    lock_release(&lock);
  }

  return;
}

void p2 (void)
{
/* process 2 here */
  while(1)
  {
    // use a lock to change the shared display
    dx_2 = 0 * sign_varx_2;
    dy_2 = 5 * sign_vary_2;
    x_2 = (x_2 + dx_2);
    y_2 = (y_2 + dy_2);
    if (x_2 >= width-4 || x_2 <= 0 + 4)
    {
      sign_varx_2 = -1*sign_varx_2;
    }
    if (y_2 >= height-4 || y_2 <= 0 + 4)
    {
      sign_vary_2 = -1*sign_vary_2;
    }

    // use a lock to change the shared display
    lock_acquire(&lock);
    disp.clearDisplay();
    disp.fillCircle(x_2, y_2, 4, WHITE);
    disp.display();
    lock_release(&lock);
  }
 return;
}

void setup()
{
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();

  if (process_create (p1, 64) < 0) {
    return;
  }
  if (process_create (p2, 64) < 0) {
  return;
  }

  // initialize the lock
  lock_init(&lock);
}

void loop()
{
  process_start();
 /* if you get here, then all processes are either finished or
    there is deadlock */
  while (1) ;
}
