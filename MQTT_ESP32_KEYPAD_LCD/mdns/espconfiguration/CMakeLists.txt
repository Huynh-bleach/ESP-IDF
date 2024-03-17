if(IDF_VERSION_MAJOR AND IDF_VERSION_MAJOR GREATER 3)
    idf_component_register(SRCS "espconfiguration.c"
        INCLUDE_DIRS "."
        REQUIRES "button_and_pwm" "dht" "keypad" "nvs_memory" "esp32-HD44780" "library_mqtt_broker")
else()
    set(COMPONENT_SRCDIRS .)
    set(COMPONENT_REQUIRES log)
    register_component()
endif()