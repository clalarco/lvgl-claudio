#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_NONE
};

// Native global variables

extern const char *get_var_time_day();
extern void set_var_time_day(const char *value);
extern const char *get_var_time_dow();
extern void set_var_time_dow(const char *value);
extern const char *get_var_time_month();
extern void set_var_time_month(const char *value);
extern const char *get_var_time_year();
extern void set_var_time_year(const char *value);
extern const char *get_var_time_time();
extern void set_var_time_time(const char *value);
extern bool get_var_time_colon_visible();
extern void set_var_time_colon_visible(bool value);
extern const char *get_var_time_hour();
extern void set_var_time_hour(const char *value);
extern const char *get_var_time_minutes();
extern void set_var_time_minutes(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/