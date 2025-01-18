#include "xsens_app.h"


/*
 * Static functions prototypes
 */
#ifndef UNUSED_CODE
static void XsensApp_gatherData(uint8_t* buffer, uint8_t bufferLen);
static Xsens_dataPacket XsensApp_getPacket(uint8_t group, uint8_t type);
static void XsensApp_processData(Xsens_dataPacket packetName);
#endif

/*
 *      |          Name          |   Enable (disable/enable)   |                     DataID (group, type)                     |    Format    |   Length   |
 */
	Xsens_configuration XsensConfiguration[Xsens_NB] = {
		{Xsens_Temperature, 	 XSENS_CFG_TEMPERATURE, 	   {XSENS_GROUP_TEMPERATURE, XSENS_TYPE_XDI_TEMPERATURE}, 		   {0xff, 0xff}, XSENS_LEN_4},  // maybe get out with length - to be decided
		{Xsens_UtcTime,			 XSENS_CFG_UTC_TIME, 		   {XSENS_GROUP_TIMESTAMP, XSENS_TYPE_XDI_UTC_TIME}, 			   {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_PacketCounter,    XSENS_CFG_PACKET_COUNTER, 	   {XSENS_GROUP_TIMESTAMP, XSENS_TYPE_XDI_PACKET_COUNTER}, 		   {0xff, 0xff}, XSENS_LEN_2},
		{Xsens_SampleTimeFine,   XSENS_CFG_SAMPLE_TIME_FINE,   {XSENS_GROUP_TIMESTAMP, XSENS_TYPE_XDI_SAMPLE_TIME_FINE}, 	   {0xff, 0xff}, XSENS_LEN_4},
		{Xsens_SampleTimeCoarse, XSENS_CFG_SAMPLE_TIME_COARSE, {XSENS_GROUP_TIMESTAMP, XSENS_TYPE_XDI_SAMPLE_TIME_COARSE}, 	   {0xff, 0xff}, XSENS_LEN_4},
		{Xsens_DeltaV, 			 XSENS_CFG_DELTA_V, 		   {XSENS_GROUP_ACCELERATION, XSENS_TYPE_XDI_DELTA_V}, 			   {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_Acceleration,	 XSENS_CFG_ACCELERATION, 	   {XSENS_GROUP_ACCELERATION, XSENS_TYPE_XDI_ACCELERATION},		   {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_AccelerationHR,	 XSENS_CFG_ACCELERATION_HR,    {XSENS_GROUP_ACCELERATION, XSENS_TYPE_XDI_ACCELERATION_HR},     {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_RateOfTurn, 		 XSENS_CFG_RATE_OF_TURN, 	   {XSENS_GROUP_ANHULAR_VELOCITY, XSENS_TYPE_XDI_RATE_OF_TURN},    {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_DeltaQ, 			 XSENS_CFG_DELTA_Q, 		   {XSENS_GROUP_ANHULAR_VELOCITY, XSENS_TYPE_XDI_DELTA_Q}, 		   {0xff, 0xff}, XSENS_LEN_16},
		{Xsens_RateOfTurnHR, 	 XSENS_CFG_RATE_OF_TURN_HR,    {XSENS_GROUP_ANHULAR_VELOCITY, XSENS_TYPE_XDI_RATE_OF_TURN_HR}, {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_MagneticField, 	 XSENS_CFG_MAGNETIC_FIELD, 	   {XSENS_GROUP_MAGNETIC, XSENS_TYPE_XDI_MAGNETIC_FIELD}, 		   {0xff, 0xff}, XSENS_LEN_12},
		{Xsens_StatusByte, 		 XSENS_CFG_STATUS_BYTE,		   {XSENS_GROUP_STATUS, XSENS_TYPE_XDI_STATUS_BYTE}, 			   {0xff, 0xff}, XSENS_LEN_1},
		{Xsens_StatusWord, 		 XSENS_CFG_STATUS_WORD, 	   {XSENS_GROUP_STATUS, XSENS_TYPE_XDI_STATUS_WORD}, 			   {0xff, 0xff}, XSENS_LEN_4},
		  };


/*
 *      |         Name         |  Data  |     DataStatus     |
 */
	Xsens_measurements XsensMeasurements[Xsens_NB] = {
		{Xsens_Temperature, 	 {0}, Xsens_DataNotAvailable},
		{Xsens_UtcTime,			 {0}, Xsens_DataNotAvailable},
		{Xsens_PacketCounter,    {0}, Xsens_DataNotAvailable},
		{Xsens_SampleTimeFine,   {0}, Xsens_DataNotAvailable},
		{Xsens_SampleTimeCoarse, {0}, Xsens_DataNotAvailable},
		{Xsens_DeltaV, 			 {0}, Xsens_DataNotAvailable},
		{Xsens_Acceleration,	 {0}, Xsens_DataNotAvailable},
		{Xsens_AccelerationHR,	 {0}, Xsens_DataNotAvailable},
		{Xsens_RateOfTurn, 		 {0}, Xsens_DataNotAvailable},
		{Xsens_DeltaQ, 			 {0}, Xsens_DataNotAvailable},
		{Xsens_RateOfTurnHR, 	 {0}, Xsens_DataNotAvailable},
		{Xsens_MagneticField, 	 {0}, Xsens_DataNotAvailable},
		{Xsens_StatusByte, 		 {0}, Xsens_DataNotAvailable},
		{Xsens_StatusWord, 		 {0}, Xsens_DataNotAvailable},
		  };

	float 				XsensTemperature = 0.0f;
	Xsens_utcTime 		XsensUtcTime = {0};
	uint16_t 			XsensPacketCounter = 0;
	uint32_t 			XsensSampleTimeFine = 0;
	uint32_t  			XsensSampleTimeCoarse = 0;
	Xsens_deltaV 		XsensDeltaV = {0};
	Xsens_acceleration 	XsensAcceleration = {0};
	Xsens_acceleration 	XsensAccelerationHR = {0};
	Xsens_rateOfTurn 	XsensRateOfTurn = {0};
	Xsens_deltaQ 		XsensDeltaQ = {0};
	Xsens_rateOfTurn 	XsensRateOfTurnHR = {0};
	Xsens_magneticField XsensMagneticField = {0};
	uint8_t 			XsensStatusByte = 0;
	uint32_t 			XsensStatusWord = 0;

	uint16_t notificationPipeLen, measurementPipeLen;
	uint8_t notificationBuffer[256];
	uint8_t measurementBuffer[256];


/*!	\brief Initialize device
*/
void XsensApp_init(void)
{
	XsensCom_GoToConfig();
	HAL_Delay(10);

	XsensApp_setOutputConfiguration();
	HAL_Delay(150);

	XsensCom_GoToMeasurement();
}

/*!	\brief Reset device
*/
void XsensApp_reset(void)
{
	XsensCom_Reset();
}

/*!	\brief Set device configuration
*/
void XsensApp_setOutputConfiguration(void)
{
	uint8_t buffer[56];
	uint8_t* buffer_ptr = buffer;
	uint8_t bufferLen = 0x00;


	for(uint8_t i = 0; i < Xsens_NB; i++)
	{
		if(XsensConfiguration[i].Enable == XSENS_ENABLE)
		{
			*buffer_ptr++ = XsensConfiguration[i].DataID[0];
			*buffer_ptr++ = XsensConfiguration[i].DataID[1];
			*buffer_ptr++ = XsensConfiguration[i].Freq[0];
			*buffer_ptr++ = XsensConfiguration[i].Freq[1];
			bufferLen += 4;
		}
	}

	if(bufferLen > 0)
	{
		XsensCom_setOutputConfiguration(buffer, bufferLen);
	}
}

/*!	\brief Update device configuration
*/
void XsensApp_updateOutputConfiguration(uint8_t* config)
{
	uint8_t* buffer_ptr = config;
	for(uint8_t i = 0; i < Xsens_NB; i++)
	{
		XsensConfiguration[i].Enable = *buffer_ptr++;
	}
}


#ifndef UNUSED_CODE
/*!	\brief Gather data from buffer and add into appropriate structure
*/
static void XsensApp_gatherData(uint8_t* buffer, uint8_t bufferLen)
{
	uint8_t dataLen = buffer[1];
	uint8_t* buffer_ptr = &buffer[2];
	uint8_t group, type, packetLen, idx;
	Xsens_dataPacket packetType;

	while(dataLen > 0)
	{
		idx = 0x00;

		group = *buffer_ptr++;
		type = *buffer_ptr++;
		packetLen = *buffer_ptr++;

		packetType = XsensApp_getPacket(group, type);

		while(packetLen > 0)
		{
			XsensMeasurements[packetType].Data[idx] = *buffer_ptr++;
			packetLen--;
		}
		XsensMeasurements[packetType].DataStatus = Xsens_DataAvailable;

		dataLen -= (packetLen + 3);
	}
}

/*!	\brief Return data type of data based on group number and type number
   \param[in] group Group defines the category of the data
   \param[in] type Type combined with Group defines the actual type of the data
*/
static Xsens_dataPacket XsensApp_getPacket(uint8_t group, uint8_t type)
{
	Xsens_dataPacket result = XSENS_ERROR_VALUE;
	switch (group) {
		case XSENS_GROUP_TEMPERATURE:
			result = Xsens_Temperature;
			break;
		case XSENS_GROUP_TIMESTAMP:
			switch (type) {
				case XSENS_TYPE_XDI_UTC_TIME:
					result = Xsens_UtcTime;
					break;
				case XSENS_TYPE_XDI_PACKET_COUNTER:
					result = Xsens_PacketCounter;
					break;
				case XSENS_TYPE_XDI_SAMPLE_TIME_FINE:
					result = Xsens_SampleTimeFine;
					break;
				case XSENS_TYPE_XDI_SAMPLE_TIME_COARSE:
					result = Xsens_SampleTimeCoarse;
					break;
				default:
					break;
			}
			break;
		case XSENS_GROUP_ACCELERATION:
			switch (type) {
				case XSENS_TYPE_XDI_DELTA_V:
					result = Xsens_DeltaV;
					break;
				case XSENS_TYPE_XDI_ACCELERATION:
					result = Xsens_Acceleration;
					break;
				case XSENS_TYPE_XDI_ACCELERATION_HR:
					result = Xsens_AccelerationHR;
					break;
				default:
					break;
			}
			break;
		case XSENS_GROUP_ANHULAR_VELOCITY:
			switch (type) {
				case XSENS_TYPE_XDI_RATE_OF_TURN:
					result = Xsens_RateOfTurn;
					break;
				case XSENS_TYPE_XDI_DELTA_Q:
					result = Xsens_DeltaQ;
					break;
				case XSENS_TYPE_XDI_RATE_OF_TURN_HR:
					result = Xsens_RateOfTurnHR;
					break;
				default:
					break;
			}
			break;
		case XSENS_GROUP_MAGNETIC:
			result = Xsens_MagneticField;
			break;
		case XSENS_GROUP_STATUS:
			switch (type) {
				case XSENS_TYPE_XDI_STATUS_BYTE:
					result = Xsens_StatusByte;
					break;
				case XSENS_TYPE_XDI_STATUS_WORD:
					result = Xsens_StatusWord;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return result;
}

/*!	\brief Transform gathered data into appropriate format and store in appropriate containers
*/
static void XsensApp_processData(Xsens_dataPacket packetName)
{
	float temp[4];

	switch (packetName) {
		case Xsens_Temperature:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_Temperature].Data, 1);
			XsensTemperature = temp[0];
			break;
		case Xsens_UtcTime:
			XsensUtcTime.Ns = XsensMiscellaneous_getUint32FromBuffer(XsensMeasurements[Xsens_UtcTime].Data, 0);
			XsensUtcTime.Year = XsensMiscellaneous_getUint16FromBuffer(XsensMeasurements[Xsens_UtcTime].Data, 4);
			XsensUtcTime.Month = XsensMeasurements[Xsens_UtcTime].Data[6];
			XsensUtcTime.Day = XsensMeasurements[Xsens_UtcTime].Data[7];
			XsensUtcTime.Hour = XsensMeasurements[Xsens_UtcTime].Data[8];
			XsensUtcTime.Minute = XsensMeasurements[Xsens_UtcTime].Data[9];
			XsensUtcTime.Second = XsensMeasurements[Xsens_UtcTime].Data[10];
			XsensUtcTime.Flags = XsensMeasurements[Xsens_UtcTime].Data[11];
			break;
		case Xsens_PacketCounter:
			XsensPacketCounter = XsensMiscellaneous_getUint16FromBuffer(XsensMeasurements[Xsens_PacketCounter].Data, 0);
			break;
		case Xsens_SampleTimeFine:
			XsensSampleTimeFine = XsensMiscellaneous_getUint32FromBuffer(XsensMeasurements[Xsens_SampleTimeFine].Data, 0);
			break;
		case Xsens_SampleTimeCoarse:
			XsensSampleTimeCoarse = XsensMiscellaneous_getUint32FromBuffer(XsensMeasurements[Xsens_SampleTimeCoarse].Data, 0);
			break;
		case Xsens_DeltaV:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_DeltaV].Data, 3);
			XsensDeltaV.vX = temp[0];
			XsensDeltaV.vY = temp[1];
			XsensDeltaV.vZ = temp[2];
			break;
		case Xsens_Acceleration:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_Acceleration].Data, 3);
			XsensAcceleration.accX = temp[0];
			XsensAcceleration.accY = temp[1];
			XsensAcceleration.accZ = temp[2];
			break;
		case Xsens_AccelerationHR:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_AccelerationHR].Data, 3);
			XsensAccelerationHR.accX = temp[0];
			XsensAccelerationHR.accY = temp[1];
			XsensAccelerationHR.accZ = temp[2];
			break;
		case Xsens_RateOfTurn:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_RateOfTurn].Data, 3);
			XsensRateOfTurn.gyrX = temp[0];
			XsensRateOfTurn.gyrY = temp[1];
			XsensRateOfTurn.gyrZ = temp[2];
			break;
		case Xsens_DeltaQ:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_DeltaQ].Data, 4);
			XsensDeltaQ.q1 = temp[0];
			XsensDeltaQ.q2 = temp[1];
			XsensDeltaQ.q3 = temp[2];
			XsensDeltaQ.q4 = temp[3];
			break;
		case Xsens_RateOfTurnHR:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_RateOfTurnHR].Data, 3);
			XsensRateOfTurnHR.gyrX = temp[0];
			XsensRateOfTurnHR.gyrY = temp[1];
			XsensRateOfTurnHR.gyrZ = temp[2];
			break;
		case Xsens_MagneticField:
			XsensMiscellaneous_uint8BufferToFloats(temp, XsensMeasurements[Xsens_MagneticField].Data, 3);
			XsensMagneticField.magX = temp[0];
			XsensMagneticField.magY = temp[1];
			XsensMagneticField.magZ = temp[2];
			break;
		case Xsens_StatusByte:
			XsensStatusByte = XsensMeasurements[Xsens_StatusByte].Data[0];
			break;
		case Xsens_StatusWord:
			XsensStatusWord = XsensMiscellaneous_getUint32FromBuffer(XsensMeasurements[Xsens_StatusWord].Data, 0);
			break;
		default:
			break;
	}
}
#endif
