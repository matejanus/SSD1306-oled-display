#include "HTS221.h"

void hts221_enable()
 {
	 i2cWriteByte(HTS221_ADDR, CTRL_REG1, 0x82);
 }

 void hts221setResolution()
 {
	 i2cWriteByte(HTS221_ADDR, AV_CONF, 0x3F);
 }

 void hts221_coefs(uint8_t coef_data[])
 {
	 i2cReadReg(HTS221_ADDR, H0_rH_x2_reg, &coef_data[H0_rH_x2], 1);
	 i2cReadReg(HTS221_ADDR, H1_rH_x2_reg, &coef_data[H1_rH_x2], 1);
	 i2cReadReg(HTS221_ADDR, T0_degC_x8_reg, &coef_data[T0_degC_x8], 1);
	 i2cReadReg(HTS221_ADDR, T1_degC_x8_reg, &coef_data[T1_degC_x8], 1);
	 i2cReadReg(HTS221_ADDR, T1_T0_msb_reg, &coef_data[T1_T0_msb], 1);

	 i2cReadReg(HTS221_ADDR, H0_T0_OUT_L_reg, &coef_data[H0_T0_OUT_L], 1);
	 i2cReadReg(HTS221_ADDR, H0_T0_OUT_H_reg, &coef_data[H0_T0_OUT_H], 1);
	 i2cReadReg(HTS221_ADDR, H1_T0_OUT_L_reg, &coef_data[H1_T0_OUT_L], 1);
	 i2cReadReg(HTS221_ADDR, H1_T0_OUT_H_reg, &coef_data[H1_T0_OUT_H], 1);

	 i2cReadReg(HTS221_ADDR, T0_OUT_L_reg, &coef_data[T0_OUT_L], 1);
	 i2cReadReg(HTS221_ADDR, T0_OUT_H_reg, &coef_data[T0_OUT_H], 1);
	 i2cReadReg(HTS221_ADDR, T1_OUT_L_reg, &coef_data[T1_OUT_L], 1);
	 i2cReadReg(HTS221_ADDR, T1_OUT_H_reg, &coef_data[T1_OUT_H], 1);
 }

 float hts221_getTemperature(const uint8_t coef_data[])
 {
	 if(hts221_checkIfDataAvailable())
	 {
	 uint8_t temp_out_l = 0;
	 uint8_t temp_out_h = 0;
	 int16_t T0_out = 0;
	 int16_t T1_out = 0;
	 int16_t T0_degC_x8_u16 = (((int16_t)(coef_data[T1_T0_msb]& 0x03))<<8) | ((int16_t)coef_data[T0_degC_x8]);
	 int16_t T1_degC_x8_u16 = (((int16_t)(coef_data[T1_T0_msb]& 0x0C))<<6) | ((int16_t)coef_data[T1_degC_x8]);
	 int16_t T0_degC = T0_degC_x8_u16 >> 3;
	 int16_t T1_degC = T1_degC_x8_u16 >> 3;
	 T0_out = (((int16_t)coef_data[T0_OUT_H]) << 8) | ((int16_t)coef_data[T0_OUT_L]);
	 T1_out = (((int16_t)coef_data[T1_OUT_H]) << 8) | ((int16_t)coef_data[T1_OUT_L]);

	 i2cReadReg(HTS221_ADDR, TEMP_OUT_L, &temp_out_l, 1);
	 i2cReadReg(HTS221_ADDR, TEMP_OUT_H, &temp_out_h, 1);
	 int16_t rawReading = (int16_t)temp_out_l | ((int16_t)temp_out_h<<8);

	 return (float)(rawReading - T0_out) * (float)(T1_degC - T0_degC) / (float)(T1_out - T0_out)  +  T0_degC;

	 }
	 else return 0;
 }

 float hts221_getHumidity(const uint8_t coef_data[])
 {
	 if(hts221_checkIfDataAvailable())
	 {
	 uint8_t hum_out_l = 0;
	 uint8_t hum_out_h = 0;
	 int16_t H0_T0_out = 0;
	 int16_t H1_T1_out = 0;

	 int16_t H0_rh = ((int16_t)coef_data[H0_rH_x2])>>1;
	 int16_t H1_rh = ((int16_t)coef_data[H1_rH_x2])>>1;

	 H0_T0_out = (((int16_t)coef_data[H0_T0_OUT_H]) << 8) | ((int16_t)coef_data[H0_T0_OUT_L]);
	 H1_T1_out = (((int16_t)coef_data[H1_T0_OUT_H]) << 8) | ((int16_t)coef_data[H1_T0_OUT_L]);

	 i2cReadReg(HTS221_ADDR, HUMIDITY_OUT_L, &hum_out_l, 1);
	 i2cReadReg(HTS221_ADDR, HUMIDITY_OUT_H, &hum_out_h, 1);
	 int16_t rawReading = (int16_t)hum_out_l | ((int16_t)hum_out_h<<8);
//
	 float tmp_f = (float)(rawReading - H0_T0_out) * (float)(H1_rh - H0_rh) / (float)(H1_T1_out - H0_T0_out)  +  H0_rh;
//
	 if (tmp_f > 100)
	 {
		 tmp_f = 100;
	 }
	 else if (tmp_f <0)
	 {
		 tmp_f = 0;
	 }
	 return tmp_f;
	 }
	 else return 0;
 }

 bool hts221_checkIfDataAvailable()
 {
	 uint8_t tmp = 0;
	 i2cReadReg(HTS221_ADDR, STATUS_REG, &tmp, 1);
	 return tmp == 3; //temperature and humidity available
 }

