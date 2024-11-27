#include "global.h"
#include "system.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;
uint32_t systern_run_time = 0;
uint16_t rtc_ms = 0;//系统时间ms rtc只能提供精确到s



/**
 * @defgroup start task相关参数
 */
#define START_TASK_PRIO 3  //任务优先级
#define START_TASK_SIZE 200 //任务堆栈大小
TaskHandle_t START_TASK_Handler;//任务句柄
void start_task(void *pvparameters);





void start_task_init(void)
{
    cm_backtrace_init("CmBacktrace", "MC 2.0", "TestVersion 2.1");
    xTaskCreate((TaskFunction_t )start_task,    //任务函数
                (const char *   )"start_task",   //任务名称
                (uint16_t       )START_TASK_SIZE, //任务堆栈大小
                (void *         )NULL,                //任务参数
                (UBaseType_t    )START_TASK_PRIO, //任务优先级
                (TaskHandle_t * )&START_TASK_Handler); //任务句句柄
}

void fault_test_by_div0(void) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */

    x = 10;
    y = 0;
    z = x / y;
    printf("z:%d\n", z);
}
static void start_task(void * pvparameters)
{
    /*获取系统初始时间*/
    rtc_time.Hours = 0;
    rtc_time.Minutes = 0;
    rtc_time.Seconds = 0;
    rtc_date.Month = 11;
    rtc_date.Date = 13;
    rtc_date.Year = 24;
    HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
    HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
    log_i("系统开机成功 \r\n");
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
        vTaskDelay(800);
    }
}

