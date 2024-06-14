#ifndef __USE_GPTIMER_H
#define __USE_GPTIMER_H

#include "driver/gptimer.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"



#include <stdint.h>

static const char *TAG_GPTIMER = "USE_GPTIMER";

typedef struct {
    uint64_t event_count;
    uint8_t timer_id; 
    int value_cb; 
} example_queue_element_t;

typedef struct {
    QueueHandle_t queue_timer;
    uint8_t timer_id;
} example_queue_timerid_t;


void create_timer(gptimer_handle_t *timer, uint8_t id_timer,example_queue_timerid_t *queue_timer_cb, gptimer_alarm_cb_t alarm_cb, int alarm_value);

void change_count_callback(gptimer_handle_t *timer, example_queue_timerid_t *queue_timer_cb, gptimer_alarm_cb_t alarm_cb, int alarm_value);

void delete_timer(gptimer_handle_t *timer, example_queue_timerid_t *queue_timer_cb);

#endif