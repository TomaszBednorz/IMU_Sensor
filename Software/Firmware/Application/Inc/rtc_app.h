#ifndef RTC_APP_H_
#define RTC_APP_H_

#include "stm32l4xx_hal.h"

#define RTC_MINS_IN_HOUR        ((uint32_t)60)
#define RTC_SECS_IN_MIN         ((uint32_t)60)
#define RTC_SUBSECS_IN_SEC      ((uint32_t)10000)

#define RTC_MSG_LEN             (7U)

void RtcApp_resetTime(void);
void RtcApp_updateTime(void);

void RtcApp_addTimeToBuffer(uint8_t buffer[], uint16_t bufferLen);

#endif /* RTC_APP_H_ */
