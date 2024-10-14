#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "main.h"
#include "rtc.h"

#define USE_FREERTOS

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief �洢���������Ϣ
 */
typedef struct
{
    const char * name;  //��������
    TaskHandle_t handle;//������
    uint16_t stack_size;//����ջ��С
}task_list_t;


#endif

#define ERROR   1
#define OK      0







extern uint32_t systern_run_time;
extern uint16_t rtc_ms;//ϵͳʱ��ms rtcֻ���ṩ��ȷ��s

extern RTC_TimeTypeDef rtc_time;
extern RTC_DateTypeDef rtc_date;


#endif
