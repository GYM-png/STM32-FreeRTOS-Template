/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-14 23:02:13
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd.h"
#include "string.h"
#include "usbd_cdc_if.h"
#include "myusart.h"
#include "global.h"
#include "stdlib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t debug_print_mutex = NULL;

#endif

int fputc(int ch, FILE *f)
{
#ifdef USE_FREERTOS

    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
#else
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
#endif
  return ch;
}

int myprint_with_time(const char*__format, ...)
{
    // HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    // HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
    printf("\r\n[%2d:%2d:%2d]", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    printf(__format);
}

int myprint(const char*__format, ...)
{
    printf(__format);
}

/**
 * @brief �����б�����
 */
#define HELP     0
#define SYSTEM_R 1
#define SYSTEM_V 2
#define SYSTEM_T 3

// /**
//  * @brief �����б�
//  */
// static cmd_t cmd_list[] = {
//     {HELP, "help", NULL},
//     {SYSTEM_R, "system -r", NULL},
//     {SYSTEM_V, "system -v", NULL},
//     {SYSTEM_T, "system -t", NULL},
// };

static cmd_t * cmd_list = NULL;
static uint16_t cmd_count = 0;//��������
#define CMD_COUNT_MAX 30
/**
 * @brief ��������ִ�ж�Ӧ�ĺ���
 * @param cmd ����
 */
void find_cmd(char * cmd)
{
	if(cmd == NULL)
	{
		return;
	}
    for (uint16_t i = 0; i < cmd_count; i++)
    {
        //����Ա�
        if (strncmp(cmd, cmd_list[i].cmd, strlen(cmd_list[i].cmd)) == 0)
        {
            cmd_list[i].callback();//ִ�лص�
            return;
        }
    }
    myprint("�����������help�鿴����\r\n");
}

static void debugPrintStart(void)
{
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
}


/**
 * @brief ��ӡ����ָ�� help ָ���Ӧ�Ļص�
 * @param  
 */
static void print_cmd_list(void)
{
    myprint("�����б�\r\n");
    for (uint16_t i = 1; i < cmd_count; i++)
    {
        printf("%s\r\n", cmd_list[i].cmd);
    }
}


static void system_rest(void)
{
    debugPrintStart();
    myprint_with_time("ϵͳ���ڸ�λ\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    HAL_NVIC_SystemReset();
    // debugPrintEnd();
}

static void system_print_version(void)
{
    debugPrintStart();
    myprint("ϵͳ�汾1.0\r\n��ʼʱ��2024��10��13��\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    // debugPrintEnd();
}

#ifdef USE_FREERTOS
extern task_list_t system_task_list[15];
extern uint8_t task_count;

static void system_print_task(void)
{
    UBaseType_t StackHighWaterMark;
    for (uint8_t i = 0; i < task_count; i++)
    {
        StackHighWaterMark = uxTaskGetStackHighWaterMark(system_task_list[i].handle);   
        myprint_with_time("����%s\t����ʣ���ջ��%d\t�����ջʹ���ʣ�%d%%\r\n", system_task_list[i].name, StackHighWaterMark, StackHighWaterMark * 100 / system_task_list[i].stack_size);
    }
    
}
#endif

/**
 * @brief �����ʼ������
 *        ���г�ʼ��һЩϵͳ��������
 */
uint8_t cmd_init(void)
{
    cmd_list = (cmd_t *)malloc(CMD_COUNT_MAX * sizeof(cmd_t));
    if (cmd_list == NULL)   
    {
        return ERROR;
    }
    
    /* �����Ӧ������ʵ�� */
	strcpy(cmd_list[HELP].cmd, "help");
    cmd_list[HELP].callback = print_cmd_list;
    cmd_count++;
    
    cmd_list[SYSTEM_R].cmd = "system -r";
    cmd_list[SYSTEM_R].callback = system_rest;
    cmd_count++;

    cmd_list[SYSTEM_V].cmd = "system -v";
    cmd_list[SYSTEM_V].callback = system_print_version;
    cmd_count++;
#ifdef USE_FREERTOS
    cmd_list[SYSTEM_T].cmd = "system -t";
    cmd_list[SYSTEM_T].callback = system_print_task;
    cmd_count++;
#endif
    return OK;
}

/**
 * @brief ����ע��
 * @param cmd �����ַ���
 * @param callback �ص�����
 */
void cmd_install(char* cmd, void(*callback)(void))
{
    if(cmd == NULL || callback == NULL)
    {
        return;
    } 
    if (cmd_count == CMD_COUNT_MAX)//��ֹ���
    {
        return;
    }
    
    cmd_list[cmd_count].cmd = cmd;
    cmd_list[cmd_count].callback = callback;
    cmd_count++;
}
