#include <AHT2x.h>

AHT2x AHT2x;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  AHT2x.begin();
}

void loop() {
  double temperature, humidity;
  if (AHT2x.measure(temperature, humidity)) {
    Serial.printf("temperature:%.3f,humidity:%.3f\n", temperature, humidity);
  } else {
    Serial.printf("error: failed to measure values.\n");
  }

  delay(5000);
}
