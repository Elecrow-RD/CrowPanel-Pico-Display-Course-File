/**************************CrowPanel Pico HMI Display Example Code************************
Code Version:	1.0
Suitable for:	CrowPanel Pico HMI Display
Product link:	https://www.elecrow.com/display/esp-hmi-display/crowpanel-pico-display.html
Code	  link:	https://github.com/Elecrow-RD/CrowPanel-Pico-Display-Course-File
Lesson	link:	https://www.youtube.com/watch?v=I0zDu-XYJ8E
Description	:	The code is currently available based on the course on YouTube, 
				        if you have any questions, please refer to the Lesson 01.
**************************************************************/


/*******************************************************************************
   Config the display driver and touch pin in User_Setup.h
 ******************************************************************************/
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(); 
uint16_t calData[5] = { 412, 3388, 380, 3303, 3 };
int mode = 1;

void setup()
{
  Serial.begin(9600);

  //Display Prepare
  tft.begin();
  tft.setRotation(1);
  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  delay(100);

  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_YELLOW);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_WHITE);
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  tft.fillCircle(25, 25, 25, TFT_BLUE);
  tft.fillRect(51, 0, 49, 49, TFT_BLUE);
  tft.fillTriangle(124, 0, 101, 50, 150, 50, TFT_BLUE);
}

void loop()
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  if( touched )
  {
    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );

    if(touchX <= 50 && touchY <= 50)
    {
      mode = 1;
    }
    else if(((touchX > 50) && (touchX <= 100)) && (touchY <= 50))
    {
      mode = 2;
    }
    else if((touchX > 100) && (touchX <= 150) && (touchY <= 50))
    {
      mode = 3;
    }
    else
    {
      switch(mode)
      {
        case 1: tft.fillCircle(touchX, touchY, 5, TFT_WHITE);
                break;
        case 2: tft.fillRect(touchX, touchY, 10, 10, TFT_WHITE);
                break;
        case 3: tft.fillTriangle(touchX, touchY, touchX-6, touchY+6, touchX+6, touchY+6, TFT_WHITE);
                break;
        default:tft.fillCircle(touchX, touchY, 5, TFT_WHITE);
                break;
      }
    }
  }
}








