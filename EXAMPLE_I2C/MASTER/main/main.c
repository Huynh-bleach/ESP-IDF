#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/Task.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_SLAVE_ADDR 0x32
#define RX_BUFFER_LEN 255
#define TIMEOUT_MS 1000
#define DELAY_MS 3000

static const char *TAG = "MASTER";

static esp_err_t set_i2c(void)
{
    i2c_config_t i2c_config = {};

    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = true;
    i2c_config.scl_pullup_en = true;
    i2c_config.master.clk_speed = 400000;
    i2c_config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

    return ESP_OK;
}

void app_main(void)
{
    int i = 0;
    uint8_t rx_data[RX_BUFFER_LEN];

    char sendata[100];

  

    ESP_ERROR_CHECK(set_i2c());

    while (1)
    {
        i++;
        sprintf(sendata, "MASTER gui du lieu lan thu %d \n", i);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        i2c_master_write_read_device(I2C_NUM_0,
                                     I2C_SLAVE_ADDR,
                                     (uint8_t *)sendata,
                                     sizeof(sendata),
                                     rx_data,
                                     RX_BUFFER_LEN,
                                     pdMS_TO_TICKS(TIMEOUT_MS));

        printf("Received data: %s\n", rx_data);
    }
}