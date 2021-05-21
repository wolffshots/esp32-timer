#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include "esp_timer.h"

void general_timer_init(esp_timer_handle_t timer, void(timer_callback)(void *arg), bool periodic, int period_in_seconds, char *timer_name);
void general_timer_deinit(esp_timer_handle_t timer);

#endif // TIMER_H