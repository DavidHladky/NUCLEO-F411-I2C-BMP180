/* Driver for BMP180.
 * David Hladky
 * 2016
 * Version 1.0 Update 26-7-2016
 */

#include "stm32f4xx_hal.h"
#include "i2c_hal.h"

/*----------------------- Definitions  -----------------------*/
#define I2C_dev_add 0xEE
#define BMP180_OSS 3 // Ultra high resolution

/*------------------- Calibration structure -------------------*/
typedef struct {
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
    int16_t B1;
    int16_t B2;
    int16_t MB;
    int16_t MC;
    int16_t MD;
}BMP180_calibration;

/*------------------------- Functions -------------------------*/
void bmp180_init(BMP180_calibration* calibration); // Initialization of BMP180

int32_t bmp180_get_UT(void); // Read uncompensated temperature value.

int32_t bmp180_get_UP(void); // Read uncompensated pressure value.

void bmp180_get_temperature_and_preasure(BMP180_calibration* calibration, int32_t *T,int32_t *P); // Calculation true values of temperature and pressure.

