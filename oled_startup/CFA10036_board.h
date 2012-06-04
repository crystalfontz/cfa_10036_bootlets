// this is the header file mapping iMX28 pins to CFA10036 specific functionality

#ifndef CFA10036_BOARD_H
#define CFA10036_BOARD_H

//remap the power pin to GPIO3_29 if we are on the rev 0 board
#ifdef CFA10036_V1
#define POWER_LED	MX28_PAD_AUART1_RX__GPIO_3
#define OLED_POWER	MX28_PAD_PWM4__PWM_4
#define DUART_RX	MX28_PAD_AUART0_CTS__DUART_RX
#define DUART_TX	MX28_PAD_AUART0_RTS__DUART_TX
#else
//remap the power pin to GPIO3_29 if we are on the rev 0 board
#define POWER_LED 	MX28_PAD_PWM4__GPIO_3_29
#endif //CFA10036_BOARD_H
