#ifndef LIBRARY_UART_H
#define LIBRARY_UART_H


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"


static const char *TAG_UART = "uart_events";

#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)

typedef void (*input_callback_t)(const uint8_t *data, uint16_t length);



void uart_init(uart_port_t uart_num,int pinRX, int pinTX);


void uart_event_task(void *pvParameters);

void uart_received(const uint8_t *data, uint16_t length);

void uart_set_callback(void *cb);

void uart_send_data(uint8_t *data);

#endif