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

#include "driver/adc.h"
#include "esp_adc_cal.h"


extern esp_adc_cal_characteristics_t *adc_chars;


void input_button(gpio_num_t PIN);
uint8_t input_get_level(gpio_num_t PIN);

void ledc_init_pwm();
void ledc_add_pin_pwm(int pin, int channel);
void ledc_set_duty_pwm(int channel, int duty);




void print_char_val_type(esp_adc_cal_value_t val_type);
void config_adc_signal_channel1(adc_unit_t unit, adc_channel_t channel, adc_bits_width_t width, adc_atten_t atten, uint16_t DEFAULT_VREF);
void read_adc_signal_channel1(uint32_t *adc_value, uint8_t sample, adc_channel_t channel);




#endif