#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: dark_calendar
//

void init_style_dark_calendar_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff000000));
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_dark_calendar_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_dark_calendar_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_dark_calendar(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_dark_calendar_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_dark_calendar(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_dark_calendar_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: dark_tabs
//

void add_style_dark_tabs(lv_obj_t *obj) {
    (void)obj;
};

void remove_style_dark_tabs(lv_obj_t *obj) {
    (void)obj;
};

//
// Style: dark_tabview
//

void init_style_dark_tabview_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff000000));
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_dark_tabview_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_dark_tabview_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_dark_tabview(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_dark_tabview_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_dark_tabview(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_dark_tabview_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: dark_textarea
//

void add_style_dark_textarea(lv_obj_t *obj) {
    (void)obj;
};

void remove_style_dark_textarea(lv_obj_t *obj) {
    (void)obj;
};

//
// Style: dark_tab
//

void init_style_dark_tab_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffcccccc));
};

lv_style_t *get_style_dark_tab_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_dark_tab_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_dark_tab(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_dark_tab_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_dark_tab(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_dark_tab_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_dark_calendar,
        add_style_dark_tabs,
        add_style_dark_tabview,
        add_style_dark_textarea,
        add_style_dark_tab,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_dark_calendar,
        remove_style_dark_tabs,
        remove_style_dark_tabview,
        remove_style_dark_textarea,
        remove_style_dark_tab,
    };
    remove_style_funcs[styleIndex](obj);
}

