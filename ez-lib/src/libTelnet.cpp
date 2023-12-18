#include "lib.h"

bool processTelnetInput(TcpClient& client, char* data, int size)
{
	return client.sendString(commandHandler.processNow(data, size));	
}

TcpServer telnetServer(processTelnetInput);

void startTelnet()
{
	telnetServer.listen(23);

	Serial.println(_F("\r\n=== TelnetServer SERVER STARTED ==="));
	Serial.println(_F("==============================\r\n"));
}

