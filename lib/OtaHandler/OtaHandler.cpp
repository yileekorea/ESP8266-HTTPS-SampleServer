#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <PrivateConfig.hpp>
#include "OtaHandler.hpp"

OtaHandler otaHandler;

void OtaHandler::setup()
{
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]()
  {
    Serial.println("\nOTA Start");
  });

  ArduinoOTA.onEnd([]()
  {
    Serial.println("\nOTA End\n");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    unsigned int percent =  (progress / (total / 100));
    Serial.printf("\rOTA Progress: %u%%", percent );
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("OTA Error[%u]: ", error);

    if (error == OTA_AUTH_ERROR)
      Serial.println("OTA Auth Failed");
    else
    if (error == OTA_BEGIN_ERROR)
      Serial.println("OTA Begin Failed");
    else
    if (error == OTA_CONNECT_ERROR)
      Serial.println("OTA Connect Failed");
    else
    if (error == OTA_RECEIVE_ERROR)
      Serial.println("OTA Receive Failed");
    else
    if (error == OTA_END_ERROR)
      Serial.println("OTA End Failed");
  });

  ArduinoOTA.begin();
  MDNS.addServiceTxt("arduino", "tcp", "fw_name", APP_NAME );
  MDNS.addServiceTxt("arduino", "tcp", "fw_version", APP_VERSION );
  Serial.println("OTA initialized");
  initialized = true;
}

void OtaHandler::handle()
{
  if( !initialized )
  {
    setup();
  }

  ArduinoOTA.handle();
}
