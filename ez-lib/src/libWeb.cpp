#include "lib.h"

HttpServer server;

void onIndex(HttpRequest &request, HttpResponse &response)
{
    Serial << "HomePage access" << endl;
	auto tmpl = new TemplateFileStream(F("index.html"));
	auto& vars = tmpl->variables();
    // vars["counter"] = String(counter);
    response.sendNamedStream(tmpl); // this template object will be deleted automatically
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.uri.getRelativePath();

	if(file[0] == '.') {
		response.code = HTTP_STATUS_FORBIDDEN;
	} else {
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

int msgCount = 0;

void wsConnected(WebsocketConnection &socket)
{
    // Serial.println(_F("Socket connected"));
}

void wsMessageReceived(WebsocketConnection &socket, const String &message)
{
    // DynamicJsonDocument doc(1024);

    // Serial << message << endl;

    // DeserializationError error = deserializeJson(doc, message);

    // // Test if parsing succeeds.
    // if (error)
    // {
    //     String sDoc;
    //     doc["error"] = error.c_str();
    //     serializeJson(doc, sDoc);
    //     socket.sendString(sDoc);
    //     return;
    // }
    // if (doc["header"] == "configure")
    // {
    //     Serial << "invoke configure !!!" << endl;

    //     DynamicJsonDocument jObj = doc["file"];
    //     ezri->configure(jObj);

    //     return;
    // }
    // String header = doc["header"].as<const char *>();
    // String cmd = doc["command"].as<const char *>();
    // String param = doc["param"].as<const char *>();

    // String command = header + " " + cmd + " " + param + '\r';

    // String response = runCommand(message, CMD_OUTPUT_TEXT);
    // String response = commandHandler.processNow(message.c_str(), message.length());

    Serial << "<<" << message << ">>\r" << endl;
    String command  = message + "\r";
    
    socket.sendString(commandHandler.processNow(command.c_str(), command.length()));

    return;


	// String response = commandHandler.processNow(message.c_str(), message.length());
	// socket.sendString(response);

	// //Normally you would use dynamic cast but just be careful not to convert to wrong object type!
	// auto user = reinterpret_cast<CUserData*>(socket.getUserData());
	// if(user != nullptr) {
	// 	user->printMessage(socket, message);
	// }

}

void wsBinaryReceived(WebsocketConnection &socket, uint8_t *data, size_t size)
{
    Serial << _F("Websocket binary data received, size: ") << size << endl;
}

void wsDisconnected(WebsocketConnection &socket)
{
    Serial.println(_F("Socket disconnected"));
}

void startWeb()
{
    server.listen(80);
    server.paths.set("/", onIndex);
    server.paths.setDefault(onFile);

    auto wsResource = new WebsocketResource();
    wsResource->setConnectionHandler(wsConnected);
    wsResource->setMessageHandler(wsMessageReceived);
    wsResource->setBinaryHandler(wsBinaryReceived);
    wsResource->setDisconnectionHandler(wsDisconnected);
    server.paths.set("/ws", wsResource);

    Serial.println(_F("\r\n=== WEB SERVER STARTED ==="));
    Serial.println(WifiStation.getIP());
    Serial.println(_F("==============================\r\n"));
}
