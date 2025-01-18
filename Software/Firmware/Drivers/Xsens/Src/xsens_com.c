#include "xsens_com.h"


/*
 * Static functions prototypes
 */
#ifndef UNUSED_CODE
static void XsensCom_write(uint8_t opcode, uint8_t* buffer, uint16_t bufferLen);
#endif
static void XsensCom_writeRaw(uint8_t* buffer, uint16_t bufferLen);
static void XsensCom_read(uint8_t opcode, uint8_t* buffer, uint16_t bufferLen);
static void XsensCom_readRaw(uint8_t* buffer, uint16_t bufferLen);

/*
 * Functions declarations
 */

/*!	\brief Create xsens message to transmit to control pipe
   \param[in] buffer Pointer to message data
   \param[in] msgID Message identifier
   \param[in] txData Pointer to data to be transmitted
   \param[in] dataLen Number of data bytes to transmit
*/
void XsensCom_createMessage(uint8_t* buffer, uint8_t msgID, uint8_t txData[], uint16_t txDataLen)
{
	uint8_t checksum = 0x01;
	uint8_t* buffer_ptr = buffer;

	*buffer_ptr++ = (uint8_t)Xsens_ControlPipe;

	*buffer_ptr++ = 0xff;
	*buffer_ptr++ = 0xff;
	*buffer_ptr++ = 0xff;

	*buffer_ptr++ = msgID;
	checksum -= msgID;

	*buffer_ptr++ = txDataLen;
	checksum -= txDataLen;

	for(uint8_t i = 0; i < txDataLen; i++)
	{
		*buffer_ptr++ = txData[i];
		checksum -= txData[i];
	}

	*buffer_ptr = checksum;
}

/*!	\brief Get xsens pipes status
   \param[in] notificationPipe Pointer to store notification pipe length
   \param[in] measurementPipe Pointer to store measurement pipe length
*/
void XsensCom_getPipeStatus(uint16_t* notificationPipe, uint16_t* measurementPipe)
{
	uint8_t rxData[4];
	XsensCom_read((uint8_t)Xsens_PipeStatus, rxData, 4);

	*notificationPipe = (uint16_t)rxData[0] | ((uint16_t)rxData[1] << 8);
	*measurementPipe = (uint16_t)rxData[2] | ((uint16_t)rxData[3] << 8);
}

/*!	\brief Get data from notification pipe
	\param[in] buffer Pointer to data to be read from notification pipe
	\param[in] bufferLen Number of data bytes to read from notification pipe
*/
void XsensCom_getNotificationData(uint8_t* buffer, uint16_t bufferLen)
{
	XsensCom_read((uint8_t)Xsens_NotificationPipe, buffer, bufferLen);
}


/*!	\brief Get data from measurement pipe
	\param[in] buffer Pointer to data to be read from measurement pipe
	\param[in] bufferLen Number of data bytes to read from measurement pipe
*/
void XsensCom_getMeasurementData(uint8_t* buffer, uint16_t bufferLen)
{
	XsensCom_read((uint8_t)Xsens_MeasurementPipe, buffer, bufferLen);
}

/*!	\brief Switch device from measurement state to config state
*/
void XsensCom_GoToConfig(void)
{
	uint8_t txBuffer[XSENS_NO_DATA_BUF_LEN];
	XsensCom_createMessage(txBuffer, (uint8_t)Xsens_GoToConfig, NULL, 0);
	XsensCom_writeRaw(txBuffer, XSENS_NO_DATA_BUF_LEN);
}

/*!	\brief Switch device from config state to measurement state
*/
void XsensCom_GoToMeasurement(void)
{
	uint8_t txBuffer[XSENS_NO_DATA_BUF_LEN];
	XsensCom_createMessage(txBuffer, (uint8_t)Xsens_GoToMeasurement, NULL, 0);
	XsensCom_writeRaw(txBuffer, XSENS_NO_DATA_BUF_LEN);
}

/*!	\brief Reset device
*/
void XsensCom_Reset(void)
{
	uint8_t txBuffer[XSENS_NO_DATA_BUF_LEN];
	XsensCom_createMessage(txBuffer, (uint8_t)Xsens_Reset, NULL, 0);
	XsensCom_writeRaw(txBuffer, XSENS_NO_DATA_BUF_LEN);
}

/*!	\brief Get actual device configuration
	\param[in] buffer Pointer to data to read actual device configuration
	\param[in] bufferLen Number of data bytes to read actual device configuration
*/
void XsensCom_getOutputConfiguration(uint8_t* buffer, uint16_t bufferLen)
{
	uint8_t txBuffer[XSENS_NO_DATA_BUF_LEN];
	XsensCom_createMessage(txBuffer, (uint8_t)Xsens_ReqOutputConfiguration, NULL, 0);
	XsensCom_writeRaw(txBuffer, XSENS_NO_DATA_BUF_LEN);

	XsensCom_readRaw(buffer, bufferLen);
}

/*!	\brief Set new device configuration
	\param[in] buffer Pointer to data to write actual device configuration
	\param[in] bufferLen Number of data bytes to write actual device configuration
*/
void XsensCom_setOutputConfiguration(uint8_t* buffer, uint16_t bufferLen)
{
	uint8_t txBuffer[bufferLen + XSENS_NO_DATA_BUF_LEN];
	XsensCom_createMessage(txBuffer, (uint8_t)Xsens_SetOutputConfiguration, buffer, bufferLen);
	XsensCom_writeRaw(txBuffer, bufferLen + XSENS_NO_DATA_BUF_LEN);
}

/*
 *  Static functions
 */

#ifndef UNUSED_CODE
/*!	\brief Blocking write transfer on the SPI bus
	\param[in] opcode Opcode to use
	\param[in] buffer Pointer to data to be written
	\param[in] bufferLen Number of data bytes to write
*/
static void XsensCom_write(uint8_t opcode, uint8_t* buffer, uint16_t bufferLen)
{
	uint8_t rxData[4];

	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_RESET);
	XsensMiscellaneous_waitUs(4);

	rxData[0] = opcode;
	rxData[1] = 0xff;
	rxData[2] = 0xff;
	rxData[3] = 0xff;

	HAL_SPI_Transmit(&hspi1, rxData, sizeof(rxData), XSENS_TIMOUT_100MS);
	HAL_SPI_Transmit(&hspi1, buffer, (uint16_t)bufferLen, XSENS_TIMOUT_100MS);

	XsensMiscellaneous_waitUs(4);
	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_SET);

}
#endif

/*!	\brief Blocking write raw data transfer on the SPI bus
	\param[in] buffer Pointer to data to be written
	\param[in] bufferLen Number of data bytes to write
*/
static void XsensCom_writeRaw(uint8_t* buffer, uint16_t bufferLen)
{
	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_RESET);
	XsensMiscellaneous_waitUs(4);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)buffer, bufferLen, XSENS_TIMOUT_10000MS);
	XsensMiscellaneous_waitUs(4);
	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_SET);
}

/*!	\brief Blocking read transfer on the SPI bus
	\param[in] opcode Opcode to use
	\param[out] buffer Pointer to result buffer
	\param[in] bufferLen Number of data bytes to read
*/
static void XsensCom_read(uint8_t opcode, uint8_t* buffer, uint16_t bufferLen)
{
	uint8_t rxData[4];

	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_RESET);
	XsensMiscellaneous_waitUs(4);

	rxData[0] = opcode;
	rxData[1] = 0xff;
	rxData[2] = 0xff;
	rxData[3] = 0xff;

	HAL_SPI_Transmit(&hspi1, rxData, sizeof(rxData), XSENS_TIMOUT_100MS);
	HAL_SPI_Receive(&hspi1, buffer, (uint16_t)bufferLen, XSENS_TIMOUT_10000MS);

	XsensMiscellaneous_waitUs(4);
	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_SET);
}

/*!	\brief Blocking write raw data transfer on the SPI bus
	\param[in] buffer Pointer to data to be written
	\param[in] bufferLen Number of data bytes to write
*/
static void XsensCom_readRaw(uint8_t* buffer, uint16_t bufferLen)
{
	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_RESET);
	XsensMiscellaneous_waitUs(4);
	HAL_SPI_Receive(&hspi1, (uint8_t*)buffer, bufferLen, XSENS_TIMOUT_10000MS);
	XsensMiscellaneous_waitUs(4);
	HAL_GPIO_WritePin(XSENS_CS_PORT, XSENS_CS_PIN, XSENS_PIN_SET);
}

