#include <lvgl.h>
#include <TFT_eSPI.h>
#include "lv_conf.h"
#include <demos/lv_demos.h>
#include "CST816S.h"
#include "pins_arduino.h"
#include <Wire.h>  // Add Wire library for I2C communication

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

/*Change to your screen resolution*/
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 280;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
CST816S touch(11, 10, 13, 14);

#define BEE_PIN 33  // Define the pin for controlling the buzzer

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf) {
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

void example_increase_lvgl_tick(void *arg) {
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

static uint8_t count = 0;
void example_increase_reboot(void *arg) {
  count++;
  if (count == 30) {
    esp_restart();
  }
}

/* Read the touchpad */
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  // uint16_t touchX, touchY;

  bool touched = touch.available();
  // touch.read_touch();
  if (!touched)
  // if( 0!=touch.data.points )
  {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /* Set the coordinates */
    data->point.x = touch.data.x;
    data->point.y = touch.data.y;

    // Serial.print("Data x ");
    // Serial.println(touch.data.x);

    // Serial.print("Data y ");
    // Serial.println(touch.data.y);
  }
}




// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Keyboard1;
lv_obj_t * ui_Arc1;
lv_obj_t * ui_Button2;
lv_obj_t * ui_Checkbox1;
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}





void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Keyboard1 = lv_keyboard_create(ui_Screen1);
    lv_obj_set_width(ui_Keyboard1, 217);
    lv_obj_set_height(ui_Keyboard1, 120);
    lv_obj_set_x(ui_Keyboard1, -2);
    lv_obj_set_y(ui_Keyboard1, 75);
    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);

    ui_Arc1 = lv_arc_create(ui_Screen1);
    lv_obj_set_width(ui_Arc1, 120);
    lv_obj_set_height(ui_Arc1, 123);
    lv_obj_set_x(ui_Arc1, -35);
    lv_obj_set_y(ui_Arc1, -56);
    lv_obj_set_align(ui_Arc1, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_Arc1, 50);


    ui_Button2 = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_Button2, 55);
    lv_obj_set_height(ui_Button2, 50);
    lv_obj_set_x(ui_Button2, 73);
    lv_obj_set_y(ui_Button2, -100);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);

    ui_Checkbox1 = lv_checkbox_create(ui_Screen1);
    lv_obj_set_width(ui_Checkbox1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Checkbox1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Checkbox1, 56);
    lv_obj_set_y(ui_Checkbox1, 2);
    lv_obj_set_align(ui_Checkbox1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Checkbox1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags


}


void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(0); /* Landscape orientation, flipped */
  touch.begin();

  // Initialize I2C
  Wire.begin(11, 10);
  Wire.setClock(400000);
  // Check devices connected to I2C bus
  byte error, address;
  bool foundDevice = false;
  for (address = 1; address < 128; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      if (address == 0x51 || address == 0x6B || address == 0x7E) {
        foundDevice = true;
      }
    }
  }

  // Control the buzzer if the device is found
  if (foundDevice) {
    pinMode(BEE_PIN, OUTPUT);
    tone(BEE_PIN, 4000);
    delay(500);       // Buzz for 0.5 seconds
    noTone(BEE_PIN);  // Stop the buzzer
  }

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /* Initialize the display */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /* Create simple label */
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello Ardino and LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  /* Try an example. See all the examples 
     * online: https://docs.lvgl.io/master/examples.html
     * source codes: https://github.com/lvgl/lvgl/tree/e7f88efa5853128bf871dde335c0ca8da9eb7731/examples */
  //lv_example_btn_1();

  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };

  const esp_timer_create_args_t reboot_timer_args = {
    .callback = &example_increase_reboot,
    .name = "reboot"
  };

  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);

  // esp_timer_handle_t reboot_timer = NULL;
  // esp_timer_create(&reboot_timer_args, &reboot_timer);
  // esp_timer_start_periodic(reboot_timer, 2000 * 1000);

  /* Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS */

  ui_init();
  if (foundDevice) {
    ui_Screen1_screen_init();
    // lv_demo_widgets();
    // lv_demo_benchmark();
  }

  Serial.println("Setup done");
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
