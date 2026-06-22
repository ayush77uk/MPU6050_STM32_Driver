#ifndef __MPU6050_CONFIG_H__
#define __MPU6050_CONFIG_H__

#include <stdint.h>

// Choose a microcontroller family
//#define STM32F0
#define STM32F1
//#define STM32F4
//#define STM32L0
//#define STM32L1
//#define STM32L4
//#define STM32F3
//#define STM32H7
//#define STM32F7
//#define STM32G0
//#define STM32C0
//#define STM32U5

// #define SLEEP_MODE
// #define LOW_POWER_MODE

/*
LP_WAKE_CTRL defines the sample rate at which the MPU6050 wakes up to read accelerometer values in Accelerometer Only Low Power Mode

    |   0 -> 1.25 Hz   | 
    |   1 -> 5 Hz      |
    |   2 -> 20 Hz     |
    |   3 -> 40 Hz     |
*/
#define LP_WAKE_CTRL 0
// #define LP_WAKE_CTRL 1
// #define LP_WAKE_CTRL 2
// #define LP_WAKE_CTRL 3

/* Standby mode for accelerometer axes-----------------------------*/

// #define STANDBY_ACC_X   
// #define STANDBY_ACC_Y   
// #define STANDBY_ACC_Y  

/* Standby mode for gyroscope axes----------------------------------*/

// #define STANDBY_GYRO_X
// #define STANDBY_GYRO_Y
// #define STANDBY_GYRO_Z

// #define TEMP_DISABLE    //Disable the temperature sensor

#endif