#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "main.h"
#include "rtc.h"

#define USE_FREERTOS

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief 存储任务相关信息
 */
typedef struct
{
    const char * name;  //任务名字
    TaskHandle_t handle;//任务句柄
    uint16_t stack_size;//任务栈大小
}task_list_t;


#endif

#define ERROR   1
#define OK      0







extern uint32_t systern_run_time;
extern uint16_t rtc_ms;//系统时间ms rtc只能提供精确到s

extern RTC_TimeTypeDef rtc_time;
extern RTC_DateTypeDef rtc_date;


#endif
