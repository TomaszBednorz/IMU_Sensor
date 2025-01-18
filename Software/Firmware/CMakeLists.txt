cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(imuSensor)
enable_language(C CXX ASM)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    -mthumb

    # This needs attention to properly set for used MCU
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32L433VCTX_FLASH.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#
set(sources_SRCS
    # Put here your source files, one in each line, relative to CMakeLists.txt file location
    ${PROJ_PATH}/Application/Src/rtc_app.c
    ${PROJ_PATH}/Application/Src/w5200_app.c
    ${PROJ_PATH}/Application/Src/main.c
    ${PROJ_PATH}/Application/Src/xsens_app.c
    ${PROJ_PATH}/Application/Src/tasks_app.c
    ${PROJ_PATH}/Startup/startup_stm32l433vctx.s
    ${PROJ_PATH}/Drivers/Ethernet/Src/socket.c
    ${PROJ_PATH}/Drivers/Ethernet/Cfg/w5200_config.c
    ${PROJ_PATH}/Drivers/Ethernet/Cfg/wizchip_conf.c
    ${PROJ_PATH}/Drivers/Ethernet/Src/W5200/w5200.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/croutine.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/event_groups.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/list.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/queue.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/stream_buffer.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/tasks.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/timers.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/portable/GCC/ARM_CM4F/port.c
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/portable/MemMang/heap_4.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/gpio.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/rtc.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/spi.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/stm32l4xx_hal_msp.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/stm32l4xx_hal_timebase_tim.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/stm32l4xx_it.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/syscalls.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/sysmem.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/system_stm32l4xx.c
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Src/tim.c
    ${PROJ_PATH}/Drivers/Xsens/Src/xsens_com.c
    ${PROJ_PATH}/Drivers/Xsens/Src/xsens_miscellaneous.c
)

#
# Include directories
#
set(include_path_DIRS
    # Put here your include dirs, one in each line, relative to CMakeLists.txt file location
    ${PROJ_PATH}/Application/Inc
    ${PROJ_PATH}/Libraries/CMSIS/Device/ST/STM32L4xx/Include
    ${PROJ_PATH}/Libraries/CMSIS/Include
    ${PROJ_PATH}/Drivers/Ethernet/Cfg
    ${PROJ_PATH}/Drivers/Ethernet/Src
    ${PROJ_PATH}/Drivers/Ethernet/Src/W5200
    ${PROJ_PATH}/Libraries/FreeRTOS/Cfg
    ${PROJ_PATH}/Libraries/FreeRTOS/Src
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/include
    ${PROJ_PATH}/Libraries/FreeRTOS/Src/portable/GCC/ARM_CM4F
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Inc
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Inc/Legacy
    ${PROJ_PATH}/Libraries/STM32L4xx_HAL_Driver/Cfg/Inc
    ${PROJ_PATH}/Drivers/Xsens/Cfg
    ${PROJ_PATH}/Drivers/Xsens/Src
)

#
# Symbols definition
#
set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with double quotes for safety purpose
    "DEBUG"
    "STM32L433xx"
    "USE_HAL_DRIVER"
    "HSE_VALUE=8000000"
    "UNUSED_CODE"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    # Full debug configuration
    # -Og -g3 -ggdb
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    # -u _printf_float                # STDIO float formatting support
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Conver to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)