/*
 * ExtendSystem.h
 *
 */

#pragma once

#include "WString.h"
// #include <Services/CommandProcessing/CommandProcessingIncludes.h>
#include <CommandProcessing/Utils.h>

class CommandOta
{
public:
	CommandOta(CommandProcessing::Handler& );
	virtual ~CommandOta();
	void initCommand();
	String execCommand(String );
	
private:
	void processCommandOta(String commandLine, ReadWriteStream& commandOutput);
	CommandProcessing::Handler* cHandler{nullptr};
};
