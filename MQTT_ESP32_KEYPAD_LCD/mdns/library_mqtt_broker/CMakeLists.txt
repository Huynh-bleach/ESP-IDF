set(app_src library_mqtt_broker.c)
set(pri_req)


idf_component_register(SRCS ${app_src}
                           INCLUDE_DIRS "."
                           REQUIRES ${pri_req} esp_partition nvs_flash esp_event esp_netif lwip esp-tls app_update mqtt)