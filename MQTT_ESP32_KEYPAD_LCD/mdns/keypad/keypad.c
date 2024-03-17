// keypad.c
#include "keypad.h"


void keypad_init(Keypad *keypad) {
    for (int i = 0; i < KEYPAD_ROWS; i++) {
         gpio_reset_pin(keypad->row_pins[i]);
        gpio_set_direction(keypad->row_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(keypad->row_pins[i], 1);
    }

    for (int i = 0; i < KEYPAD_COLS; i++) {
        gpio_reset_pin(keypad->col_pins[i]);
        gpio_set_direction(keypad->col_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(keypad->col_pins[i], GPIO_PULLUP_ONLY);
    }
}

char keypad_scan(Keypad *keypad) {
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        gpio_set_level(keypad->row_pins[i], 0);

        for (int j = 0; j < KEYPAD_COLS; j++) {
            if (gpio_get_level(keypad->col_pins[j]) == 0) {
                while (gpio_get_level(keypad->col_pins[j]) == 0) {
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                }
                gpio_set_level(keypad->row_pins[i], 1);
                return keypad->keys[i][j];
            }
        }

        gpio_set_level(keypad->row_pins[i], 1);
    }

    return '\0';
}

void keypad_cleanup(Keypad *keypad) {
    // You can add cleanup logic here if needed
   // ESP_LOGI(TAGKEYPAD, "Keypad cleanup");
}
