#ifndef XSENS_XSENS_COM_H_
#define XSENS_XSENS_COM_H_

#include "xsens_config.h"
#include "xsens_miscellaneous.h"

#define XSENS_NO_DATA_BUF_LEN				(7)



/*
 *  Functions prototypes
 */
void XsensCom_createMessage(uint8_t* buffer, uint8_t msgID, uint8_t* txData, uint16_t txDataLen);

void XsensCom_getPipeStatus(uint16_t* notificationPipe, uint16_t* measurementPipe);
void XsensCom_getNotificationData(uint8_t* buffer, uint16_t bufferLen);
void XsensCom_getMeasurementData(uint8_t* buffer, uint16_t bufferLen);

void XsensCom_GoToConfig(void);

void XsensCom_GoToMeasurement(void);

void XsensCom_Reset(void);

void XsensCom_getOutputConfiguration(uint8_t* buffer, uint16_t bufferLen);
void XsensCom_setOutputConfiguration(uint8_t* buffer, uint16_t bufferLen);

#endif /* XSENS_XSENS_COM_H_ */
