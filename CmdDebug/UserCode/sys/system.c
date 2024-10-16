/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-15 20:15:30
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-16 22:11:10
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\sys\system.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "system.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

task_list_t system_task_list[OS_TASK_NUM_MAX] = {0};//任务列表 存储了所有任务的名字和句柄
uint8_t task_count = 0;//任务数量


/**
 * @brief 在任务列表中注册任务
 * @param name 任务名字
 * @param handle 任务句柄
 * @param stack_size 任务栈大小
 * @param priority 任务优先级
 */
static void task_install(const char* name, TaskHandle_t handle, const configSTACK_DEPTH_TYPE stack_size, UBaseType_t priority)
{
    if (task_count >= OS_TASK_NUM_MAX)
    {
        return;
    }
    /*实现根据任务优先级从大到小排序*/
    int i;
    for (i = task_count - 1; i >= 0; i--)
    {
        if (priority <= system_task_list[i].priority) 
        {
            break;
        }
    }

    for (int j = task_count - 1; j > i; j--)
    {
        system_task_list[j + 1] = system_task_list[j];
    }

    system_task_list[i + 1].name = name;
    system_task_list[i + 1].handle = handle;
    system_task_list[i + 1].stack_size = stack_size;
    system_task_list[i + 1].priority = priority;    task_count++;
}


/**
 * @brief 创建任务
 * @param pxTaskCode    任务函数
 * @param pcName        任务名
 * @param usStackDepth  栈深度
 * @param pvParameters  参数
 * @param uxPriority    优先级
 * @param pxCreatedTask 句柄
 * @return 
 */
BaseType_t myTaskCreate(TaskFunction_t pxTaskCode,
                        const char * const pcName,	
                        const configSTACK_DEPTH_TYPE usStackDepth,
                        void * const pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t * const pxCreatedTask ) 
{
    taskENTER_CRITICAL();
    BaseType_t re_value = xTaskCreate(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask);
    task_install(pcName, *pxCreatedTask, usStackDepth, uxPriority);
    taskEXIT_CRITICAL();
    return re_value;
}




