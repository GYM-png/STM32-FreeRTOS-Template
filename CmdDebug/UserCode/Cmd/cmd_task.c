/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-14 20:42:59
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-16 20:14:17
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd_task.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd_task.h"
#include "cmd.h"

#include "myusart.h"
#include "system.h"
#include "global.h"

extern uint16_t cmd_count;//��������

static void system_print_version(void)
{
    mylog("ϵͳ�汾2.0\t��ʼʱ��2024��10��13��\r\n");
	myprintf("\r\n");
}

extern task_list_t system_task_list[OS_TASK_NUM_MAX];
extern uint8_t task_count;

static void system_print_task(void)
{
    UBaseType_t StackHighWaterMark;
	mylog("=======������Ϣ=======\r\n"); 
    for (uint8_t i = 0; i < task_count; i++)
    {
        StackHighWaterMark = uxTaskGetStackHighWaterMark(system_task_list[i].handle);  
        myprintf("%s\t����ʣ���ջ��%d\t�����ջʹ���ʣ�%d%%\r\n", system_task_list[i].name, StackHighWaterMark, StackHighWaterMark * 100 / system_task_list[i].stack_size);
		vTaskDelay(1);
    }
	myprintf("\r\n");
}

static void show_time(void)
{
    mylog("��ǰ���ڣ�%04d��%02d��%02d��\r\n", rtc_date.Year, rtc_date.Month, rtc_date.Date);
    mylog("��ǰʱ�䣺%02d:%02d:%02d\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
}



#define CMD_TASK_PRIO 4  //�������ȼ�
#define CMD_TASK_SIZE 200 //�����ջ��С
TaskHandle_t CMD_TASK_Handler;//������
void cmd_task(void *pvparameters);


void cmd_task_init(void)
{
    cmd_install("system -v", system_print_version);
    cmd_install("system -t", system_print_task);
    cmd_install("get -t", show_time);

    myTaskCreate((TaskFunction_t)cmd_task,
                (const char *)"cmd_task",
                (uint16_t)CMD_TASK_SIZE,
                (void *)NULL,
                (UBaseType_t)CMD_TASK_PRIO,
                (TaskHandle_t *)&CMD_TASK_Handler);
}



uart_dma_t debug_uart = {0};      //���Դ��ھ��
extern DMA_HandleTypeDef hdma_usart1_rx;


static void cmd_task(void *pvparameters)
{
	uart_dma_init(&debug_uart, &huart1, &hdma_usart1_rx);
    mylog("cmd_task work\r\n");
    for(;;)
    {
        if(debug_uart.rx_flag == 1)
        {
			debug_uart.rx_flag = 0;
            find_cmd((char*)debug_uart.rx_buffer);
        }
		vTaskDelay(20);
    }
}
