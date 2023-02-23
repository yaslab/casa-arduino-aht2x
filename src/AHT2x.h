#ifndef AHT2x_h
#define AHT2x_h

#include <cstddef>
#include <cstdint>
#include <string>

#if defined(ARDUINO)
#include <Wire.h>
#endif

class AHT2x {
#if defined(ARDUINO)
public:
  AHT2x(TwoWire &wire = Wire)
      : _wire(&wire) {}

private:
  TwoWire *_wire;
#endif

#if defined(__linux__)
public:
  AHT2x(uint8_t bus = 1)
      : _path("/dev/i2c-" + std::to_string(bus)) {}
  AHT2x(const std::string &path)
      : _path(path) {}

private:
  std::string _path;
#endif

public:
  bool begin();

  bool init();
  bool measure(double &temperature, double &humidity);
  bool reset();

protected:
  virtual size_t read(uint8_t address, uint8_t *data, size_t count);
  virtual size_t write(uint8_t address, const uint8_t *data, size_t count);

  virtual void delay(uint32_t ms);

private:
  uint8_t crc8(const uint8_t *data, int count);
};

#endif
