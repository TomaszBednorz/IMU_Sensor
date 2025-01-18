#include "rtc_app.h"


extern RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef currentTime;
static RTC_DateTypeDef currentDate;

static RTC_TimeTypeDef defaultTime = 
{
    .Hours = 0x0,
    .Minutes = 0x0,
    .Seconds = 0x0,
    .SubSeconds = 0x0
};

/*!	
 *\brief Reset actual RTC time.
 */
void RtcApp_resetTime(void)
{
  if (HAL_RTC_SetTime(&hrtc, &defaultTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    // Error_Handler();
  }
}


/*!	
 *\brief Function updates time and data.
 */
void RtcApp_updateTime(void)
{
    if (HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        // Error_Handler();
    }
    if (HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        // Error_Handler();
    }
}

/*!	\brief Function fills buffer with actual time.
	\param[in] buffer Buffer to append data.
	\param[in] bufferLen Length of buffer.
*/
void RtcApp_addTimeToBuffer(uint8_t buffer[], uint16_t bufferLen)
{
  uint32_t time;

  time = (RTC_SUBSECS_IN_SEC - currentTime.SubSeconds) + 
         (uint32_t)currentTime.Seconds * RTC_SUBSECS_IN_SEC +
         (uint32_t)currentTime.Minutes * RTC_SUBSECS_IN_SEC * RTC_SECS_IN_MIN +
         (uint32_t)currentTime.Hours * RTC_SUBSECS_IN_SEC * RTC_SECS_IN_MIN * RTC_MINS_IN_HOUR;

  buffer[1] += 7; 

  buffer[bufferLen - 1] = 0xf0;
  buffer[bufferLen] = 0x10;
  buffer[bufferLen + 1] = 0x04;
  buffer[bufferLen + 2] = (uint8_t)((time & 0xff000000) >> 24);
  buffer[bufferLen + 3] = (uint8_t)((time & 0x00ff0000) >> 16);
  buffer[bufferLen + 4] = (uint8_t)((time & 0x0000ff00) >> 8);
  buffer[bufferLen + 5] = (uint8_t)((time & 0x000000ff) >> 0);

}
