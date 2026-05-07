#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "onewire.h"
#include "ds18x20.h"
#include "sdkconfig.h"

#define GPIO_TERMO 1    //pin gpio termometru
static const char *TAG = "TERMOMETR";

void measureTemp();
void app_main(void)
{

    measureTemp();
}

void measureTemp(){
    float temp = 0;                     //zmierzona temperatura
    size_t found = 0;                   //liczba znalezionych termometrow (docelowo 1)
    ESP_LOGI(TAG, "uruchamiam odczyt");
    onewire_addr_t addrs[8];            //adres termometru 
      
    esp_err_t found_dev = ds18x20_scan_devices(GPIO_TERMO, addrs, 8, &found);

    ESP_LOGI(TAG, "Znalezione czujniki: %d", found);
    ESP_LOGI(TAG, "Czujnik %d: %016llX", 0, (unsigned long long)addrs[0]);

    while(1){
        esp_err_t res = ds18x20_measure(GPIO_TERMO, addrs[0], true);
        esp_err_t read = ds18x20_read_temperature(GPIO_TERMO, addrs[0], &temp);
            
        if (res == ESP_OK && read == ESP_OK) {
            // Wypisujemy wynik na dwa sposoby:
            printf("Odczyt: %f C\n", temp);
            ESP_LOGI(TAG, "Stabilny pomiar: %f", temp);
        } else {
            ESP_LOGE(TAG, "Nie udalo sie odczytac temperatury (Kod: %d)", res);
        }
        vTaskDelay(2000); // Czekaj 2 sekundy
    }
}
