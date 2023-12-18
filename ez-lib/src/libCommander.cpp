#include "lib.h"

// #include <LittleFS.h>
// #include <Libraries/LittleFS/littlefs/lfs.h>
// #include <Libraries/LittleFS/src/include/LittleFS/FileSystem.h>

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "ILUVATAR" // Put your SSID and password here
#define WIFI_PWD "muad$dib"
#endif
// ExampleCommand exampleCommand;
CommandProcessing::Handler commandHandler;
CommandProcessing::Handler commandHandlerSerial;

CommandShow commandShow(commandHandler);
CommandCfg commandCfg(commandHandler);
CommandOta commandOta(commandHandler);


CommandShow commandShowSerial(commandHandlerSerial);
CommandCfg commandCfgSerial(commandHandlerSerial);
CommandOta commandOtaSerial(commandHandlerSerial);

// CommandExecutor *commandExecutor = nullptr;


// void startExampleApplicationCommand()
// {
// 	exampleCommand.initCommand();
// 	commandHandler.registerCommand(
// 		CommandDelegate(F("example"), F("Example Command from Class"), F("Application"), processApplicationCommands));
// }

void processApplicationCommands(String commandLine, ReadWriteStream& commandOutput)
{
    commandOutput << _F("This command is handle by the application") << endl;
}

void startCommandShow()
{
	commandShow.initCommand();
	commandShowSerial.initCommand();
	// commandHandler.registerCommand(
	// 	CommandProcessing::Command(F("show"), F("Show Command"), F("Firmware"), processApplicationCommands));
}

void startCommandCfg()
{
	commandCfg.initCommand();
	commandCfgSerial.initCommand();
	// commandHandler.registerCommand(
	// 	CommandProcessing::Command(F("cfg"), F("Configuration Command"), F("Firmware"), processApplicationCommands));
}

void startCommandOta()
{
	commandOta.initCommand();
	commandOtaSerial.initCommand();	
	// commandHandler.registerCommand(
	// 	CommandProcessing::Command(F("ota"), F("Configuration Command"), F("Firmware"), processApplicationCommands));
}

