#include "i2cDriver.h"
#include "stm32f0xx_hal.h"

static I2C_HandleTypeDef hi2c;

void i2c_driver_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**I2C1 GPIO Configuration
	 PB8     ------> I2C1_SCL
	 PB9     ------> I2C1_SDA
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* I2C1 clock enable */
	__HAL_RCC_I2C1_CLK_ENABLE();

	hi2c.Instance = I2C1;
	hi2c.Init.Timing = 0x2000090E;
	hi2c.Init.OwnAddress1 = 0;
	hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c.Init.OwnAddress2 = 0;
	hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c, 0) != HAL_OK)
	{
		Error_Handler();
	}
}


bool i2cReadReg(uint8_t i2cDeviceAddress, uint8_t i2cRegisterAddres, uint8_t data[], uint16_t len)
{
	return HAL_I2C_Mem_Read(&hi2c, i2cDeviceAddress, i2cRegisterAddres,I2C_MEMADD_SIZE_8BIT, data, len, 10000) == HAL_OK;
}
bool i2cWriteReg(uint8_t i2cDeviceAddress, uint8_t i2cRegisterAddres, uint8_t data[], uint16_t len)
{
	return HAL_I2C_Mem_Write(&hi2c, i2cDeviceAddress, i2cRegisterAddres, I2C_MEMADD_SIZE_8BIT, data, len, 10000) == HAL_OK;
}
bool i2cWriteByte(uint8_t i2cDeviceAddress, uint8_t i2cRegisterAddres, uint8_t data)
{
	uint8_t val = data;
	return i2cWriteReg(i2cDeviceAddress, i2cRegisterAddres, &val, 1);
}

bool i2cIsDeviceReady(uint8_t i2cDeviceAddress)
{
	return HAL_I2C_IsDeviceReady(&hi2c, i2cDeviceAddress, 10, 1000) == HAL_OK;
}
