#include "lib.h"

void subscriber_extension(String, String);

EZRI_DEFINE;

void init()
{

	EZRI_INIT;
	registerCommandExtension(subscriber_extension);
}

void subscriber_extension(String command, String payload)
{

	DynamicJsonDocument doc(256);
	String serializedDoc;

	doc["chipid"] = ezri->getChipID();
	doc["lastseen"] = String(SystemClock.now(eTZ_UTC));
	doc["command"] = command;
	doc["payload"] = payload;

	serializeJson(doc, serializedDoc);
	mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/subscriber_extension", serializedDoc, MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE));

	return;
}
