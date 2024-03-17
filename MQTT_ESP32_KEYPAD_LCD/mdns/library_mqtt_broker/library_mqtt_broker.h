#ifndef LIBRARY_MQTT_BROKER_H
#define LIBRARY_MQTT_BROKER_H



#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_partition.h"

#include "E:\JSON\cJSON-master\cJSON.h"

#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "E:\ESP_IDF\Espressif\frameworks\esp-idf-v5.0.2\examples\common_components\protocol_examples_common\include\protocol_examples_common.h"

#include "esp_log.h"
//#include "E:\ESP_IDF\Espressif\frameworks\esp-idf-v5.0.2\components\mqtt\esp-mqtt\include\mqtt_client.h"
//#include "E:\ESP_IDF\Espressif\frameworks\esp-idf-v5.0.2\components\esp-tls\esp_tls.h"
//#include "E:\ESP_IDF\Espressif\frameworks\esp-idf-v5.0.2\components\app_update\include\esp_ota_ops.h"
#include <sys/param.h>

#include "mqtt_client.h"
#include "esp_tls.h"  


#include "esp_ota_ops.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include <inttypes.h>


static const char *TAG_MQTT = "MQTTS_EXAMPLE";

extern uint32_t MQTT_CONNEECTED;


#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipseprojects_io_pem_start[] = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipseprojects_io_pem_start[] asm("_binary_mqtt_eclipseprojects_io_pem_start");
#endif
extern const uint8_t mqtt_eclipseprojects_io_pem_end[] asm("_binary_mqtt_eclipseprojects_io_pem_end");


extern esp_mqtt_client_handle_t client;
typedef void (*input_callback_t)(const char *);



void mqtt_app_start(void);

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);


void mqtt_set_callback_received(void * cb);

void mqtt_send_data(int kind, int value);
#endif