
#include "nvs_memory.h"


esp_err_t nvs_storage_init() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    return ret;
}

int nvs_read_int(const char* key) {
    // Open NVS handle
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        return 0;  // Return a default value or handle the error accordingly
    }

    // Read data from NVS
    int32_t value;
    ret = nvs_get_i32(nvs_handle, key, &value);
    if (ret != ESP_OK) {
        // Handle the error (e.g., key not found)
        value = 0;  // Set a default value in case of an error
    }

    // Close NVS handle
    nvs_close(nvs_handle);

    return value;
}

esp_err_t nvs_write_int(const char* key, int value) {
    // Open NVS handle
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        return ret;
    }

    // Write data to NVS
    ret = nvs_set_i32(nvs_handle, key, value);
    if (ret == ESP_OK) {
        // Commit the changes
        ret = nvs_commit(nvs_handle);
    }

    // Close NVS handle
    nvs_close(nvs_handle);

    return ret;
}

void nvs_save_and_check(int value_save, const char* keyword){

    esp_err_t check_save = nvs_write_int(keyword, value_save);
    if (check_save != ESP_OK) {
        printf("Error writing to NVS: %s\n", esp_err_to_name(check_save));
        // Handle write error
    }

    else{
        printf("saved %d to %s successfully \n", value_save, keyword);
    }


}
