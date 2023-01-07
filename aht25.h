#ifndef AHT25_H
#define AHT25_H

#include <stdint.h>
#include <stdbool.h>

#define AHT25_ADDRESS (0x38)

struct aht25_t {
    int fd;
    uint8_t address;
};

bool aht25_open(struct aht25_t *aht25, int bus, uint8_t address);
bool aht25_close(const struct aht25_t *aht25);

bool aht25_init(const struct aht25_t *aht25);
bool aht25_get_values(const struct aht25_t *aht25, double *temperature, double *humidity);

#endif /* AHT25_H */
