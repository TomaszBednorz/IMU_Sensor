#ifndef XSENS_XSENS_APP_H_
#define XSENS_XSENS_APP_H_

#include "xsens_com.h"


#define XSENS_GROUP_TEMPERATURE				((uint8_t)0x08)
#define XSENS_GROUP_TIMESTAMP				((uint8_t)0x10)
#define XSENS_GROUP_ACCELERATION			((uint8_t)0x40)
#define XSENS_GROUP_ANHULAR_VELOCITY		((uint8_t)0x80)
#define XSENS_GROUP_MAGNETIC				((uint8_t)0xC0)
#define XSENS_GROUP_STATUS					((uint8_t)0xE0)

#define XSENS_TYPE_XDI_TEMPERATURE			((uint8_t)0x10)
#define XSENS_TYPE_XDI_UTC_TIME				((uint8_t)0x10)
#define XSENS_TYPE_XDI_PACKET_COUNTER		((uint8_t)0x20)
#define XSENS_TYPE_XDI_SAMPLE_TIME_FINE		((uint8_t)0x60)
#define XSENS_TYPE_XDI_SAMPLE_TIME_COARSE	((uint8_t)0x70)
#define XSENS_TYPE_XDI_DELTA_V				((uint8_t)0x10)
#define XSENS_TYPE_XDI_ACCELERATION			((uint8_t)0x20)
#define XSENS_TYPE_XDI_ACCELERATION_HR		((uint8_t)0x40)
#define XSENS_TYPE_XDI_RATE_OF_TURN			((uint8_t)0x20)
#define XSENS_TYPE_XDI_DELTA_Q				((uint8_t)0x30)
#define XSENS_TYPE_XDI_RATE_OF_TURN_HR		((uint8_t)0x40)
#define XSENS_TYPE_XDI_MAGNETIC_FIELD		((uint8_t)0x20)
#define XSENS_TYPE_XDI_STATUS_BYTE			((uint8_t)0x10)
#define XSENS_TYPE_XDI_STATUS_WORD			((uint8_t)0x20)

/*
 *  Functions prototypes
 */
void XsensApp_init(void);
void XsensApp_reset(void);
void XsensApp_setOutputConfiguration(void);
void XsensApp_updateOutputConfiguration(uint8_t* config);

#endif /* XSENS_XSENS_APP_H_ */
