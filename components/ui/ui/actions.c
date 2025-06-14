#include "actions.h"
#include "ui.h"
#include "screens.h"

// static void calendar_event_cb(lv_event_t * e);

void action_switch_to_calendar_page(lv_event_t * e) {
    loadScreen(SCREEN_ID_CALENDAR_PAGE);
}

void action_switch_to_time_page(lv_event_t * e) {
    loadScreen(SCREEN_ID_MAIN_PAGE);
}


// static lv_obj_t * calendar;

void action_show_calendar(lv_event_t * e)
{
    return;
    // lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t * ta = lv_event_get_target(e);

    // if(code == LV_EVENT_FOCUSED) {
    //     if(lv_indev_get_type(lv_indev_get_act()) == LV_INDEV_TYPE_POINTER) {
    //         if(calendar == NULL) {
    //             lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    //             calendar = lv_calendar_create(lv_layer_top());
    //             lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_50, 0);
    //             lv_obj_set_style_bg_color(lv_layer_top(), lv_palette_main(LV_PALETTE_GREY), 0);
    //             lv_obj_set_size(calendar, 180, 200);
    //             lv_calendar_set_showed_date(calendar, 1990, 01);
    //             lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 30);
    //             lv_obj_add_event_cb(calendar, calendar_event_cb, LV_EVENT_ALL, ta);

    //             lv_calendar_header_dropdown_create(calendar);
    //         }
    //     }
    // }
}

// static void calendar_event_cb(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t * ta = lv_event_get_user_data(e);
//     lv_obj_t * obj = lv_event_get_current_target(e);
//     if(code == LV_EVENT_VALUE_CHANGED) {
//         lv_calendar_date_t d;
//         lv_calendar_get_pressed_date(obj, &d);
//         char buf[32];
//         lv_snprintf(buf, sizeof(buf), "%02d.%02d.%d", d.day, d.month, d.year);
//         lv_textarea_set_text(ta, buf);

//         lv_obj_del(calendar);
//         calendar = NULL;
//         lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
//         lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
//     }
// }