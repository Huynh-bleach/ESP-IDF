#include <stdio.h>
#include "use_gptimer.h"
#include <stdint.h>

#include <unistd.h>
#include "esp_timer.h"

#include "button_and_pwm.h"

adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_handle_t adc2_handle;

adc_channel_t pin_light = ADC_CHANNEL_5;
adc_channel_t pin_soil = ADC_CHANNEL_9;

static bool IRAM_ATTR example_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
    example_queue_timerid_t *queue_timer_cb = (example_queue_timerid_t *)user_data;

    int adc_value;
    if (queue_timer_cb->timer_id == 1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, pin_light, &adc_value));
    }
    else
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, pin_soil, &adc_value));
    }

    // Retrieve count value and send to queue
    example_queue_element_t ele = {
        .event_count = edata->count_value,
        .timer_id = queue_timer_cb->timer_id, // Set the timer_id based on which timer called the ISR
        .value_cb = adc_value,

    };
    xQueueSendFromISR(queue_timer_cb->queue_timer, &ele, &high_task_awoken);
    // return whether we need to yield at the end of ISR
    return (high_task_awoken == pdTRUE);
}

void queue_processing_task(void *param)
{
    QueueHandle_t queue = (QueueHandle_t)param;
    example_queue_element_t ele;

    while (1)
    {
        if (xQueueReceive(queue, &ele, pdMS_TO_TICKS(7000)))
        {
            if (ele.timer_id == 1)
            {
                ESP_LOGI(TAG_GPTIMER, "Timer 1 reloaded, count=%llu", ele.event_count);
                ESP_LOGI(TagReadADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, pin_light, ele.value_cb);
            }
            else if (ele.timer_id == 2)
            {
                ESP_LOGI(TAG_GPTIMER, "Timer 2 reloaded, count=%llu", ele.event_count);
                ESP_LOGI(TagReadADC, "ADC%d Channel[%d] Raw Data: %d\n\n", ADC_UNIT_2 + 1, pin_soil, ele.value_cb);
            }
        }
        else
        {
            ESP_LOGW(TAG_GPTIMER, "Missed one count event");
        }
    }
}

void app_main()
{
    example_queue_timerid_t queue_timerid1;
    example_queue_timerid_t queue_timerid2;

    adc_oneshot_config(ADC_UNIT_1, &adc1_handle, pin_light, NULL, NULL);

    adc_oneshot_config(ADC_UNIT_2, &adc2_handle, pin_soil, NULL, NULL);

    gptimer_handle_t gptimer1;
    create_timer(&gptimer1, 1, &queue_timerid1, example_alarm_cb, 1000000); // Pass the address of queue_timerid1

    gptimer_handle_t gptimer2;
    create_timer(&gptimer2, 2, &queue_timerid2, example_alarm_cb, 4000000); // Pass the address of queue_timerid2

    // Create tasks for processing queues
    xTaskCreate(queue_processing_task, "queue_processing_task_timer1", 4048, (void *)queue_timerid1.queue_timer, 10, NULL);
    xTaskCreate(queue_processing_task, "queue_processing_task_timer2", 4048, (void *)queue_timerid2.queue_timer, 10, NULL);

    for (int i = 0; i < 5; ++i)
    {
        ESP_ERROR_CHECK(esp_timer_dump(stdout));
        usleep(2000000);
    }

    change_count_callback(&gptimer1, &queue_timerid1, example_alarm_cb, 3000000);
    change_count_callback(&gptimer2, &queue_timerid2, example_alarm_cb, 6000000);

    for (int i = 0; i < 9; ++i)
    {
        ESP_ERROR_CHECK(esp_timer_dump(stdout));
        usleep(2000000);
    }

    delete_timer(&gptimer1, &queue_timerid1);
    delete_timer(&gptimer2, &queue_timerid2);
}
