#include "esp_log.h"
#include "esp_netif_sntp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "misc/lv_area.h"
#include "ui/screens.h"
#include "ui/vars.h"

#include "app_clock.h"

#define TAG "APP_CLOCK"

static time_t now;
static struct tm timeinfo;

// Array of month names
static const char *month_names[] = {
    "Enero", "Febrero", "Marzo",      "Abril",   "Mayo",      "Junio",
    "Julio", "Agosto",  "Septiembre", "Octubre", "Noviembre", "Diciembre"};

// Array of day names
static const char *day_names[] = {"Domingo", "Lunes",   "Martes", "Miércoles",
                                  "Jueves",  "Viernes", "Sábado"};

// Initialize the clock with a specific time
void clock_init() {
  update_time();
  ESP_LOGI(TAG, "Clock initialized to %02d:%02d:%02d %d %s %d",
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday,
           month_names[timeinfo.tm_mon], timeinfo.tm_year + 1900);
  xTaskCreate(clock_task, "clock_task", 2048, NULL, 5, NULL);
  ESP_LOGI(TAG, "Clock task started");
}

void update_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  now = tv.tv_sec;
  localtime_r(&now, &timeinfo);
}

// Get current time in HH:MM format with colon ticking
static char buffer[64];
const char *get_var_time_time() {
  char colon_char = timeinfo.tm_sec % 2 ? ' ' : ':';
  snprintf(buffer, 64, "%02d%c%02d", timeinfo.tm_hour, colon_char,
           timeinfo.tm_min);
  return buffer;
}

// Get current time in HH:MM format with colon ticking
const char *get_var_time_hour() {
  static char buffer[16];
  snprintf(buffer, sizeof(buffer), "%02d", timeinfo.tm_hour);
  return buffer;
}

void set_var_time_hour(const char *value) {}

const char *get_var_time_minutes() {
  static char buffer[16];
  snprintf(buffer, sizeof(buffer), "%02d", timeinfo.tm_min);
  return buffer;
}

void set_var_time_minutes(const char *value) {}

bool get_var_time_colon_visible() { return timeinfo.tm_sec % 2 > 0; }

void set_var_time_colon_visible(bool value) {}

const char *get_var_time_() {
  char colon_char = timeinfo.tm_sec % 2 ? ' ' : ':';
  snprintf(buffer, 64, "%02d%c%02d", timeinfo.tm_hour, colon_char,
           timeinfo.tm_min);
  return buffer;
}

void set_var_time_time(const char *value) {}

// Get current day of week in words
const char *get_var_time_dow(void) { return day_names[timeinfo.tm_wday]; }

void set_var_time_dow(const char *value) {}

// Get current day
const char *get_var_time_day(void) {
  static char day_buffer[4];
  snprintf(day_buffer, sizeof(day_buffer), "%d", timeinfo.tm_mday);
  return day_buffer;
}

void set_var_time_day(const char *value) {}

// Get current month in words
const char *get_var_time_month(void) { return month_names[timeinfo.tm_mon]; }

void set_var_time_month(const char *value) {}

// Get current year
const char *get_var_time_year(void) {
  static char year_buffer[16];
  int year = timeinfo.tm_year + 1900; // Convert from years since 1900
  snprintf((char *)year_buffer, sizeof(year_buffer), "%04d", year);
  return year_buffer;
}

void set_var_time_year(const char *value) {}

// Clock task that updates time every second
void clock_task(void *pvParameters) {
  while (1) {
    // No need to manually tick the clock as we're using system time
    update_time();
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
  }
}
