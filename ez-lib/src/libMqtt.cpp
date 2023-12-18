#include "lib.h"

MqttClient mqtt;
Timer procTimer;

void (*ptr_subscriber_extension)(String, String);

void registerCommandExtension(void (*ptr)(String, String))
{
    ptr_subscriber_extension = ptr;
}

void myHelo()
{
    StaticJsonDocument<512> doc;
    String serializedDoc;

    // DateTime dt(SystemClock.now(eTZ_UTC));

    doc["chipid"] = ezri->getChipID();
    doc["millis"] = millis();
    doc["lastseen"] = String(SystemClock.now(eTZ_UTC));

    doc["mac"] = WifiStation.getMacAddress().toString();
    doc["fw_ver"] = ezri->getRomVersion();
    doc["ezri_ver"] = ezri->getEzriVersion();
    doc["scope"] = ezri->getVar("user", "rom_type");
    doc["ip"] = WifiStation.getIP().toString();
    doc["ssid"] = WifiStation.getSSID();
    doc["cpu_freq"] = system_get_cpu_freq();
    doc["freeheap"] = system_get_free_heap_size();

    serializeJson(doc, serializedDoc);

    mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/helo", serializedDoc);

    return;
}

void myPing(String payload)
{
    StaticJsonDocument<512> doc;
    String serializedDoc;

    doc["lastseen"] = String(SystemClock.now(eTZ_UTC));
    doc["chipid"] = ezri->getChipID();
    doc["ip"] = WifiStation.getIP().toString();
    doc["payload"] = String(payload);

    serializeJson(doc, serializedDoc);

    mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/ping", serializedDoc, MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE));

    return;
}

void checkMQTTDisconnect(TcpClient &client, bool flag)
{
    if (flag == true)
    {
        Serial.println(_F("MQTT Broker Disconnected!!"));
        ezri->logger(_F("MQTT Broker Disconnected!!"));
    }
    else
    {
        Serial.println(_F("MQTT Broker Unreachable!!"));
        ezri->logger(_F("MQTT Broker Unreachable!!"));
    }

    // Restart connection attempt after few seconds
    ezri->setVar("status", "mqtt", "disconnected");
    procTimer.initializeMs(2 * 1000, startMqttClient).start(); // every 2 seconds
}

// Publish our message
void checkMQTTConnect()
{
    if (mqtt.getConnectionState() != eTCS_Connected)
    {
        startMqttClient(); // Auto reconnect
    }

    ezri->setVar("status", "mqtt", "connected");
    Serial << _F("Let's publish message now. Memory free=") << system_get_free_heap_size() << endl;
    // mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/frameworks", F("Hello friends, from Internet of things :)"));

    // mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/important", F("Request Return Delivery"),
    //              MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE));
    DynamicJsonDocument _doc(128);
    _doc["status"] = "alive";
    _doc["ip"] = WifiStation.getIP().toString();
    _doc["chipid"] = ezri->getChipID();

    String sDoc;
    serializeJson(_doc, sDoc);
    mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/report", sDoc,
                 MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE));

    if (ezri->getVar("status", "rebooted") == "true")
    {
        mqttLog("System Rebooted - MQTT connected");
        ezri->setVar("status", "rebooted", "false");
    }
    else
    {
        mqttLog("MQTT connected");
    }

    mqttLog(ezri->getLog());
    ezri->resetLog();
}

int onMessageDelivered(MqttClient &client, mqtt_message_t *message)
{
    Serial << _F("Message with id ") << message->puback.message_id << _F(" and QoS ") << message->puback.qos
           << _F(" was delivered successfully.") << endl;
    return 0;
}

void mqttLog(String payload)
{
    if (mqtt.getConnectionState() != eTCS_Connected)
    {
        startMqttClient(); // Auto reconnect
    }

    // Serial << _F("Let's publish message now. Memory free=") << system_get_free_heap_size() << endl;
    // mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/frameworks", F("Hello friends, from Internet of things :)"));

    // mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/important", F("Request Return Delivery"),
    //              MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE));

    mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/log", payload,
                 MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE));
}

// Callback for messages, arrived from MQTT server
int onMessageReceived(MqttClient &client, mqtt_message_t *message)
{
    Serial << _F("Received: ") << MqttBuffer(message->publish.topic_name) << ':' << endl;
    Serial << '\t' << MqttBuffer(message->publish.content) << endl;

    String content = MqttBuffer(message->publish.content);

    String command = "", payload = "";

    for (int a = 0; a < content.length(); a++)
    {
        Serial << "at " << a << " is '" << content.charAt(a) << "'" << endl;
        if (content.charAt(a) == ' ')
        {
            command = content.substring(0, a);
            payload = content.substring(a + 1);
            break;
            ;
        }
    }

    Serial << "Command: >" << command << "<" << endl;
    Serial << "Payload: >" << payload << "<" << endl;

    if (command == "")
    {
        command = content;
        payload = "";
    }

    command.toLowerCase();

    if (command == "helo")
    {
        myHelo();
    }
    else if (command == "ping")
    {
        myPing(payload);
    }
    else if (command == "report")
    {
        checkMQTTConnect();
    }
    else if (command == "run")
    {
        payload = payload + '\r';
        mqttLog(commandHandler.processNow(payload.c_str(), payload.length()));
    }
    else if (command == "configure")
    {
        DynamicJsonDocument doc(1024);

        Serial << ">>" << payload << "<<" << endl;

        DeserializationError error = deserializeJson(doc, payload);

        // Test if parsing succeeds.
        if (error)
        {
            String sDoc;
            doc["command"] = "configure";
            doc["error"] = error.c_str();
            serializeJson(doc, sDoc);
            mqttLog(sDoc);
        }
        else
        {
            Serial << "invoke configure !!!" << endl;
            ezri->configure(doc);
            mqttLog("Configure: done !!!");
        }
    }
    else if (ptr_subscriber_extension)
    {
        Serial << _F("execute external MQTT callback") << endl;
        (*ptr_subscriber_extension)(command, payload);
    }
    else
    {
        mqtt.publish(F(MQTT_HEADER) + ezri->getChipID() + "/unknown", "Command <" + content + "> unknown !!!");
    }

    return 0;
}

void startMqttClient()
{
    procTimer.stop();

    // 1. [Setup]
    if (!mqtt.setWill(F(MQTT_HEADER) + ezri->getChipID() + "/will", F("The connection from this device is lost:("),
                      MqttClient::getFlags(MQTT_QOS_AT_LEAST_ONCE, MQTT_RETAIN_TRUE)))
    {
        debugf("Unable to set the last will and testament. Most probably there is not enough memory on the device.");
    }

    mqtt.setEventHandler(MQTT_TYPE_PUBACK, onMessageDelivered);

    mqtt.setConnectedHandler([](MqttClient &client, mqtt_message_t *message)
                             {
		Serial << _F("Connected to ") << client.getRemoteIp() << endl;

		// Start publishing message now
		checkMQTTConnect();
		// and schedule a timer to send messages every 5 seconds
		procTimer.initializeMs(5 * 1000, myHelo).start();
		return 0; });

    mqtt.setCompleteDelegate(checkMQTTDisconnect);
    mqtt.setMessageHandler(onMessageReceived);

    // 2. [Connect]
    Url url(ezri->getMqttURL());
    Serial << _F("Connecting to ") << url << endl;
    mqtt.connect(url, ezri->getChipID());
    mqtt.subscribe(F(MQTT_HEADER) + ezri->getChipID() + "/" + "command/#");
    mqtt.subscribe(F(MQTT_HEADER) + "command/#");
}
