// Config File

#ifndef __PRIVATE_CONFIG_HPP__
#define __PRIVATE_CONFIG_HPP__

#include <Arduino.h>

#define APP_NAME "ESP8266 HTTPS Sample server"
#define APP_VERSION "0.1.14"

#define OTA_HOSTNAME  "test1"
#define OTA_PASSWORD  "otapass"

//add ntp
#define NTP_SERVER_NAME "pool.ntp.org"
#define NTP_TIME_SHIFT  3600

// LED MODE = 0 => LED 'low' is ON, LED MODE = 1 => LED 'high' is ON
#define LED_MODE  0

#define LED_ON (LED_MODE ^ 0)
#define LED_OFF (LED_MODE ^ 1)

#define WIFI_SSID "dd-wrt"
#define WIFI_PASS "!234567890"

#define BOARD_LED D4

#endif
