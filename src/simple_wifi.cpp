#include "simple_wifi.hpp"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include <string.h>

static const char *TAG = "simple_wifi";

static esp_err_t event_handler(void *ctx, system_event_t *event) {
  switch (event->event_id) {
  case SYSTEM_EVENT_STA_START:
    esp_wifi_connect();
    break;
  case SYSTEM_EVENT_STA_GOT_IP:
    ESP_LOGI(TAG, "got ip:%s",
             ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED: {
    ESP_LOGI(TAG, "disconnected; retrying connection to the AP");
    esp_wifi_connect();
    ESP_LOGI(TAG, "connect to the AP fail\n");
    break;
  }
  default:
    break;
  }
  return ESP_OK;
}

void simple_wifi::start() {
  ESP_ERROR_CHECK(nvs_flash_init());

  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  cfg.nvs_enable = false;
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  const char *ssid = CONFIG_SIMPLE_WIFI_SSID;
  const char *password = CONFIG_SIMPLE_WIFI_PASSWORD;

  wifi_config_t wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  memcpy(wifi_config.sta.ssid, ssid, strlen(ssid));
  memcpy(wifi_config.sta.password, password, strlen(password));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "wifi_init_sta finished.");
  ESP_LOGI(TAG, "connect to ap");
}
