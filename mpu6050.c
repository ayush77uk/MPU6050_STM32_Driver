#include "mpu6050.h"

const uint16_t i2c_timeout = 100;
const uint16_t Accel_range_div_array[] = {16384, 8192, 4096, 2048};
const float Gyro_range_div_array[] = {131.0, 65.5, 32.8, 16.4};

//---------------------------------

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c, MPU6050 *mpu6050, uint8_t address)
{
    if(address != 0x68 && address != 0x69) return 1;

    mpu6050->i2c = hi2c;
    mpu6050->addr = address;
    mpu6050->i2c_addr = (mpu6050->addr) << 1;
    mpu6050->accel_range = accel_2g;
    mpu6050->gyro_range = gyro_250dps;
    mpu6050->accel_range_div = Accel_range_div_array[mpu6050->accel_range];
    mpu6050->gyro_range_div = Gyro_range_div_array[mpu6050->gyro_range];

    uint8_t addr_check;
    uint8_t data;

    if(HAL_I2C_Mem_Read(mpu6050->i2c, mpu6050->i2c_addr, WHO_AM_I_REG, 1, &addr_check, 1, i2c_timeout) != HAL_OK) return 1;

    if(addr_check == mpu6050->addr){

        //Device Reset
        data = 0 | (1 << 7);
        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, PWR_MGMT_1_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;
        HAL_Delay(20);

        //Power configuration setup
        data = 0;
        mpu6050->state = STATE_INITIALIZED;
        #ifdef SLEEP_MODE_ACTIVATE
        data |= (1 << 6);
        mpu6050->state = STATE_SLEEP_MODE;
        #endif
        #if defined(LOW_POWER_MODE_ACTIVATE) && !defined(SLEEP_MODE_ACTIVATE)
        data |= (1 << 5);
        mpu6050->state = STATE_LOW_POWER_MODE;
        #endif
        #ifdef TEMP_DISABLE
        data |= (1 << 3);
        #endif
        
        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, PWR_MGMT_1_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;

        //Set up Low Power Mode frequency and axes Standby config
        data = 0 | (LP_WAKE_CTRL << 7);
        #ifdef STANDBY_ACC_X
        data |= (1 << 5);
        #endif
        #ifdef STANDBY_ACC_Y
        data |= (1 << 4);
        #endif
        #ifdef STANDBY_ACC_Z
        data |= (1 << 3);
        #endif
        #ifdef STANDBY_GYRO_X
        data |= (1 << 2);
        #endif
        #ifdef STANDBY_GYRO_Y
        data |= (1 << 1);
        #endif
        #ifdef STANDBY_GYRO_Z
        data |= 1;
        #endif

        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, PWR_MGMT_2_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;

        //Set Digital Low Pass Filter
        data = 3;
        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, CONFIG_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;

        // Set data Sample Rate by writing SMPLRT_DIV register
        data = 0x07;
        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, SMPLRT_DIV_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;

        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> 2g
        data = 0 | (mpu6050->accel_range << 4);
        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, ACCEL_CONFIG_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;

        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> 250 /s
        data = 0 | (mpu6050->gyro_range << 4);
        if(HAL_I2C_Mem_Write(mpu6050->i2c, mpu6050->i2c_addr, GYRO_CONFIG_REG, 1, &data, 1, i2c_timeout) != HAL_OK) return 1;

        return 0;   //Initialization successful
    }
    return 1;
}

//---------------------------------

uint8_t MPU6050_ReadAccel(MPU6050 *mpu6050)
{
    uint8_t Data_buff[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    if(HAL_I2C_Mem_Read(mpu6050->i2c, mpu6050->i2c_addr, ACCEL_XOUT_H_REG, 1, Data_buff, 6, i2c_timeout) != HAL_OK) return 1;

    mpu6050->accel_raw_x = (int16_t)(Data_buff[0] << 8 | Data_buff[1]);
    mpu6050->accel_raw_y = (int16_t)(Data_buff[2] << 8 | Data_buff[3]);
    mpu6050->accel_raw_z = (int16_t)(Data_buff[4] << 8 | Data_buff[5]);

    /*
    Convert the raw accelerometer readings into g values.
    If the accelerometer range is not set by the user, by default the range is set to 2g in the MPU6050_Init() function
    */
    mpu6050->accel_gx = mpu6050->accel_raw_x / mpu6050->accel_range_div;
    mpu6050->accel_gy = mpu6050->accel_raw_y / mpu6050->accel_range_div;
    mpu6050->accel_gz = mpu6050->accel_raw_z / mpu6050->accel_range_div;

    return 0;
}

//---------------------------------

uint8_t MPU6050_ReadGyro(MPU6050 *mpu6050)
{
    uint8_t Data_buff[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register
    if(HAL_I2C_Mem_Read(mpu6050->i2c, mpu6050->i2c_addr, GYRO_XOUT_H_REG, 1, Data_buff, 6, i2c_timeout) != HAL_OK) return 1;

    mpu6050->gyro_raw_x = (int16_t)(Data_buff[0] << 8 | Data_buff[1]);
    mpu6050->gyro_raw_y = (int16_t)(Data_buff[2] << 8 | Data_buff[3]);
    mpu6050->gyro_raw_z = (int16_t)(Data_buff[4] << 8 | Data_buff[5]);

    /*
    Convert the raw gyroscope readings into Degree/second values.
    If the gyroscope range is not set by the user, by default the range is set to 250deg/s in the MPU6050_Init() function
    */
    mpu6050->gyro_dps_x = mpu6050->gyro_raw_x / mpu6050->gyro_range_div;
    mpu6050->gyro_dps_y = mpu6050->gyro_raw_y / mpu6050->gyro_range_div;
    mpu6050->gyro_dps_z = mpu6050->gyro_raw_z / mpu6050->gyro_range_div;

    return 0;
}

//---------------------------------

uint8_t MPU6050_ReadTemp(MPU6050 *mpu6050)
{
    uint8_t DataBuff[2];

    // Read 2 BYTES of data starting from TEMP_OUT_H_REG register
    if(HAL_I2C_Mem_Read(mpu6050->i2c, mpu6050->i2c_addr, TEMP_OUT_H_REG, 1, DataBuff, 2, i2c_timeout) != HAL_OK) return 1;

    mpu6050->temp_raw = (int16_t)(DataBuff[0] << 8 | DataBuff[1]);
    mpu6050->temp_c = (float)((int16_t)mpu6050->temp_raw / (float)340.0 + (float)36.53);

    return 0;
}

//---------------------------------

uint8_t MPU6050_ReadAll(MPU6050 *mpu6050)
{
    uint8_t Data_buff[14];

    /*
    Read all measurements in one transaction:
    ACCEL_X,Y,Z (6 bytes)
    TEMP        (2 bytes)
    GYRO_X,Y,Z  (6 bytes)
    */

    if(HAL_I2C_Mem_Read(mpu6050->i2c, mpu6050->i2c_addr, ACCEL_XOUT_H_REG, 1, Data_buff, 14, i2c_timeout) != HAL_OK) return 1;

    // Accelerometer

    mpu6050->accel_raw_x = (int16_t)((Data_buff[0] << 8) | Data_buff[1]);
    mpu6050->accel_raw_y = (int16_t)((Data_buff[2] << 8) | Data_buff[3]);
    mpu6050->accel_raw_z = (int16_t)((Data_buff[4] << 8) | Data_buff[5]);

    mpu6050->accel_gx = (float)mpu6050->accel_raw_x / mpu6050->accel_range_div;
    mpu6050->accel_gy = (float)mpu6050->accel_raw_y / mpu6050->accel_range_div;
    mpu6050->accel_gz = (float)mpu6050->accel_raw_z / mpu6050->accel_range_div;

    // Temperature

    mpu6050->temp_raw = (int16_t)((Data_buff[6] << 8) | Data_buff[7]);
    mpu6050->temp_c = ((float)mpu6050->temp_raw / 340.0f) + 36.53f;

    // Gyroscope

    mpu6050->gyro_raw_x = (int16_t)((Data_buff[8] << 8) | Data_buff[9]);
    mpu6050->gyro_raw_y = (int16_t)((Data_buff[10] << 8) | Data_buff[11]);
    mpu6050->gyro_raw_z = (int16_t)((Data_buff[12] << 8) | Data_buff[13]);

    mpu6050->gyro_dps_x = (float)mpu6050->gyro_raw_x / mpu6050->gyro_range_div;
    mpu6050->gyro_dps_y = (float)mpu6050->gyro_raw_y / mpu6050->gyro_range_div;
    mpu6050->gyro_dps_z = (float)mpu6050->gyro_raw_z / mpu6050->gyro_range_div;

    return 0;
}