/*
 * MIT License
 *
 * Copyright (c) 2021 wolffshots
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * A note on the copyright line: https://www.copyright.gov/title17/92chap4.html#408
 */

#include "timer.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

static const char *TAG = CONFIG_TIMER_LOG_TAG;
/**
* set up a periodic or oneshot timer
* @param timer a locally defined timer passed through to this function
* @param timer_callback a callback function with the params (void *arg) which is called when the alarm triggers
* @param periodic a boolean checking whether the timer should be set up to be reset when it is triggered
* @param period_in_seconds the period between start and trigger
* @param timer_name a name for the timer for the sake of debugging
*/
void general_timer_init(esp_timer_handle_t timer, void(timer_callback)(void *arg), bool periodic, int period_in_seconds, char *timer_name)
{
    const esp_timer_create_args_t timer_args = {
        .callback = timer_callback,                         // function to call when timer is reached
        .name = timer_name};                                // name is optional, but may help identify the timer when debugging
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer)); // create the timer but don't start it
    if (periodic)                                           // check if periodic flag
    {
        ESP_ERROR_CHECK(esp_timer_start_periodic(timer, 1000000 * period_in_seconds)); // start the timer periodically for a given period
    }
    else
    {
        ESP_ERROR_CHECK(esp_timer_start_once(timer, 1000000 * period_in_seconds)); // start the timer once for a given period
    }
    ESP_LOGI(TAG, "started timer: %s, time since boot: %lld us", timer_name, esp_timer_get_time()); // log out that the timers have started
}
/**
* deinit a timer
* @param timer a locally defined timer passed through to this function
*/
void general_timer_deinit(esp_timer_handle_t timer)
{
    ESP_ERROR_CHECK(esp_timer_stop(timer));     // stop timer
    ESP_ERROR_CHECK(esp_timer_delete(timer));   // free up resources
    ESP_LOGI(TAG, "stopped and deleted timer"); // log out that the timer has stopped
}
