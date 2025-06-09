#include "core/lv_obj.h"
#include "esp_lcd_io_spi.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_sh8601.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/task.h"
#include "hal/lv_hal_disp.h"
#include "hal/lv_hal_indev.h"
#include "i2c_bsp.h"
#include "touch_bsp.h"

#include "app_lvgl.h"
#include "board_config.h"

/* Function pointer declarations for UI callbacks */
static ui_tick_callback_t ui_tick_cb = NULL;
static ui_init_callback_t ui_init_cb = NULL;

static SemaphoreHandle_t lvgl_mux = NULL;

#if BOARD_USE_TOUCH
static void app_lvgl_touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  uint16_t tp_x;
  uint16_t tp_y;
  uint8_t win = 0;
  win = getTouch(&tp_x, &tp_y);
  if (win) {
#if (BOARD_DIRECTION == BOARD_NORMAL)
    data->point.x = tp_x;
    data->point.y = tp_y;
#else
    data->point.x = tp_y;
    data->point.y = BOARD_LCD_V_RES - tp_x;
#endif
    data->state = LV_INDEV_STATE_PRESSED;
    ESP_LOGD("tp_user", "Touch position: %d,%d", tp_x, tp_y);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
#endif

static bool app_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io,
                                        esp_lcd_panel_io_event_data_t *edata,
                                        void *user_ctx) {
  lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
  lv_disp_flush_ready(disp_driver);
  return false;
}

static void app_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area,
                              lv_color_t *color_map) {
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
#if (BOARD_DIRECTION == BOARD_NORMAL)
  int offsetx1 = area->x1 + 35;
  int offsetx2 = area->x2 + 35;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
#elif (BOARD_DIRECTION == BOARD_ROTATE)
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1 + 35;
  int offsety2 = area->y2 + 35;
#endif
  // copy a buffer's content to a specific area of the display
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1,
                            offsety2 + 1, color_map);
}

static void app_increase_lvgl_tick(void *arg) {
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(APP_LVGL_TICK_PERIOD_MS);
}

static bool app_lvgl_lock(int timeout_ms) {
  assert(lvgl_mux && "bsp_display_start must be called first");

  const TickType_t timeout_ticks =
      (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
  return xSemaphoreTake(lvgl_mux, timeout_ticks) == pdTRUE;
}

static void app_lvgl_unlock(void) {
  assert(lvgl_mux && "bsp_display_start must be called first");
  xSemaphoreGive(lvgl_mux);
}

static void app_lvgl_port_task(void *arg) {
  uint32_t task_delay_ms = APP_LVGL_TASK_MAX_DELAY_MS;
  for (;;) {
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (app_lvgl_lock(-1)) {
      task_delay_ms = lv_timer_handler();
      // Release the mutex
      app_lvgl_unlock();
    }
    if (task_delay_ms > APP_LVGL_TASK_MAX_DELAY_MS) {
      task_delay_ms = APP_LVGL_TASK_MAX_DELAY_MS;
    } else if (task_delay_ms < APP_LVGL_TASK_MIN_DELAY_MS) {
      task_delay_ms = APP_LVGL_TASK_MIN_DELAY_MS;
    }
    vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    if (ui_tick_cb != NULL) {
      ui_tick_cb();
    }
  }
}

void app_lvgl_configure(void) {
  static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
  static lv_disp_drv_t disp_drv; // contains callback functions

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
      .dc_gpio_num = PIN_NUM_DC,
      .cs_gpio_num = PIN_NUM_CS,
      .pclk_hz = 20 * 1000 * 1000,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
      .spi_mode = 0,
      .trans_queue_depth = 10,
      .on_color_trans_done = app_notify_lvgl_flush_ready,
      .user_ctx = &disp_drv,
  };
  sh8601_vendor_config_t vendor_config = {
      .init_cmds = lcd_init_cmds,
      .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,
                                           &io_config, &io_handle));

  esp_lcd_panel_handle_t panel_handle = NULL;
  const esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = PIN_NUM_RST,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
      .bits_per_pixel = 16,
      .vendor_config = &vendor_config,
      .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
  };
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_sh8601(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  // ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
  I2C_master_Init();
#if BOARD_USE_TOUCH
  touch_Init();
#endif
  lv_init();
  lv_color_t *buf1 = heap_caps_malloc(BOARD_LCD_H_RES * BOARD_LCD_DMA_Line *
                                          sizeof(lv_color_t),
                                      MALLOC_CAP_DMA);
  assert(buf1);
  lv_color_t *buf2 = heap_caps_malloc(BOARD_LCD_H_RES * BOARD_LCD_DMA_Line *
                                          sizeof(lv_color_t),
                                      MALLOC_CAP_DMA);
  assert(buf2);
  lv_disp_draw_buf_init(&disp_buf, buf1, buf2,
                        BOARD_LCD_H_RES * BOARD_LCD_DMA_Line);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = BOARD_LCD_H_RES;
  disp_drv.ver_res = BOARD_LCD_V_RES;
  disp_drv.flush_cb = app_lvgl_flush_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = panel_handle;
  lv_disp_drv_register(&disp_drv);
#if BOARD_USE_TOUCH
  static lv_indev_drv_t indev_drv; // Input device driver (Touch)
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = app_lvgl_touch_cb;
  lv_indev_drv_register(&indev_drv);
#endif
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &app_increase_lvgl_tick, .name = "lvgl_tick"};
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer,
                                           APP_LVGL_TICK_PERIOD_MS * 1000));
  lvgl_mux = xSemaphoreCreateMutex();
  assert(lvgl_mux);
  xTaskCreate(app_lvgl_port_task, "LVGL", APP_LVGL_TASK_STACK_SIZE, NULL,
              APP_LVGL_TASK_PRIORITY, NULL);
  if (app_lvgl_lock(-1)) {
    if (ui_init_cb != NULL) {
      ui_init_cb();
    }
    app_lvgl_unlock();
  }
}

/* Public function implementations */
void app_lvgl_set_tick_callback(ui_tick_callback_t callback) {
  ui_tick_cb = callback;
}

void app_lvgl_set_init_callback(ui_init_callback_t callback) {
  ui_init_cb = callback;
}