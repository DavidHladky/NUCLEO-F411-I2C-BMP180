/* BMP180 driver fo STM32f103c8
 * David Hladky
 * 2015
 * Version 1.0 - Update 30.9.2015
 */

#include "bmp180.h"


void bmp180_init(BMP180_calibration* calibration)
{
	uint8_t buffer[22]; //Read 22 calibration bytes
	uint8_t start_address = 0xAA;

	HAL_I2C_Master_Transmit(&I2C1_struct,I2C_dev_add,&start_address,1,1000); // Start address of the first byte of calibration data.
	HAL_I2C_Master_Receive(&I2C1_struct,I2C_dev_add,buffer,sizeof(buffer),1000); // Reading calibration data from the calibration register.

	calibration->AC1 = (buffer[0]<<8)|buffer[1];
	calibration->AC2 = (buffer[2]<<8)|buffer[3];
	calibration->AC3 = (buffer[4]<<8)|buffer[5];
	calibration->AC4 = (buffer[6]<<8)|buffer[7];
	calibration->AC5 = (buffer[8]<<8)|buffer[9];
	calibration->AC6 = (buffer[10]<<8)|buffer[11];
	calibration->B1 = (buffer[12]<<8)|buffer[13];
	calibration->B2 = (buffer[14]<<8)|buffer[15];
	calibration->MB = (buffer[16]<<8)|buffer[17];
	calibration->MC = (buffer[18]<<8)|buffer[19];
	calibration->MD = (buffer[20]<<8)|buffer[21];
}

int32_t bmp180_get_UT(void)
{
	int32_t UT;
	uint8_t buffer[2], setup_buff[]={0xF4,0x2E}; // The command data to start temperature measurement.
	uint8_t command = 0xF6;

	HAL_I2C_Master_Transmit(&I2C1_struct,I2C_dev_add,setup_buff,sizeof(setup_buff),1000); // Start measure.
	HAL_Delay(5); // Waiting for 5 ms.
	HAL_I2C_Master_Transmit(&I2C1_struct,I2C_dev_add,&command,1,1000); // Read measured data from the register.
	HAL_I2C_Master_Receive(&I2C1_struct,I2C_dev_add,buffer,sizeof(buffer),1000);

	UT = (buffer[0]<<8)|buffer[1];
	return UT;
}

int32_t bmp180_get_UP(void)
{
	int32_t UP;
	uint8_t buffer[3], setup_buff[]={0xF4,(0x34+(BMP180_OSS<<6))}; // The command data to start pressure measurement.
	uint8_t command = 0xF6;

	HAL_I2C_Master_Transmit(&I2C1_struct,I2C_dev_add,setup_buff,sizeof(setup_buff),1000); // Start measure.
	HAL_Delay(25); // Waiting for 25 ms.
	HAL_I2C_Master_Transmit(&I2C1_struct,I2C_dev_add,&command,1,1000); // Read measured data from the register.
	HAL_I2C_Master_Receive(&I2C1_struct,I2C_dev_add,buffer,sizeof(buffer),1000);

	UP = ((buffer[0]<<16)+(buffer[1]<<8)+buffer[2])>>(8-BMP180_OSS);
	return UP;
}

void bmp180_get_temperature_and_preasure(BMP180_calibration* calibration, int32_t *T,int32_t *P)
{
	int32_t X1, X2, X3, B3, B5, B6, UT, UP;
	uint32_t B4, B7;

	UT = bmp180_get_UT(); // Read uncompensated temperature value.
	UP =  bmp180_get_UP(); // Read uncompensated pressure value.

	// Calculate the actual temperature of uncompensated values and calibration coefficients.
	X1 = (((UT)-(calibration->AC6))*(calibration->AC5))/(1<<15);
	X2 = ((calibration->MC)*(1<<11))/(X1+(calibration->MD));
	B5 = X1+X2;
	*T = (B5+8)/(1<<4); // Return actual value of the temperature.

	// Calculate the actual pressure of uncompensated values and calibration coefficients.
	B6 = B5-4000;
	X1 = ((calibration->B2)*(B6*B6/(1<<12)))/(1<<11);
	X2 = ((calibration->AC2)*B6)/(1<<11);
	X3 = X1+X2;
	B3 = ((((calibration->AC1)*4+X3)<<BMP180_OSS)+2)/4;
	X1 = ((calibration->AC3)*B6)/(1<<13);
	X2 = ((calibration->B1)*(B6*B6/(1<<12)))/(1<<16);
	X3 = ((X1+X2)+2)/(1<<2);
	B4 = (calibration->AC4)*(uint32_t)(X3+32768)/(1<<15);
	B7 = ((uint32_t)(UP)-B3)*(50000>>BMP180_OSS);
		if(B7<0x80000000)
		{
				*P = (B7*2)/B4;
		}
		else
		{
				*P = (B7/B4)*2;
		}
	X1 = ((*P)/(1<<8))*((*P)/(1<<8));
	X1 = (X1*3038)/(1<<16);
	X2 = (-7357*(*P))/(1<<16);
	*P  = (*P)+(X1+X2+3791)/(1<<4); // Return actual value of the pressure.

}

