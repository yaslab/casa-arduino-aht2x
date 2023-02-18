#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "raspberrypi.h"

size_t raspberrypi_read(uint8_t address, uint8_t *data, size_t count, void *opaque) {
  Payload *payload = (Payload *)opaque;
  int fd = open(payload->path, O_RDWR);
  if (fd < 0) {
    return 0;
  }

  ssize_t result = 0;

  if (ioctl(fd, I2C_SLAVE, address) == 0) {
    result = read(fd, data, count);
  }

  close(fd);

  return (result < 0) ? 0 : (size_t)result;
}

size_t raspberrypi_write(uint8_t address, const uint8_t *data, size_t count, void *opaque) {
  Payload *payload = (Payload *)opaque;
  int fd = open(payload->path, O_RDWR);
  if (fd < 0) {
    return 0;
  }

  ssize_t result = 0;

  if (ioctl(fd, I2C_SLAVE, address) == 0) {
    result = write(fd, data, count);
  }

  close(fd);

  return (result < 0) ? 0 : (size_t)result;
}

void raspberrypi_delay(uint32_t ms) {
  usleep(ms * 1000);
}
