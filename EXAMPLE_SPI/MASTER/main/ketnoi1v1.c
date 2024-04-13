// Master as transmitter for SPI communication

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "driver/spi_master.h"
#include "esp_log.h"


#include "driver/gpio.h"
#include "esp_intr_alloc.h"

// Pins in use
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 5

// Main application
void app_main(void)
{
    esp_err_t ret;

    spi_device_handle_t handle;

    // Configuration for the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    // Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 5000000,
        .duty_cycle_pos = 128, // 50% duty cycle
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
        .queue_size = 3};

    char sendbuf[128] = {0};
    char recvbuf[128] = {0};
    int i = 0;

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    

    spi_bus_add_device(VSPI_HOST, &devcfg, &handle);
    

    printf("Master output:\n");
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        i++;
        snprintf(sendbuf, sizeof(sendbuf), "MASTER gui du lieu den lan thu %d", i);
        t.length = sizeof(sendbuf) * 8;
        t.tx_buffer = sendbuf;
        t.rx_buffer = recvbuf;

        spi_device_transmit(handle, &t);

        if(strstr((char *)recvbuf, "Slave nhan duoc du lieu")){
        printf("Transmitted: %s\n", sendbuf);
        printf("Received: %s\n", recvbuf);
        printf("\n\n");
        }
        
    }
}