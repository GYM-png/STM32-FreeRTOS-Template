/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-13 16:52:59
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
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
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
    printf("\r\n命令错误，输入help查看命令\r\n");
}

static void debugPrintStart(void)
{
    // DebugPrintFunc("[]", 3);
    // my_printf("123");
    HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
}

static void debugPrintEnd(void)
{
    // DebugPrintFunc("\r\n", 3);
    printf("\r\n");
}

/**
 * @brief 打印所有指令 help 指令对应的回调
 * @param  
 */
static void print_cmd_list(void)
{
    printf("\r\n命令列表\r\n");
    for (uint16_t i = 1; i < cmd_count; i++)
    {
        printf("%s\r\n", cmd_list[i].cmd);
    }
}


static void systemRest(void)
{
    debugPrintStart();
    printf("\r\n[%2d:%2d:%2d]系统正在复位\r\n", rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
    HAL_NVIC_SystemReset();
    // debugPrintEnd();
}

static void systemVersion(void)
{
    debugPrintStart();
    printf("\r\n[%2d:%2d:%2d]系统版本1.0\r\n开始时间2024年10月13日\r\n", rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
    // debugPrintEnd();
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
    
    /* 命令对应函数的实现 */
	strcpy(cmd_list[HELP].cmd, "help");
    cmd_list[HELP].callback = print_cmd_list;
    cmd_count++;
    
    cmd_list[SYSTEM_R].cmd = "system -r";
    cmd_list[SYSTEM_R].callback = systemRest;
    cmd_count++;

    cmd_list[SYSTEM_V].cmd = "system -v";
    cmd_list[SYSTEM_V].callback = systemVersion;
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
