#pragma once
#include <string.h>
#include "fonts.h"
#include "i2cDriver.h"

#define SSD1306_address ((uint8_t)(0x78))
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64

typedef enum {
	SSD1306_COLOR_BLACK = 0x00, // Black
	SSD1306_COLOR_WHITE = 0x01  // white
} SSD1306_COLOR_t;

uint8_t initDisplay();

void SSD1306_clear();

void SSD1306_white();

void SSD1306_fillScreen(SSD1306_COLOR_t color);

void SSD1306_UpdateScreen();

void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);

char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);

char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);
