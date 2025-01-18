#ifndef XSENS_XSENS_MISCELLANEOUS_H_
#define XSENS_XSENS_MISCELLANEOUS_H_

#define NOP()  asm("nop")

#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#define XSENS_LEN_1		(0x01)
#define XSENS_LEN_2		(0x02)
#define XSENS_LEN_4		(0x04)
#define XSENS_LEN_8		(0x08)
#define XSENS_LEN_12	(0x0c)
#define XSENS_LEN_16	(0x10)

#define XSENS_ERROR_VALUE (0xad)


/*
 * Xsens opcodes
 */

typedef enum
{
	Xsens_ProtocolInfo = 1,
	Xsens_ConfigureProtocol,
	Xsens_ControlPipe,
	Xsens_PipeStatus,
	Xsens_NotificationPipe,
	Xsens_MeasurementPipe
}XsensOpcode;

/*
 * Xsens message identifiers
 */

typedef enum
{
	Xsens_ReqDID = 0x00,
	Xsens_DeviceID = 0x01,
	Xsens_ReqConfiguration = 0x0C,
	Xsens_Configuration = 0x0D,
	Xsens_RestoreFactoryDef = 0x0E,
	Xsens_GoToMeasurement = 0x10,
	Xsens_ReqFWRev = 0x12,
	Xsens_FirmwareRev = 0x13,
	Xsens_ReqBaudrate = 0x18,
	Xsens_SetBaudrate = 0x18,
	Xsens_ReqProductCode = 0x1C,
	Xsens_ProductCode = 0x1D,
	Xsens_ReqHardwareVersion = 0x1E,
	Xsens_HardwareVersion = 0x1F,
	Xsens_RunSelftest = 0x24,
	Xsens_SelftestResults = 0x25,
	Xsens_ReqSyncSettings = 0x2C,
	Xsens_SetSyncSettings = 0x2C,
	Xsens_GoToConfig = 0x30,
	Xsens_ReqData = 0x34,
	Xsens_MTData2 = 0x36,
	Xsens_WakeUp = 0x3E,
	Xsens_Reset = 0x40,
	Xsens_Error = 0x42,
	Xsens_ReqOptionFlags = 0x48,
	Xsens_SetOptionFlags = 0x48,
	Xsens_ReqUTCTime = 0x60,
	Xsens_SetUTCTime = 0x60,
	Xsens_UTCTime = 0x61,
	Xsens_ReqLatLonAlt = 0x6E,
	Xsens_SetLatLonAlt = 0x6E,
	Xsens_ReqLatLonAltAck = 0x6F,
	Xsens_SetLatLonAltAck = 0x6F,
	Xsens_ReqLocationID = 0x84,
	Xsens_SetLocationID = 0x84,
	Xsens_AdjustUTCTime = 0xA8,
	Xsens_ReqOutputConfiguration = 0xC0,
	Xsens_SetOutputConfiguration = 0xC0,
	Xsens_ReqAlignmentRotation = 0xEC,
	Xsens_SetAlignmentRotation = 0xEC
}XsensMID;

/*
 * Xsens ...
 */

//typedef struct
//{
//	uint16_t XDI_Temperature : 1;
//	uint16_t XDI_UtcTime : 1;
//	uint16_t XDI_PacketCounter : 1;
//	uint16_t XDI_SampleTimeFine : 1;
//	uint16_t XDI_SampleTimeCoarse : 1;
//	uint16_t XDI_DeltaV : 1;
//	uint16_t XDI_Acceleration : 1;
//	uint16_t XDI_AccelerationHR : 1;
//	uint16_t XDI_RateOfTurn : 1;
//	uint16_t XDI_DeltaQ : 1;
//	uint16_t XDI_RateOfTurnHR : 1;
//	uint16_t XDI_MagneticField : 1;
//	uint16_t XDI_StatusByte : 1;
//	uint16_t XDI_StatusWord : 1;
//}Xsens_dataStatus;


typedef enum
{
	 Xsens_DataNotAvailable = 0,
	 Xsens_DataAvailable
}Xsens_dataAvailable;

typedef enum
{
	 Xsens_Temperature = 0,
	 Xsens_UtcTime,
	 Xsens_PacketCounter,
	 Xsens_SampleTimeFine,
	 Xsens_SampleTimeCoarse,
	 Xsens_DeltaV,
	 Xsens_Acceleration,
	 Xsens_AccelerationHR,
	 Xsens_RateOfTurn,
	 Xsens_DeltaQ,
	 Xsens_RateOfTurnHR,
	 Xsens_MagneticField,
	 Xsens_StatusByte,
	 Xsens_StatusWord,
	 Xsens_NB
}Xsens_dataPacket;


typedef struct
{
	Xsens_dataPacket PacketName;
	_Bool 			 Enable;
	uint8_t 		 DataID[2];
	uint8_t			 Freq[2];
	uint8_t 		 Length;
}Xsens_configuration;

typedef struct
{
	Xsens_dataPacket    PacketName;
	uint8_t			    Data[16];
	Xsens_dataAvailable DataStatus;
}Xsens_measurements;

typedef struct
{
	uint32_t Ns;
	uint16_t Year;
	uint8_t  Month;
	uint8_t  Day;
	uint8_t  Hour;
	uint8_t  Minute;
	uint8_t  Second;
	uint8_t  Flags;
}Xsens_utcTime;

typedef struct
{
	float   vX;
	float   vY;
	float   vZ;
}Xsens_deltaV;

typedef struct
{
	float   accX;
	float   accY;
	float   accZ;
}Xsens_acceleration;


typedef struct
{
	float   gyrX;
	float   gyrY;
	float   gyrZ;
}Xsens_rateOfTurn;

typedef struct
{
	float   q1;
	float   q2;
	float   q3;
	float   q4;
}Xsens_deltaQ;


typedef struct
{
	float   magX;
	float   magY;
	float   magZ;
}Xsens_magneticField;

/*
 *  Functions prototypes
 */
void XsensMiscellaneous_waitUs(uint32_t microseconds);
void XsensMiscellaneous_uint8BufferToFloats(float* floats, uint8_t* buffer, uint8_t howManyFloats);
uint8_t XsensMiscellaneous_getUint8FromBuffer(uint8_t* buffer, uint8_t index);
uint16_t XsensMiscellaneous_getUint16FromBuffer(uint8_t* buffer, uint8_t index);
uint32_t XsensMiscellaneous_getUint32FromBuffer(uint8_t* buffer, uint8_t index);


#endif /* XSENS_XSENS_MISCELLANEOUS_H_ */
