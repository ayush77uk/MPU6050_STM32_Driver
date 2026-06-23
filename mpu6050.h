#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <stdint.h>
#include "mpu6050_config.h"

#if defined(STM32WB)
#include "stm32wbxx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32L5)
#include "stm32l5xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32C0)
#include "stm32c0xx_hal.h"
#elif defined(STM32U5)
#include "stm32u5xx_hal.h"
#endif

#if defined(SLEEP_MODE) && !defined(LOW_POWER_MODE)
#define SLEEP_MODE_ACTIVATE
#endif
#if defined(LOW_POWER_MODE) && !defined(SLEEP_MODE)
#define LOW_POWER_MODE_ACTIVATE
#endif


#define RAD_TO_DEG 57.295779513082320876798154814105

//MPU6050 registers address
#define WHO_AM_I_REG          0x75
#define PWR_MGMT_1_REG        0x6B
#define PWR_MGMT_2_REG        0x6C
#define CONFIG_REG            0x1A
#define SMPLRT_DIV_REG        0x19
#define ACCEL_CONFIG_REG      0x1C
#define GYRO_CONFIG_REG       0x1B
#define ACCEL_XOUT_H_REG      0x3B
#define GYRO_XOUT_H_REG       0x43
#define TEMP_OUT_H_REG        0x41

//MPU6050 device instance structure
typedef struct
{
    /* Hardware Interface ------------------------*/
    I2C_HandleTypeDef *i2c;
    uint8_t addr;
    uint8_t i2c_addr;

    /* Raw Sensor Values -------------------------*/
    int16_t accel_raw_x;
    int16_t accel_raw_y;
    int16_t accel_raw_z;

    int16_t gyro_raw_x;
    int16_t gyro_raw_y;
    int16_t gyro_raw_z;

    int16_t temp_raw;

    /* Converted Values --------------------------*/
    float accel_gx;
    float accel_gy;
    float accel_gz;
    
    float gyro_dps_x;
    float gyro_dps_y;
    float gyro_dps_z;
    
    float temp_c;

    /* Calibration -------------------------------*/
    float accel_offset_x;
    float accel_offset_y;
    float accel_offset_z;

    float gyro_offset_x;
    float gyro_offset_y;
    float gyro_offset_z;

    /* Configuration -----------------------------*/
    enum {
        accel_2g,
        accel_4g,
        accel_8g,
        accel_16g

    }accel_range;
    float accel_range_div;

    enum {
        gyro_250dps,
        gyro_500dps,
        gyro_1000dps,
        gyro_2000dps
    }gyro_range;
    float gyro_range_div;

    uint8_t dlpf;
    uint8_t sample_div;

    /* State Flags -------------------------------*/
    enum {
        STATE_INITIALIZED,
        STATE_SLEEP_MODE,
        STATE_LOW_POWER_MODE
    }state;

} MPU6050;


//Function declarations----------------------------/

/* Initialize the MPU6050 device */
uint8_t MPU6050_Init(I2C_HandleTypeDef *i2c_handle, MPU6050 *device_instance, uint8_t device_adress);

/* Read accelerometer readings */
uint8_t MPU6050_Read_Accel(MPU6050 *device_instance);

/* Read Gyroscope readings */
uint8_t MPU6050_Read_Gyro(MPU6050 *device_instance);

/* Read Temperature reading */
uint8_t MPU6050_Read_Temp(MPU6050 *device_instace);

/* Read all readings at once */
uint8_t MPU6050_Read_All(MPU6050 *device_instance);

/* Set accelerometer full scale range */
uint8_t MPU6050_Set_Accel_Range(MPU6050 *device_instance, uint8_t range);

/* Set gyroscope full scale range */
uint8_t MPU6050_Set_Gyro_Range(MPU6050 *device_instance, uint8_t range);

/* 
Calibrate the MPU6050 moduel:
Orient the MPU6050 in the desired position (usually in flat position) to set the offset of accelerometer and gyroscope.
In the second parameter, enter the desired number of sample readings to take for calibration (The final offset is the average of all readings).
*/
uint8_t MPU6050_Calibrate(MPU6050 *mpu, uint16_t samples);

/* Set accelerometer offset manually */
uint8_t MPU6050_Set_Accel_Offset(MPU6050 *mpu, float off_ax, float off_ay, float off_az);

/* Set gyroscope offset manually */
uint8_t MPU6050_Set_Gyro_Offset(MPU6050 *mpu, float off_gx, float off_gy, float off_gz);


#endif  /* #ifndef __MPU6050_H__ */