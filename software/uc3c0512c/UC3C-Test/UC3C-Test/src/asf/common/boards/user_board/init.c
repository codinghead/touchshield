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
#include <flashc.h>
#include <pm_uc3c.h>
#include <scif_uc3c.h>

void initSpi0(void);
void initClocks(void);

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

    /* Initialise the clock and flash wait states */
    initClocks();

    /* Initialise the SPI0 on PD03/04/05/06 in Slave Mode */
    initSpi0();
    
}

void initClocks(void)
{
    /* Start the 120MHz internal RCosc (RC120M) */
    scif_start_rc120M();
    
    /* We will use a 60MHz clock - need 1 wait state for flash memory */
    flashc_set_wait_state(1);
    
    /* Set CPU clock domain to 120MHz / 2 = 60MHz */
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_CPU, PM_CKSEL_DIVRATIO_2);
    
    /* Set Peripheral Bus A to same frequency */
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBA, PM_CKSEL_DIVRATIO_2);
    
    /* Set Peripheral Bus B to same frequency (needed for FLASHC configuration) */
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBB, PM_CKSEL_DIVRATIO_2);
    
    /* Set Peripheral Bus C to same frequency (needed for SPI0 configuration) */
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBC, PM_CKSEL_DIVRATIO_2);
    
    /* Switch to the RC120M oscillator as clock source */
    pm_set_mclk_source(PM_CLK_SRC_RC120M);
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