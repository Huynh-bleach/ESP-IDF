#ifndef BUTTON_AND_PWM
#define BUTTON_AND_PWM

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "esp_adc/adc_oneshot.h"

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

         

const static char *TagReadADC = "EXAMPLE";



void input_button(gpio_num_t PIN);
uint8_t input_get_level(gpio_num_t PIN);

void ledc_init_pwm();
void ledc_add_pin_pwm(int pin, int channel);
void ledc_set_duty_pwm(int channel, int duty);





void adc_oneshot_config(adc_unit_t adc_uint, adc_oneshot_unit_handle_t *adc_handle, adc_channel_t channel, adc_bitwidth_t *resolution, adc_atten_t *atten_db);

void adc_converts_voltage_config(adc_unit_t uint, adc_cali_handle_t *adc_cali_handle, adc_atten_t atten_db, bool *do_calibration1);
bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);


void adc_oneshot_delete(adc_oneshot_unit_handle_t *adc_handle, adc_cali_handle_t *adc_cali_handle, bool *do_calibration1);
void example_adc_calibration_deinit(adc_cali_handle_t handle);

#endif