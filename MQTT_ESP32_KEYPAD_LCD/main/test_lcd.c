#include "lcd.h"
#include "nvs_memory.h"

extern lcd_handle_t lcd_handle;

static const char *TAGLCD = "lcd_example";

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

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    initialise();

    lcd_set_cursor(&lcd_handle, 0, 0);
    lcd_write_str(&lcd_handle, "lap trinh esp32");

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    lcd_set_cursor(&lcd_handle, 0, 1);
    lcd_write_str(&lcd_handle, "VO Manh Huynh");
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    lcd_set_cursor(&lcd_handle, 0, 2);
    lcd_write_str(&lcd_handle, "Dai Hoc Sai Gon");
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    lcd_set_cursor(&lcd_handle, 0, 3);
    lcd_write_str(&lcd_handle, "Dien tu Vien thong");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}
