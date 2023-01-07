#include <stdio.h>
#include <string.h>

#include "aht25.h"

static bool parse(int argc, char *argv[]);
static void usage();

bool arg_temperature;
bool arg_humidity;

int main(int argc, char *argv[]) {
    if (!parse(argc, argv)) {
        usage();
        return 1;
    }

    struct aht25_t aht25 = {};

    if (!aht25_open(&aht25, 1, AHT25_ADDRESS)) {
        fprintf(stderr, "error: open\n");
        return 1;
    }

    int status = 0;

    if (!aht25_init(&aht25)) {
        fprintf(stderr, "error: init\n");
        status = 1;
        goto CLEAN_UP;
    }

    double temperature = 0.0;
    double humidity = 0.0;
    if (!aht25_get_values(&aht25, &temperature, &humidity)) {
        fprintf(stderr, "error: get values\n");
        status = 1;
        goto CLEAN_UP;
    }

    char text[64] = "";
    char work[64];

    if (arg_temperature) {
        sprintf(work, "%.2f", temperature);
        strcat(text, work);
    }
    if (arg_humidity) {
        if (arg_temperature) {
            strcat(text, " ");
        }
        sprintf(work, "%.2f", humidity);
        strcat(text, work);
    }

    printf("%s\n", text);

CLEAN_UP:

    aht25_close(&aht25);

    return status;
}

static bool parse(int argc, char *argv[]) {
    if (argc <= 1) {
        arg_temperature = true;
    }
    for (int i = 1; i < argc; i += 1) {
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--temperature") == 0) {
            arg_temperature = true;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--humidity") == 0) {
            arg_humidity = true;
        } else {
            return false;
        }
    }
    return true;
}

static void usage() {
    fprintf(stderr, "syntax:\n");
    fprintf(stderr, "  aht25 [-t] [-h]\n");
    fprintf(stderr, "options:\n");
    fprintf(stderr, "  -t, --temperature : Prints the temperature.\n");
    fprintf(stderr, "  -h, --humidity    : Prints the humidity.\n");
}
