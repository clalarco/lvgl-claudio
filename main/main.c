#include "board_config.h"
#include "ui/ui.h"

#include "modules/app_lvgl.h"
#include "modules/app_clock.h"

void app_main(void) {
  board_configure();

  // Initialize and start the clock
  clock_init(12, 0, 0, 1, 1, 2024); // Initialize to noon on January 1, 2024
  clock_start();

  // Configuring lvgl module.
#if (BOARD_USE_DISPLAY == 1 || BOARD_USE_TOUCH)
  app_lvgl_set_init_callback(ui_init);
  app_lvgl_set_tick_callback(ui_tick);
  app_lvgl_configure();
#endif
}
