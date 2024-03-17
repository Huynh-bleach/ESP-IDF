set(app_src button_and_pwm.c)
set(pri_req)

idf_component_register(SRCS ${app_src}
                           INCLUDE_DIRS "."
                           REQUIRES ${pri_req} driver esp_adc)