# MPU6050 Embedded Driver for STM32 over I2C

Reusable MPU6050 firmware driver written in **C** for **STM32 microcontrollers** using **STM32 HAL**.

Supports accelerometer, gyroscope, and temperature acquisition with configurable operating modes and multi-device support over a shared I2C bus.

---

## ✨ Features

* Register-level MPU6050 configuration
* Accelerometer reading (raw + converted to **g**)
* Gyroscope reading (raw + converted to **°/s**)
* Temperature reading (**°C**)
* Burst-read acquisition using `MPU6050_ReadAll()`
* Multi-device support over a shared I2C bus
* Configurable:

  * Power modes
  * Sample rate
  * Accelerometer range
  * Gyroscope range
  * Digital Low Pass Filter (DLPF)

---

## 🛠 Supported Platforms

Compatible with STM32 devices using STM32 HAL.

Supported families:

`STM32F0 • STM32F1 • STM32F3 • STM32F4 • STM32F7 • STM32G0 • STM32H7 • STM32L0 • STM32L1 • STM32L4 • STM32U5 • STM32WB`

---

## 📂 Project Structure

```plaintext
MPU6050/
├── mpu6050.c
├── mpu6050.h
├── mpu6050_config.h
└── README.md
```

---

## 🔌 Device Address

| AD0  | Address |
| ---- | ------- |
| LOW  | `0x68`  |
| HIGH | `0x69`  |

Supports **up to 2 MPU6050 modules** on the same I2C bus.

---

## 🚀 Quick Start

```c
#include "mpu6050.h"

MPU6050 imu;

MPU6050_Init(&hi2c1, &imu, 0x68);

MPU6050_ReadAll(&imu);

printf("AX: %.2f g\n", imu.accel_gx);
printf("GX: %.2f dps\n", imu.gyro_dps_x);
printf("Temp: %.2f C\n", imu.temp_c);
```

---

## 📚 API

```c
MPU6050_Init()
MPU6050_ReadAccel()
MPU6050_ReadGyro()
MPU6050_ReadTemp()
MPU6050_ReadAll()
```

### Return Values

| Code | Meaning |
| ---- | ------- |
| `0`  | Success |
| `1`  | Error   |

---