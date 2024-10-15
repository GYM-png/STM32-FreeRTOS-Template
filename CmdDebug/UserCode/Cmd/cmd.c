/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-15 22:43:06
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd.h"
#include "myusart.h"
#include "global.h"
#include "stdlib.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> // 引入可变参数头文件

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

SemaphoreHandle_t debug_print_mutex = NULL;
QueueHandle_t debug_print_queue = NULL;
extern uart_dma_t debug_uart;      //调试串口句柄

// int fputc(int ch, FILE *f)
// {
//     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//     return ch;
// }

static char print_buff[UART_TX_LEN_MAX] = {0};

/**
 * @brief 输出日志 带时间戳
 * @param __format 
 * @param  
 */
void mylog(const char*__format, ...)
{
    xSemaphoreTake(debug_print_mutex, 1);
    sprintf(print_buff, "%02d:%02d:%02d:%03d   ", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds, rtc_ms);
    va_list args; // 创建一个va_list类型的变量，用来存储可变参数
    va_start(args, __format); // 使用va_start宏初始化args，使之指向第一个可选参数
    vsprintf(print_buff+12+4, __format, args); // 使用vsprintf而不是sprintf，因为它可以处理可变参数列表
    va_end(args); // 使用va_end宏清理args
    xQueueSendToBack(debug_print_queue, print_buff, 1);
    xSemaphoreGive(debug_print_mutex);
}

/**
 * @brief 打印信息 
 * @param __format 
 * @param  
 */
void myprintf(const char*__format, ...)
{
    va_list args; // 创建一个va_list类型的变量，用来存储可变参数
    va_start(args, __format); // 使用va_start宏初始化args，使之指向第一个可选参数
    xSemaphoreTake(debug_print_mutex, 1);
    vsprintf(print_buff, __format, args); // 使用vsprintf而不是sprintf，因为它可以处理可变参数列表
    va_end(args); // 使用va_end宏清理args
    xQueueSendToBack(debug_print_queue, print_buff, 1);
    xSemaphoreGive(debug_print_mutex);
}

/**
 * @brief 命令列表索引
 */
#define HELP     0
#define SYSTEM_R 1


static cmd_t * cmd_list = NULL;
uint16_t cmd_count = 0;//总命令数
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
    myprintf("命令错误，输入help查看命令\r\n");
}


/**
 * @brief 打印所有指令 help 指令对应的回调
 * @param  
 */
static void print_cmd_list(void)
{
    myprintf("====命令列表====\r\n");
    for (uint16_t i = 1; i < cmd_count; i++)
    {
        myprintf("%s\r\n", cmd_list[i].cmd);
    }
}

/**
 * @brief 系统重启
 * @param  
 */
static void system_rest(void)
{
    mylog("系统正在复位\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    vTaskDelay(1);
    HAL_NVIC_SystemReset();
}


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
    debug_print_mutex = xSemaphoreCreateMutex();
    debug_print_queue = xQueueCreate(10, sizeof(char)*50);

    
    /* 命令对应函数的实现 */
	strcpy(cmd_list[HELP].cmd, "help");
    cmd_list[HELP].callback = print_cmd_list;
    cmd_count++;
    
    cmd_list[SYSTEM_R].cmd = "system -r";
    cmd_list[SYSTEM_R].callback = system_rest;
    cmd_count++;

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
