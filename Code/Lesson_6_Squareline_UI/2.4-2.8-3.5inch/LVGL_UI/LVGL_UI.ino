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
TFT_eSPI tft = TFT_eSPI(); /* TFT entity */

/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include "ui.h"
// #include <demos/lv_demos.h>
// #include <examples/lv_examples.h>
/**************************LVGL and UI END************************/

/*******************************************************************************
 * Please define the corresponding macros based on the board you have purchased.
 * CrowPanel_24 means CrowPanel Pico Display 2.4inch Board
 * CrowPanel_28 means CrowPanel Pico Display 2.8inch Board
 * CrowPanel_35 means CrowPanel Pico Display 3.5inch Board
 ******************************************************************************/
#define CrowPanel_24
//#define CrowPanel_28
//#define CrowPanel_35

#if defined (CrowPanel_35)
/*screen resolution*/
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;
uint16_t calData[5] = { 192, 3590, 335, 3362, 1  };
#elif defined (CrowPanel_24)
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
uint16_t calData[5] = { 412, 3388, 380, 3303, 3 };
#elif defined (CrowPanel_28)
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
uint16_t calData[5] = { 208, 3520, 380, 3373, 1 };
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[ screenWidth * screenHeight / 10 ];


//_______________________
/* display flash */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  tft.endWrite();

  lv_disp_flush_ready( disp );
}

uint16_t touchX, touchY;
/*touch read*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{

  bool touched = tft.getTouch( &touchX, &touchY, 600);
  if ( !touched )
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*set location*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );
  }
}



void setup()
{
  Serial.begin(9600);

  //Display Prepare
  tft.begin();
  tft.setRotation(1);
  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);
  delay(100);

  //lvgl init
  lv_init();
  
  lv_disp_draw_buf_init( &draw_buf, buf1, NULL, screenWidth * screenHeight / 10 );

  /* Initialize the display */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  tft.fillScreen(TFT_BLACK);

//  lv_demo_widgets();    // LVGL demo

  ui_init();

  Serial.println( "Setup done" );
}

void loop()
{
  lv_timer_handler();
  delay(5);
}








