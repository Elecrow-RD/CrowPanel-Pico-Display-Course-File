#include <PicoDVI.h>                  // Core display & graphics library
#include "touch.h"


// Here's how a 320x240 16-bit color framebuffer is declared. Double-buffering
// is not an option in 16-bit color mode, just not enough RAM; all drawing
// operations are shown as they occur. Second argument is a hardware
// configuration -- examples are written for Adafruit Feather RP2040 DVI, but
// that's easily switched out for boards like the Pimoroni Pico DV (use
// 'pimoroni_demo_hdmi_cfg') or Pico DVI Sock ('pico_sock_cfg').
DVIGFX16 display(  DVI_RES_320x240p60, picodvi_dvi_cfg);

// A 400x240 mode is possible but pushes overclocking even higher than
// 320x240 mode. SOME BOARDS MIGHT SIMPLY NOT BE COMPATIBLE WITH THIS.
// May require selecting QSPI div4 clock (Tools menu) to slow down flash
// accesses, may require further over-volting the CPU to 1.25 or 1.3 V.
// DVIGFX16 display(DVI_RES_400x240p60, picodvi_dvi_cfg);

int mode = 1;

void setup() {

  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200);

  //LCD init
  display.begin();
  touch_init(320, 240);
  display.setRotation(0);  // Takes effect on next drawing command

  display.fillScreen(0xF800);
  delay(1000);
  display.fillScreen(0xFFE0);
  delay(1000);
  display.fillScreen(0x001F);
  delay(1000);
  display.fillScreen(0x07E0);
  delay(1000);
  
  display.fillCircle(25, 25, 25, 0x001F);
  display.fillRect(51, 0, 49, 49, 0x001F);
  display.fillTriangle(124, 0, 101, 50, 150, 50, 0x001F);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(touch_touched())
  {
    Serial1.print( "Data x " );
    Serial1.println( touch_last_x );

    Serial1.print( "Data y " );
    Serial1.println( touch_last_y );

    if(touch_last_x <= 50 && touch_last_y <= 50)
    {
      mode = 1;
      display.fillCircle(25, 25, 25, 0xF800);
      display.fillRect(51, 0, 49, 49, 0x001F);
      display.fillTriangle(124, 0, 101, 49, 150, 49, 0x001F);
    }
    else if(((touch_last_x > 50) && (touch_last_x <= 100)) && (touch_last_y <= 50))
    {
      mode = 2;
      display.fillCircle(25, 25, 25, 0x001F);
      display.fillRect(51, 0, 49, 49, 0xF800);
      display.fillTriangle(124, 0, 101, 50, 150, 50, 0x001F);
    }
    else if((touch_last_x > 100) && (touch_last_x <= 150) && (touch_last_y <= 50))
    {
      mode = 3;
      display.fillCircle(25, 25, 25, 0x001F);
      display.fillRect(51, 0, 49, 49, 0x001F);
      display.fillTriangle(124, 0, 101, 50, 150, 50, 0xF800);
    }
    else
    {
      switch(mode)
      {
        case 1: display.fillCircle(touch_last_x, touch_last_y, 5, 0xFFE0);
                break;
        case 2: display.fillRect(touch_last_x, touch_last_y, 10, 10, 0xFFE0);
                break;
        case 3: display.fillTriangle(touch_last_x, touch_last_y, touch_last_x-6, touch_last_y+6, touch_last_x+6, touch_last_y+6, 0xFFE0);
                break;
        default:display.fillCircle(touch_last_x, touch_last_y, 5, 0xFFE0);
                break;
      }
    }
  }
}


