#include "driver/spi_common.h"
#include "board_config.h"

void board_configure() {

  spi_bus_config_t buscfg = {
#if BOARD_USE_SDCARD
      .miso_io_num = PIN_NUM_MISO,
#endif
      .mosi_io_num = PIN_NUM_MOSI,
      .sclk_io_num = PIN_NUM_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = BOARD_LCD_H_RES * BOARD_LCD_DMA_Line *
                         sizeof(uint16_t), // RGB565 , 传输屏幕的1/10行的数据
  };
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
#if BOARD_USE_SDCARD
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed =
          false, // If mount fails, create partition table and format SD card
      .max_files = 5,              // Max number of open files
      .allocation_unit_size = 512, // Similar to sector size
  };
  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = PIN_NUM_SDCS;
  slot_config.host_id = LCD_HOST;
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = LCD_HOST;
  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_fat_sdspi_mount(
      SDlist, &host, &slot_config, &mount_config, &card)); // Mount SD card
  if (card != NULL) {
    sdmmc_card_print_info(stdout, card); // Print card information
    printf("Size: %.2f(GB)\n",
           (float)(card->csd.capacity) / 2048 / 1024); // in GB
  }
#endif
}