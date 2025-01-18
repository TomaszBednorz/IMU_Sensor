#include "w5200_config.h"


wiz_NetInfo gWIZNETINFO = { .mac = {W5200_MAC1, W5200_MAC2, W5200_MAC3, W5200_MAC4, W5200_MAC5, W5200_MAC6}, //<-mac should be unique.
                            .ip = {W5200_IP1, W5200_IP2, W5200_IP3, W5200_IP4},
                            .sn = {255,255,0,0},
                            .gw = {0, 0, 0, 0},
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC };

/*
 * Static function prototypes
 */
static void w5200_cs_sel(void);
static void w5200_cs_desel(void);
static uint8_t w5200_spi_rb(void);
static void w5200_spi_wb(uint8_t wb);
static void w5200_spi_rb_burst(uint8_t *pBuf, uint16_t len);
static void w5200_spi_wb_burst(uint8_t *pBuf, uint16_t len);
static void w5200_cris_en(void);
static void w5200_cris_ex(void);

/*!	
 *\brief W5200 initialization function. Function set callbacks, IP, MAC and interrupts of ethernet module.
 */
void W5200Config_init(void)
{
	reg_wizchip_cs_cbfunc(w5200_cs_sel, w5200_cs_desel);
	reg_wizchip_spi_cbfunc(w5200_spi_rb, w5200_spi_wb);
	reg_wizchip_spiburst_cbfunc(w5200_spi_rb_burst, w5200_spi_wb_burst);
	reg_wizchip_cris_cbfunc(w5200_cris_en, w5200_cris_ex);

	wizchip_setnetinfo(&gWIZNETINFO);

	wizchip_setinterruptmask(W5200_USED_SOCKET);
	setSn_IMR(W5200_USED_SOCKET_NUM, 0x04);
}

static void w5200_cs_sel(void)
{
	HAL_GPIO_WritePin(W5200_SPI_NSS_PORT, W5200_SPI_NSS_PIN, GPIO_PIN_RESET);
}

static void w5200_cs_desel(void)
{
	HAL_GPIO_WritePin(W5200_SPI_NSS_PORT, W5200_SPI_NSS_PIN, GPIO_PIN_SET);
}

static uint8_t w5200_spi_rb(void)
{
	uint8_t rx_data;
	HAL_SPI_Receive(W5200_SPI_HANDLE, &rx_data, 1, HAL_MAX_DELAY);
	return rx_data;
}

static void w5200_spi_wb(uint8_t wb)
{
	HAL_SPI_Transmit(W5200_SPI_HANDLE, &wb, 1, HAL_MAX_DELAY);
}

static void w5200_spi_rb_burst(uint8_t *pBuf, uint16_t len)
{
	HAL_SPI_Receive(W5200_SPI_HANDLE, pBuf, len, HAL_MAX_DELAY);
}

static void w5200_spi_wb_burst(uint8_t *pBuf, uint16_t len)
{
	HAL_SPI_Transmit(W5200_SPI_HANDLE, pBuf, len, HAL_MAX_DELAY);
}

static void w5200_cris_en(void)
{
	__disable_irq();
}

static void w5200_cris_ex(void)
{
	__enable_irq();
}

