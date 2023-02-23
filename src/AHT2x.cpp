#include <cmath>

#include "AHT2x.h"

#define I2C_ADDRESS_AHT2x (0x38)

bool AHT2x::begin() {
    return reset() && init();
}

bool AHT2x::init() {
    delay(100);

    {
        uint8_t tx_buffer[] = { 0x71 }; // init
        size_t result = write(I2C_ADDRESS_AHT2x, tx_buffer, sizeof(tx_buffer));
        if (result != sizeof(tx_buffer)) {
            return false;
        }
    }

    {
        uint8_t rx_buffer[1];
        size_t result = read(I2C_ADDRESS_AHT2x, rx_buffer, sizeof(rx_buffer));
        if (result != sizeof(rx_buffer)) {
            return false;
        }

        uint8_t status = rx_buffer[0];
        if ((status & 0x18) != 0x18) {
            return false;
        }
    }

    return true;
}

bool AHT2x::measure(double &temperature, double &humidity) {
    delay(10);

    {
        uint8_t tx_buffer[] = { 0xac, 0x33, 0x00 }; // trigger measurment
        size_t result = write(I2C_ADDRESS_AHT2x, tx_buffer, sizeof(tx_buffer));
        if (result != sizeof(tx_buffer)) {
            return false;
        }
    }

    for (int retry = 0; retry < 10; retry += 1) {
        delay(80);

        uint8_t rx_buffer[7];
        size_t result = read(I2C_ADDRESS_AHT2x, rx_buffer, sizeof(rx_buffer));
        if (result != sizeof(rx_buffer)) {
            return false;
        }

        uint8_t crc = crc8(rx_buffer, sizeof(rx_buffer) - 1);
        if (rx_buffer[6] != crc) {
            return false;
        }

        uint8_t measuring = (rx_buffer[0] >> 7) & 1;
        uint8_t calibrated = (rx_buffer[0] >> 3) & 1;
        if (!measuring && calibrated) {
            humidity = (rx_buffer[1] << 12) | (rx_buffer[2] << 4) | (rx_buffer[3] >> 4);
            humidity = (humidity / std::pow(2.0, 20.0)) * 100.0;

            temperature = ((rx_buffer[3] & 0x0f) << 16) | (rx_buffer[4] << 8) | rx_buffer[5];
            temperature = (temperature / std::pow(2.0, 20.0)) * 200.0 - 50.0;

            return true;
        }
    }

    return false;
}

bool AHT2x::reset() {
    uint8_t tx_buffer[] = { 0xba }; // soft reset
    size_t result = write(I2C_ADDRESS_AHT2x, tx_buffer, sizeof(tx_buffer));
    if (result != sizeof(tx_buffer)) {
        return false;
    }

    return true;
}

uint8_t AHT2x::crc8(const uint8_t *data, int count) {
    uint8_t crc = 0xff;

    for (int i = 0; i < count; i += 1) {
        crc ^= data[i];
        for (int j = 0; j < 8; j += 1) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}
