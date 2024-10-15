/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-13 00:22:43
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-15 23:06:07
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\global\global.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "global.h"
#include "system.h"
#include "usart_task.h"
#include "cmd_task.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;



uint32_t systern_run_time = 0;
uint16_t rtc_ms = 0;//ϵͳʱ��ms rtcֻ���ṩ��ȷ��s



/**
 * @defgroup start task��ز���
 */
#define START_TASK_PRIO 3  //�������ȼ�
#define START_TASK_SIZE 100 //�����ջ��С
TaskHandle_t START_TASK_Handler;//������
void start_task(void *pvparameters);





void start_task_init(void)
{
    myTaskCreate((TaskFunction_t )start_task,    //������
                (const char *   )"start_task",   //��������
                (uint16_t       )START_TASK_SIZE, //�����ջ��С
                (void *         )NULL,                //�������
                (UBaseType_t    )START_TASK_PRIO, //�������ȼ�
                (TaskHandle_t * )&START_TASK_Handler); //�������
}


static void start_task(void * pvparameters)
{
    /*��ȡϵͳ��ʼʱ��*/
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
    usart_task_init();
    cmd_task_init();

    for(;;)
    {
        if(rtc_ms >= 1000)
        {
            rtc_ms = 0;
            rtc_time.Seconds++;
        }
        if(rtc_time.Seconds >= 60)
        {
            rtc_time.Seconds = 0;
            rtc_time.Minutes++;
        }
        if(rtc_time.Minutes >= 60)
        {
            rtc_time.Minutes = 0;
            rtc_time.Hours++;
        }

        HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
        HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
        vTaskDelay(50);
    }
}

