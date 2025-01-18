#include "tasks_app.h"

#include "rtc_app.h"

uint16_t notificationPipeLen, measurementPipeLen;
uint8_t notificationBuffer[256];
uint8_t measurementBuffer[256];

extern RTC_HandleTypeDef hrtc;

/*!	
 *\brief RTOS task initialization.
 */
void TasksApp_init(void)
{
    BaseType_t status;

    status = xTaskCreate(TasksApp_xsensRoutine, "XsensRoutine_Task", 400, NULL, 3, &taskXsensHandle);
    configASSERT(status = pdPASS);

    status = xTaskCreate(TasksApp_ethLoop, "EthLoop_Task", 400, NULL, 1, &taskEthLoopHandle);
    configASSERT(status = pdPASS);

    status = xTaskCreate(TasksApp_ethRx, "EthRx_Task", 400, NULL, 2, &taskEthRxHandle);
    configASSERT(status = pdPASS);

}

/*!	
 *\brief Task with routine after Xsens interrupt.
 */
void TasksApp_xsensRoutine(void* parameters)
{
    BaseType_t status;

    while(1)
    {
        status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(15));  // Wait for interrupt or 15ms


        if(pdTRUE == status)
        {
            XsensCom_getPipeStatus(&notificationPipeLen, &measurementPipeLen);

            if(notificationPipeLen > 0)
            {
                XsensCom_getNotificationData(notificationBuffer, notificationPipeLen);
            }

            if(measurementPipeLen > 0)
            {
                XsensCom_getMeasurementData(measurementBuffer, measurementPipeLen);
                RtcApp_addTimeToBuffer(measurementBuffer, measurementPipeLen);
                
                W5200App_tx(measurementBuffer, measurementPipeLen + RTC_MSG_LEN);
            }
        }
        else if((pdFALSE == status) && (GPIO_PIN_SET == HAL_GPIO_ReadPin(IMU_DRDY_GPIO_Port, IMU_DRDY_Pin)))
        {
            XsensCom_getPipeStatus(&notificationPipeLen, &measurementPipeLen);

            if(notificationPipeLen > 0)
            {
                XsensCom_getNotificationData(notificationBuffer, notificationPipeLen);
            }

            if(measurementPipeLen > 0)
            {
                XsensCom_getMeasurementData(measurementBuffer, measurementPipeLen);
                RtcApp_addTimeToBuffer(measurementBuffer, measurementPipeLen);

                W5200App_tx(measurementBuffer, measurementPipeLen + RTC_MSG_LEN);
            }
        }
    }
}

/*!	
 *\brief W5200 counitinous routine.
 */
void TasksApp_ethLoop(void* parameters)
{
    while(1)
    {
        W5200App_loopback();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

/*!	
 *\brief Task with routine after W5200 module interrupt.
 */
void TasksApp_ethRx(void* parameters)
{
    BaseType_t status;
    uint8_t buf[20] = {0};
    while(1)
    {
        status = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
   
        if(pdTRUE == status)
        {
            W5200App_rx(buf);
            if(buf[0] != 0x00)
            {
                if(buf[0] == 0x80)  // Synchronize timers
                {
                    // Do nothing
                }
                if(buf[0] == 0x81)  // Update Xsens configuration
                {   
                    vTaskSuspend(taskXsensHandle);  // Disable receiving data during configuration update 
                    XsensApp_updateOutputConfiguration(&buf[1]);
                    XsensApp_init();
                    vTaskResume(taskXsensHandle);  
                }
                memset(buf, 0x00, 20);
            }
        }
    }
}
