#ifndef __APP_SDCARD_H__
#define __APP_SDCARD_H__

#if BOARD_USE_SDCARD

void s_app_init();

/*
Write data
path: path
*/
esp_err_t s_app_write_file(const char *path, char *data);

/*
Read data
path: path
*/
esp_err_t s_app_read_file(const char *path,char *pxbuf,uint32_t *outLen)

#endif
#endif