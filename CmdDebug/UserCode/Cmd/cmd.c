/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-13 00:57:34
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd.h"
#include "string.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include "global.h"

#include <stdarg.h>  // ���� va_list��va_start��va_arg��va_end �Ⱥ궨��
#include <string.h>

// ��������������ת��Ϊ�ַ�����֧��ָ����С��Ⱥ����
void int_to_str(int num, char *str, int width, char pad) {
    int i = 0, is_negative = 0;

    if (num < 0) {  // ������
        is_negative = 1;
        num = -num;
    }

    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    while (i < width) {  // �����
        str[i++] = pad;
    }

    if (is_negative) {  // ��Ӹ���
        str[i++] = '-';
    }

    str[i] = '\0';

    // ��ת�ַ���
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
}

// ����������������ת��Ϊ�ַ�����֧��ָ��С����λ��
void float_to_str(double num, char *str, int precision) {
    int int_part = (int)num;  // ��������
    double frac_part = num - int_part;  // С������

    char int_str[20];
    int_to_str(int_part, int_str, 0, ' ');  // ��������ת��Ϊ�ַ���
    strcpy(str, int_str);

    if (precision > 0) {
        char frac_str[20];
        strcat(str, ".");  // ���С����

        // ����С������
        for (int i = 0; i < precision; i++) {
            frac_part *= 10;
        }
        int_to_str((int)frac_part, frac_str, precision, '0');  // ���С��λ
        strcat(str, frac_str);
    }
}

void my_printf(const char *format, ...) {
    char buffer[256];
    char *buf_ptr = buffer;
    const char *p;
    va_list args;

    va_start(args, format);

    for (p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            int width = 0;
            char pad = ' ';

            // �����Ⱥ����
            if (*p == '0') {
                pad = '0';
                p++;
            }
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p++ - '0');
            }

            int precision = -1;
            if (*p == '.') {  // ����С���㾫��
                p++;
                precision = 0;
                while (*p >= '0' && *p <= '9') {
                    precision = precision * 10 + (*p++ - '0');
                }
            }

            switch (*p) {
                case 'd': {  // ��������
                    int i = va_arg(args, int);
                    char num_str[20];
                    int_to_str(i, num_str, width, pad);
                    strcpy(buf_ptr, num_str);
                    buf_ptr += strlen(num_str);
                    break;
                }
                case 'f': {  // ��������
                    double f = va_arg(args, double);
                    char float_str[40];
                    float_to_str(f, float_str, precision >= 0 ? precision : 6);  // Ĭ��6λС��
                    strcpy(buf_ptr, float_str);
                    buf_ptr += strlen(float_str);
                    break;
                }
                case 's': {  // �����ַ���
                    char *s = va_arg(args, char *);
                    strcpy(buf_ptr, s);
                    buf_ptr += strlen(s);
                    break;
                }
                case 'c': {  // �����ַ�
                    char c = (char) va_arg(args, int);
                    *buf_ptr++ = c;
                    break;
                }
                default:
                    *buf_ptr++ = '%';
                    *buf_ptr++ = *p;
                    break;
            }
        } else {  // ��ͨ�ַ�
            *buf_ptr++ = *p;
        }
    }

    *buf_ptr = '\0';
    va_end(args);

    // HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    CDC_Transmit_HS((uint8_t*)buffer, strlen(buffer));
}

/**
 * @brief �����б�����
 */
#define SYSTEM_R 0
#define SYSTEM_V 1
#define SYSTEM_T 2

/**
 * @brief �����б�
 */
static cmd_t cmdList[] = {
    {SYSTEM_R, "system -r", NULL},
    {SYSTEM_V, "system -v", NULL},
    {SYSTEM_T, "system -t", NULL},
};

/**
 * @brief ��������ִ�ж�Ӧ�ĺ���
 * @param cmd ����
 */
void findCommand(char * cmd)
{
    for (uint16_t i = 0; i < sizeof(cmdList)/sizeof(cmdList[0]); i++)
    {
        if (strncmp(cmd, cmdList[i].cmd, strlen(cmdList[i].cmd)) == 0)
        {
            cmdList[i].func();
            return;
        }
        
    }
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
    my_printf("\r\n");
}

static void systemRest(void)
{
    debugPrintStart();
    my_printf("[%2d:%2d:%2d]ϵͳ���ڸ�λ\r\n", rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
    HAL_NVIC_SystemReset();
    // debugPrintEnd();
}

static void systemVersion(void)
{
    debugPrintStart();
    my_printf("[%2d:%2d:%2d]ϵͳ�汾1.0\r\n��ʼʱ��2024��10��13��\r\n", rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
    // debugPrintEnd();
}

void cmdInit()
{
    // DebugPrintFunc = print;

    /* �����Ӧ������ʵ�� */
    cmdList[SYSTEM_R].func = systemRest;
    cmdList[SYSTEM_V].func = systemVersion;
}