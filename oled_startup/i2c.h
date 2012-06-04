#ifndef INCLUDE_I2C_H
#define INCLUDE_I2C_H
#include "types.h"

extern void i2c_init(int speed);
extern int i2c_probe(uint8_t chip);
extern int i2c_write(uint8_t chip, uint32_t addr, int alen, uint8_t *buffer, int len);
extern int i2c_read(uint8_t chip, uint32_t addr, int alen, uint8_t *buffer, int len);
#endif //INCLUDE_I2C_H
