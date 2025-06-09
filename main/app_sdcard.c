/* Write data
path: file path
data: data to write
*/
#include "app_sdcard.h"
#include "board_config.h"

#if BOARD_USE_SDCARD

#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

sdmmc_card_t *card = NULL; // Handle
#define SDlist "/sd_card"  // Directory, acts like a standard
esp_err_t s_app_write_file(const char *path, char *data);
esp_err_t s_app_read_file(const char *path,char *pxbuf,uint32_t *outLen);
float sd_card_get_value(void);

void s_app_init() {
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

}

esp_err_t s_app_write_file(const char *path, char *data)
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
  fprintf(f, "%s", data); // Write data
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
esp_err_t s_app_read_file(const char *path,char *pxbuf,uint32_t *outLen)
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
  if(outLen != NULL) {
      *outLen = unlen;
  }
  fclose(f);
  return ESP_OK;
}
#endif