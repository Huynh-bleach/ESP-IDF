#ifndef NVS_MEMORY
#define NVS_MEMORY

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"





esp_err_t nvs_storage_init();
int nvs_read_int(const char* key);
esp_err_t nvs_write_int(const char* key, int value);
void nvs_save_and_check(int value_save, const char* keyword);


#endif  // NVS_STORAGE_H
