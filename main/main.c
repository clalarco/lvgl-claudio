#include "app_lvgl.h"
#include "board_config.h"
#include "ui/ui.h"

void app_main(void) {
board_configure();

#if (BOARD_USE_DISPLAY == 1 || BOARD_USE_TOUCH)
  app_lvgl_set_init_callback(ui_init);
  app_lvgl_set_tick_callback(ui_tick);
  app_lvgl_configure();
#endif
}
