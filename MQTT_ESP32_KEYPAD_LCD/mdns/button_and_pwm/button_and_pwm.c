#include "button_and_pwm.h"

void input_button(gpio_num_t PIN)
{
    gpio_reset_pin(PIN);
    gpio_set_direction(PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN, GPIO_PULLUP_ONLY);
    // gpio_set_intr_type(PIN, type);
    // gpio_install_isr_service(0);
    // gpio_isr_handler_add(PIN, gpio_input_handler, (void *)PIN);
}

uint8_t input_get_level(gpio_num_t PIN)
{
    return gpio_get_level(PIN);
}

void ledc_init_pwm()
{

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_1,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);
}

void ledc_add_pin_pwm(int pin, int chanel)
{
    ledc_channel_config_t ledc_channel =
        {
            .channel = chanel,
            .duty = 0,
            .gpio_num = pin,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint = 0,
            .timer_sel = LEDC_TIMER_1};

    ledc_channel_config(&ledc_channel);
}

void ledc_set_duty_pwm(int channel, int duty)
{
    int out_pwm = duty * 2.55;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, out_pwm);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

void adc_oneshot_config(adc_unit_t adc_uint, adc_oneshot_unit_handle_t *adc_handle, adc_channel_t channel, adc_bitwidth_t *resolution, adc_atten_t *atten_db)
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = adc_uint,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    adc_oneshot_chan_cfg_t config;

    if (resolution == NULL && atten_db == NULL)
    {
        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        config.atten = ADC_ATTEN_DB_11;
    }
    else if (resolution != NULL && atten_db == NULL)
    {

        config.bitwidth = *resolution;
        config.atten = ADC_ATTEN_DB_11;
    }

    else if (resolution == NULL && atten_db != NULL)
    {

        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        config.atten = *atten_db;
    }
    else
    {
        config.bitwidth = *resolution;
        config.atten = *atten_db;
    }

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, adc_handle));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, channel, &config));
}

void adc_converts_voltage_config(adc_unit_t uint, adc_cali_handle_t *adc_cali_handle, adc_atten_t atten_db, bool *do_calibration1)
{
    *do_calibration1 = example_adc_calibration_init(uint, atten_db, adc_cali_handle);
}

bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated)
    {
        ESP_LOGI(TagReadADC, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated)
    {
        ESP_LOGI(TagReadADC, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK)
    {
        ESP_LOGI(TagReadADC, "Calibration Success");
    }
    else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated)
    {
        ESP_LOGW(TagReadADC, "eFuse not burnt, skip software calibration");
    }
    else
    {
        ESP_LOGE(TagReadADC, "Invalid arg or no memory");
    }

    return calibrated;
}

void adc_oneshot_delete(adc_oneshot_unit_handle_t *adc_handle, adc_cali_handle_t *adc_cali_handle, bool *do_calibration1)
{

    ESP_ERROR_CHECK(adc_oneshot_del_unit(*adc_handle));
    if (*do_calibration1)
    {
        example_adc_calibration_deinit(*adc_cali_handle);
    }
}

void example_adc_calibration_deinit(adc_cali_handle_t handle)
{
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    ESP_LOGI(TagReadADC, "deregister %s calibration scheme", "Curve Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(handle));

#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    ESP_LOGI(TagReadADC, "deregister %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(handle));
#endif
}
