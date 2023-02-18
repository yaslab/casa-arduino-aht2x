#ifndef AHT2x_h
#define AHT2x_h

#include <cstddef>
#include <cstdint>

class AHT2x {
public:
    AHT2x(
        size_t(*read)(uint8_t, uint8_t *, size_t, void *), 
        size_t(*write)(uint8_t, const uint8_t *, size_t, void *), 
        void(*delay)(uint32_t),
        void *opaque
    ) : _read(read), _write(write), _delay(delay), _opaque(opaque) {};
#if defined(ARDUINO)
    AHT2x();
#endif

    bool begin();

    bool init();
    bool measure(double &temperature, double &humidity);
    bool reset();

private:
    size_t(*_read)(uint8_t address, uint8_t *data, size_t count, void *opaque);
    size_t(*_write)(uint8_t address, const uint8_t *data, size_t count, void *opaque);
    void(*_delay)(uint32_t ms);
    void *_opaque;

    uint8_t crc8(const uint8_t *data, int count);
};

#endif
