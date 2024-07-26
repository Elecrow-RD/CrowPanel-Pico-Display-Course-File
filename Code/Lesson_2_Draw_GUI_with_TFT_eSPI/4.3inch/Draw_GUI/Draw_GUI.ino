// Basic full-color PicoDVI test. Provides a 16-bit color video framebuffer to
// which Adafruit_GFX calls can be made. It's based on the EYESPI_Test.ino sketch.

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



void setup() { // Runs once on startup
  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200);

  pinMode(28, OUTPUT);
  pinMode(24, OUTPUT);
  digitalWrite(24, LOW); // Enable backlight

  display.begin();
  display.setRotation(0);  // Takes effect on next drawing command

  touch_init(320, 240);

  display.fillScreen(0x0000);
  delay(1000);
  display.fillScreen(0x1FE0);
  display.print("hello, elecrow");

  delay(100);

  Serial1.println("drawing done");
  
}


void loop() {
  digitalWrite(28, HIGH); 
  delay(1000);
  digitalWrite(28, LOW);
  delay(1000);
}



