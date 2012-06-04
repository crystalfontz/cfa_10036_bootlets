// This file is used to configure the CFA10036 board during power on
// We set up the power LED, debug ports, OLED PWM power.
#include "CFA10036_board.h"

void init_CFA10036( void) {

// turn on the power indicator LED
	mxs_iomux_setup_pad(POWER_LED |
                        MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP);
        gpio_direction_output(POWER_LED, 0);

// Put the OLED in the reset state
	mxs_iomux_setup_pad(OLED_RESET |
                        MXS_PAD_12MA | MXS_PAD_3V3 | MXS_PAD_PULLUP);
        gpio_direction_output(OLED_RESET, 0);


// Move the DUART pins so that we can use I2C1
	mxs_iomux_setup_pad(DUART_RX |
                        MXS_PAD_12MA | MXS_PAD_3V3);
	mxs_iomux_setup_pad(DUART_TX |
                        MXS_PAD_12MA | MXS_PAD_3V3);

	printf("\r\ndebugging on alternate pins\r\n");
// Set up the I2C1 channel SLC and SDA

	mxs_iomux_setup_pad(I2C1_SCL |
                        MXS_PAD_12MA | MXS_PAD_3V3);

	mxs_iomux_setup_pad(I2C1_SDA |
                        MXS_PAD_12MA | MXS_PAD_3V3);
	printf("I2C1 is now configured\n");

}
