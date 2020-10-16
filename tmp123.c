#include <spibus/spibus.h>
#include <stdlib.h>
#include <stdio.h>
#include "tmp123.h"

int tmp123_init(tmp123 *dev, unsigned spibus, unsigned spics, int gpiocs)
{
    dev->bus = malloc (sizeof(spibus));

    dev->bus->bus = spibus;
    dev->bus->cs = spics;
    dev->bus->mode = SPI_MODE_0;
    dev->bus->bits = 8;
    dev->bus->speed = 1000000;
    dev->bus->lsb = 0;
    dev->bus->sleeplen = 0;

    if (gpiocs < 0)
        dev->bus->cs_internal = 1;
    else
        dev->bus->cs_internal = 0;
    dev->bus->cs_gpio = gpiocs;

    return spibus_init(dev->bus);
}

int tmp123_read(tmp123 *dev)
{
    unsigned char b[2], in[2];
    int status = spibus_xfer_full(dev->bus, b, 2, in, 2);
    if (status < 0)
    {
        fprintf(stderr, "TMP123: Temperature readout error\n");
        return -5600;
    }
    dev->temp = in[0] | in[1] << 8;
    return (dev->temp / 8 * 6.25);
}

void tmp123_destroy(tmp123 *dev)
{
    spibus_destroy(dev->bus);
    free(dev->bus);
}