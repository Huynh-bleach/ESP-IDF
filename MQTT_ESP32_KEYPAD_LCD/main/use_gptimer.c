#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gptimer.h"
#include "esp_log.h"

static const char *TAG_GPTIMER = "example";

typedef struct
{
    uint64_t event_count;
} example_queue_element_t;

static bool IRAM_ATTR example_alarm_cb_timer1(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t queue = (QueueHandle_t)user_data;
    
    
    // Retrieve count value and send to queue
    example_queue_element_t ele = {
        .event_count = edata->count_value};
    xQueueSendFromISR(queue, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    return (high_task_awoken == pdTRUE);
}

void app_main()
{
    example_queue_element_t ele_timer1;

    example_queue_element_t ele_timer2;

    QueueHandle_t queue_timer1 = xQueueCreate(10, sizeof(example_queue_element_t));
    if (!queue_timer1)
    {
        ESP_LOGE(TAG_GPTIMER, "Creating queue_timer1 failed");
        return;
    }

    QueueHandle_t queue_timer2 = xQueueCreate(10, sizeof(example_queue_element_t));
    if (!queue_timer2)
    {
        ESP_LOGE(TAG_GPTIMER, "Creating queue_timer2 failed");
        return;
    }

    ESP_LOGI(TAG_GPTIMER, "Create gptimer1 handle");
    gptimer_handle_t gptimer1 = NULL;
    gptimer_config_t timer1_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer1_config, &gptimer1));

     gptimer_event_callbacks_t cbs_timer1 = {
        .on_alarm = example_alarm_cb_timer1,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer1, &cbs_timer1, queue_timer1));

    ESP_LOGI(TAG_GPTIMER, "Enable timer");
    ESP_ERROR_CHECK(gptimer_enable(gptimer1));

    ESP_LOGI(TAG_GPTIMER, "Start timer, stop it at alarm event");
    gptimer_alarm_config_t alarm_config1 = {
        .reload_count = 0,
        .alarm_count = 1000000, // period = 1s
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer1, &alarm_config1));
    ESP_ERROR_CHECK(gptimer_start(gptimer1));
   
    while (1) {
        if (xQueueReceive(queue_timer1, &ele_timer1, pdMS_TO_TICKS(2000))) {
            ESP_LOGI(TAG_GPTIMER, "Timer reloaded, count=%llu", ele_timer1.event_count);
            
        } else {
            ESP_LOGW(TAG_GPTIMER, "Missed one count event");
        }
    }




    ESP_LOGI(TAG_GPTIMER, "Create gptimer2 handle");
    gptimer_handle_t gptimer2 = NULL;
    gptimer_config_t timer2_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer2_config, &gptimer2));


}