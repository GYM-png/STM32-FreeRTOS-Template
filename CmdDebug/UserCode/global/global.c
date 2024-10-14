#include "global.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;


#ifdef USE_FREERTOS

uint32_t systern_run_time = 0;
uint16_t rtc_ms = 0;//系统时间ms rtc只能提供精确到s

task_list_t system_task_list[15];//任务列表 存储了所有任务的名字和句柄
uint8_t task_count = 0;//任务数量

/**
 * @defgroup start task相关参数
 */
#define START_TASK_PRIO 3  //任务优先级
#define START_TASK_SIZE 100 //任务堆栈大小
TaskHandle_t START_TASK_Handler;//任务句柄
void start_task(void *pvparameters);

void task_install(const char* name, TaskHandle_t handle, uint16_t stack_size)
{
    if (task_count >= 15)
    {
        return;
    }
    system_task_list[task_count].name = name;
    system_task_list[task_count].handle = handle;
    system_task_list[task_count].stack_size = stack_size;
    task_count++;
}



void task_init(void)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t )start_task,    //任务函数
                (const char *   )"start_task",   //任务名称
                (uint16_t       )START_TASK_SIZE, //任务堆栈大小
                (void *         )NULL,                //任务参数
                (UBaseType_t    )START_TASK_PRIO, //任务优先级
                (TaskHandle_t * )&START_TASK_Handler); //任务句句柄
    taskEXIT_CRITICAL();
}


static void start_task(void * pvparameters)
{
    /*获取系统初始时间*/
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);

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

#endif
