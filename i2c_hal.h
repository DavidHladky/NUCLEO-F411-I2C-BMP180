/*----------- Include files -----------*/

#include "stm32f4xx_hal.h"

/*---------- I2C definitions ----------*/

#define I2C1_SDA	GPIO_PIN_7
#define I2C1_SCL	GPIO_PIN_6

/*------------ Initialization structures -----------*/

GPIO_InitTypeDef I2C1_GPIO_struct;
I2C_HandleTypeDef I2C1_struct;

/*--------- i2c initialization function ---------*/

void i2c_hal_intialization();
