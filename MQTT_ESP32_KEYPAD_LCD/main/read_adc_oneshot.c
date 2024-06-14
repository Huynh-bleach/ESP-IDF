#include "button_and_pwm.h"

static int adc_raw[2][10];
static int voltage[2][10];

void app_main(void)
{
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_config(ADC_UNIT_1, &adc1_handle, ADC_CHANNEL_5, NULL, NULL);

    adc_cali_handle_t adc1_cali_handle = NULL;
    bool do_calibration;
    adc_converts_voltage_config(ADC_UNIT_1, &adc1_cali_handle, ADC_ATTEN_DB_11, &do_calibration);

    adc_oneshot_unit_handle_t adc2_handle;
    adc_oneshot_config(ADC_UNIT_2, &adc2_handle, ADC_CHANNEL_9, NULL, NULL);

    adc_cali_handle_t adc2_cali_handle = NULL;
    bool do_calibration2;
    adc_converts_voltage_config(ADC_UNIT_2, &adc2_cali_handle, ADC_ATTEN_DB_11, &do_calibration2);

    while (1)
    {
        /* code */
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_5, &adc_raw[0][0]));
        ESP_LOGI(TagReadADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC_CHANNEL_5, adc_raw[0][0]);
        if (do_calibration)
        {
            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw[0][0], &voltage[0][0]));
            ESP_LOGI(TagReadADC, "ADC%d Channel[%d] Cali Voltage: %d mV\n", ADC_UNIT_1 + 1, ADC_CHANNEL_5, voltage[0][0]);
        }

        ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC_CHANNEL_9, &adc_raw[0][1]));
        ESP_LOGI(TagReadADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC_CHANNEL_9, adc_raw[0][1]);
        if (do_calibration2)
        {
            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc2_cali_handle, adc_raw[0][1], &voltage[0][1]));
            ESP_LOGI(TagReadADC, "ADC%d Channel[%d] Cali Voltage: %d mV\n\n", ADC_UNIT_2 + 1, ADC_CHANNEL_9, voltage[0][1]);
        }


        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}