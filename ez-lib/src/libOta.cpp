#include <SmingCore.h>

#include "lib.h"

std::unique_ptr<Ota::Network::HttpUpgrader> otaUpdater;
Storage::Partition spiffsPartition;
OtaUpgrader ota;

// Storage::Partition findSpiffsPartition(Storage::Partition appPart)
// {
//     String name = F("spiffs");
//     name += ota.getSlot(appPart);
//     auto part = Storage::findPartition(name);
//     if (!part)
//     {
//         debug_w("Partition '%s' not found", name.c_str());
//     }
//     return part;
// }

Storage::Partition findSpiffsPartition(String name)
{
    auto part = Storage::findPartition(name);
    if (!part)
    {
        debug_w("Partition '%s' not found", name.c_str());
    }
    return part;
}

void upgradeFWCallback(Ota::Network::HttpUpgrader &client, bool result)
{
    Serial.println(_F("In OTA-FW callback..."));
    Serial << "Results: " << result << endl;
    if (result == true)
    {
        // success
        ota.end();

        auto part = ota.getNextBootPartition();
        // set to boot new rom and then reboot
        Serial << _F("Firmware updated, rebooting to '") << part.name() << _F("' @ 0x") << String(part.address(), HEX) << endl;
        ezri->logger(_F("Firmware updated, rebooting to '") + part.name() + _F("' @ 0x") + String(part.address(), HEX));
        
        int slot = ota.getSlotForPartition(part);
        Serial << _F("Slot for partition: ") << slot << endl;
        bool res = ota.setBootPartition(part, true);

        Serial << _F(" ota.setBootPartition: ") << res << endl;
        Serial << _F("OTA boot partition name: ") << ota.getBootPartition().name() << endl;
        ezri->logger(_F("OTA boot partition name: ") + ota.getBootPartition().name());

        ezri->restart(_F("RESTART: OTA Firmware updated"));
    }
    else
    {
        ota.abort();
        // fail
        Serial.println(_F("Firmware update failed!"));
        ezri->logger(_F("Firmware update failed!"));        
    }
}

void upgradeSDCallback(Ota::Network::HttpUpgrader &client, bool result)
{
    Serial.println(_F("In OTA-SD callback..."));
    Serial << "Results: " << result << endl;
    if (result == true)
    {
        // success
        ota.end();
        ezri->logger(_F("SD updated"));          
        ezri->restart(_F("RESTART: OTA SD updated"));
    }
    else
    {
        ota.abort();
        ezri->logger(_F("SD update failed!"));  
        Serial.println(_F("SD update failed!"));
    }
}
void OtaFWUpgrade()
{
    Serial << _F("OTA Firmware Update started") << endl;
    ezri->logger(_F("OTA Firmware Update started"));
    // need a clean object, otherwise if run before and failed will not run again
    otaUpdater.reset(new Ota::Network::HttpUpgrader);

    // select rom slot to flash
    auto part = ota.getNextBootPartition();
    Serial << "ROM URL: " << ezri->getOtaFWUrl() << endl;
    // String otaUrl = ezri->getOtaFWUrl() + "/" + ezri->getUserVar("environment") + "/" + ezri->getUserVar("version") + ".bin";
    otaUpdater->addItem(ezri->getOtaFWUrl(), part);
    Serial << "Partition Name: " << part.name() << endl;

    ota.begin(part);

    // auto spiffsPart = findSpiffsPartition(part);
    // Serial << "SPIFFS Address: " << spiffsPart.address() << endl;

    // if (spiffsPart)
    // {
    //     // use user supplied values (defaults for 4mb flash in hardware config)
    //     Serial << "ROM URL: " << ezri->getOtaSpiffsUrl() << endl;
    //     otaUpdater->addItem(ezri->getOtaSpiffsUrl(), spiffsPart, new Storage::PartitionStream(spiffsPart));
    // }

    // request switch and reboot on success
    // otaUpdater->switchToRom(slot);
    // and/or set a callback (called on failure or success without switching requested)
    otaUpdater->setCallback(upgradeFWCallback);

    // start update
    otaUpdater->start();
}

void OtaSDUpgrade()
{
    Serial << _F("OTA SD Update started") << endl;
    ezri->logger(_F("OTA SD Update started"));
    // need a clean object, otherwise if run before and failed will not run again
    otaUpdater.reset(new Ota::Network::HttpUpgrader);

    Serial << "SD URL: " << ezri->getOtaSDUrl() << endl;
    auto spiffsPart = Storage::findPartition(SPIFFS_NAME);
    if (spiffsPart)
    {
        Serial << "SPIFF part: " << spiffsPart.getDeviceName() << endl;
        otaUpdater->addItem(ezri->getOtaSDUrl(), spiffsPart);
    }

    otaUpdater->setCallback(upgradeSDCallback);
    otaUpdater->start();
}

void OtaDoSwitch()
{
    auto before = ota.getRunningPartition();
    auto after = ota.getNextBootPartition();

    Serial << _F("Swapping from ") << before.name() << " @ 0x" << String(before.address(), HEX) << " to "
           << after.name() << " @ 0x" << String(after.address(), HEX) << endl;
    if (ota.setBootPartition(after))
    {
        Serial.println(F("Restarting...\r\n"));
        System.restart();
    }
    else
    {
        Serial.println(F("Switch failed."));
    }
}

void initOta()
{
    // mount spiffs
    // auto partition = ota.getRunningPartition();
    // spiffsPartition = findSpiffsPartition(partition);
    spiffsPartition = findSpiffsPartition("spiffs0");
    if (spiffsPartition)
    {
        debugf("trying to mount %s @ 0x%08x, length %d", spiffsPartition.name().c_str(), spiffsPartition.address(),
               spiffsPartition.size());
        spiffs_mount(spiffsPartition);
    }
}