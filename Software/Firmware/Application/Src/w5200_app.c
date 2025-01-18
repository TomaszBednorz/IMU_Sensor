#include "w5200_app.h"

uint8_t dest_IP[4] = {W5200_DEST_IP_1, W5200_DEST_IP_2, W5200_DEST_IP_3, W5200_DEST_IP_4};
uint32_t port = W5200_PORT_NUM;

/*!	
 *\brief W5200 module initialization.
 */
void W5200App_init(void)
{
    W5200App_reset();
    W5200Config_init();
}

/*!	
 *\brief W5200 module reset.
 */
void W5200App_reset(void)
{
	HAL_GPIO_WritePin(W5200_SPI_NRESET_PORT, W5200_SPI_NRESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(W5200_SPI_NRESET_PORT, W5200_SPI_NRESET_PIN, GPIO_PIN_SET);
	HAL_Delay(185);
}

/*!	
 *\brief W5200 TCP loopback mode.
 */
int32_t W5200App_loopback(void)
{
	int32_t ret;

	// Port number for TCP client (will be increased)
	static uint16_t any_port = 	50000;

	// Check the W5200 Socket W5200_USED_SOCKET_NUM status register
	switch(getSn_SR(W5200_USED_SOCKET_NUM))
	{
		case SOCK_ESTABLISHED :
			if(getSn_IR(W5200_USED_SOCKET_NUM) & Sn_IR_CON)  // Socket interrupt register mask; Sn_IR_CON = connection with peer is successful
			{
			setSn_IR(W5200_USED_SOCKET_NUM, Sn_IR_CON);  // Clear interrupt
			}
			break;

		case SOCK_CLOSE_WAIT :
			if((ret=disconnect(W5200_USED_SOCKET_NUM)) != SOCK_OK) return ret;
			break;

		case SOCK_INIT :
			if( (ret = connect(W5200_USED_SOCKET_NUM, dest_IP, port)) != SOCK_OK) return ret;	//	Try to TCP connect to the TCP server (destination)
			break;

		case SOCK_CLOSED:
			close(W5200_USED_SOCKET_NUM);
			if((ret=socket(W5200_USED_SOCKET_NUM, Sn_MR_TCP, any_port++, 0x00)) != W5200_USED_SOCKET_NUM){
			if(any_port == 0xffff) any_port = 50000;
			return ret; // TCP socket open with 'any_port' port number
		} 
			break;
		default:
			break;
	}

	return 1;
}

/*!	\brief Function receive data from W5200 module.
	\param[in] buf Buffer for received data
*/
int32_t W5200App_rx(uint8_t* buf)
{
	int32_t ret; // return value for SOCK_ERRORs
    uint16_t size = 0;

	// Port number for TCP client (will be increased)
	static uint16_t any_port = 	50000;

	// Check the W5200 Socket W5200_USED_SOCKET_NUM status register
	switch(getSn_SR(W5200_USED_SOCKET_NUM))
	{
		case SOCK_ESTABLISHED :
			if(getSn_IR(W5200_USED_SOCKET_NUM) & Sn_IR_CON)  // Socket interrupt register mask; Sn_IR_CON = connection with peer is successful
			{
			setSn_IR(W5200_USED_SOCKET_NUM, Sn_IR_CON);  // Clear interrupt
			}

			if((size = getSn_RX_RSR(W5200_USED_SOCKET_NUM)) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
			{
				if(size > W5200_DATA_BUF_SIZE) size = W5200_DATA_BUF_SIZE; // W5200_DATA_BUF_SIZE means user defined buffer size (array)
				ret = recv(W5200_USED_SOCKET_NUM, buf, size); // Data Receive process (H/W Rx socket buffer -> User's buffer)

				if(ret <= 0) return ret; // If the received data length <= 0, receive failed and process end
			}
			break;

		case SOCK_CLOSE_WAIT :
			if((ret=disconnect(W5200_USED_SOCKET_NUM)) != SOCK_OK) return ret;
			break;

		case SOCK_INIT :
			if( (ret = connect(W5200_USED_SOCKET_NUM, dest_IP, port)) != SOCK_OK) return ret;	//	Try to TCP connect to the TCP server (destination)
			break;

		case SOCK_CLOSED:
			close(W5200_USED_SOCKET_NUM);
			if((ret=socket(W5200_USED_SOCKET_NUM, Sn_MR_TCP, any_port++, 0x00)) != W5200_USED_SOCKET_NUM){
			if(any_port == 0xffff) any_port = 50000;
			return ret; // TCP socket open with 'any_port' port number
		} 
			break;
		default:
			break;
	}

	return 1;
}

/*!	\brief Function transmit data to W5200 module.
	\param[in] buf Buffer with data to transmit
	\param[in] len Length of data to transmit
*/
void W5200App_tx(uint8_t * buf, uint16_t len)
{
	send(W5200_USED_SOCKET_NUM, buf, len);
}
