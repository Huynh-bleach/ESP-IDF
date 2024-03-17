// keypad.h
#ifndef KEYPAD_H
#define KEYPAD_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

//static const char *TAGKEYPAD = "KEYPAD4x4";


typedef struct {
    int row_pins[KEYPAD_ROWS];
    int col_pins[KEYPAD_COLS];
    char keys[KEYPAD_ROWS][KEYPAD_COLS];
} Keypad;

void keypad_init(Keypad *keypad);
char keypad_scan(Keypad *keypad);
void keypad_cleanup(Keypad *keypad);

#endif // KEYPAD_H
