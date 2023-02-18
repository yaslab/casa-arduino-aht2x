#ifndef raspberrypi_h
#define raspberrypi_h

#include <cstddef>
#include <cstdint>

struct Payload {
  char path[128];
};

size_t raspberrypi_read(uint8_t address, uint8_t *data, size_t count, void *opaque);
size_t raspberrypi_write(uint8_t address, const uint8_t *data, size_t count, void *opaque);
void raspberrypi_delay(uint32_t ms);

#endif
