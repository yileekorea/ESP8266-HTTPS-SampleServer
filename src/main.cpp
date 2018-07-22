/*
   HelloServerBearSSL - Simple HTTPS server example
   This example demonstrates a basic ESP8266WebServerSecure HTTPS server
   that can serve "/" and "/inline" and generate detailed 404 (not found)
   HTTP respoinses.  Be sure to update the SSID and PASSWORD before running
   to allow connection to your WiFi network.
   Adapted by Earle F. Philhower, III, from the HelloServer.ino example.
   This example is released into the public domain.
 */

#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#include <OtaHandler.hpp>
#include <PrivateConfig.hpp>

#include "test1.local.crt.h"
#include "test1.local.key.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const int led = BOARD_LED;

BearSSL::ESP8266WebServerSecure server(443);
time_t now = 0l;

void showChipInfo()
{
  Serial.println("-- CHIPINFO --");

  Serial.printf("Chip Id = %08X\n", ESP.getChipId() );
  Serial.printf("CPU Frequency = %dMHz\n", ESP.getCpuFreqMHz() );

  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  Serial.printf("\nFlash real id:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash real size: %u\n", realSize);
  Serial.printf("Flash ide  size: %u\n", ideSize);
  Serial.printf("Flash chip speed: %u\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ?
                                          "QIO" : ideMode == FM_QOUT ?
                                          "QOUT" : ideMode == FM_DIO ?
                                          "DIO" : ideMode == FM_DOUT ?
                                          "DOUT" : "UNKNOWN"));
  if (ideSize != realSize)
  {
    Serial.println("Flash Chip configuration wrong!\n");
  }
  else
  {
    Serial.println("Flash Chip configuration ok.\n");
  }
}

void handleRoot()
{
  digitalWrite(led, LED_ON);
  IPAddress clientIpAddress = server.client().remoteIP();

  server.send(200, "text/plain", "Hello from esp8266 over HTTPS!");

  Serial.println();
  time(&now);
  Serial.print( ctime(&now) );
  Serial.println( "  - handle root page");
  Serial.print( "  - from client ip = ");
  Serial.println( clientIpAddress );

  digitalWrite(led, LED_OFF);
}

void handleNotFound()
{
  digitalWrite(led, LED_ON);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i=0; i<server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, LED_OFF);
}

void setup(void)
{
  Serial.begin(115200);

  pinMode(led, OUTPUT);
  digitalWrite(led, LED_OFF);

  delay(3000); // wait for PlatformIO monitor

  Serial.println("\n");

  showChipInfo();

  Serial.println("\n" APP_NAME ", Version " APP_VERSION );
  Serial.println( "Build date: " __DATE__ " " __TIME__  );

  ESP.eraseConfig();         // workaround for some older ESP8266-01
  WiFi.mode( WIFI_OFF );     // the WiFi connect will take a few seconds
  delay(500);                // longer but it is stable ;-)

  Serial.print( "Connecting WiFi " );

  WiFi.begin();                // initialize wifi interface

  WiFi.hostname(OTA_HOSTNAME); // first set hostname, to find it in your
                               // dhcp client list

  WiFi.begin(ssid, password);  // connect in WIFI_STA mode to your WiFi network

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
    digitalWrite( led, 1 ^ digitalRead(led));
  }

  digitalWrite(led, LED_OFF );
  configTime( NTP_TIME_SHIFT, 0, NTP_SERVER_NAME );

  Serial.println("\n");
  Serial.print("Connected to \"");
  Serial.print(ssid);
  Serial.println("\"");
  Serial.print("Hostname   : ");
  Serial.println(WiFi.hostname());
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask    : ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway    : ");
  Serial.println(WiFi.gatewayIP());
  Serial.println();

  if (MDNS.begin(OTA_HOSTNAME))
  {
    Serial.println("MDNS responder started");
  }

  // wait for ntp time
  Serial.print( "Wait for NTP sync " );
  while(!now)
  {
    time(&now);
    Serial.print(".");
    delay(100);
  }

  Serial.println( " done." );
  Serial.println(ctime(&now));

  server.setRSACert(
    new BearSSLX509List(test1_local_crt_der,test1_local_crt_der_len),
    new BearSSLPrivateKey(test1_local_key_der,test1_local_key_der_len));

  server.on("/", handleRoot);

  server.on("/inline", []()
  {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTPS server started");

  MDNS.addService("_https", "_tcp", 443 );
  MDNS.addServiceTxt("_https", "_tcp", "fw_name", APP_NAME );

}

void loop(void)
{
  server.handleClient();
  otaHandler.handle();
}
