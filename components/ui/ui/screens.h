#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main_page;
    lv_obj_t *calendar_page;
    lv_obj_t *panel_date_time;
    lv_obj_t *hour;
    lv_obj_t *dow;
    lv_obj_t *year;
    lv_obj_t *day;
    lv_obj_t *month;
    lv_obj_t *time_colon;
    lv_obj_t *minutes;
    lv_obj_t *show_clock;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN_PAGE = 1,
    SCREEN_ID_CALENDAR_PAGE = 2,
};

void create_screen_main_page();
void tick_screen_main_page();

void create_screen_calendar_page();
void tick_screen_calendar_page();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/