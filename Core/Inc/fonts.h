/*
 * fonts.h
 *
 *  Created on: Dec 7, 2019
 *      Author: janus
 */

#pragma once

#include "string.h"
#include "stm32f0xx_hal.h"

typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;

extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;

char* font_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);


