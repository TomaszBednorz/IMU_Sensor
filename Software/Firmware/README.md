# IMU_Sensor - Firmware

Main features:
- Gathering data from the IMU,
- Communication and configuration via the TCP protocol.

## Folder structure

```
Firmware/
├── Application/	           // Application layer
├── Drivers/ 
│   ├── Ethernet		   // W5200 driver
│   └── Xsens			   // Xsens Mti-1 driver
├── Libraries/ 
│   ├── CMSIS			   // Register description file provided by ARM
│   ├── FreeRTOS		   // Real time operating system
│   └── STM32L4xx_HAL_Driver       // Microcontroller hardware abstraction layer
├── Startup/			   // Startup code
├── build/			   // Build output
├── CMakeLists.txt		   // Main CMake file
├── gcc-arm-none-eabi.cmake	   // Toolchain information file
├── STM32L4x3.svd		   // Registers description to debug purpose
└── STM32L433VCTX_FLASH.ld         // Linker file

```
