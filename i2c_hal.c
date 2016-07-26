/* Driver for I2C peripheral
 * David Hladky
 * 2016
 * Version 1.0 Update 6-30-2016
 */

/*-----------Include files-----------*/
#include "i2c_hal.h"

void i2c_hal_intialization()
{
 /*------------------- Initialization I2C bus -------------------*/
    /*---------- Setup GPIOB - I2C1 port --------------*/

    __GPIOB_CLK_ENABLE(); // Enable clock source for GPIOB

    I2C1_GPIO_struct.Pin = I2C1_SDA|I2C1_SCL;
    I2C1_GPIO_struct.Mode = GPIO_MODE_AF_OD;
    I2C1_GPIO_struct.Pull = GPIO_PULLUP;
    I2C1_GPIO_struct.Speed = GPIO_SPEED_HIGH;
    I2C1_GPIO_struct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, & I2C1_GPIO_struct);

    /*------------------- Setup I2C1 -------------------*/

    __I2C1_CLK_ENABLE(); // Enable clock source for I2C peripherals

    I2C1_struct.Instance = I2C1;
    I2C1_struct.Init.ClockSpeed = 400000;
    I2C1_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;
    I2C1_struct.Init.OwnAddress1 = 0;
    I2C1_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2C1_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    I2C1_struct.Init.OwnAddress2 = 0;
    I2C1_struct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    I2C1_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    HAL_I2C_Init(&I2C1_struct);
}
