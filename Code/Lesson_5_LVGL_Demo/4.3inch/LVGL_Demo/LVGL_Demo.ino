#include <PicoDVI.h>                  // Core display & graphics library
#include "touch.h"

DVIGFX16 display(  DVI_RES_320x240p60, picodvi_dvi_cfg);


/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include <demos/lv_demos.h>
#include <examples/lv_examples.h>
/**************************LVGL and UI END************************/

#define screenWidth     320
#define screenHeight    240

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   display.drawRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

   lv_disp_flush_ready( disp );

}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal()) 
  {
    if (touch_touched()) 
    {
      data->state = LV_INDEV_STATE_PRESSED;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
      Serial1.print( "Data x " );
      Serial1.println( data->point.x );
      Serial1.print( "Data y " );
      Serial1.println( data->point.y );
    }
    else if (touch_released()) 
    {
      data->state = LV_INDEV_STATE_RELEASED;
    }
  } 
  else 
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}



void setup() {

  Serial1.setRX(1);
  Serial1.setTX(0);
  Serial1.begin(115200);

  pinMode(24, OUTPUT);
  digitalWrite(24, LOW); // Enable backlight

  //LCD init
  display.begin();
  touch_init(320, 240);
  display.setRotation(0);  // Takes effect on next drawing command
  display.fillScreen(0xFE80);
  delay(2000);

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, NULL, screenWidth * screenHeight / 10 );
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.full_refresh = 0;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  lv_demo_widgets();

  // lv_obj_t *label = lv_label_create( lv_scr_act() );
  // lv_label_set_text( label, "Hello Ardino and LVGL!");
  // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
  
  Serial1.println( "Setup done" );

}


void loop() {
  lv_timer_handler();
  delay(5);
}


