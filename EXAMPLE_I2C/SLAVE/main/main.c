#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/Task.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_SLAVE_ADDR 0x32
#define TIMEOUT_MS 1000
#define DELAY_MS 1000

#define RX_BUFFER_LEN 255
#define TX_BUFFER_LEN 255

static const char *TAG = "SLAVE";

static esp_err_t set_i2c(void)
{
    i2c_config_t i2c_config = {};

    i2c_config.mode = I2C_MODE_SLAVE;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = true;
    i2c_config.scl_pullup_en = true;

    i2c_config.slave.addr_10bit_en = 0,
    i2c_config.slave.slave_addr = I2C_SLAVE_ADDR,

    i2c_config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_SLAVE, RX_BUFFER_LEN, TX_BUFFER_LEN, 0));

    return ESP_OK;
}

void i2c_handle_task(void *pv)
{
    uint8_t rx_data[RX_BUFFER_LEN] = {0};
    uint8_t tx_data[32] = "Hello from Slave";

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        int len = i2c_slave_read_buffer(I2C_NUM_0, rx_data, RX_BUFFER_LEN, pdMS_TO_TICKS(100));

        if (len > 0)
        {
            ESP_LOGI(TAG, "Data Recived = %s", rx_data);

            i2c_slave_write_buffer(I2C_NUM_0, (const uint8_t *)tx_data, 32, pdMS_TO_TICKS(100));

            bzero(rx_data, RX_BUFFER_LEN);
        }
    }
}

void app_main(void)
{

    ESP_ERROR_CHECK(set_i2c());
    
    xTaskCreate(i2c_handle_task, "handle", 4096,NULL,1,NULL);
}