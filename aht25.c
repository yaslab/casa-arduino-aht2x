#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "aht25.h"

enum AHT25_COMMAND {
    AHT25_COMMAND_INIT = 0x71,
    AHT25_COMMAND_TRIGGER_MEASURMENT = 0xac,
};

static uint8_t aht25_crc8(uint8_t *data, int count);

bool aht25_open(struct aht25_t *aht25, int bus, uint8_t address) {
    char device[128];
    sprintf(device, "/dev/i2c-%d", bus);

    int fd = open(device, O_RDWR);
    if (fd < 0) {
        return false;
    }

    aht25->fd = fd;
    aht25->address = address;

    return true;
}

bool aht25_close(const struct aht25_t *aht25) {
    return close(aht25->fd) == 0;
}

bool aht25_init(const struct aht25_t *aht25) {
    usleep(100 * 1000); // 100 ms

    uint8_t command = AHT25_COMMAND_INIT;
    uint8_t status = 0;

    struct i2c_msg messages[] = {
        { aht25->address, 0,        1, &command }, // WR
        { aht25->address, I2C_M_RD, 1, &status  }, // RD
    };

    struct i2c_rdwr_ioctl_data data = { messages, 2 };

    if (ioctl(aht25->fd, I2C_RDWR, &data) != 2) {
        return false;
    }

    if ((status & 0x18) != 0x18) {
        return false;
    }

    return true;
}

bool aht25_get_values(const struct aht25_t *aht25, double *temperature, double *humidity) {
    usleep(10 * 1000); // 10 ms

    {
        uint8_t tx[] = { AHT25_COMMAND_TRIGGER_MEASURMENT, 0x33, 0x00 };

        struct i2c_msg messages[] = {
            { aht25->address, 0, 3, tx }, // WR
        };

        struct i2c_rdwr_ioctl_data data = { messages, 1 };

        if (ioctl(aht25->fd, I2C_RDWR, &data) != 1) {
            return false;
        }
    }

    usleep(80 * 1000); // 80 ms

    {
        uint8_t rx[7];

        struct i2c_msg messages[] = {
            { aht25->address, I2C_M_RD, 7, rx }, // RD
        };

        struct i2c_rdwr_ioctl_data data = { messages, 1 };

        if (ioctl(aht25->fd, I2C_RDWR, &data) != 1) {
            return false;
        }

        if (rx[6] != aht25_crc8(rx, 6)) {
            return false;
        }

        bool busy = rx[0] & 0x80;
        //bool calibrated = rx[0] & 0x08;

        if (busy) {
            // TODO: retry
            return false;
        }

        double _humidity = (rx[1] << 12) | (rx[2] << 4) | (rx[3] >> 4);
        _humidity = (_humidity / pow(2.0, 20.0)) * 100.0;

        double _temperature = ((rx[3] & 0x0f) << 16) | (rx[4] << 8) | rx[5];
        _temperature = (_temperature / pow(2.0, 20.0)) * 200.0 - 50.0;

        if (humidity != NULL) {
            *humidity = _humidity;
        }

        if (temperature != NULL) {
            *temperature = _temperature;
        }
    }

    return true;
}

static uint8_t aht25_crc8(uint8_t *data, int count) {
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
