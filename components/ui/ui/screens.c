#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 172, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.time = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_love_days_64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffff3ae), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // DOW
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.dow = obj;
            lv_obj_set_pos(obj, 0, 142);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_style_text_font(obj, &ui_font_love_days_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6d82e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // Day
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.day = obj;
            lv_obj_set_pos(obj, 0, 160);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_love_days_48, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6d82e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // Month
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.month = obj;
            lv_obj_set_pos(obj, 0, 217);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_love_days_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6d82e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // year
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.year = obj;
            lv_obj_set_pos(obj, 0, 264);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_love_days_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6d82e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "time_year");
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    {
        const char *new_val = get_var_time_time();
        const char *cur_val = lv_label_get_text(objects.time);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.time;
            lv_label_set_text(objects.time, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_time_dow();
        const char *cur_val = lv_label_get_text(objects.dow);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.dow;
            lv_label_set_text(objects.dow, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_time_day();
        const char *cur_val = lv_label_get_text(objects.day);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.day;
            lv_label_set_text(objects.day, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_time_month();
        const char *cur_val = lv_label_get_text(objects.month);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.month;
            lv_label_set_text(objects.month, new_val);
            tick_value_change_obj = NULL;
        }
    }
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}
