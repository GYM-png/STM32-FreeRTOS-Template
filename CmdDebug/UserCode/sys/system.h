#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define OS_TASK_NUM_MAX 15//�����������


/**
 * @brief �洢���������Ϣ
 */
typedef struct
{
    const char * name;  //��������
    TaskHandle_t handle;//������
    uint16_t stack_size;//����ջ��С
    UBaseType_t priority;//�������ȼ�
}task_list_t;

BaseType_t myTaskCreate(TaskFunction_t pxTaskCode,
                        const char * const pcName,	
                        const configSTACK_DEPTH_TYPE usStackDepth,
                        void * const pvParameters,
                        UBaseType_t uxPriority,
                        TaskHandle_t * const pxCreatedTask );



#endif
