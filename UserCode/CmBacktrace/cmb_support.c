/**
 * ���ļ���Ϊ����CmBacktrace�Ա����̵�����
 */
#include "cmb_support.h"
#include "elog_file.h"
#include "sys.h"
#include "delay.h"
#include "malloc.h"
#include <stdio.h>
#include <stdarg.h>

uint8_t fault_flag = 0;//����Ƿ�ڴ����쳣
static char * log_buffer = NULL;
static uint32_t log_length = 0;

#define LOG_TAG "cmtrace"
#define LOG_BUFFER_SIZE 1024
#define OUT_BUFFER_SIZE 256

/**
 * @brief cmbʹ�õĴ�ӡ������ұ�����־��log_buffer
 * @param format 
 * @param  
 */
void cmb_print(const char *format, ...)
{
    static char * output_buffer = NULL;
    if (fault_flag == 0)//��һ�ε��� �����ڴ�
    {
       log_buffer = mymalloc(SRAMIN, LOG_BUFFER_SIZE);
       output_buffer = mymalloc(SRAMIN, OUT_BUFFER_SIZE);
       fault_flag = 1; 
    }
    //д��output_buffer
    va_list args;
    va_start(args, format);
    vsprintf(output_buffer, format, args);
    va_end(args);

    //����ͱ���log
    printf("%s\r\n", output_buffer);
    log_length += sprintf(log_buffer + log_length, "%s\r\n", output_buffer);
    mymemset(output_buffer, 0, OUT_BUFFER_SIZE);
}

/**
 * @brief cm_backtrace_fault ����������
 * @param  
 */
void cmb_fault_end(void)
{
    elog_file_write(log_buffer, log_length);
	printf("Ӳ�����ϣ�������������������������");
	delay_xms(5000);
	Sys_Soft_Reset(LOG_TAG);
}
