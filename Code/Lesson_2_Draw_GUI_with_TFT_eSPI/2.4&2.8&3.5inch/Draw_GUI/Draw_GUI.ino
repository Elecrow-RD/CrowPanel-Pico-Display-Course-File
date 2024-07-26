#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#define LED 5

TFT_eSPI tft = TFT_eSPI(); /* TFT instance screenWidth, screenHeight */

uint16_t calData[5] = { 412, 3388, 380, 3303, 3 };


void setup()
{
  Serial.begin( 115200 );

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  tft.begin();          /* TFT init */
  tft.setRotation(1); /* Landscape orientation, flipped */

  /*Set the touchscreen calibration data,
    the actual data for your display can be acquired using
    the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  tft.setTouch(calData);
  
  tft.fillScreen(TFT_RED);
  delay(1000);
  tft.fillScreen(TFT_YELLOW);
  delay(1000);
  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle(30, 30, 30, TFT_RED);
  tft.fillTriangle(160, 140, 100, 190, 220, 190, TFT_GOLD);
  tft.fillRectHGradient(110, 190, 100, 50, TFT_CYAN, TFT_BLUE);
  tft.setTextFont(2);
  tft.setCursor(120, 60);
  tft.setTextSize(2);
  tft.println("ElECROW!");
  tft.setCursor(100, 90);
  tft.setTextSize(1);
  tft.println("Make your making easier");

  Serial.println("drawing done");
}

void loop()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}



