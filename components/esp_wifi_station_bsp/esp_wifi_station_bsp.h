#ifndef ESP_WIFI_STA_BSP_H
#define ESP_WIFI_STA_BSP_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_wifi.h"  //WIFI

void espWifi_Station_Init(const char* ssid, const char* password);


#endif