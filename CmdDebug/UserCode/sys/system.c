/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-15 20:15:30
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-16 22:11:10
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\sys\system.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "system.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

task_list_t system_task_list[OS_TASK_NUM_MAX] = {0};//�����б� �洢��������������ֺ;��
uint8_t task_count = 0;//��������


/**
 * @brief �������б���ע������
 * @param name ��������
 * @param handle ������
 * @param stack_size ����ջ��С
 * @param priority �������ȼ�
 */
static void task_install(const char* name, TaskHandle_t handle, const configSTACK_DEPTH_TYPE stack_size, UBaseType_t priority)
{
    if (task_count >= OS_TASK_NUM_MAX)
    {
        return;
    }
    /*ʵ�ָ����������ȼ��Ӵ�С����*/
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
 * @brief ��������
 * @param pxTaskCode    ������
 * @param pcName        ������
 * @param usStackDepth  ջ���
 * @param pvParameters  ����
 * @param uxPriority    ���ȼ�
 * @param pxCreatedTask ���
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




