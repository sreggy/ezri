/*
 * Debug.cpp
 *
 */
#include <SmingCore.h>
#include <CommandProcessing/Utils.h>
#include "lib.h"

#include <string>

CommandShow::CommandShow(CommandProcessing::Handler &cHandler)
	: cHandler(&cHandler)
{
	debugf("CommandShow Instantiating");
}

CommandShow::~CommandShow()
{
}

void CommandShow::processCommandShow(String commandLine, ReadWriteStream &commandOutput)
{
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ', commandToken);
	commandOutput << endl;

	if (numToken == 1)
	{
		commandOutput << endl;
		commandOutput << "available commands for 'show':" << endl;
		commandOutput << "  help - display this message" << endl;
		commandOutput << "  time - show current time" << endl;
		commandOutput << "  info - show rom info" << endl;
		commandOutput << "  var <name> - show user variable with <name>" << endl;
		commandOutput << "  wifi - show WiFi configuration" << endl;
		commandOutput << "  fs - show partitions" << endl;
		commandOutput << "  file <fName> - show file <fName>" << endl;
		commandOutput << "  conf - show config file" << endl;
		commandOutput << "  log - show log file" << endl;
		commandOutput << "  mem - show conf in memory" << endl;
		commandOutput << "  ota - show ota config" << endl;
	}
	else
	{
//		commandOutput << runCommand(commandLine, CMD_OUTPUT_TEXT).c_str() << endl;
		commandOutput << this->execCommand(commandLine) << endl;

	}
}

void CommandShow::initCommand()
{
	cHandler->registerCommand(
		CommandProcessing::Command("show", "Show Command", "Firmware",
								   CommandProcessing::Command::Callback(&CommandShow::processCommandShow, this)));
}

void showOtaInfo(DynamicJsonDocument &_doc)
{

	_doc["ROM version"] = ezri->getRomVersion();
	_doc["time (UTC)"] = String(SystemClock.getSystemTimeString(eTZ_UTC));
	_doc["reboot (UTC)"] = ezri->getLastBoot();
	_doc["uptime (Minutes)"] = millis() / 60000.00;
	_doc["sdk"] = system_get_sdk_version();
	_doc["freeheap"] = system_get_free_heap_size();
	_doc["cpufrequency"] = system_get_cpu_freq();
	_doc["chipid"] = String(system_get_chip_id(), HEX);

	_doc["hostname"] = WifiStation.getHostname();
	_doc["ip"] = WifiStation.getIP().toString();
	_doc["mac"] = WifiStation.getMacAddress().toString();

	_doc["flashid"] = String(Storage::spiFlash->getId(), HEX);
	_doc["flashsize"] = String(Storage::spiFlash->getSize(), HEX);

	auto before = ota.getRunningPartition();
	auto after = ota.getNextBootPartition();

	_doc["currentrom"] = before.name();
	_doc["currentromaddress"] = "0x" + String(before.address(), HEX);

	_doc["futurerom"] = after.name();
	_doc["futureromaddress"] = "0x" + String(after.address(), HEX);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String CommandShow::execCommand(String commandLine)
{
	Vector<String> commandToken;
	DynamicJsonDocument _doc(2048);
	int numToken = splitString(commandLine, ' ', commandToken);

	commandToken[1].toLowerCase();
	debugf("Received %s - Token[1] >%s<\r\n", commandLine.c_str(), commandToken[1]);

	if (commandToken[1] == "time")
	{
		_doc["time"] = String(SystemClock.getSystemTimeString(eTZ_Local));
	}
	else if (commandToken[1] == "wifi")
	{
		_doc["ssid"] = WifiStation.getSSID();
		_doc["password"] = WifiStation.getPassword();
		_doc["mac"] = WifiStation.getMAC();
		_doc["ip"] = WifiStation.getIP().toString();
		_doc["connectionstatusname"] = WifiStation.getConnectionStatusName();
		_doc["hostname"] = WifiStation.getHostname();
	}
	else if (commandToken[1] == "var")
	{
		if (numToken == 3)
		{
			Serial << ">" << ezri->getUserVar(commandToken[2]) << "<" << endl;
			_doc["key"] = commandToken[2];
			_doc["value"] = ezri->getUserVar(commandToken[2]);
		}
		else
		{
			_doc["error"] = "var not specified";
		}
	}
	else if (commandToken[1] == "info")
	{
		showOtaInfo(_doc);
	}
	else if (commandToken[1] == "fs")
	{
		listSpiffsPartitions(_doc);
	}
	else if (commandToken[1] == "file")
	{
		if (numToken == 3)
		{
			dumpFile(commandToken[2], _doc);
		}
	}
	else if (commandToken[1] == "conf")
	{
		dumpFile(SENSE_CONFIG_FILE, _doc);
	}
	else if (commandToken[1] == "log")
	{
		dumpFile(SENSE_LOG_FILE, _doc);
	}
	else if (commandToken[1] == "mem")
	{
		_doc = ezri->getConf();
	}
	else if (commandToken[1] == "ota")
	{
		_doc = ezri->getOtaConf();
	}
	else if (commandToken[1] == "ver")
	{
		_doc["version"] = ezri->getRomVersion();
		_doc["ezri"] = ezri->getEzriVersion();
	}
	else
	{
		_doc["error"] = "Commmand not recognized";
	}

	String jDoc;
	serializeJsonPretty(_doc, jDoc);

	return jDoc;
}
