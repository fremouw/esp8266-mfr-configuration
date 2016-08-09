#include "mfr_configuration.h"
#include <Arduino.h>

#define _MFR_CONFIGURATION_START 3

namespace mfr {
void Configuration::clear() {
    Serial.println("Configuration::clear()");

    for (int i = 0; i < kEEPROMSize; ++i) {
        EEPROM.write(i, 0x0);
    }

    EEPROM.commit();

    EEPROM.end();
}

void Configuration::loadConfig() {
    Serial.println("Configuration::loadConfig()");

    EEPROM.begin(kEEPROMSize);

    if ((EEPROM.read(0) == _MFR_IDENTIFIER[0]) &&
        (EEPROM.read(1) == _MFR_IDENTIFIER[1]) &&
        (EEPROM.read(2) == _MFR_IDENTIFIER[2])) {
        for (size_t t = _MFR_CONFIGURATION_START; t < sizeof(config); t++) {
            *((char *)&config + t) = EEPROM.read(t);
        }
    } else {
        Serial.println("No Config.");
    }

    JsonObject& _root = jsonBuffer.parseObject(config.jsonString);

    for (auto & it : _root) {
        // We create a copy of the key/value pair.
        root.set(String(it.key), String(it.value.asString()));
    }
}

void Configuration::saveConfig() {
    //
    memset(config.jsonString, 0x0, sizeof(config.jsonString));

    //
    root.printTo(config.jsonString, sizeof(config.jsonString));

    //
    size_t size = (sizeof(config) - sizeof(config.jsonString)) + strlen(
        config.jsonString);

    for (size_t j = 0; j < size; j++) {
        EEPROM.write(j, *((char *)&config + j));
    }

    EEPROM.commit();

    EEPROM.end();
}

void Configuration::get(const String& key, String& value) {
    const String v = root[key];

    value = v;
}

void Configuration::set(const String& key, const String& value) {
    root[key] = String(value);
}
}
