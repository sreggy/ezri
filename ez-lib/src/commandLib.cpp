#include <SmingCore.h>
#include "lib.h"

bool getline(FileStream &is, std::string &s, char delim = '\n')
{
    s.clear();
    char c;
    std::string temp;
    while ((is.readBytes(&c, 1)) && (c != delim))
    {
//        Serial << " read: " << c << endl;
        if (c != '\r')
            s.push_back(c);
    }

//    Serial << "line: " << s.c_str() << endl;

    bool fsStatus = is.isFinished();
    Serial << "fsStatus: " << fsStatus << endl;

    return fsStatus;
}
void dumpFile(String fileName, DynamicJsonDocument &_doc)
{

    _doc["filename"] = fileName;
    if (fileExist(fileName))
    {
        FileStream f;
        f.open(fileName);

        JsonArray ja = _doc.createNestedArray("file");
        std::string line;

        while (!getline(f, line, '\n'))
        {
            ja.add(line);
        }
        ja.add(line);
    }
    _doc["size"] = fileGetSize(fileName);
    return;
}

void listSpiffsPartitions(DynamicJsonDocument &_doc)
{
    // commandOutput->printf("** Enumerate registered SPIFFS partitions\r\n");
    for (auto part : Storage::findPartition(Storage::Partition::SubType::Data::spiffs))
    {

        // commandOutput->printf(">> Mounting '%s'\r\n", part.name().c_str());
        bool ok = spiffs_mount(part);
        // commandOutput->printf(ok ? "Listing files:\r\n" : "Mount failed!\r\n");
        if (ok)
        {
            _doc["partition"] = part.name();
            _doc["size"] = part.size();
            Directory dir;
            if (dir.open())
            {
                // commandOutput->println("-----------\r\n");
                JsonArray file = _doc.createNestedArray("file");

                while (dir.next())
                {
                    String fAdd;
                    fAdd = String(dir.stat().name) + " --> " + String(dir.stat().size);
                    file.add(fAdd);
                    // commandOutput->printf("%s\r\n", dir.stat().name.c_str());
                }
                // commandOutput->println("-----------\r\n");
            }
            _doc["count"] = dir.count();
            // commandOutput->printf("%d files found\r\n", dir.count());
        }
    }
    // String jDoc;
    // serializeJsonPretty(_doc, jDoc);
    // Serial << jDoc << endl;
    return;
}

// String formatOutput(DynamicJsonDocument doc, int format)
// {
//     String jDoc;
//     if (format == CMD_OUTPUT_JSON)
//     {

//         serializeJson(doc, jDoc);
//     }
//     else if (format == CMD_OUTPUT_TEXT)
//     {

//         serializeJsonPretty(doc, jDoc);
//     }
//     return jDoc;
// }

// String runCommand(String commandLine, int format)
// {

//     Vector<String> commandToken;
//     int numToken = splitString(commandLine, ' ', commandToken);

//     DynamicJsonDocument doc(2048);
//     String sDoc;

//     String command = commandToken[0];
//     command.toLowerCase();

//     debugf("Received %s - Token[0] >%s<\r\n", commandLine.c_str(), command);

//     if (command == "show")
//     {
//         commandShow.execCommand(commandLine, doc);
//     }
//     else if (command == "cfg")
//     {
//         commandCfg.execCommand(commandLine, doc);
//     }
//     else if (command == "ota")
//     {
//         commandOta.execCommand(commandLine, doc);
//     }
//     else
//     {
//         doc["error"] = "unknow command";
//         doc["command"] = command;
//     }

//     //    serializeJson(doc, sDoc);
//     return (formatOutput(doc, format));
// }
