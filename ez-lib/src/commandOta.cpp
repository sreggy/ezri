/*
 * Debug.cpp
 *
 */
#include <SmingCore.h>
#include <CommandProcessing/Utils.h>
#include "lib.h"

CommandOta::CommandOta(CommandProcessing::Handler& cHandler)
	: cHandler(&cHandler)
{
    debugf("CommandOta Instantiating");
}

CommandOta::~CommandOta()
{
}


void CommandOta::processCommandOta(String commandLine, ReadWriteStream& commandOutput)
{
    Vector<String> commandToken;
    int numToken = splitString(commandLine, ' ', commandToken);
	commandOutput << endl;

    if (numToken == 1)
    {
		commandOutput << endl;
		commandOutput << "available commands for 'ota':" << endl;
		commandOutput << "  help - display this message" << endl;
		commandOutput << "  fw - start OTA Firmware Update" << endl;
		commandOutput << "  sd - start OTA SD Update" << endl;                               
		commandOutput << "  switch - reboot from net partition" << endl;
		commandOutput << "  cfg <type> <environment> <version> - start OTA with type to <type>, environment to <environment>, version to <version>" << endl;
		commandOutput << "  url - show OTA url" << endl;
		commandOutput << "  info - show OTA config" << endl;
    }
    else
    {
        //		commandLine.toLowerCase();
//        commandOutput << runCommand(commandLine, CMD_OUTPUT_TEXT).c_str() << endl;
		commandOutput << this->execCommand(commandLine) << endl;        
    }
}


void CommandOta::initCommand()
{
    cHandler->registerCommand(
        CommandProcessing::Command("ota", "Configuration Command", "Firmware",
                                                   CommandProcessing::Command::Callback(&CommandOta::processCommandOta, this)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String CommandOta::execCommand(String commandLine)
{
    Vector<String> commandToken;
	DynamicJsonDocument _doc(2048);    
    int numToken = splitString(commandLine, ' ', commandToken);
    Serial << "Num Token: " << numToken << endl;

    commandToken[1].toLowerCase();

    if (commandToken[1] == "fw")
    {
        _doc["message"] = "OTA Firmware upgrade started !!";
        initOta();
        OtaFWUpgrade();
    }
    else if (commandToken[1] == "sd")
    {
        _doc["message"] = "OTA Disk upgrade started !!";
        initOta();
        OtaSDUpgrade();
    }
    else if (commandToken[1] == "cfg" && numToken >= 2)
    {
        if (numToken == 5)
        {
            commandToken[2].toLowerCase();
            commandToken[3].toLowerCase();
            commandToken[4].toLowerCase();
            ezri->setOta(commandToken[2], commandToken[3], commandToken[4]);
        }
        else if (numToken == 4)
        {
            commandToken[2].toLowerCase();
            commandToken[3].toLowerCase();
            ezri->setOta(commandToken[2], commandToken[3], "");
        }
        else if (numToken == 3)
        {
            commandToken[2].toLowerCase();
            ezri->setOta(commandToken[2], "", "");
        }
        else
        {
            ezri->setOta(ezri->getUserVar("rom_type"), ezri->getUserVar("rom_environment"), null);
        }
        _doc = ezri->getOtaConf();
    }
    else if (commandToken[1] == "info")
    {
        _doc = ezri->getOtaConf();
    }
    else if (commandToken[1] == "switch")
    {
        _doc["message"] = "OTA do Switch !!";
        OtaDoSwitch();
    }
    else if (commandToken[1] == "url")
    {
        _doc["fw"] = ezri->getOtaFWUrl();
        _doc["sd"] = ezri->getOtaSDUrl();
    }
    else
    {
        _doc["error"] = "Commmand not recognized";
    }
	String jDoc;
	serializeJsonPretty(_doc, jDoc);

	return jDoc;

}
