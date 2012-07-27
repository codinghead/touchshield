/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>

uint16_t data[256];
uint16_t pointer = 0;

int main (void)
{
    uint16_t spi0Data;
    spi_status_t spiStatus;
    
	board_init();

	// Insert application code here, after the board has been initialized.
    
    while(1)
    {
        /* After initialisation, seems there is one NULL byte in the register */
        /* #TODO: check if we need to clear something else at initialisation */
        spiStatus = spi_read(&AVR32_SPI0, &spi0Data);
        data[pointer] = spi0Data;
        ++pointer;
        if (pointer > 255)
        {
            pointer = 0;
        }
        if (spiStatus == SPI_ERROR_TIMEOUT)
        {
            gpio_toggle_pin(AVR32_PIN_PA04);
        }
        if (spiStatus == SPI_OK)
        {
            gpio_toggle_pin(AVR32_PIN_PA07);
            gpio_set_gpio_pin(AVR32_PIN_PA04);
            spi_write(&AVR32_SPI0, spi0Data);
        }
    }
}
