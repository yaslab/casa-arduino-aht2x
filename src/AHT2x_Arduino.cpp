#if defined(ARDUINO)

#include "AHT2x.h"

size_t AHT2x::read(uint8_t address, uint8_t *data, size_t count) {
    _wire->requestFrom(address, count);
    return _wire->readBytes(data, count);
}

size_t AHT2x::write(uint8_t address, const uint8_t *data, size_t count) {
    _wire->beginTransmission(address);
    size_t written = _wire->write(data, count);
    _wire->endTransmission();
    return written;
}

void AHT2x::delay(uint32_t ms) {
    ::delay(ms);
}

#endif
