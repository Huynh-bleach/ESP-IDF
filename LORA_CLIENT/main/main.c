#include <stdio.h>
#include "button_and_pwm.h"
#include "library_uart.h"
#include "E:\JSON\cJSON-master\cJSON.h"

#define EX_UART_NUM UART_NUM_2

#define button_uart 14
void Task_readbuttonled(void *pvParameters);
void lora_event_received(char *data);

char send_data[70];
int i = 90;
int pwm = 100;

int NT = 0;
int DA = 0;
int DAD = 0;
int AS = 0;

void nhandulieu(uint8_t *data, uint16_t length)
{

    printf("%s\n", data);
    if (strstr((char *)data, "kind"))
    {
        lora_event_received((char *)data);
    }
}

void app_main(void)
{

    esp_log_level_set(TAG_UART, ESP_LOG_INFO);
    gpio_set_direction(18, GPIO_MODE_OUTPUT);
    gpio_set_level(18, 0);
    gpio_set_direction(5, GPIO_MODE_OUTPUT);
    gpio_set_level(5, 0);
    input_button(button_uart);

    uart_init(EX_UART_NUM, 16, 17);

    uart_set_callback(nhandulieu);

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
    xTaskCreate(Task_readbuttonled, "read button led", 5096, NULL, 5, NULL);
}

void Task_readbuttonled(void *pvParameters)
{
    while (1)
    {
        if (input_get_level(button_uart) == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            if (input_get_level(button_uart) == 0)
            {
                printf("da nhan button\n");

                //sprintf(send_data, "{\"kind\":LORA_CLIENT,\"val\":%d,\"pwm\":%d}\n", i, pwm);

                sprintf(send_data, "{\"nhietdo\":%d,\"doam\":%d,\"doamdat\":%d,\"anhsang\":%d}\n", NT, DA, DAD, AS);
                uart_send_data((uint8_t *)send_data);
                NT++;
                DA++;
                DAD++;
                AS++;
            }
            while (input_get_level(button_uart) == 0)
                ;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void lora_event_received(char *data)
{
    // printf("Received MQTT data: %s\r\n", data);

    // Parse the received JSON string
    cJSON *root = cJSON_Parse(data);
    if (root == NULL)
    {
        printf("Error parsing JSON data.\r\n");
        return;
    }

    // Extract values from JSON
    cJSON *kind = cJSON_GetObjectItemCaseSensitive(root, "kind");
    cJSON *val = cJSON_GetObjectItemCaseSensitive(root, "val");
    cJSON *pwm = cJSON_GetObjectItemCaseSensitive(root, "pwm");

    if (cJSON_IsNumber(kind) && cJSON_IsNumber(val) && cJSON_IsNumber(pwm))
    {
        int kindValue = kind->valueint;
        int valValue = val->valueint;
        int pwmValue = pwm->valueint;

        printf("Received MASTER JSON values: kind=%d, val=%d, pwm=%d\r\n", kindValue, valValue, pwmValue);

        // TODO: Process the values as needed
        // process_data_received(kindValue,valValue,pwmValue);
    }

    else
    {
        printf("Invalid JSON format.\r\n");
    }

    // Free the cJSON structure
    cJSON_Delete(root);
}
