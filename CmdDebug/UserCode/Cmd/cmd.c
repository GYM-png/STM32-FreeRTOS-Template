/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-17 22:33:49
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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
#include <stdarg.h> // ����ɱ����ͷ�ļ�

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t debug_print_mutex = NULL;
extern uart_dma_t debug_uart;      //���Դ��ھ��



/**
 * @brief ��ӡ��Ϣ 
 * @param __format 
 * @param  
 */
void myprintf(const char*__format, ...)
{
	uart_dma_t * print_uart = &debug_uart;
    va_list args; // ����һ��va_list���͵ı����������洢�ɱ����
    va_start(args, __format); // ʹ��va_start���ʼ��args��ʹָ֮���һ����ѡ����
    xSemaphoreTake(debug_print_mutex, 1);
    vsprintf((char*)print_uart->tx_data, __format, args); // ʹ��vsprintf������sprintf����Ϊ�����Դ���ɱ�����б�
    va_end(args); // ʹ��va_end������args
    xSemaphoreGive(debug_print_mutex);
	HAL_UART_Transmit(print_uart->uart_t, (uint8_t*)print_uart->tx_data, strlen((char*)print_uart->tx_data), 10);

}


/**
 * @brief �����־ ��ʱ���
 * @param __format 
 * @param  
 */
void mylog(const char*__format, ...)
{
	uart_dma_t * print_uart = &debug_uart;
    xSemaphoreTake(debug_print_mutex, 1);
    sprintf((char*)print_uart->tx_data, "%02d:%02d:%02d:%03d\t", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds, rtc_ms);
    va_list args; // ����һ��va_list���͵ı����������洢�ɱ����
    va_start(args, __format); // ʹ��va_start���ʼ��args��ʹָ֮���һ����ѡ����
    vsprintf((char*)print_uart->tx_data+13, __format, args); // ʹ��vsprintf������sprintf����Ϊ�����Դ���ɱ�����б�
    va_end(args); // ʹ��va_end������args
    xSemaphoreGive(debug_print_mutex);
	HAL_UART_Transmit(print_uart->uart_t, (uint8_t*)print_uart->tx_data, strlen((char*)print_uart->tx_data), 10);
}



/**
 * @brief �����б�����
 */
#define HELP     0
#define SYSTEM_R 1


static cmd_t  cmd_list[CMD_COUNT_MAX] = {0};
uint16_t cmd_count = 0;//��������
/**
 * @brief ��������ִ�ж�Ӧ�ĺ���
 * @param cmd ����
 */
void find_cmd(char * cmd)
{
	if(cmd[0] == NULL)
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
	myprintf("�����������help�鿴����\r\n");
}


/**
 * @brief ��ӡ����ָ�� help ָ���Ӧ�Ļص�
 * @param  
 */
static void print_cmd_list(void)
{
    myprintf("====�����б�====\r\n");
    for (uint16_t i = 1; i < cmd_count; i++)
    {
        myprintf("%s\r\n", cmd_list[i].cmd);
    }
}

/**
 * @brief ϵͳ����
 * @param  
 */
static void system_rest(void)
{
    mylog("ϵͳ���ڸ�λ\r\n");
    vTaskDelay(1);
    HAL_NVIC_SystemReset();
}


/**
 * @brief �����ʼ������
 *        ���г�ʼ��һЩϵͳ��������
 */
uint8_t cmd_init(void)
{
    debug_print_mutex = xSemaphoreCreateMutex();

    /* �����Ӧ������ʵ�� */
	strcpy(cmd_list[HELP].cmd, "help");
    cmd_list[HELP].callback = print_cmd_list;
    cmd_count++;
    
    cmd_list[SYSTEM_R].cmd = "system -r";
    cmd_list[SYSTEM_R].callback = system_rest;
    cmd_count++;

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
