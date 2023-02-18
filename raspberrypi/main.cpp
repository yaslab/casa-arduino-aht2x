#include <cstdio>
#include <AHT2x.h>

#include "raspberrypi.h"

static Payload _payload;

static AHT2x _aht2x(
  raspberrypi_read, 
  raspberrypi_write, 
  raspberrypi_delay, 
  &_payload);

static void setup() {
  int i2c_bus = 1;
  sprintf(_payload.path, "/dev/i2c-%d", i2c_bus);
  _aht2x.begin();
}

static void loop() {
  double temperature;
  double humidity;
  if (_aht2x.measure(temperature, humidity)) {
    printf("temperature = %.3f, humidity = %.3f\n", temperature, humidity);
  }

  raspberrypi_delay(5000);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setup();
  for (;;) { loop(); }
  return 0;
}
