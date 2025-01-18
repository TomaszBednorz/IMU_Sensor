#include "xsens_miscellaneous.h"

/*
 * 1 us delay for 64 MHz clock
 */
void XsensMiscellaneous_waitUs(uint32_t microseconds)
{
	for (uint32_t n = 0; n < microseconds; n++)
	{
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();
	}
}

/*
 * Convert every 4 bytes in buffer into floats
 */
void XsensMiscellaneous_uint8BufferToFloats(float* floats, uint8_t* buffer, uint8_t howManyFloats)
{
	uint32_t float_u32;

	for(uint8_t i = 0; i < howManyFloats; i++)
	{
		float_u32 = ((uint32_t)buffer[(i*4)+0] << 24) | ((uint32_t)buffer[(i*4)+1] << 16) | ((uint32_t)buffer[(i*4)+2] << 8) | (buffer[(i*4)+3]);
		memcpy(&floats[i], &float_u32, 4);
	}
}

/*
 * Convert two uint8_t values into uint16_t
 */
uint16_t XsensMiscellaneous_getUint16FromBuffer(uint8_t* buffer, uint8_t index)
{
	uint16_t result;
	result = ((uint16_t)buffer[index] << 8) | ((uint16_t)buffer[index+1]);
	return result;
}

/*
 * Convert four uint8_t values into uint32_t
 */

uint32_t XsensMiscellaneous_getUint32FromBuffer(uint8_t* buffer, uint8_t index)
{
	uint32_t result;
	result = ((uint32_t)buffer[index] << 24) | ((uint32_t)buffer[index+1] << 16) | ((uint32_t)buffer[index+2] << 8) | (buffer[index+3]);
	return result;

}



