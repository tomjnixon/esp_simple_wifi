idf_component_register(
  SRCS "src/simple_wifi.cpp"
  INCLUDE_DIRS "include"
  PRIV_REQUIRES esp_wifi nvs_flash)
