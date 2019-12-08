#pragma  once

#include <stdint.h>
#include <stdbool.h>

void i2c_driver_init(void);

bool i2cReadReg(uint8_t i2cDeviceAddress, uint8_t i2cRegisterAddres, uint8_t data[], uint16_t len);
bool i2cWriteReg(uint8_t i2cDeviceAddress, uint8_t i2cRegisterAddres, uint8_t data[], uint16_t len);
bool i2cWriteByte(uint8_t i2cDeviceAddress, uint8_t i2cRegisterAddres, uint8_t data);

bool i2cIsDeviceReady(uint8_t i2cDeviceAddress);
