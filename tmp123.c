#include <spibus/spibus.h>
#include <stdlib.h>
#include <stdio.h>
#include "tmp123.h"

int tmp123_init(tmp123 *dev, unsigned spi_bus, unsigned spi_cs, int gpio_cs)
{
    dev->bus = (spibus *)malloc(sizeof(spibus));

    dev->bus->bus = spi_bus;
    dev->bus->cs = spi_cs;
    dev->bus->mode = SPI_MODE_0;
    dev->bus->bits = 8;
    dev->bus->speed = 1000000;
    dev->bus->lsb = 0;
    dev->bus->sleeplen = 0;

    if (gpio_cs < 0)
        dev->bus->cs_internal = 1;
    else
        dev->bus->cs_internal = 0;
    dev->bus->cs_gpio = gpio_cs;

    return spibus_init(dev->bus);
}

int tmp123_read(tmp123 *dev)
{
    static unsigned char b[2] = {0, 0};
    int status = spibus_xfer_full(dev->bus, b, 2, (uint8_t *)(&(dev->temp)), 2);
    if (status < 0)
    {
        fprintf(stderr, "TMP123: Temperature readout error\n");
        return -5600;
    }
    return (dev->temp * 6.25 / 8);
}

void tmp123_destroy(tmp123 *dev)
{
    spibus_destroy(dev->bus);
    free(dev->bus);
}

#ifdef UNIT_TEST_TMP123

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t done = 0;

void sighandler(int sig)
{
    done = 1;
}

#define NUM_CS 7

int main(int argc, char *argv[])
{
    signal(SIGINT, &sighandler);
    int tmp_cs[NUM_CS] = {
        0x0,
    };
    tmp123 **dev = (tmp123 **)malloc(NUM_CS * sizeof(tmp123 *));
    for (int i = 0; i < NUM_CS; i++)
    {
        dev[i] = (tmp123 *)malloc(sizeof(tmp123));
        tmp_cs[i] = i + 8;
        int stat = tmp123_init(dev[i], 1, 2, tmp_cs[i]);
        if (stat < 0)
        {
            printf("Error initializing tmpsensor error %d\n", stat);
            goto end;
        }
    }
    while (!done)
    {
        printf("Temperature sensor readout: ");
        for (int i = 0; i < NUM_CS; i++)
            printf("%.2f C ", tmp123_read(dev[i])/100.0);
        fflush(stdout);
        sleep(1);
        printf("\r");
    }
    printf("\n");
end:
    for (int i = 0; i < NUM_CS; i++)
        free(dev[i]);
    free(dev);
    return 0;
}
#endif // UNIT_TEST