/*
 * Debug.cpp
 *
 */
#include <SmingCore.h>
#include <CommandProcessing/Utils.h>
#include "lib.h"

CommandCfg::CommandCfg(CommandProcessing::Handler& cHandler)
	: cHandler(&cHandler)
{
    debugf("CommandCfg Instantiating");
}

CommandCfg::~CommandCfg()
{
}


void CommandCfg::processCommandCfg(String commandLine, ReadWriteStream& commandOutput)
{
    Vector<String> commandToken;
    int numToken = splitString(commandLine, ' ', commandToken);
	commandOutput << endl;

    if (numToken == 1)
    {
		commandOutput << endl;
		commandOutput << "available commands for 'cfg':" << endl;
		commandOutput << "  help - display this message" << endl;
		commandOutput << "  reboot - reboot ROM" << endl;
		commandOutput << "  factory reset - reset ROM to factory and reboot" << endl;
		commandOutput << "  load - load configuration file" << endl;
		commandOutput << "  save - save current config to persistent configuration file" << endl;
		commandOutput << "  wifi <ssid> <pwd> - set WiFi <SSID> and <PWD>" << endl;
		commandOutput << "  var <section> <tag> <value> - set var <tag> in <section> to <value>" << endl;
		commandOutput << "  print - display running WiFi configuration" << endl;
    }
    else
    {
        //		commandLine.toLowerCase();
//        commandOutput << runCommand(commandLine, CMD_OUTPUT_TEXT).c_str() << endl;
		commandOutput << this->execCommand(commandLine) << endl;
    }
}

void CommandCfg::initCommand()
{
    cHandler->registerCommand(
        CommandProcessing::Command("cfg", "Configuration Command", "Firmware",
                                                   CommandProcessing::Command::Callback(&CommandCfg::processCommandCfg, this)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String CommandCfg::execCommand(String commandLine)
{
	Vector<String> commandToken;
	DynamicJsonDocument _doc(2048);
    int numToken = splitString(commandLine, ' ', commandToken);

    commandToken[1].toLowerCase();

    if (commandToken[1] == "reboot")
    {
        ezri->restart(_F("RESTART: CFG REBOOT request"));
    }
    else if (commandToken[1] == "load")
    {
        ezri->loadConfig();
        _doc["message"] = "Configuration Loaded";
    }
    else if (commandToken[1] == "save")
    {
        ezri->saveConfig();
        _doc["message"] = "Configuration Saved";
    }
    else if (commandToken[1] == "var")
    {
        if (numToken == 5)
        {
            ezri->setVar(commandToken[2], commandToken[3], commandToken[4]);
            _doc["message"] = "Var " + commandToken[2] + "->" + commandToken[3] + " = " + ezri->getVar(commandToken[2], commandToken[3]);
        }
        else
        {
            _doc["error"] = "Commmand error";
        }
    }
    else if (commandToken[1] == "wifi")
    {
        if (numToken == 4)
        {
            ezri->setWiFiSSID(commandToken[2]);
            ezri->setWiFiPWD(commandToken[3]);
            WifiStation.config(ezri->getWiFiSSID(), ezri->getWiFiPWD());
            _doc["message"] = "New Wifi settings " + ezri->getWiFiSSID() + " - " + ezri->getWiFiPWD();
        }
        else
        {
            _doc["error"] = "Commmand error";
        }
    }
    else if (commandToken[1] == "print")
    {
        ezri->printConfig(_doc);
    }
    else if (commandToken[1] == "factory")
    {
        if (commandToken[2] == "reset")
        {
            ezri->deleteConfig();
        }
    }
    else
    {
        _doc["error"] = "Commmand not recognized";
    }

	String jDoc;
	serializeJsonPretty(_doc, jDoc);

	return jDoc;
}
