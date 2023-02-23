#include <AHT2x.h>
#include <unistd.h>

#include <cstdio>

AHT2x AHT2x;

void setup() {
  AHT2x.begin();
}

void loop() {
  double temperature, humidity;
  if (AHT2x.measure(temperature, humidity)) {
    printf("temperature:%.3f,humidity:%.3f\n", temperature, humidity);
  } else {
    printf("error: failed to measure values.\n");
  }

  sleep(5);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  setup();
  for (;;) {
    loop();
  }
  return 0;
}
