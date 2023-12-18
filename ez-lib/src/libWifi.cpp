#include "lib.h"

NtpClient ntpClient("time.inrim.it", 30);

// Timer staTimer;
Timer heloTimer;

void onNtpReceive(NtpClient& client, time_t timestamp)
{
	SystemClock.setTime(timestamp, eTZ_UTC);

	Serial << _F("Time synchronized: ") << SystemClock.getSystemTimeString() << endl;
}


void startServers()
{

	startWeb();
	startFtp();
	startTelnet();
}

// void staTask()
// {
// 	debugf("Enbling AP\r\n");

// 	WifiAccessPoint.enable(true);

// 	debugf("Access Point Enabled, SSID -> %s\r\n", WifiAccessPoint.getSSID().c_str());

// 	while (!WifiAccessPoint.isEnabled())
// 		;

// 	startServers();

// 	debugf("AP & Servers Started\r\n");
// }

void gotIP(IpAddress ip, IpAddress netmask, IpAddress gateway)
{

	if (WifiAccessPoint.isEnabled())
		WifiAccessPoint.enable(false);

	startServers();
	startMqttClient();
	heloTimer.initializeMs(5000, heloTask).start();
}

void lostIP(String ssid, MacAddress bssid, WifiDisconnectReason reason)
{

	debugf("Lost IP ...\r\n");

	heloTimer.initializeMs(5000, heloTask).stop();
	if (!WifiAccessPoint.isEnabled())
	{
		debugf("Starting AP and servers");
		WifiAccessPoint.enable(true);
		while (!WifiAccessPoint.isEnabled());

		startServers();
	}
}



void initWifi()
{

	debugf("InitWiFi Started\r\n");
	
	WifiAccessPoint.enable(false);
	
	WifiStation.enable(true);
	WifiStation.setHostname(ezri->getChipID());
	WifiStation.config(ezri->getWiFiSSID(), ezri->getWiFiPWD());

	//	staTimer.initializeMs(40000, staTask).startOnce();
	// Run our method when station was connected to AP
	WifiEvents.onStationGotIP(gotIP);
	WifiEvents.onStationDisconnect(lostIP);

	debugf("InitWiFi Ended\r\n");
}


