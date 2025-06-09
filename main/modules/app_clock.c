#include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/event_groups.h"
// #include "freertos/task.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "ui/vars.h"

static const char *TAG = "APP_CLOCK";

static time_t now;
static struct tm timeinfo;

// Array of month names
static const char *month_names[] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};

// Array of day names
static const char *day_names[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                                  "Thursday", "Friday", "Saturday"};

// Initialize the clock with a specific time
void clock_init() {
  now = 1749442904L; // Monday, June 9, 2025 4:21:44 AM
  localtime_r(&now, &timeinfo);
  struct timeval tv = {.tv_sec = now, .tv_usec = 0};

  settimeofday(&tv, NULL);
  ESP_LOGI(TAG, "Clock initialized to %02d:%02d:%02d %d %s %d", timeinfo.tm_hour, timeinfo.tm_min,
           timeinfo.tm_sec, timeinfo.tm_mday, month_names[timeinfo.tm_mon - 1], timeinfo.tm_year);
}

// Get current time in HH:MM format with colon ticking
static char buffer[64];
const char *get_var_time_time() {
  char colon_char = now % 2 ? ' ' : ':';
  snprintf(buffer, 64, "%02d%c%02d", timeinfo.tm_hour, colon_char, timeinfo.tm_min);
  return buffer;
}

void set_var_time_time(const char *value) {}

// Get current day of week in words
const char *get_var_time_dow(void) {
  // struct tm timeinfo;
  // time_t current_time;
  // time(&current_time);
  // localtime_r(&current_time, &timeinfo);
  return day_names[timeinfo.tm_wday];
}

void set_var_time_dow(const char *value) {}

// Get current day
const char *get_var_time_day(void) {
  // struct tm timeinfo;
  // time_t now;
  // time(&now);
  // localtime_r(&now, &timeinfo);
  static char day_buffer[4];
  snprintf(day_buffer, sizeof(day_buffer), "%d", timeinfo.tm_mday);
  return day_buffer;
}

void set_var_time_day(const char *value) {}


// Get current month in words
const char *get_var_time_month(void) {
  struct tm timeinfo;
  time_t now;
  time(&now);
  localtime_r(&now, &timeinfo);
  return month_names[timeinfo.tm_mon];
}

void set_var_time_month(const char *value) {}

// Get current year
const char year_buffer[16];
const char *get_var_time_year(void) {
  struct tm timeinfo;
  time_t now;
  time(&now);
  localtime_r(&now, &timeinfo);
  int year = timeinfo.tm_year + 1900; // Convert from years since 1900
  snprintf((char*)year_buffer, sizeof(year_buffer), "%04d", year);
  return year_buffer;
}

void set_var_time_year(const char *value) {}

// Clock task that updates time every second
void clock_task(void *pvParameters) {
  while (1) {
    //
    // No need to manually tick the clock as we're using system time
    time(&now);
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
  }
}

// Start the clock task
void clock_start(void) {
  xTaskCreate(clock_task, "clock_task", 2048, NULL, 5, NULL);
  ESP_LOGI(TAG, "Clock task started");
}
