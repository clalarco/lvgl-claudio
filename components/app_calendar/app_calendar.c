#include <sys/time.h>
#include <time.h>
#include "ui/screens.h"

void app_calendar_update_date() {
    static struct tm timeinfo;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const time_t now = tv.tv_sec;
    localtime_r(&now, &timeinfo);

    lv_calendar_set_today_date(objects.calendar, timeinfo.tm_year+1900, timeinfo.tm_mon, timeinfo.tm_mday);
    lv_calendar_set_showed_date(objects.calendar, timeinfo.tm_year+1900, timeinfo.tm_mon);

}