/*
 * Ezri.h
 *
 */

#pragma once

#include "WString.h"
// #include <Services/CommandProcessing/CommandProcessingIncludes.h>

#define SENSE_CONFIG_FILE ".ezri.json"
#define SENSE_LOG_FILE ".ezri.log"

#if !defined(__ARCH__)

#if defined(ARCH_ESP8266)
#define __ARCH__ "esp8266"
#elif defined(ARCH_ESP32)
#define __ARCH__ "ESP32"
#else
#define __ARCH___ "None"
#endif

#endif

// enum TriggerType { eTT_None = 0, eTT_Temperature, eTT_Humidity };

struct SenseConfig
{
    // SenseConfig()
    // {
    // 	AddT = 0;
    // 	AddRH = 0;
    // 	AddTZ = 0;
    // 	Trigger = eTT_Temperature;
    // 	RangeMin = 18;
    // 	RangeMax = 29;
    // }

    String NetworkSSID;
    String NetworkPassword;

    int factory;
    String lastBoot;

    // float AddT;  // Temperature adjustment
    // float AddRH; // Humidity adjustment
    // float AddTZ; // TimeZone - local time offset

    // TriggerType Trigger; // Sensor trigger type
    // float RangeMin;
    // float RangeMax;
};

class Ezri
{
public:
    Ezri(String, String);
    virtual ~Ezri();
    void init();

    void configure(DynamicJsonDocument &);
    void loadConfig();
    void saveConfig();

    void setMqttURL(String);
    String getMqttURL();

    void setWiFiSSID(String);
    String getWiFiSSID();

    void setWiFiPWD(String);
    String getWiFiPWD();

    void setFactory(int);
    int getFactory();

    void setLastBoot(String);
    String getLastBoot();

    void setUserVar(String, String);
    String getUserVar(String);

    String getChipID();
    void setChipID();

    void setOta(String, String, String);

    String getOtaFWUrl();
    String getOtaSDUrl();
    String getOtaSpiffsUrl();
    String getRomVersion();
    String getEzriVersion();

    JsonObject getConf();
    JsonObject getOtaConf();

    void printConfig(DynamicJsonDocument &);
    void deleteConfig();

    void setVar(String section, String key, String value);
    void setVar(String section, String key, int value);
    String getVar(String section, String key);

    void initLog();
    void logger(String line);
    String getLog();
    void resetLog();

    void restart(String msg);

private:
    void setOtaVar(String, String);
    String getOtaVar(String);
    void test();
    JsonArray LogJArray;

    //    SenseConfig config;

    StaticJsonDocument<1024> config;
    StaticJsonDocument<1024> log;
    StaticJsonDocument<256> otaConfig;

    // CommandProcessing::Handler commandHandler;

    // CommandShow commandShow(commandHandler);
    // CommandCfg commandCfg(commandHandler);
    // CommandOta commandOta(commandHandler);

};
