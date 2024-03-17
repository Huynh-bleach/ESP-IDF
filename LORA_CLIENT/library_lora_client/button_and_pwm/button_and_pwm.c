#include "button_and_pwm.h"


esp_adc_cal_characteristics_t *adc_chars;



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
  //int out_pwm = duty*2.55;
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}



void config_adc_signal_channel1(adc_unit_t unit, adc_channel_t channel, adc_bits_width_t width, adc_atten_t atten, uint16_t DEFAULT_VREF)
{
    // Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("eFuse Two Point: Supported\n");
    }
    else
    {
        printf("eFuse Two Point: NOT supported\n");
    }

    // Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
    {
        printf("eFuse Vref: Supported\n");
    }
    else
    {
        printf("eFuse Vref: NOT supported\n");
    }

    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}

void read_adc_signal_channel1(uint32_t *adc_value, uint8_t sample, adc_channel_t channel)
{
    for (int i = 0; i < sample; i++)
    {
        *adc_value += adc1_get_raw(channel);
    }
    *adc_value /= sample;
}

void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("Characterized using Two Point Value\n");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("Characterized using eFuse Vref\n");
    }
    else
    {
        printf("Characterized using Default Vref\n");
    }
}



