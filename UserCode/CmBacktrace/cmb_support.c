/**
 * 此文件是为了做CmBacktrace对本工程的适配
 */
#include "cmb_support.h"
#include "system.h"
#include "myusart.h"
#include <stdio.h>
#include <stdarg.h>

uint8_t fault_flag = 0;//标记是否第触发异常
static char * log_buffer = NULL;
static uint32_t log_length = 0;

#define LOG_TAG "cmtrace"
#define LOG_BUFFER_SIZE 1024
#define OUT_BUFFER_SIZE 256

/**
 * @brief cmb使用的打印输出并且保存日志到log_buffer
 * @param format 
 * @param  
 */
void cmb_print(const char *format, ...)
{
//    static char * output_buffer = NULL;
//    if (fault_flag == 0)//第一次调用 申请内存
//    {
//       log_buffer = mymalloc(SRAMIN, LOG_BUFFER_SIZE);
//       output_buffer = mymalloc(SRAMIN, OUT_BUFFER_SIZE);
//       fault_flag = 1; 
//    }
//    //写入output_buffer
//    va_list args;
//    va_start(args, format);
//    vsprintf(output_buffer, format, args);
//    va_end(args);

//    //输出和保存log
//    printf("%s\r\n", output_buffer);
//    log_length += sprintf(log_buffer + log_length, "%s\r\n", output_buffer);
//    mymemset(output_buffer, 0, OUT_BUFFER_SIZE);
}

/**
 * @brief cm_backtrace_fault 函数最后调用
 * @param  
 */
void cmb_fault_end(void)
{
//  elog_file_write(log_buffer, log_length);
//	printf("硬件故障！！！！！！！重启！！！！");
//	delay_xms(5000);
//	Sys_Soft_Reset(LOG_TAG);
}
