#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define OS_TASK_NUM_MAX 15//最大任务数量


/**
 * @brief 存储任务相关信息
 */
typedef struct
{
    const char * name;  //任务名字
    TaskHandle_t handle;//任务句柄
    uint16_t stack_size;//任务栈大小
    UBaseType_t priority;//任务优先级
}task_list_t;

BaseType_t myTaskCreate(TaskFunction_t pxTaskCode,
                        const char * const pcName,	
                        const configSTACK_DEPTH_TYPE usStackDepth,
                        void * const pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t * const pxCreatedTask );



#endif
