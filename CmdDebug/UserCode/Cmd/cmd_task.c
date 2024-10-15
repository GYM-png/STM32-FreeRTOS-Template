/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-14 20:42:59
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-15 22:51:59
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd_task.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd_task.h"
#include "cmd.h"

#include "system.h"
#include "global.h"






extern uint16_t cmd_count;//��������






static void system_print_version(void)
{
    mylog("ϵͳ�汾2.0\r\n��ʼʱ��2024��10��13��\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
}

extern task_list_t system_task_list[OS_TASK_NUM_MAX];
extern uint8_t task_count;

static void system_print_task(void)
{
    UBaseType_t StackHighWaterMark;
    for (uint8_t i = 0; i < task_count; i++)
    {
        StackHighWaterMark = uxTaskGetStackHighWaterMark(system_task_list[i].handle);  
        mylog("====������Ϣ\r\n===="); 
        myprintf("����%s\t����ʣ���ջ��%d\t�����ջʹ���ʣ�%d%%\r\n", system_task_list[i].name, StackHighWaterMark, StackHighWaterMark * 100 / system_task_list[i].stack_size);
    }
    
}










#define CMD_TASK_PRIO 4  //�������ȼ�
#define CMD_TASK_SIZE 100 //�����ջ��С
TaskHandle_t CMD_TASK_Handler;//������
void cmd_task(void *pvparameters);


void cmd_task_init(void)
{
    cmd_init();
    cmd_install("system -v", system_print_version);
    cmd_install("system -t", system_print_task);

    myTaskCreate((TaskFunction_t)cmd_task,
                (const char *)"cmd_task",
                (uint16_t)CMD_TASK_SIZE,
                (void *)NULL,
                (UBaseType_t)CMD_TASK_PRIO,
                (TaskHandle_t *)&CMD_TASK_Handler);
}




QueueHandle_t cmd_receive_queue = NULL;//�����������

static void cmd_task(void *pvparameters)
{
    cmd_receive_queue = xQueueCreate(5, sizeof(char));
    char * cmd_receive = (char *)pvPortMalloc(sizeof(char) * 20);//����һ�������ַ��������Ϊ20
    for(;;)
    {
        if(xQueueReceive(cmd_receive_queue, cmd_receive, 1000) == pdTRUE)
        {
            find_cmd(cmd_receive);
        }

    }
}
