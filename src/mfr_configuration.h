#include <ArduinoJson.h>
#include <EEPROM.h>

#ifndef MFR_CONFIGURATION_H_
#define MFR_CONFIGURATION_H_

#define _MFR_IDENTIFIER "MF1"

namespace mfr {
        class Configuration {
public:
                Configuration() : root(jsonBuffer.createObject()) {
                };

                void clear();
                void loadConfig();
                void saveConfig();

                void get(const String& key, String& value);
                void set(const String& key, const String& value);

private:
                static constexpr size_t kEEPROMSize = 512;

                struct eepromLayout {
                        char id[4];
                        char jsonString[kEEPROMSize - 4];
                } config = {
                        _MFR_IDENTIFIER,
                        ""
                };

                StaticJsonBuffer<kEEPROMSize> jsonBuffer;
                JsonObject& root;
        };
}

#endif // MFR_CONFIGURATION_H_
