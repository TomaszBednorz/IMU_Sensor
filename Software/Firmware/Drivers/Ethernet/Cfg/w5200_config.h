#ifndef W5200_CONFIG_H_
#define W5200_CONFIG_H_

#include "stm32l4xx_hal.h"
#include "wizchip_conf.h"
#include "socket.h"

/*
 * Configurable items
 */

// SPI configguration
extern SPI_HandleTypeDef hspi2;
#define W5200_SPI_HANDLE		(&hspi2)
#define W5200_SPI_NSS_PIN		(GPIO_PIN_9)
#define W5200_SPI_NSS_PORT		(GPIOD)
#define W5200_SPI_NRESET_PIN	(GPIO_PIN_11)
#define W5200_SPI_NRESET_PORT	(GPIOB)

// TCP/IP
#define W5200_USED_SOCKET       (IK_SOCK_1)
#define W5200_USED_SOCKET_NUM   (1)

// Port
#define W5200_PORT_NUM          (5006)

/* Source device configuration */
// MAC
#define W5200_MAC1              (0x00)
#define W5200_MAC2              (0x08)
#define W5200_MAC3              (0xdc)
#define W5200_MAC4              (0xab)
#define W5200_MAC5              (0xcd)
#define W5200_MAC6              (0xee)

// IP
#define W5200_IP1               (169)
#define W5200_IP2               (254)
#define W5200_IP3               (240)
#define W5200_IP4               (110)

/* Destination device configuration */
// IP
#define W5200_DEST_IP_1         (169)
#define W5200_DEST_IP_2         (254)
#define W5200_DEST_IP_3         (240)
#define W5200_DEST_IP_4         (83)

/*
 * Other defines
 */
#define W5200_DATA_BUF_SIZE	    (2048)

/*
 * Function prototypes
 */
void W5200Config_init(void);


#endif /* W5200_CONFIG_H_ */
