#ifndef __APP_CLOCK_H__
#define __APP_CLOCK_H__

#include <stddef.h>

// Initialize the clock with a specific time
void clock_init(int hour, int minute, int second, int day, int month, int year);

// Get current time in HH:MM format with colon ticking
void clock_get_time_str(char *buffer, size_t buffer_size);

// Get current day of week in words
const char* clock_get_day_str(void);

// Get current month in words
const char* clock_get_month_str(void);

// Get current year
int clock_get_year(void);

// Start the clock task
void clock_start(void);

#endif // __APP_CLOCK_H__