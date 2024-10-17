/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-17 22:33:49
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd.h"
#include "config.h"
#include "usart.h"
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

SemaphoreHandle_t debug_print_mutex = NULL;
extern uart_dma_t debug_uart;      //调试串口句柄



/**
 * @brief 打印信息 
 * @param __format 
 * @param  
 */
void myprintf(const char*__format, ...)
{
	uart_dma_t * print_uart = &debug_uart;
    va_list args; // 创建一个va_list类型的变量，用来存储可变参数
    va_start(args, __format); // 使用va_start宏初始化args，使之指向第一个可选参数
    xSemaphoreTake(debug_print_mutex, 1);
    vsprintf((char*)print_uart->tx_data, __format, args); // 使用vsprintf而不是sprintf，因为它可以处理可变参数列表
    va_end(args); // 使用va_end宏清理args
    xSemaphoreGive(debug_print_mutex);
	HAL_UART_Transmit(print_uart->uart_t, (uint8_t*)print_uart->tx_data, strlen((char*)print_uart->tx_data), 10);

}


/**
 * @brief 输出日志 带时间戳
 * @param __format 
 * @param  
 */
void mylog(const char*__format, ...)
{
	uart_dma_t * print_uart = &debug_uart;
    xSemaphoreTake(debug_print_mutex, 1);
    sprintf((char*)print_uart->tx_data, "%02d:%02d:%02d:%03d\t", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds, rtc_ms);
    va_list args; // 创建一个va_list类型的变量，用来存储可变参数
    va_start(args, __format); // 使用va_start宏初始化args，使之指向第一个可选参数
    vsprintf((char*)print_uart->tx_data+13, __format, args); // 使用vsprintf而不是sprintf，因为它可以处理可变参数列表
    va_end(args); // 使用va_end宏清理args
    xSemaphoreGive(debug_print_mutex);
	HAL_UART_Transmit(print_uart->uart_t, (uint8_t*)print_uart->tx_data, strlen((char*)print_uart->tx_data), 10);
}



/**
 * @brief 命令列表索引
 */
#define HELP     0
#define SYSTEM_R 1


static cmd_t  cmd_list[CMD_COUNT_MAX] = {0};
uint16_t cmd_count = 0;//总命令数
/**
 * @brief 根据命令执行对应的函数
 * @param cmd 命令
 */
void find_cmd(char * cmd)
{
	if(cmd[0] == NULL)
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
    mylog("系统正在复位\r\n");
    vTaskDelay(1);
    HAL_NVIC_SystemReset();
}


/**
 * @brief 命令初始化函数
 *        其中初始化一些系统级的命令
 */
uint8_t cmd_init(void)
{
    debug_print_mutex = xSemaphoreCreateMutex();

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
