/*
 * ExtendSystem.h
 *
 */

#pragma once

#include "WString.h"
// #include <Services/CommandProcessing/CommandProcessingIncludes.h>
#include <CommandProcessing/Utils.h>

class CommandCfg
{
public:
	CommandCfg(CommandProcessing::Handler& );;
	virtual ~CommandCfg();
	void initCommand();
	String execCommand(String);
	
private:
 	void processCommandCfg(String commandLine, ReadWriteStream& commandOutput);
	CommandProcessing::Handler* cHandler{nullptr};
};
