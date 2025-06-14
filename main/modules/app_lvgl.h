#ifndef __APP_LVGL_H__
#define __APP_LVGL_H__

#include "freertos/idf_additions.h"
#define APP_LVGL_TICK_PERIOD_MS    20
#define APP_LVGL_TASK_MAX_DELAY_MS 1500
#define APP_LVGL_TASK_MIN_DELAY_MS 1
#define APP_LVGL_TASK_STACK_SIZE   (4 * 1024)
#define APP_LVGL_TASK_PRIORITY     2

/* Function pointer type definitions */
typedef void (*ui_tick_callback_t)(void);
typedef void (*ui_init_callback_t)(void);

/* Public function declarations */
void app_lvgl_set_tick_callback(ui_tick_callback_t callback);
void app_lvgl_set_init_callback(ui_init_callback_t callback);
void app_lvgl_configure(void);

#endif
