/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <board.h>
#include <conf_board.h>
#include <gpio.h>
#include <spi.h>

void initSpi0(void);

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
    
    /* Use LED on PA4 as an output - NOTE: STK600 inverts output to LEDs */
    gpio_configure_pin(AVR32_PIN_PA04,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    //gpio_toggle_pin(AVR32_PIN_PA04);
    
    /* Use LED on PA7 as an output - NOTE: STK600 inverts output to LEDs */
    gpio_configure_pin(AVR32_PIN_PA07,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    //gpio_toggle_pin(AVR32_PIN_PA07);

#if 0    
    gpio_configure_pin(AVR32_PIN_PD03, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    while(1)
    {
        gpio_clr_gpio_pin(AVR32_PIN_PD03);
        gpio_set_gpio_pin(AVR32_PIN_PD03);
        gpio_clr_gpio_pin(AVR32_PIN_PD03);
        gpio_set_gpio_pin(AVR32_PIN_PD03);
    }
    
    /* Test for SPI interface */
    gpio_configure_pin(AVR32_PIN_PD03, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    while(1)
    {
        gpio_set_pin_high(AVR32_PIN_PD03);
        gpio_set_pin_low(AVR32_PIN_PD03);
        gpio_set_pin_high(AVR32_PIN_PD03);
        gpio_set_pin_low(AVR32_PIN_PD03);
    }
    /* Test for SPI interface */
    gpio_configure_pin(AVR32_PIN_PD03, GPIO_DIR_INPUT | GPIO_PULL_UP);
    while(1)
    {
        if (gpio_get_pin_value(AVR32_PIN_PD03) == 1)
        {
            gpio_set_pin_high(AVR32_PIN_PA07);
        }
        else if (gpio_get_pin_value(AVR32_PIN_PD03) == 0)
        {
            gpio_set_pin_low(AVR32_PIN_PA07);
        }
    }
#endif
    /* Initialise the SPI0 on PD03/04/05/06 in Slave Mode */
    initSpi0();
    
}

void initSpi0(void)
{
    uint16_t spi0Data;
    
    /* Use SPI0 with pins PD03/04/05/06 */
    static const gpio_map_t SPI0_SPIO_MAP =
    {
        {AVR32_SPI0_NPCS_0_0_PIN, AVR32_SPI0_NPCS_0_0_FUNCTION},    // CS
        {AVR32_SPI0_SCK_0_1_PIN, AVR32_SPI0_SCK_0_1_FUNCTION},      // SCK
        {AVR32_SPI0_MISO_0_1_PIN, AVR32_SPI0_MISO_0_1_FUNCTION},    // MISO
        {AVR32_SPI0_MOSI_0_1_PIN, AVR32_SPI0_MOSI_0_1_FUNCTION}     // MOSI
    };
    
    /* Map the GPIOs */
    gpio_enable_module(SPI0_SPIO_MAP, sizeof(SPI0_SPIO_MAP) / sizeof(SPI0_SPIO_MAP[0]));
    
    /* Initialise the slave */
    spi_initSlave(&AVR32_SPI0, 8, SPI_MODE_0);
    
    /* Enable the SPI module */
    spi_enable(&AVR32_SPI0);
    
    /* Clear the buffer */
    spi_read(&AVR32_SPI0, &spi0Data);
}