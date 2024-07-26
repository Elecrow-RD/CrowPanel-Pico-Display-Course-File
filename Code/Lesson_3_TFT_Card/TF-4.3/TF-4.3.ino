#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

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

const int _MISO = 16;  // AKA SPI RX
const int _MOSI = 7;  // AKA SPI TX
const int _CS = 17;
const int _SCK = 6;
File root;


void setup() {

  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200);

  //LCD init
  display.begin();
  display.setRotation(0);  // Takes effect on next drawing command

  touch_init(320, 240);

  Serial1.println("\nInitializing SD card...");
  bool sdInitialized = false;
  SPI1.setRX(_MISO);
  SPI1.setTX(_MOSI);
  SPI1.setSCK(_SCK);
  sdInitialized = SD.begin(_CS, SPI1);

  delay(100);

  if (!sdInitialized) {
    Serial1.println("initialization failed. Things to check:");
    Serial1.println("* is a card inserted?");
    Serial1.println("* is your wiring correct?");
    Serial1.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial1.println("Wiring is correct and a card is present.");
  }

  // display.fillScreen(0x1FE0);
  // display.setCursor(140, 120);
  // display.setTextSize(2, 2);
  // display.print("Init complete");
  // delay(2000);
  // root = SD.open("/");
  //printDirectory(root, 0);
  
}

void loop() {
  // put your main code here, to run repeatedly:

    // drawBmp("/4-3_01.bmp", 0, 0);//display image1
    // delay(1000);
    // drawBmp("/4-3_02.bmp", 0, 0);//display image2
    // delay(1000);
    // drawBmp("/4-3_03.bmp", 0, 0);//display image3
    // delay(1000);
    // drawBmp("/4-3_04.bmp", 0, 0);//display image4
    // delay(1000);
    // drawBmp("/4-3_05.bmp", 0, 0);//display image5
    // delay(1000);
}

void drawBmp(const char *filename, int16_t x, int16_t y) {

  if ((x >= display.width()) || (y >= display.height())) return;

  // Open requested file on SD card
  File bmpFS = SD.open(filename, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42)
  {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
    {
      y += h - 1;

      // bool oldSwapBytes = display.getSwapBytes();
      // display.setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16-bit colours
        for (uint16_t col = 0; col < w; col++)
        {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        //tft.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
        display.drawRGBBitmap(x, y--, (uint16_t*)lineBuffer, w, 1);
      }
//      display.setSwapBytes(oldSwapBytes);
      Serial.print("Loaded in "); Serial.print(millis() - startTime);
      Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
