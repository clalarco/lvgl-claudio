// Board settings.
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "esp_lcd_sh8601.h"
#include <stdint.h>

#define BOARD_USE_DISPLAY    1
#define BOARD_USE_TOUCH      1
#define BOARD_USE_SDCARD     0

#define LCD_HOST  SPI2_HOST
#define PIN_NUM_MOSI 4
#define PIN_NUM_CLK  5
#define PIN_NUM_CS   7
#define PIN_NUM_RST  14
#define PIN_NUM_DC   6
#define PIN_NUM_MISO 19
#define PIN_NUM_SDCS 20

#define BOARD_ROTATE 1        //旋转90
#define BOARD_NORMAL 0        //正常显示
#define BOARD_DIRECTION BOARD_NORMAL

#if (BOARD_DIRECTION == BOARD_NORMAL)
  #define BOARD_LCD_H_RES 170   //宽度 水平分辨率
  #define BOARD_LCD_V_RES 320   //高度 竖直分辨率
#elif (BOARD_DIRECTION == BOARD_ROTATE)
  #define BOARD_LCD_H_RES 320   //宽度 水平分辨率
  #define BOARD_LCD_V_RES 170   //高度 竖直分辨率
#endif

#define BOARD_LCD_DMA_Line (BOARD_LCD_V_RES / 2)

static const sh8601_lcd_init_cmd_t lcd_init_cmds[] =
{
  #if (BOARD_DIRECTION == BOARD_NORMAL)
  //{0x36, (uint8_t []){0x00}, 1, 0},
  #elif (BOARD_DIRECTION == BOARD_ROTATE)
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

void board_configure();

#endif