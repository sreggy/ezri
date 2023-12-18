#include "lib.h"

FtpServer ftp;


void startFtp()
{

	// Start FTP server
	ftp.listen(21);
	ftp.addUser("me", "123", IFS::UserRole::User);
	ftp.addUser("admin", "1234", IFS::UserRole::Admin);
	Serial.println(_F("\r\n=== FTP SERVER STARTED ==="));
	Serial.println(_F("==============================\r\n"));
}

