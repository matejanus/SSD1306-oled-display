/*
 * HTS221.h
 *
 *  Created on: Dec 8, 2019
 *      Author: janus
 */


#pragma once
#include "i2cDriver.h"

#define HTS221_ADDR   	((uint8_t)0xBE)

enum HTS221_registers
{
	WHO_AM_I = 0x0F,
	AV_CONF = 0x10,
	CTRL_REG1 = 0x20,
	CTRL_REG2 = 0x21,
	CTRL_REG3 = 0x22,
	STATUS_REG = 0x27,
	HUMIDITY_OUT_L = 0x28,
	HUMIDITY_OUT_H = 0x29,
	TEMP_OUT_L = 0x2A,
	TEMP_OUT_H = 0x2B,
	H0_rH_x2_reg = 0x30,
	H1_rH_x2_reg = 0x31,
	T0_degC_x8_reg = 0x32,
	T1_degC_x8_reg = 0x33,
	T1_T0_msb_reg = 0x35,
	H0_T0_OUT_L_reg  = 0x36,
	H0_T0_OUT_H_reg = 0x37,
	H1_T0_OUT_L_reg = 0x3A,
	H1_T0_OUT_H_reg = 0x3B,
	T0_OUT_L_reg = 0x3C,
	T0_OUT_H_reg = 0x3D,
	T1_OUT_L_reg = 0x3E,
	T1_OUT_H_reg = 0x3F,
};

enum HTS221_calibration_bits
{
	H0_rH_x2 = 0,
	H1_rH_x2,
	T0_degC_x8,
	T1_degC_x8,
	T1_T0_msb,
	H0_T0_OUT_L,
	H0_T0_OUT_H,
	H1_T0_OUT_L,
	H1_T0_OUT_H,
	T0_OUT_L,
	T0_OUT_H,
	T1_OUT_L,
	T1_OUT_H,
};
void hts221_enable();

void hts221setResolution();

void hts221_coefs(uint8_t coef_data[]);

float hts221_getTemperature(const uint8_t coef_data[]);

float hts221_getHumidity(const uint8_t coef_data[]);

bool hts221_checkIfDataAvailable();

