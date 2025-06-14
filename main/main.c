#include "board_config.h"
#include "esp_log.h"
#include "esp_wifi_station_bsp.h"
#include "ui/ui.h"
#include "esp_sntp.h"
#include "esp_netif_sntp.h"

#include "modules/app_lvgl.h"
#include "app_clock.h"
#include "app_calendar.h"

#define TAG "MAIN"

#define WIFI_SSID       "Casa Ele"
#define WIFI_PASSWORD   "Pepatancia123."

void app_sntp_init();
void app_calendar_init();

void app_main(void) {
  board_configure();

  espWifi_Station_Init(WIFI_SSID, WIFI_PASSWORD);

  app_sntp_init();

  clock_init();

  // Configuring lvgl module.
#if (BOARD_USE_DISPLAY == 1 || BOARD_USE_TOUCH)
  app_lvgl_set_init_callback(ui_init);
  app_lvgl_set_tick_callback(ui_tick);
  app_lvgl_configure();
#endif

  app_calendar_update_date();

}

void app_sntp_init() {
    struct tm timeinfo;

    // Set timezone to Chile (Santiago)
    setenv("TZ",  "CLT4,CLST3,M9.1.0/0,M4.1.0/0", 1);
    tzset();

    // Log the current timezone
    char tz[32];
    strftime(tz, sizeof(tz), "%Z %z", &timeinfo);
    ESP_LOGI(TAG, "Setting timezone to: %s", getenv("TZ"));

    // Initialize SNTP
    esp_sntp_config_t sntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&sntp_config);

    // Wait for SNTP sync
    int retry = 0;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) != ESP_OK && retry < 5) {
        ESP_LOGI(TAG, "Waiting for SNTP sync...");
        retry++;
    }


    // Log the time with timezone information
    strftime(tz, sizeof(tz), "%Z %z", &timeinfo);
}
