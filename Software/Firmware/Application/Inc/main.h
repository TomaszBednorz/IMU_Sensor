#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

#include "string.h"
#include "stdint.h"

#include "FreeRTOS.h"
#include "task.h"

#include "xsens_app.h"
#include "xsens_com.h"
#include "w5200_app.h"
#include "tasks_app.h"



// Task handles
TaskHandle_t taskXsensHandle;
TaskHandle_t taskEthLoopHandle;
TaskHandle_t taskEthRxHandle;


void Error_Handler(void);
void SystemClock_Config(void);

#define LED_USER_Pin GPIO_PIN_2
#define LED_USER_GPIO_Port GPIOC
#define IMU_DRDY_Pin GPIO_PIN_0
#define IMU_DRDY_GPIO_Port GPIOA
#define IMU_SCK_Pin GPIO_PIN_1
#define IMU_SCK_GPIO_Port GPIOA
#define UART_TX_Pin GPIO_PIN_2
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin GPIO_PIN_3
#define UART_RX_GPIO_Port GPIOA
#define IMU_MISO_Pin GPIO_PIN_6
#define IMU_MISO_GPIO_Port GPIOA
#define IMU_MOSI_Pin GPIO_PIN_7
#define IMU_MOSI_GPIO_Port GPIOA
#define IMU_nCS_Pin GPIO_PIN_4
#define IMU_nCS_GPIO_Port GPIOC
#define WIZ_nRESET_Pin GPIO_PIN_11
#define WIZ_nRESET_GPIO_Port GPIOB
#define WIZ_PWDN_Pin GPIO_PIN_13
#define WIZ_PWDN_GPIO_Port GPIOB
#define WIZ_MISO_Pin GPIO_PIN_14
#define WIZ_MISO_GPIO_Port GPIOB
#define WIZ_MOSI_Pin GPIO_PIN_15
#define WIZ_MOSI_GPIO_Port GPIOB
#define WIZ_nSS_Pin GPIO_PIN_9
#define WIZ_nSS_GPIO_Port GPIOD
#define WIZ_nINT_Pin GPIO_PIN_10
#define WIZ_nINT_GPIO_Port GPIOD
#define CARD_DETECT_Pin GPIO_PIN_10
#define CARD_DETECT_GPIO_Port GPIOA

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
