/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-14 23:02:13
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
 * @brief 命令列表索引
 */
#define HELP     0
#define SYSTEM_R 1
#define SYSTEM_V 2
#define SYSTEM_T 3

// /**
//  * @brief 命令列表
//  */
// static cmd_t cmd_list[] = {
//     {HELP, "help", NULL},
//     {SYSTEM_R, "system -r", NULL},
//     {SYSTEM_V, "system -v", NULL},
//     {SYSTEM_T, "system -t", NULL},
// };

static cmd_t * cmd_list = NULL;
static uint16_t cmd_count = 0;//总命令数
#define CMD_COUNT_MAX 30
/**
 * @brief 根据命令执行对应的函数
 * @param cmd 命令
 */
void find_cmd(char * cmd)
{
	if(cmd == NULL)
	{
		return;
	}
    for (uint16_t i = 0; i < cmd_count; i++)
    {
        //命令对比
        if (strncmp(cmd, cmd_list[i].cmd, strlen(cmd_list[i].cmd)) == 0)
        {
            cmd_list[i].callback();//执行回调
            return;
        }
    }
    myprint("命令错误，输入help查看命令\r\n");
}

static void debugPrintStart(void)
{
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BCD);
}


/**
 * @brief 打印所有指令 help 指令对应的回调
 * @param  
 */
static void print_cmd_list(void)
{
    myprint("命令列表\r\n");
    for (uint16_t i = 1; i < cmd_count; i++)
    {
        printf("%s\r\n", cmd_list[i].cmd);
    }
}


static void system_rest(void)
{
    debugPrintStart();
    myprint_with_time("系统正在复位\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    HAL_NVIC_SystemReset();
    // debugPrintEnd();
}

static void system_print_version(void)
{
    debugPrintStart();
    myprint("系统版本1.0\r\n开始时间2024年10月13日\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
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
        myprint_with_time("任务：%s\t任务剩余堆栈：%d\t任务堆栈使用率：%d%%\r\n", system_task_list[i].name, StackHighWaterMark, StackHighWaterMark * 100 / system_task_list[i].stack_size);
    }
    
}
#endif

/**
 * @brief 命令初始化函数
 *        其中初始化一些系统级的命令
 */
uint8_t cmd_init(void)
{
    cmd_list = (cmd_t *)malloc(CMD_COUNT_MAX * sizeof(cmd_t));
    if (cmd_list == NULL)   
    {
        return ERROR;
    }
    
    /* 命令对应函数的实现 */
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
 * @brief 命令注册
 * @param cmd 命令字符串
 * @param callback 回调函数
 */
void cmd_install(char* cmd, void(*callback)(void))
{
    if(cmd == NULL || callback == NULL)
    {
        return;
    } 
    if (cmd_count == CMD_COUNT_MAX)//防止溢出
    {
        return;
    }
    
    cmd_list[cmd_count].cmd = cmd;
    cmd_list[cmd_count].callback = callback;
    cmd_count++;
}
