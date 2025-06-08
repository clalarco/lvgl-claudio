
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "lvgl.h"
#include "lv_demos.h"
#include "esp_timer.h"
#include "esp_lcd_sh8601.h"
#include "i2c_bsp.h"
#include "touch_bsp.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#define LCD_HOST  SPI2_HOST

static SemaphoreHandle_t lvgl_mux = NULL;
#define EXAMPLE_LVGL_TICK_PERIOD_MS    2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1
#define EXAMPLE_LVGL_TASK_STACK_SIZE   (4 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY     2

#define Rotate 1        //旋转90
#define Normal 0        //正常显示
#define Direction Normal

#if (Direction == Normal)
  #define EXAMPLE_LCD_H_RES 170   //宽度 水平分辨率
  #define EXAMPLE_LCD_V_RES 320   //高度 竖直分辨率
#elif (Direction == Rotate)
  #define EXAMPLE_LCD_H_RES 320   //宽度 水平分辨率
  #define EXAMPLE_LCD_V_RES 170   //高度 竖直分辨率
#endif

#define EXAMPLE_LCD_DMA_Line (EXAMPLE_LCD_V_RES / 2)

#define EXAMPLE_USE_Disp       1
#define EXAMPLE_USE_TOUCH      1
#define EXAMPLE_USE_SDCARD     0

#if EXAMPLE_USE_SDCARD
#define PIN_NUM_MISO 19
#define PIN_NUM_SDCS 20
sdmmc_card_t *card = NULL; // Handle
#define SDlist "/sd_card"  // Directory, acts like a standard
esp_err_t s_example_write_file(const char *path, char *data);
esp_err_t s_example_read_file(const char *path,char *pxbuf,uint32_t *outLen);
float sd_card_get_value(void);
#endif

#define PIN_NUM_MOSI 4
#define PIN_NUM_CLK  5
#define PIN_NUM_CS   7
#define PIN_NUM_RST  14
#define PIN_NUM_DC   6
#define EXAMPLE_LVGL_TICK_PERIOD_MS    2
#if (EXAMPLE_USE_Disp == 1)
static const sh8601_lcd_init_cmd_t lcd_init_cmds[] =
{
  #if (Direction == Normal)
  //{0x36, (uint8_t []){0x00}, 1, 0},
  #elif (Direction == Rotate)
  {0x36, (uint8_t []){0x70}, 1, 0},
  #endif

  //{0x3a, (uint8_t []){0x55}, 1, 0},
  {0xb2, (uint8_t []){0x0c,0x0c,0x00,0x33,0x33}, 5, 0},
  {0xb7, (uint8_t []){0x35}, 1, 0},
  {0xbb, (uint8_t []){0x13}, 1, 0},
  {0xc0, (uint8_t []){0x2c}, 1, 0},
  {0xc2, (uint8_t []){0x01}, 1, 0},
  {0xc3, (uint8_t []){0x0b}, 1, 0},
  {0xc4, (uint8_t []){0x20}, 1, 0},
  {0xc6, (uint8_t []){0x0f}, 1, 0},
  {0xd0, (uint8_t []){0xa4,0xa1}, 2, 0},
  {0xd6, (uint8_t []){0xa1}, 1, 0},
  {0xe0, (uint8_t []){0x00,0x03,0x07,0x08,0x07,0x15,0x2A,0x44,0x42,0x0A,0x17,0x18,0x25,0x27}, 14, 0},
  {0xe1, (uint8_t []){0x00,0x03,0x08,0x07,0x07,0x23,0x2A,0x43,0x42,0x09,0x18,0x17,0x25,0x27}, 14, 0},
  {0x21, (uint8_t []){0x21}, 0, 0},
  {0x11, (uint8_t []){0x11}, 0, 120},
  {0x29, (uint8_t []){0x29}, 0, 0},
};
#endif
#if EXAMPLE_USE_TOUCH
static void example_lvgl_touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  uint16_t tp_x;
  uint16_t tp_y;
  uint8_t win = 0;
  win = getTouch(&tp_x,&tp_y);
  if(win)
  {
#if (Direction==Normal)
    data->point.x = tp_x;
    data->point.y = tp_y;
#else
    data->point.x = tp_y;
    data->point.y = EXAMPLE_LCD_V_RES - tp_x;
#endif
    data->state = LV_INDEV_STATE_PRESSED;
    ESP_LOGD("tp_user", "Touch position: %d,%d", tp_x, tp_y);
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
#endif
static bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
  lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
  lv_disp_flush_ready(disp_driver);
  return false;
}
static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
#if (Direction == Normal)
  int offsetx1 = area->x1 + 35;
  int offsetx2 = area->x2 + 35;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
#elif (Direction == Rotate)
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1 + 35;
  int offsety2 = area->y2 + 35;
#endif
  // copy a buffer's content to a specific area of the display
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}
static void example_increase_lvgl_tick(void *arg)
{
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}
static bool example_lvgl_lock(int timeout_ms)
{
  assert(lvgl_mux && "bsp_display_start must be called first");

  const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
  return xSemaphoreTake(lvgl_mux, timeout_ticks) == pdTRUE;
}
static void example_lvgl_unlock(void)
{
  assert(lvgl_mux && "bsp_display_start must be called first");
  xSemaphoreGive(lvgl_mux);
}
static void example_lvgl_port_task(void *arg)
{
  uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
  for(;;)
  {
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (example_lvgl_lock(-1))
    {
      task_delay_ms = lv_timer_handler();
      // Release the mutex
      example_lvgl_unlock();
    }
    if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS)
    {
      task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    }
    else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS)
    {
      task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
    }
    vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
  }
}
void app_main(void)
{
  static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
  static lv_disp_drv_t disp_drv;      // contains callback functions
  spi_bus_config_t buscfg =
  {
#if EXAMPLE_USE_SDCARD
    .miso_io_num = PIN_NUM_MISO,
#endif
    .mosi_io_num = PIN_NUM_MOSI,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz =  EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DMA_Line * sizeof(uint16_t), // RGB565 , 传输屏幕的1/10行的数据
  };
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
#if EXAMPLE_USE_SDCARD
  esp_vfs_fat_sdmmc_mount_config_t mount_config =
  {
    .format_if_mount_failed = false,     // If mount fails, create partition table and format SD card
    .max_files = 5,                     // Max number of open files
    .allocation_unit_size = 512,         // Similar to sector size
  };
  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = PIN_NUM_SDCS;
  slot_config.host_id = LCD_HOST;
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = LCD_HOST;
  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_fat_sdspi_mount(SDlist, &host, &slot_config, &mount_config, &card)); // Mount SD card
  if(card != NULL)
  {
    sdmmc_card_print_info(stdout, card); // Print card information
    printf("Size: %.2f(GB)\n", (float)(card->csd.capacity) / 2048 / 1024); // in GB
  }
#endif
#if (EXAMPLE_USE_Disp == 1)
  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config =
  {
    .dc_gpio_num = PIN_NUM_DC,
    .cs_gpio_num = PIN_NUM_CS,
    .pclk_hz = 20 * 1000 * 1000,
    .lcd_cmd_bits = 8,
    .lcd_param_bits = 8,
    .spi_mode = 0,
    .trans_queue_depth = 10,
    .on_color_trans_done = example_notify_lvgl_flush_ready,
    .user_ctx = &disp_drv,
  };
  sh8601_vendor_config_t vendor_config =
  {
    .init_cmds = lcd_init_cmds,
    .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

  esp_lcd_panel_handle_t panel_handle = NULL;
  const esp_lcd_panel_dev_config_t panel_config =
  {
    .reset_gpio_num = PIN_NUM_RST,
    .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
    .bits_per_pixel = 16,
    .vendor_config = &vendor_config,
    .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_sh8601(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  //ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
  I2C_master_Init();
#if EXAMPLE_USE_TOUCH
  touch_Init();
#endif
  lv_init();
  lv_color_t *buf1 = heap_caps_malloc(EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DMA_Line * sizeof(lv_color_t), MALLOC_CAP_DMA);
  assert(buf1);
  lv_color_t *buf2 = heap_caps_malloc(EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DMA_Line * sizeof(lv_color_t), MALLOC_CAP_DMA);
  assert(buf2);
  lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DMA_Line);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = example_lvgl_flush_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = panel_handle;
  lv_disp_drv_register(&disp_drv);
#if EXAMPLE_USE_TOUCH
  static lv_indev_drv_t indev_drv;    // Input device driver (Touch)
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = example_lvgl_touch_cb;
  lv_indev_drv_register(&indev_drv);
#endif
  const esp_timer_create_args_t lvgl_tick_timer_args =
  {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));
  lvgl_mux = xSemaphoreCreateMutex();
  assert(lvgl_mux);
  xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, NULL, EXAMPLE_LVGL_TASK_PRIORITY, NULL);
  if (example_lvgl_lock(-1))
  {
    lv_demo_widgets();      /* A widgets example */
    //lv_demo_music();          /* A modern, smartphone-like music player demo. */
    // lv_demo_stress();      /* A stress test for LVGL. */
    //lv_demo_benchmark();    /* A demo to measure the performance of LVGL or to compare different settings. */
    // Release the mutex
    example_lvgl_unlock();
  }
#endif
}
/* Write data
path: file path
data: data to write
*/
#if EXAMPLE_USE_SDCARD
esp_err_t s_example_write_file(const char *path, char *data)
{
  esp_err_t err;
  if(card == NULL)
  {
    return ESP_ERR_NOT_FOUND;
  }
  err = sdmmc_get_status(card); // First check if the SD card is present
  if(err != ESP_OK)
  {
    return err;
  }
  FILE *f = fopen(path, "w"); // Open the file path
  if(f == NULL)
  {
    printf("Error: Write path incorrect\n");
    return ESP_ERR_NOT_FOUND;
  }
  fprintf(f, data); // Write data
  fclose(f);
  return ESP_OK;
}
float sd_card_get_value(void)
{
  if(card != NULL)
  {
    return (float)(card->csd.capacity) / 2048 / 1024; // GB
  }
  else
    return 0;
}
/*
Read data
path: path
*/
esp_err_t s_example_read_file(const char *path,char *pxbuf,uint32_t *outLen)
{
  esp_err_t err;
  if(card == NULL)
  {
    printf("path:card == NULL\n");
    return ESP_ERR_NOT_FOUND;
  }
  err = sdmmc_get_status(card); // Check if the SD card is present
  if(err != ESP_OK)
  {
    printf("path:card == NO\n");
    return err;
  }
  FILE *f = fopen(path, "rb");
  if (f == NULL)
  {
    printf("path:Read Wrong path\n");
    return ESP_ERR_NOT_FOUND;
  }
  fseek(f, 0, SEEK_END);     // Move the pointer to the end
  uint32_t unlen = ftell(f);
  //fgets(pxbuf, unlen, f); // Read text
  fseek(f, 0, SEEK_SET); // Move the pointer to the beginning
  //uint32_t poutLen = fread((void *)pxbuf,sizeof(char),unlen,f);
  //printf("pxlen: %ld\n",unlen);
  fread((void *)pxbuf,sizeof(char),unlen,f);
  if(outLen != NULL)
  *outLen = unlen;
  fclose(f);
  return ESP_OK;
}
#endif