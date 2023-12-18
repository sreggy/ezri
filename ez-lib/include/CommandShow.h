/*
 * CommandShow.h
 *
 */

#pragma once

#include "WString.h"
// #include <Services/CommandProcessing/CommandProcessingIncludes.h>
#include <CommandProcessing/Utils.h>

class CommandShow
{
public:
	CommandShow(CommandProcessing::Handler& );
	virtual ~CommandShow();
	void initCommand();
	String execCommand(String); // , DynamicJsonDocument &);

private:
	void processCommandShow(String commandLine, ReadWriteStream& commandOutput);
	CommandProcessing::Handler* cHandler{nullptr};

};