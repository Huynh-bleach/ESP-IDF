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
  int out_pwm = duty*2.55;
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, out_pwm);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

