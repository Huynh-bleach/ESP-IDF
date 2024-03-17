#include "library_uart.h"

input_callback_t uart_callback_received = NULL;


static QueueHandle_t uart0_queue;

void uart_init(uart_port_t uart_num,int pinRX, int pinTX)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    //Install UART driver, and get the queue.
    uart_driver_install(uart_num, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(uart_num, &uart_config);

    //Set UART log level
    esp_log_level_set(TAG_UART, ESP_LOG_INFO);
    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(uart_num, pinTX, pinRX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(uart_num, '+', PATTERN_CHR_NUM, 9, 0, 0);
    //Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(uart_num, 20);

}


void uart_event_task(void *pvParameters){
    uart_event_t event;
  //  size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG_UART, "uart[%d] event:", UART_NUM_2);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    ESP_LOGI(TAG_UART, "[UART DATA]: %d", event.size);
                    uart_read_bytes(UART_NUM_2, dtmp, event.size, portMAX_DELAY);
                    uart_received(dtmp, event.size);
                    
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG_UART, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(UART_NUM_2);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG_UART, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(UART_NUM_2);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG_UART, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG_UART, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG_UART, "uart frame error");
                    break;
                //UART_PATTERN_DET
                /*case UART_PATTERN_DET:
                    uart_get_buffered_data_len(UART_NUM_2, &buffered_size);
                    int pos = uart_pattern_pop_pos(UART_NUM_2);
                    ESP_LOGI(TAG_UART, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                    if (pos == -1) {
                        // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                        // record the position. We should set a larger queue size.
                        // As an example, we directly flush the rx buffer here.
                        uart_flush_input(UART_NUM_2);
                    } else {
                        uart_read_bytes(UART_NUM_2, dtmp, pos, 100 / portTICK_PERIOD_MS);
                        uint8_t pat[PATTERN_CHR_NUM + 1];
                        memset(pat, 0, sizeof(pat));
                        uart_read_bytes(UART_NUM_2, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                        ESP_LOGI(TAG_UART, "read data: %s", dtmp);
                        ESP_LOGI(TAG_UART, "read pat : %s", pat);
                    }
                    break;
                //Others
                */
                default:
                    ESP_LOGI(TAG_UART, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}


void uart_received(const uint8_t *data, uint16_t length){
    uart_callback_received(data, length);
}

void uart_set_callback(void *cb){
    if(cb){
        uart_callback_received = cb;
    }
}


void uart_send_data(uint8_t *data){

    size_t length = strlen((char*)data);

    uart_write_bytes(UART_NUM_2, (char*)data,length);
}