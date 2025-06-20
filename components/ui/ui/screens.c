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

void create_screen_main_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 172, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_SCROLLED);
    {
        lv_obj_t *parent_obj = obj;
        {
            // panel_date_time
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.panel_date_time = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_add_event_cb(obj, action_switch_to_calendar_page, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_SCROLLED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Hour
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.hour = obj;
                    lv_obj_set_pos(obj, 15, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_CENTER);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_64, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffff3ae), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // DOW
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.dow = obj;
                    lv_obj_set_pos(obj, 0, -40);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_32, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffd52ef6), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // year
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.year = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_32, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6d82e), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // Day
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.day = obj;
                    lv_obj_set_pos(obj, 0, 20);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_96, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6d82e), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // Month
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.month = obj;
                    lv_obj_set_pos(obj, 0, -50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_32, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff2ef63a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // time_colon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.time_colon = obj;
                    lv_obj_set_pos(obj, 66, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_CENTER);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_64, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffff3ae), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, ":");
                }
                {
                    // Minutes
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.minutes = obj;
                    lv_obj_set_pos(obj, 81, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_CENTER);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER);
                    lv_obj_set_style_text_font(obj, &ui_font_love_days_64, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffff3ae), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
    }
    
    tick_screen_main_page();
}

void tick_screen_main_page() {
    {
        const char *new_val = get_var_time_hour();
        const char *cur_val = lv_label_get_text(objects.hour);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.hour;
            lv_label_set_text(objects.hour, new_val);
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
        const char *new_val = get_var_time_year();
        const char *cur_val = lv_label_get_text(objects.year);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.year;
            lv_label_set_text(objects.year, new_val);
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
    {
        bool new_val = get_var_time_colon_visible();
        bool cur_val = lv_obj_has_flag(objects.time_colon, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.time_colon;
            if (new_val) lv_obj_add_flag(objects.time_colon, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.time_colon, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_time_minutes();
        const char *cur_val = lv_label_get_text(objects.minutes);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.minutes;
            lv_label_set_text(objects.minutes, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_calendar_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.calendar_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 172, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            // calendar
            lv_obj_t *obj = lv_calendar_create(parent_obj);
            objects.calendar = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 172, 270);
            lv_calendar_header_arrow_create(obj);
            lv_calendar_set_today_date(obj, 2022, 11, 1);
            lv_calendar_set_showed_date(obj, 2022, 11);
            add_style_dark_calendar(obj);
        }
        {
            // Show Clock
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.show_clock = obj;
            lv_obj_set_pos(obj, 0, 270);
            lv_obj_set_size(obj, 107, 50);
            lv_obj_add_event_cb(obj, action_switch_to_time_page, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 25, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_clock);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 256, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // Show Settings
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.show_settings = obj;
            lv_obj_set_pos(obj, 107, 270);
            lv_obj_set_size(obj, 65, 50);
            lv_obj_add_event_cb(obj, action_switch_to_settings_page, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 4, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_img_settings);
                }
            }
        }
    }
    
    tick_screen_calendar_page();
}

void tick_screen_calendar_page() {
}

void create_screen_settings_page() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_page = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 172, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 32);
            lv_obj_set_pos(obj, 0, 31);
            lv_obj_set_size(obj, 176, 289);
            add_style_dark_tabview(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // settings_wifi
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Wifi");
                    objects.settings_wifi = obj;
                    add_style_dark_tab(obj);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffcccccc), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffcccccc), LV_PART_MAIN | LV_STATE_FOCUSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, -4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Network Name (SSID)");
                        }
                        {
                            // Wifi SSID
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.wifi_ssid = obj;
                            lv_obj_set_pos(obj, -4, 16);
                            lv_obj_set_size(obj, 150, 36);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "Network");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            add_style_dark_textarea(obj);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, -4, 68);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Password");
                        }
                        {
                            // Wifi Password
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.wifi_password = obj;
                            lv_obj_set_pos(obj, -4, 84);
                            lv_obj_set_size(obj, 150, 36);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "Password");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, true);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Timezone");
                    objects.obj1 = obj;
                    add_style_dark_tab(obj);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_CHECKED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, -4, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "TimeZone Spec");
                        }
                        {
                            // timezone_spec
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.timezone_spec = obj;
                            lv_obj_set_pos(obj, -4, 16);
                            lv_obj_set_size(obj, 150, 36);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "Timezone");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 61, 8);
            lv_obj_set_size(obj, 111, 16);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Settings");
        }
        {
            // leave_settings
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.leave_settings = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 46, 31);
            lv_obj_add_event_cb(obj, action_switch_to_time_page, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "<");
                }
            }
        }
    }
    
    tick_screen_settings_page();
}

void tick_screen_settings_page() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main_page,
    tick_screen_calendar_page,
    tick_screen_settings_page,
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
    
    create_screen_main_page();
    create_screen_calendar_page();
    create_screen_settings_page();
}
