/*
 * SSD1306.c
 *
 *  Created on: Dec 7, 2019
 *      Author: janus
 */


#include "SSD1306.h"

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];


typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

uint8_t initDisplay()
{
	i2c_driver_init();

	if(i2cIsDeviceReady(SSD1306_address) != HAL_OK)
	{
		return 1;
	}

	i2cWriteByte(SSD1306_address, 0x00, displayOff);
	i2cWriteByte(SSD1306_address, 0x00, 0x20); //Set Memory Addressing Mode
	i2cWriteByte(SSD1306_address, 0x00, 0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	i2cWriteByte(SSD1306_address, 0x00, 0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	i2cWriteByte(SSD1306_address, 0x00, 0xC8); //Set COM Output Scan Direction
	i2cWriteByte(SSD1306_address, 0x00, 0x00); //---set low column address
	i2cWriteByte(SSD1306_address, 0x00, 0x10); //---set high column address
	i2cWriteByte(SSD1306_address, 0x00, 0x40); //--set start line address
	i2cWriteByte(SSD1306_address, 0x00, contrastRegister);
	i2cWriteByte(SSD1306_address, 0x00, maxContrast);
	i2cWriteByte(SSD1306_address, 0x00, 0xA1); //--set segment re-map 0 to 127
	i2cWriteByte(SSD1306_address, 0x00, 0xA6); //--set normal display
	i2cWriteByte(SSD1306_address, 0x00, 0xA8); //--set multiplex ratio(1 to 64)
	i2cWriteByte(SSD1306_address, 0x00, 0x3F); //
	i2cWriteByte(SSD1306_address, 0x00, 0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	i2cWriteByte(SSD1306_address, 0x00, 0xD3); //-set display offset
	i2cWriteByte(SSD1306_address, 0x00, 0x00); //-not offset
	i2cWriteByte(SSD1306_address, 0x00, 0xD5); //--set display clock divide ratio/oscillator frequency
	i2cWriteByte(SSD1306_address, 0x00, 0xF0); //--set divide ratio
	i2cWriteByte(SSD1306_address, 0x00, 0xD9); //--set pre-charge period
	i2cWriteByte(SSD1306_address, 0x00, 0x22); //
	i2cWriteByte(SSD1306_address, 0x00, 0xDA); //--set com pins hardware configuration
	i2cWriteByte(SSD1306_address, 0x00, 0x12);
	i2cWriteByte(SSD1306_address, 0x00, 0xDB); //--set vcomh
	i2cWriteByte(SSD1306_address, 0x00, 0x20); //0x20,0.77xVcc
	i2cWriteByte(SSD1306_address, 0x00, DCDC_register);
	i2cWriteByte(SSD1306_address, 0x00, enableDCDC);
	i2cWriteByte(SSD1306_address, 0x00, displayOn);
	SSD1306_clear();
	SSD1306_UpdateScreen();

	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;

	SSD1306.Initialized = 1;


	return 0;
}

void SSD1306_ON(void) {
	i2cWriteByte(SSD1306_address, 0x00, DCDC_register);
	i2cWriteByte(SSD1306_address, 0x00, enableDCDC);
	i2cWriteByte(SSD1306_address, 0x00, displayOn);
}
void SSD1306_OFF(void) {
	i2cWriteByte(SSD1306_address, 0x00, DCDC_register);
	i2cWriteByte(SSD1306_address, 0x00, disableDCDC);
	i2cWriteByte(SSD1306_address, 0x00, displayOff);
}

void SSD1306_clear() {
	/* Set memory */
	SSD1306_fillScreen(SSD1306_COLOR_BLACK);
}

void SSD1306_white() {
	/* Set memory */
	SSD1306_fillScreen(SSD1306_COLOR_WHITE);
}

void SSD1306_fillScreen(SSD1306_COLOR_t color) {
	/* Set memory */
	memset(SSD1306_Buffer, color==SSD1306_COLOR_BLACK ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void SSD1306_UpdateScreen() {
	uint8_t m;

	for (m = 0; m < 8; m++) {
		i2cWriteByte(SSD1306_address, 0x00, 0xB0 + m);
		i2cWriteByte(SSD1306_address, 0x00, 0x00);
		i2cWriteByte(SSD1306_address, 0x00, 0x10);

		/* Write multi data */
		i2cWriteReg(SSD1306_address,0x40, &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}

char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}

	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}

	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;

	/* Return character written */
	return ch;
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}

	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}

	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1306_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}

		/* Increase string pointer */
		str++;
	}

	/* Everything OK, zero should be returned */
	return *str;
}
