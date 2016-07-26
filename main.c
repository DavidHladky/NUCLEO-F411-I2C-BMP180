/* Example of using temperature and pressure sensor  BMP180.
 * David Hladky
 * 2016
 * Version 1.0 Update 26-7-2016
 */

#include <stdio.h>
#include <stdlib.h>
#include "bmp180.h"
#include "HD44780.h"
#include "i2c_hal.h"

int main()
{
	BMP180_calibration CALIBRATION_DATA;
	int32_t temperature, pressure;
	int16_t temp_dec, temp_frac, press_dec, press_frac;
	char temp[10], press[10];

	HAL_Init(); // Initialization HAL library and system.

	i2c_hal_intialization(); // Initialization I2C peripherals MCU.

	display_HD44780_init();

	bmp180_init(&CALIBRATION_DATA); // Initialization BMP180.

	display_HD44780_write_string_position("Temp:",1,1);
	display_HD44780_write_string_position("Pres:",2,1);

	while (1)
    	{
			bmp180_get_temperature_and_preasure(&CALIBRATION_DATA, &temperature, &pressure);

			temp_dec = temperature/10;
			temp_frac = temperature%10;

			press_dec = pressure/100;
			press_frac = pressure%100;

			sprintf(temp,"%d.%1d C",temp_dec,temp_frac); // Create string from processed data - temperature.
			sprintf(press,"%d.%2d hPa",press_dec,press_frac);  // Create string from processed data - humidity.

			display_HD44780_write_string_position(temp,1,7);
			display_HD44780_write_string_position(press,2,7);

			HAL_Delay(1000);
    	}
}

