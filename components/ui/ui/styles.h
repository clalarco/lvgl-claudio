#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: dark_calendar
lv_style_t *get_style_dark_calendar_MAIN_DEFAULT();
void add_style_dark_calendar(lv_obj_t *obj);
void remove_style_dark_calendar(lv_obj_t *obj);

// Style: dark_tabs
void add_style_dark_tabs(lv_obj_t *obj);
void remove_style_dark_tabs(lv_obj_t *obj);

// Style: dark_tabview
lv_style_t *get_style_dark_tabview_MAIN_DEFAULT();
void add_style_dark_tabview(lv_obj_t *obj);
void remove_style_dark_tabview(lv_obj_t *obj);

// Style: dark_textarea
void add_style_dark_textarea(lv_obj_t *obj);
void remove_style_dark_textarea(lv_obj_t *obj);

// Style: dark_tab
lv_style_t *get_style_dark_tab_MAIN_DEFAULT();
void add_style_dark_tab(lv_obj_t *obj);
void remove_style_dark_tab(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/