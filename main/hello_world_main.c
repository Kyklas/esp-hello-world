/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "soc/efuse_reg.h"
#include "esp_efuse.h"
#include "esp_partition.h"
#include "esp_flash_encrypt.h"
#include "esp_efuse_table.h"

static const char* TAG = "HelloWorld";

static void example_print_flash_encryption_status(void)
{
    uint32_t flash_crypt_cnt = 0;
    esp_efuse_read_field_blob(ESP_EFUSE_FLASH_CRYPT_CNT, &flash_crypt_cnt, 7);
    printf("FLASH_CRYPT_CNT eFuse value is %d\n", flash_crypt_cnt);

    esp_flash_enc_mode_t mode = esp_get_flash_encryption_mode();
    if (mode == ESP_FLASH_ENC_MODE_DISABLED) {
        printf("Flash encryption feature is disabled\n");
    } else {
        printf("Flash encryption feature is enabled in %s mode\n",
            mode == ESP_FLASH_ENC_MODE_DEVELOPMENT ? "DEVELOPMENT" : "RELEASE");
    }
}


static void example_read_flash(void)
{
    const esp_partition_t* partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "storage");
    assert(partition);

    const size_t data_size = 64;

    uint8_t read_data[data_size];
    memset(read_data,0,data_size);
    printf("Reading with esp_partition_read:\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_partition_read(partition, 0, read_data, data_size));
    ESP_LOG_BUFFER_HEXDUMP(TAG, read_data, data_size, ESP_LOG_INFO);

    memset(read_data,0,data_size);
    printf("Reading with spi_flash_read:\n");
    ESP_ERROR_CHECK_WITHOUT_ABORT(spi_flash_read(partition->address, read_data, data_size));
    ESP_LOG_BUFFER_HEXDUMP(TAG, read_data, data_size, ESP_LOG_INFO);
}

void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    example_print_flash_encryption_status();

    example_read_flash();

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
