/**
 * @file tmp123.h
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @version 0.1
 * @date 2020-10-15
 * @brief Structures and prototypes for TI TMP123 interface driver (SPI)
 * 
 * @copyright Copyright (c) 2020
 * 
 * @license GPL v3
 */ 

#ifndef TMP123_H
#define TMP123_H
#include <stdint.h>
#include <spibus/spibus.h>

#define TMP123_REDUCTION_FACTOR_INT 0x8 /// Divide by this number to get temperatures in units of 0.0625 C
#define TMP123_REDUCTION_TO_CELSIUS 0.0625 /// Multiply by this number to get temperature from reduced int to C
#define TMP123_CONVERSION 0.0625/0x8 /// Multiply by this number to get temperature in C
#define TMP123_ERROR -5600 /// Minimum temperature read is -55 C, so -5600 as return code indicates error

/**
 * @brief Device descriptor for TI TMP123 temperature sensor
 */ 
typedef struct __attribute__((packed))
{
    spibus bus[1];  /// SPI Bus
    int16_t temp; /// temperature
} tmp123;

/**
 * @brief Function to initialize a TMP123 device on the SPI bus.
 * 
 * @param dev Pointer to struct for the device
 * @param spi_bus Bus ID of the SPI bus on which the device is located (X in /dev/spidevX.Y)
 * @param spi_cs HW chip select ID of the device (Y in /dev/spidevX.Y)
 * @param gpio_cs GPIO chip select ID of the device (pin number defined in gpiodev LUT)
 * Non-negative for GPIO based chip select, negative for hardware chipselect.
 * 
 * @returns Positive on success, negative on error.
 */ 
int tmp123_init(tmp123 *dev, unsigned spi_bus, unsigned spi_cs, int gpio_cs);

/**
 * @brief Function to read temperature from the device. This function
 * stores the readout in the units of 0.0625 C in the internal temp variable
 * and returns the temperature in units of 0.01 Celsius (temperature = return/100)
 * as an integer.
 * 
 * @param dev Pointer to struct for the device
 * 
 * @returns Temperature in units of 0.01 Celsius
 */ 
int tmp123_read(tmp123 *dev);

/**
 * @brief Destroy the device struct and close the SPI bus. Memory management is
 * left to the caller.
 */ 
void tmp123_destroy(tmp123 *dev);
#endif // TMP123_H