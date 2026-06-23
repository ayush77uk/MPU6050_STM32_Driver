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
  * Offset
  * Digital Low Pass Filter (DLPF)

---

## 🛠 Supported Platforms

Compatible with STM32 devices using STM32 HAL.

Supported families:

`STM32F0 • STM32F1 • STM32F3 • STM32F4 • STM32F7 • STM32G0 • STM32H7 • STM32L0 • STM32L1 • STM32L4 • STM32U5 • STM32WB`

---

## 🔌 Device Address

| AD0  | Address |
| ---- | ------- |
| LOW  | `0x68`  |
| HIGH | `0x69`  |

Supports **up to 2 MPU6050 modules** on the same I2C bus.

---

## 📚 Quick Start Guide

Make an MPU6050 object :
```c
MPU6050 example_object;
```

Initialize the MPU6050 module.
Parameters: (&I2C_Handle, &example_object, device_address):
```c
MPU6050_Init(&hi2c1, &example_object, 0x68);
```

Automatic offset calibration.
Parameters: (Address of device instance, Number of sample readings to take):
```c
MPU6050_Calibrate(&example_object, sample);
```

Read all sensors:
```c
MPU6050_Read_All(&example_object);
```

Read Accelerometer (g value):
```c
MPU6050_Read_Accel(&example_object);
```

Read Gyroscope (Degrees per second):
```c
MPU6050_Read_Gyro(&example_object);
```

Read module internal temperature (Degrees Celsius):
```c
MPU6050_Read_Temp(&example_object);
```


### Return Values

| Code | Meaning |
| ---- | ------- |
| `0`  | Success |
| `1`  | Error   |

---