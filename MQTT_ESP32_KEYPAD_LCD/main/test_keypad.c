#include "keypad.h"
#include "nvs_memory.h"

Keypad myKeypad = (Keypad){
    .row_pins = {13, 12, 14, 27},
    .col_pins = {26, 25, 33, 32},
    .keys = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}}};

void read_keypad(void *pvParameter)
{
    while (1)
    {
        char key = keypad_scan(&myKeypad);
        if (key != '\0')
        {
            printf("Pressed: %c\n", key);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    keypad_init(&myKeypad);

    xTaskCreate(read_keypad, "read keypad", 10000, NULL, 5, NULL);
}