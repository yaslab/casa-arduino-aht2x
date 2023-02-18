#if defined(ARDUINO)

#include <Wire.h>

#include "AHT2x.h"

static size_t arduino_read(uint8_t address, uint8_t *data, size_t count, void *opaque) {
    (void)opaque;
    Wire.requestFrom(address, count);
    return Wire.readBytes(data, count);
}

static size_t arduino_write(uint8_t address, const uint8_t *data, size_t count, void *opaque) {
    (void)opaque;
    Wire.beginTransmission(address);
    size_t written = Wire.write(data, count);
    Wire.endTransmission();
    return written;
}

static void arduino_delay(uint32_t ms) {
    delay(ms);
}

AHT2x::AHT2x()
    : AHT2x(arduino_read, arduino_write, arduino_delay, nullptr) {}

#endif
