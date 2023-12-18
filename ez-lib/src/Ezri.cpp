/*
 * Ezri.cpp
 *
 */
#include <SmingCore.h>
#include "lib.h"

Ezri::Ezri(String rom_type, String rom_env)
{

    Serial.begin(SERIAL_BAUD_RATE); // 115200 by default

    bool rc = spiffs_mount(Storage::findPartition(SPIFFS_NAME));
    // bool rc = spiffs_mount();
    Serial << "SPIFFS mount: " << rc << endl;

    debugf("Ezri Instantiating");
    
    String boot = SystemClock.getSystemTimeString(eTZ_UTC);

    this->loadConfig();
    this->setLastBoot(boot);
    this->setVar("status", "rebooted", "true");
    this->setUserVar("rom_version", _F(__ROM_VERSION__));
    this->setUserVar("ezri_version", _F(__EZRI_VERSION__));
    this->setUserVar("rom_environment", rom_env);
    this->setUserVar("rom_type", rom_type);
    this->setUserVar("rom_arc", _F(__ARCH__));

    this->setOtaVar("ota_version", _F(""));
    this->setOtaVar("ota_environment", _F(__ROM_ENVIRONMENT__));
    this->setOtaVar("ota_type", _F(__ROM_TYPE__));
    this->setOtaVar("ota_arc", _F(__ARCH__));

    this->saveConfig();

    this->initLog();
}

Ezri::~Ezri()
{
}
void Ezri::init()
{

    initWifi();

    commandHandler.registerSystemCommands();
    // this->commandShow().initCommand();
    // this->commandCfg().initCommand();
    // this->commandOta().initCommand();

    startCommandShow();
    startCommandCfg();
    startCommandOta();

//    Debug.setDebug(Serial);

    Serial.systemDebugOutput(true); // Enable debug output to serial
	commandHandler.setVerbose(true);

	// Register Input/Output streams
	CommandProcessing::enable(commandHandlerSerial, Serial);
}

void Ezri::setVar(String section, String key, String value)
{

    const int lSection = section.length();
    char *aSction = new char[lSection + 1];
    strcpy(aSction, section.c_str());

    const int lKey = key.length();
    char *aKey = new char[lKey + 1];
    strcpy(aKey, key.c_str());

    auto jObj = this->config[aSction];
    if (jObj.isNull())
    {
        jObj = this->config.createNestedObject(aSction);
    }

    debugf("key <%s> value <%s>\r\n", aKey, value.c_str());

    jObj[aKey] = value;

    return;
}

void Ezri::setVar(String section, String key, int value)
{

    const int lSection = section.length();
    char *aSction = new char[lSection + 1];
    strcpy(aSction, section.c_str());

    const int lKey = key.length();
    char *aKey = new char[lKey + 1];
    strcpy(aKey, key.c_str());

    auto jObj = this->config[aSction];
    if (jObj.isNull())
    {
        jObj = this->config.createNestedObject(aSction);
    }

    debugf("key <%s> value <%d>\r\n", aKey, value);

    jObj[aKey] = value;

    return;
}

String Ezri::getVar(String section, String key)
{
    JsonObject jObj = this->config[section.c_str()];
    if (!jObj.isNull())
    {
        return String(jObj[key.c_str()]);
    }
    return null;
}

void Ezri::setUserVar(String key, String value)
{
    return this->setVar("user", key, value);
}

String Ezri::getUserVar(String key)
{
    return this->getVar("user", key);
}

void Ezri::setWiFiSSID(String ssid)
{

    return this->setVar("network", "ssid", ssid);
}

String Ezri::getWiFiSSID()
{

    return this->getVar("network", "ssid");
}

void Ezri::setMqttURL(String url)
{

    return this->setVar("network", "mqtt", url);
}

String Ezri::getMqttURL()
{

    return this->getVar("network", "mqtt");
}

void Ezri::setWiFiPWD(String pwd)
{
    return this->setVar("network", "password", pwd);
}
String Ezri::getWiFiPWD()
{
    return this->getVar("network", "password");
}

void Ezri::setLastBoot(String lastboot)
{
    return this->setVar("system", "lastboot", lastboot);
}
String Ezri::getLastBoot()
{
    return this->getVar("system", "lastboot");
}

void Ezri::setFactory(int factory)
{
    return this->setVar("system", "factory", factory);
}
int Ezri::getFactory()
{
    String factory = this->getVar("system", "factory");
    int xc = factory.toInt();

    return xc;
}
void Ezri::setChipID()
{
    String chipid = String(system_get_chip_id(), HEX);
    chipid.toUpperCase();

    if (this->getFactory())
    {
        this->setVar("system", "chipid", "F-" + chipid);
    }
    // else
    // {
    //     this->setVar("system", "chipid", "E0" + chipid);
    // }
}

String Ezri::getChipID()
{
    return this->getVar("system", "chipid");
}
void Ezri::setOtaVar(String key, String value)
{

    const int lKey = key.length();
    char *aKey = new char[lKey + 1];
    strcpy(aKey, key.c_str());

    debugf("key <%s> value <%s>\r\n", aKey, value.c_str());
    this->otaConfig[aKey] = value;

    return;
}

void Ezri::setOta(String type, String env, String ver)
{

    //    Serial << "t:" << type << " e: " << env << " v: " << ver << endl;

    this->setOtaVar("ota_version", ver);

    this->setOtaVar("ota_environment", env);

    this->setOtaVar("ota_type", type);
}

String Ezri::getOtaVar(String key)
{
    return String(this->otaConfig[key.c_str()]);
}

String Ezri::getOtaFWUrl()
{

    String otaUrl = String(this->getVar("system", "ota_rom_url")) + "/" + this->getChipID() + "/" +
                    this->getOtaVar("ota_type") + "/" +
                    this->getOtaVar("ota_arc");

    if (this->getOtaVar("ota_environment") != "")
    {
        otaUrl = otaUrl + "/" + this->getOtaVar("ota_environment");
    }
    if (this->getOtaVar("ota_version") != "")
    {
        otaUrl = otaUrl + "/" + this->getOtaVar("ota_version");
    }

    otaUrl = otaUrl + "/fw/";

    return otaUrl;
}

String Ezri::getOtaSDUrl()
{

    String otaUrl = String(this->getVar("system", "ota_rom_url")) + "/" + this->getChipID() + "/" +
                    this->getOtaVar("ota_type") + "/" +
                    this->getOtaVar("ota_arc");

    if (this->getOtaVar("ota_environment") != "")
    {
        otaUrl = otaUrl + "/" + this->getOtaVar("ota_environment");
    }
    // if (this->getOtaVar("ota_version") != "")
    // {
    //     otaUrl = otaUrl + "/" + this->getOtaVar("ota_version");
    // }

    otaUrl = otaUrl + "/sd/";

    return otaUrl;
}

// String Ezri::getOtaFWUrl()
// {

//     String otaUrl = String(this->getVar("system", "ota_rom_url")) + "/" +
//                     this->getOtaVar("ota_type") + "-" +
//                     this->getOtaVar("ota_arc");

//     if (this->getOtaVar("ota_environment") != "")
//     {
//         otaUrl = otaUrl + "-" + this->getOtaVar("ota_environment");
//     }
//     if (this->getOtaVar("ota_version") != "")
//     {
//         otaUrl = otaUrl + "-" + this->getOtaVar("ota_version");
//     }

//     otaUrl = otaUrl + ".bin";

//     return otaUrl;
// }

String Ezri::getOtaSpiffsUrl()
{
    return this->getVar("network", "ota_spiffs_url");
}

void Ezri::initLog()
{

    if (!Json::loadFromFile(log, SENSE_LOG_FILE))
    {
        Serial << "Unable to load " << SENSE_LOG_FILE << " - initialize it" << endl;
        this->LogJArray = log.createNestedArray("log");
        this->logger(SystemClock.getSystemTimeString(eTZ_UTC));
    }
}

void Ezri::resetLog()
{

    this->log.clear();
    this->LogJArray = log.createNestedArray("log");
    this->logger(SystemClock.getSystemTimeString(eTZ_UTC));
}

void Ezri::logger(String line)
{

    this->LogJArray.add(line);
    Json::saveToFile(log, SENSE_LOG_FILE, Json::Pretty);
}

String Ezri::getLog()
{
    String sDoc;
    serializeJson(log, sDoc);
    return (sDoc);
}

void Ezri::loadConfig()
{

    if (!Json::loadFromFile(config, SENSE_CONFIG_FILE))
    {
        Serial << "Unable to load " << SENSE_CONFIG_FILE << " - initialize it" << endl;
        this->setWiFiSSID(WIFI_SSID);
        this->setWiFiPWD(WIFI_PWD);
        this->setMqttURL(MQTT_URL);
        this->setFactory(1);
        this->setLastBoot("null");
        this->setChipID();
    }
    else
    {
        Serial << "Configuration LOADED !!!" << endl;
        this->setUserVar(_F("rom-timestamp"), _F(__STAMP__));
    }
}

void Ezri::saveConfig()
{

    this->setUserVar(_F("rom-timestamp"), __STAMP__);

    Json::saveToFile(config, SENSE_CONFIG_FILE, Json::Pretty);
}

void Ezri::configure(DynamicJsonDocument &theConf)
{

    Json::saveToFile(theConf, SENSE_CONFIG_FILE, Json::Pretty);

    system_restart();

    return;
}

String Ezri::getRomVersion()
{
    return this->getUserVar("rom_version");
}
String Ezri::getEzriVersion()
{
    return this->getUserVar("ezri_version");
}

JsonObject Ezri::getConf()
{
    return this->config.as<JsonObject>();
}

JsonObject Ezri::getOtaConf()
{
    return this->otaConfig.as<JsonObject>();
}

void Ezri::printConfig(DynamicJsonDocument &_doc)
{
    _doc["activessid"] = this->getWiFiSSID();
    _doc["activepwd"] = this->getWiFiPWD();
    _doc["factory"] = this->getFactory();
    _doc["lastboot"] = this->getLastBoot();
}

void Ezri::deleteConfig()
{
    fileDelete(SENSE_CONFIG_FILE);
    system_restart();
}

void Ezri::restart(String msg)
{
    ezri->logger(msg + " - " + SystemClock.getSystemTimeString(eTZ_UTC));
    System.restart();
}