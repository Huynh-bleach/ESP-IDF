#include "use_gptimer.h"

void create_timer(gptimer_handle_t *timer, uint8_t id_timer, example_queue_timerid_t *queue_timer_cb, gptimer_alarm_cb_t alarm_cb, int alarm_value)
{

    queue_timer_cb->queue_timer = xQueueCreate(10, sizeof(example_queue_element_t));
    if (!(queue_timer_cb->queue_timer))
    {
        ESP_LOGE(TAG_GPTIMER, "Creating queue gptimer %d failed", id_timer);
        return;
    }

    queue_timer_cb->timer_id = id_timer;

    ESP_LOGI(TAG_GPTIMER, "Create gptimer %d handle", id_timer);
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, timer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(*timer, &cbs, queue_timer_cb));

    ESP_ERROR_CHECK(gptimer_enable(*timer));

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = alarm_value, // period = 1s
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(*timer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_start(*timer));
}

void change_count_callback(gptimer_handle_t *timer, example_queue_timerid_t *queue_timer_cb, gptimer_alarm_cb_t alarm_cb, int alarm_value)
{
    ESP_LOGI(TAG_GPTIMER, "Stop timer");
    ESP_ERROR_CHECK(gptimer_stop(*timer));

    ESP_LOGI(TAG_GPTIMER, "Disable timer");
    ESP_ERROR_CHECK(gptimer_disable(*timer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = alarm_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(*timer, &cbs, queue_timer_cb));
    ESP_LOGI(TAG_GPTIMER, "Enable timer");
    ESP_ERROR_CHECK(gptimer_enable(*timer));

    ESP_LOGI(TAG_GPTIMER, "Start timer, update alarm value: %d", alarm_value);
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = alarm_value,
        .flags.auto_reload_on_alarm = true,
    };

    ESP_ERROR_CHECK(gptimer_set_alarm_action(*timer, &alarm_config));
    ESP_ERROR_CHECK(gptimer_start(*timer));
}
void delete_timer(gptimer_handle_t *timer, example_queue_timerid_t *queue_timer_cb){

    ESP_LOGI(TAG_GPTIMER, "Stop timer");
    ESP_ERROR_CHECK(gptimer_stop(*timer));
    ESP_LOGI(TAG_GPTIMER, "Disable timer");
    ESP_ERROR_CHECK(gptimer_disable(*timer));
    ESP_LOGI(TAG_GPTIMER, "Delete timer");
    ESP_ERROR_CHECK(gptimer_del_timer(*timer));

    vQueueDelete(queue_timer_cb->queue_timer);

}
