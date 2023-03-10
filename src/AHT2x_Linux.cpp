#if defined(__linux__)

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "AHT2x.h"

size_t AHT2x::with_fd(uint8_t address, std::function<ssize_t(int)> body) {
  int fd = open(_path.c_str(), O_RDWR);
  if (fd < 0) {
    return 0;
  }

  ssize_t result = 0;

  if (ioctl(fd, I2C_SLAVE, address) == 0) {
    result = body(fd);
  }

  close(fd);

  return (result < 0) ? 0 : (size_t)result;
}

size_t AHT2x::read(uint8_t address, uint8_t *data, size_t count) {
  return with_fd(address, [&](int fd) {
    return ::read(fd, data, count);
  });
}

size_t AHT2x::write(uint8_t address, const uint8_t *data, size_t count) {
  return with_fd(address, [&](int fd) {
    return ::write(fd, data, count);
  });
}

void AHT2x::delay(uint32_t ms) {
  usleep(ms * 1000);
}

#endif
