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




void input_button(gpio_num_t PIN);
uint8_t input_get_level(gpio_num_t PIN);

void ledc_init_pwm();
void ledc_add_pin_pwm(int pin, int channel);
void ledc_set_duty_pwm(int channel, int duty);






#endif