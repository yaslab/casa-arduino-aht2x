#include <Wire.h>
#include <AHT2x.h>

AHT2x AHT2x;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  AHT2x.begin();
}

void loop() {
  double temperature;
  double humidity;
  if (AHT2x.measure(temperature, humidity)) {
    Serial.printf("temperature:%.3f,humidity:%.3f\n", temperature, humidity);
  } else {
    Serial.println("error: failed to measure values.");
  }

  delay(5000);
}
