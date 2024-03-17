#include "library_mqtt_broker.h"

uint32_t MQTT_CONNEECTED = 0;

esp_mqtt_client_handle_t client = NULL;

input_callback_t mqtt_callback_received = NULL;

void mqtt_received(const char *data)
{

    mqtt_callback_received(data);
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // ESP_LOGD(TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
        MQTT_CONNEECTED = 1;
        msg_id = esp_mqtt_client_subscribe(client, "Huynh1611/esp32client", 0);
        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);
        /*
                msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
                ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

                msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
                ESP_LOGI(TAG_MQTT, "sent unsubscribe successful, msg_id=%d", msg_id);
                */
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
        MQTT_CONNEECTED = 0;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        mqtt_received(event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtts://69eee63065914a2d80ab76484db20ba5.s2.eu.hivemq.cloud",
        .broker.address.port = 8883,
        .credentials.username = "huynhdcv1201",
        .credentials.authentication.password = "1611mqtt",
        .session.protocol_ver = MQTT_PROTOCOL_V_5,

        .broker.verification.certificate = (const char *)mqtt_eclipseprojects_io_pem_start,

    };

    // ESP_LOGI(TAG_MQTT, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void mqtt_set_callback_received(void *cb)
{
    mqtt_callback_received = cb;
}

void mqtt_send_data(int kind, int value)
{

    char send[100]; // Adjust the size based on your needs

    // Create a JSON-like string
    sprintf(send, "{\"kind\":%d,\"val\":%d}", kind,value);
    esp_mqtt_client_publish(client, "Huynh1611/clientweb", send, 0, 0, 0);
    ESP_LOGI(TAG_MQTT, "GUI DU LIEU: %s", send);
    
}