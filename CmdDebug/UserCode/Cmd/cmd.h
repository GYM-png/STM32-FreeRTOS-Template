/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:37
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-13 15:30:41
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CDM_H
#define __CDM_H

#include "main.h"


#define CMD_COUNT_MAX 30 //�����������


typedef struct
{
    uint16_t index;         //
    char * cmd;             //�����ַ���
    void (*callback)(void); //�ص�����
}cmd_t;


void myprintf(const char *format, ...); 
void mylog(const char*__format, ...);

uint8_t cmd_init(void);
void find_cmd(char * cmd);
void cmd_install(char* cmd, void(*callback)(void));



#endif
