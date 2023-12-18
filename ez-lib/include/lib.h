#pragma once

#include <SmingCore.h>
#include <CommandProcessing/Utils.h>
// #include <Network/TelnetServer.h>
#include <Network/Http/Websocket/WebsocketResource.h>
#include <Network/Mqtt/MqttBuffer.h>
// #include <Network/Http/Websocket/WsCommandHandlerResource.h>
#include <Storage/PartitionStream.h>
#include <Storage/SpiFlash.h>
#include <Data/Stream/MemoryDataStream.h>
#include <Data/Stream/IFS/DirectoryTemplate.h>
#include <Ota/Network/HttpUpgrader.h>
#include <Ota/Upgrader.h>

#include <JsonObjectStream.h>

#if ENABLE_CMD_EXECUTOR
// #include <Services/CommandProcessing/CommandExecutor.h>
// #include <Services/CommandProcessing/CommandProcessingIncludes.h>
#endif

// #include <Debug.h>
#include "WString.h"

#include "version.h"
#include "ezri-version.h"
#include "rom-type.h"

#include "Ezri.h"

#include "CommandShow.h"
#include "CommandCfg.h"
#include "CommandOta.h"

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "ILUVATAR" // Put your SSID and password here
#define WIFI_PWD "muad$dib"
#define MQTT_URL "mqtt://192.168.0.21:1883"
#endif

#define MQTT_HEADER "sming/"
#define SPIFFS_NAME "spiffs0"

#define CMD_OUTPUT_JSON 1
#define CMD_OUTPUT_TEXT 2

#define EZRI_DEFINE \
    Ezri *ezri;

#define EZRI_INIT   \
    ezri = new Ezri(__ROM_TYPE__, __ROM_ENVIRONMENT__); \
    ezri->init();

                  
extern Ezri *ezri;
extern MqttClient mqtt;
extern OtaUpgrader ota;

extern CommandProcessing::Handler commandHandler;
extern CommandProcessing::Handler commandHandlerSerial;
extern CommandShow commandShow;
extern CommandCfg commandCfg;
extern CommandOta commandOta;

void startMqttClient();
// void (*ptr_subscriber_extension)(String);
void registerCommandExtension(void (*ptr)(String, String));
void mqttLog(String);

void OtaFWUpgrade();
void OtaSDUpgrade();
void OtaDoSwitch();
unsigned mySplitString(String &, String &, Vector<String> &);
void dumpFile(String fileName, DynamicJsonDocument &);
void listSpiffsPartitions(DynamicJsonDocument &);
String runCommand(String, int);
String formatOutput(DynamicJsonDocument, int);
void showOtaInfo(DynamicJsonDocument &);
void heloTask();
void initOta();
void startWeb();
void startFtp();
void startTelnet();
void startCommandShow();
void startCommandCfg();
void startCommandOta();
void initWifi();
void gotIP(IpAddress, IpAddress, IpAddress);
