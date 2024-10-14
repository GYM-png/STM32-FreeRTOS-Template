#include "global.h"

RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;


#ifdef USE_FREERTOS

uint32_t systern_run_time = 0;
uint16_t rtc_ms = 0;//ϵͳʱ��ms rtcֻ���ṩ��ȷ��s

task_list_t system_task_list[15];//�����б� �洢��������������ֺ;��
uint8_t task_count = 0;//��������

/**
 * @defgroup start task��ز���
 */
#define START_TASK_PRIO 3  //�������ȼ�
#define START_TASK_SIZE 100 //�����ջ��С
TaskHandle_t START_TASK_Handler;//������
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
    xTaskCreate((TaskFunction_t )start_task,    //������
                (const char *   )"start_task",   //��������
                (uint16_t       )START_TASK_SIZE, //�����ջ��С
                (void *         )NULL,                //�������
                (UBaseType_t    )START_TASK_PRIO, //�������ȼ�
                (TaskHandle_t * )&START_TASK_Handler); //�������
    taskEXIT_CRITICAL();
}


static void start_task(void * pvparameters)
{
    /*��ȡϵͳ��ʼʱ��*/
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
