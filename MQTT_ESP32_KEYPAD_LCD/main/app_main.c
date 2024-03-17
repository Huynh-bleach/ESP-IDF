#include <stdio.h>
#include <stdlib.h>
#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "espconfiguration.h"
// #include "library_mqtt_broker.h"
#include "wifi_manager.h"

#include <string.h>

static const char TAG[] = "main";
// static const char *TAG_MQTT = "MQTT_EXAMPLE";

uint8_t tamvay = 0;

uint16_t nhietdo = 0;
uint16_t doam = 0;
uint16_t doamdat = 0;
uint16_t anhsang = 0;

static void initialise(void);
void mqtt_event_received_callback(char *data);
void cb_connection_ok(void *pvParameter);
void Publisher_Task(void *params);
void monitoring_task(void *pvParameter);

void process_data_received(int key, int value);
void static_now_media();
void config_pwm();

// static void lcd_demo(void);

// Keypad myKeypad;

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* start the wifi manager */
    wifi_manager_start();

    /* register a callback as an example to how you can integrate your code with the wifi manager */
    wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);

    /* your code should go here. Here we simply create a task on core 2 that monitors free heap memory */

    mqtt_app_start();

    mqtt_set_callback_received(mqtt_event_received_callback);

    take_data_nvs();

    config_button_control();
    config_pwm();
    static_now_media();

    keypad_init(&myKeypad);
    initialise();

    xTaskCreate(read_keypad, "read keypad", 10000, NULL, 5, NULL);

    xTaskCreatePinnedToCore(&monitoring_task, "monitoring_task", 4048, NULL, 5, NULL, 1);
    xTaskCreate(Publisher_Task, "Publisher_Task", 2048 * 5, NULL, 2, NULL);

    xTaskCreate(Task_readbuttonled, "read button led", 5096, NULL, 5, NULL);
    xTaskCreate(Task_readbuttonfan, "read button fan", 5096, NULL, 5, NULL);
    xTaskCreate(Task_readbuttonpump, "read button pump", 5096, NULL, 5, NULL);
}

// Perform initilisation functions
static void initialise(void)
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    // Initialise i2c
    ESP_LOGD(TAGLCD, "Installing i2c driver in master mode on channel %d", I2C_MASTER_NUM);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0));
    ESP_LOGD(TAGLCD,
             "Configuring i2c parameters.\n\tMode: %d\n\tSDA pin:%d\n\tSCL pin:%d\n\tSDA pullup:%d\n\tSCL pullup:%d\n\tClock speed:%.3fkHz",
             i2c_config.mode, i2c_config.sda_io_num, i2c_config.scl_io_num,
             i2c_config.sda_pullup_en, i2c_config.scl_pullup_en,
             i2c_config.master.clk_speed / 1000.0);
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &i2c_config));

    // Modify default lcd_handle details
    lcd_handle.i2c_port = I2C_MASTER_NUM;
    lcd_handle.address = LCD_ADDR;
    lcd_handle.columns = LCD_COLUMNS;
    lcd_handle.rows = LCD_ROWS;
    lcd_handle.backlight = LCD_BACKLIGHT;

    // Initialise LCD
    ESP_ERROR_CHECK(lcd_init(&lcd_handle));

    return;
}

void monitoring_task(void *pvParameter)
{
    for (;;)
    {
        ESP_LOGI(TAG, "free heap: %d", esp_get_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

/**
 * @brief this is an exemple of a callback that you can setup in your own app to get notified of wifi manager event.
 */
void cb_connection_ok(void *pvParameter)
{
    ip_event_got_ip_t *param = (ip_event_got_ip_t *)pvParameter;

    /* transform IP to human readable string */
    char str_ip[16];
    esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

    sprintf(IPaddress, "%s", str_ip);

    ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);
}

void Publisher_Task(void *params)
{
    for (;;)
    {
        if (MQTT_CONNEECTED)
        {
            char send[100]; // Adjust the size based on your needs

            // Create a JSON-like string
             sprintf(send, "{\"kind\":1,\"val\":%d}", tamvay);

          //  sprintf(send, "{\"kind\":10,\"led\":%d,\"fan\":%d,\"pump\":%d,\"buttonled\":%d}", nhietdo, doam, doamdat, anhsang);
            esp_mqtt_client_publish(client, "Huynh1611/clientweb", send, 0, 0, 0);
            ESP_LOGI(TAG_MQTT, "GUI DI KIND 1: %s", send);
            tamvay += 1;
        }

        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}

void mqtt_event_received_callback(char *data)
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

    if (cJSON_IsNumber(kind) && cJSON_IsNumber(val))
    {
        int kindValue = kind->valueint;
        int valValue = val->valueint;

        printf("Received JSON values: kind=%d, val=%d\r\n", kindValue, valValue);

        process_data_received(kindValue, valValue);

        // TODO: Process the values as needed
    }

    else
    {
        printf("Invalid JSON format.\r\n");
    }

    // Free the cJSON structure
    cJSON_Delete(root);
}

void config_button_control()
{
    input_button(buttonled);
    input_button(buttonfan);
    input_button(buttonpump);
}


void config_pwm(){
    ledc_init_pwm();
    ledc_add_pin_pwm(pinled, LEDC_CHANNEL_0);
    ledc_add_pin_pwm(pinfan, LEDC_CHANNEL_1);
    ledc_add_pin_pwm(pinpump, LEDC_CHANNEL_2);

}

void static_now_media(){
    if(onOffled == 1){
        ledc_set_duty_pwm(LEDC_CHANNEL_0, brightnessLED);
    }
    else{
        ledc_set_duty_pwm(LEDC_CHANNEL_0, 0);
    }

    if(onOfffan == 1){
        ledc_set_duty_pwm(LEDC_CHANNEL_1, speedFAN);
    }
    else{
        ledc_set_duty_pwm(LEDC_CHANNEL_1, 0);
    }

    if(onOffpump == 1){
        ledc_set_duty_pwm(LEDC_CHANNEL_2, speedPUMP);
    }
    else{
        ledc_set_duty_pwm(LEDC_CHANNEL_2, 0);
    }


}


void Task_readbuttonled(void *pvParameters)
{
    while (1)
    {
        if (input_get_level(buttonled) == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            if (input_get_level(buttonled) == 0)
            {
                if (onOffled == 0)
                {
                    onOffled = 1;
                    nvs_save_and_check((int)onOffled, "state_led");
                    mqtt_send_data(17, onOffled);
                    ledc_set_duty_pwm(LEDC_CHANNEL_0, brightnessLED);
                }
                else
                {
                    onOffled = 0;
                    nvs_save_and_check((int)onOffled, "state_led");
                    mqtt_send_data(17, onOffled);
                    ledc_set_duty_pwm(LEDC_CHANNEL_0, 0);
                }
            }
            while (input_get_level(buttonled) == 0)
                ;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Task_readbuttonfan(void *pvParameters)
{
    while (1)
    {
        if (input_get_level(buttonfan) == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            if (input_get_level(buttonfan) == 0)
            {
                if (onOfffan == 0)
                {
                    onOfffan = 1;
                    nvs_save_and_check((int)onOfffan, "state_fan");
                    mqtt_send_data(18, onOfffan);
                    ledc_set_duty_pwm(LEDC_CHANNEL_1, speedFAN);
                }
                else
                {
                    onOfffan = 0;
                    nvs_save_and_check((int)onOfffan, "state_fan");
                    mqtt_send_data(18, onOfffan);
                    ledc_set_duty_pwm(LEDC_CHANNEL_1, 0);
                }
            }
            while (input_get_level(buttonfan) == 0)
                ;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Task_readbuttonpump(void *pvParameters)
{
    while (1)
    {
        if (input_get_level(buttonpump) == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            if (input_get_level(buttonpump) == 0)
            {
                if (onOffpump == 0)
                {
                    onOffpump = 1;
                    nvs_save_and_check((int)onOffpump, "state_pump");
                    mqtt_send_data(19, onOffpump);
                    ledc_set_duty_pwm(LEDC_CHANNEL_2, speedPUMP);
                }
                else
                {
                    onOffpump = 0;
                    nvs_save_and_check((int)onOffpump, "state_pump");
                    mqtt_send_data(19, onOffpump);
                    ledc_set_duty_pwm(LEDC_CHANNEL_2, 0);
                }
            }
            while (input_get_level(buttonpump) == 0)
                ;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void take_data_nvs()
{

    onOffled = nvs_read_int("state_led");
    onOfffan = nvs_read_int("state_fan");
    onOffpump = nvs_read_int("state_pump");
    brightnessLED = nvs_read_int("pwm_led");
    speedFAN = nvs_read_int("pwm_fan");
    speedPUMP = nvs_read_int("pwm_pump");

    printf("Read state button = led: %d, fan: %d, pump: %d\n", onOffled, onOfffan, onOffpump);
    printf("Read control mode: pwm_led: %d, pwm_fan: %d, pwm_pump: %d\n", brightnessLED, speedFAN, speedPUMP);
}

void process_data_received(int key, int value)
{
    if (key == 0)
    {
        char sendmode[150]; // Adjust the size based on your needs
        // Create a JSON-like string
        // sprintf(sendmode, "{\"kind\":10,\"led\":%d,\"fan\":%d,\"pump\":%d}", brightnessLED, speedFAN, speedPUMP);
        sprintf(sendmode, "{\"kind\":10,\"led\":%d,\"fan\":%d,\"pump\":%d,\"buttonled\":%d,\"buttonfan\":%d,\"buttonpump\":%d}", brightnessLED, speedFAN, speedPUMP, onOffled, onOfffan, onOffpump);
        esp_mqtt_client_publish(client, "Huynh1611/clientweb", sendmode, 0, 0, 0);
    }
    else if (key == 6)
    {
        onOffled = value;
        nvs_save_and_check((int)onOffled, "state_led");
        if(value == 1){
            ledc_set_duty_pwm(LEDC_CHANNEL_0, brightnessLED);
        }
        else{
            ledc_set_duty_pwm(LEDC_CHANNEL_0, 0);
        }
    }
    else if (key == 7)
    {
        onOfffan = value;
        nvs_save_and_check((int)onOfffan, "state_fan");

        if(value == 1){
            ledc_set_duty_pwm(LEDC_CHANNEL_1, speedFAN);
        }
        else{
            ledc_set_duty_pwm(LEDC_CHANNEL_1, 0);
        }
    }
    else if (key == 8)
    {
        onOffpump = value;
        nvs_save_and_check((int)onOffpump, "state_pump");
        if(value == 1){
            ledc_set_duty_pwm(LEDC_CHANNEL_2, speedPUMP);
        }
        else{
            ledc_set_duty_pwm(LEDC_CHANNEL_2, 0);
        }
    }

    else if (key == 11)
    {
        brightnessLED = value;
        nvs_save_and_check((int)brightnessLED, "pwm_led");
    }

    else if (key == 12)
    {
        speedFAN = value;
        nvs_save_and_check((int)speedFAN, "pwm_fan");
    }

    else if (key == 13)
    {
        speedPUMP = value;
        nvs_save_and_check((int)speedPUMP, "pwm_pump");
    }
}
/*
void dht_test(void *pvParameters)
{
    float temperature, humidity;
#ifdef CONFIG_EXAMPLE_INTERNAL_PULLUP
    gpio_set_pull_mode(dht_gpio, GPIO_PULLUP_ONLY);
#endif
    while (1)
    {
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK)
            printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
        else
            printf("Could not read data from sensor\n");

        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
*/


