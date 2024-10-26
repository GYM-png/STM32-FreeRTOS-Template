/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-12 22:14:23
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-26 22:29:16
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Cmd\cmd.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cmd.h"
#include <stdio.h>
#include <stdarg.h> // 引入可变参数头文件
#include <string.h>
#include "global.h"
#include "system.h"
#include "FreeRTOS_CLI.h"
SemaphoreHandle_t write_mutex = NULL;
static char write_buffer[200] = {0};
static void myprintf(const char*__format, ...);
static uint8_t find_parameters(const char *pcParameter, const uint8_t n, char **parameterArr);


/*************************************************以下是用户代码******************************************************************** */

/**
 * @defgroup 命令函数声明 
 * {
 */
static BaseType_t system_reset(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t get_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

/**
 * }
 */

/**
 * @defgroup FreeRTOS CLI命令组件 
 * {
 */
static const CLI_Command_Definition_t user_command[] = {
    {
    .pcCommand = "system",\
    .pcHelpString = "system:\t\t-t:get task; -r:reset mcu\r\n", \
    .pxCommandInterpreter = system_reset, \
    .cExpectedNumberOfParameters = 1}, 
    {
    .pcCommand = "get",\
    .pcHelpString = "get:\t\t-t:get time\r\n", \
    .pxCommandInterpreter = get_command, \
    .cExpectedNumberOfParameters = 1} 
};
/**
 * }
 */


static BaseType_t system_reset(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
static UBaseType_t uxParameterNumber = 1;   //总参数量
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

    char **parameterArray = (char **)pvPortMalloc(sizeof(char *) * uxParameterNumber);
    if (parameterArray == NULL)
    {
        log_e("内存分配失败 \r\n");
        vPortFree(parameterArray);
        return pdFALSE;
    }
    if (find_parameters(pcCommandString, uxParameterNumber, parameterArray) == 0)
    {
        log_i("没有找到参数 \r\n");
        return pdFALSE;
    }
    if (strstr(parameterArray[0], "t"))
    {
        const char *const pcHeader = "    状态   优先级   堆栈     #\r\n************************************************\r\n";
        BaseType_t xSpacePadding;
        ( void ) pcCommandString;
        ( void ) xWriteBufferLen;
        configASSERT( pcWriteBuffer );
        strcpy( pcWriteBuffer, "任务" );
        pcWriteBuffer += strlen( pcWriteBuffer );
        configASSERT( configMAX_TASK_NAME_LEN > 3 );
        for( xSpacePadding = strlen( "任务" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
        {
            *pcWriteBuffer = ' ';
            pcWriteBuffer++;
            *pcWriteBuffer = 0x00;
        }
        strcpy( pcWriteBuffer, pcHeader );
        vTaskList( pcWriteBuffer + strlen( pcHeader ) );
        return pdFALSE;
    }
    else if(strstr(parameterArray[0], "r"))
    {
        log_i("系统即将重启\r\n");
        vTaskDelay(1000); // 1s后重启
        HAL_NVIC_SystemReset();
    }
	return pdFALSE;
}




static BaseType_t get_command( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
static UBaseType_t uxParameterNumber = 1;   //总参数量
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    pcWriteBuffer[0] = '\0';        //清除上次输出残留
    char **parameterArray = (char **)pvPortMalloc(sizeof(char *) * uxParameterNumber);
    if (parameterArray == NULL)
    {
        log_e("内存分配失败 \r\n");
        vPortFree(parameterArray);
        return pdFALSE;
    }
    if (find_parameters(pcCommandString, uxParameterNumber, parameterArray) == 0)
    {
        log_i("没有找到参数 \r\n");
        return pdFALSE;
    }
    log_i("%s", parameterArray[0]);
    if (strstr(parameterArray[0], "t"))
    {
        log_v("当前系统时间:\r\n");
        log_v("%04d年%02d月%02d日 %02d:%02d:%02d", rtc_date.Year, rtc_date.Month, rtc_date.Date, rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    }
	return pdFALSE;
}



/******************************************以下是用户自定义命令回调函数模板******************************************/
/**
 *  0参数版本
 *  #if 1：使用FreeRTOS CLI组件输出
 *  #if 0：不使用组件输出，自定义输出或者不需要输出
 */
// static BaseType_t null_parameter_command_callback( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
// {
//     ( void ) pcCommandString;
//     ( void ) xWriteBufferLen;
//     configASSERT( pcWriteBuffer );
// #if 1
// const char *const pcHeader = " hello world\r\n";//第一行输出，不需要则置空
// BaseType_t xSpacePadding;
//     if(xSemaphoreTake(write_mutex, 100) == pdPASS)//保护缓冲区
//     {
//         memset(write_buffer, 0, sizeof(write_buffer));
//         if(pcHeader != NULL)
//             strcpy(write_buffer, pcHeader);
//         /****************USERCODE BEGIN*****************/
//         myprintf("这是通过FreeRTOS CLI组件输出的信息\r\n");
        
//         /****************USERCODE END*****************/
//         strcpy( pcWriteBuffer, write_buffer );
//         xSemaphoreGive(write_mutex);
//     }
// 	return pdFALSE;
// #else
//     /****************USERCODE BEGIN*****************/
//     log_i("这是通过Easylogger输出的信息\r\n");

//     /****************USERCODE END*****************/
// 	/* There is no more data to return after this single string, so return
// 	pdFALSE. */
// 	return pdFALSE;
// #endif
// }

/**
 *  1参数版本
 */
// static BaseType_t one_parameter_command_callback( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
// {
// const char *pcParameter;
// BaseType_t xParameterStringLength, xReturn;
// static UBaseType_t uxParameterNumber = 1;
//     ( void ) pcCommandString;
//     ( void ) xWriteBufferLen;
//     configASSERT( pcWriteBuffer );
// 	pcParameter = FreeRTOS_CLIGetParameter
// 					(
// 						pcCommandString,		/* The command string itself. */
// 						uxParameterNumber,		/* Return the next parameter. */
// 						&xParameterStringLength	/* Store the parameter string length. */
// 					);

// 	/* Sanity check something was returned. */
// 	configASSERT( pcParameter );

// 	/* Return the parameter string. */
// 	memset( pcWriteBuffer, 0x00, xWriteBufferLen );
// 	sprintf( pcWriteBuffer, "%d: ", ( int ) uxParameterNumber );
// 	strncat( pcWriteBuffer, pcParameter, ( size_t ) xParameterStringLength );
// 	strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );
//  if( uxParameterNumber == 1U )
// 	{
//         if(strcmp(pcParameter,"t")==0)//参数是 t
//         {
//              please write your codes here:
//         }
//         else if(strcmp(pcParameter,"r")==0)//参数是 r
//         {
//              please write your codes here:
//         }
//         /*
//         else if (strcmp(pcParameter,"v")==0)//if parameter is v
//         {
//             please write your codes here:
//         }
//         */
//     }
// 	xReturn = pdFALSE;
// 	return xReturn;
// }


/*************************************************以下是内部调用函数******************************************************************** */

/**
 * @brief 用户命令初始化
 *        在任务调度开始之前使用
 * @param  
 */
void cmd_init(void)
{
    write_mutex = xSemaphoreCreateMutex();
    for (uint8_t i = 0; i < sizeof(user_command)/sizeof(user_command[0]); i++)
    {
        FreeRTOS_CLIRegisterCommand( &user_command[i] );	
    }
}

/**
 * @brief 仅作为命令回调函数内使用的打印函数
 * @param __format 
 * @param  
 */
static void myprintf(const char*__format, ...)
{
    va_list args; // 创建一个va_list类型的变量，用来存储可变参数
    va_start(args, __format); // 使用va_start宏初始化args，使之指向第一个可选参数
    vsnprintf(write_buffer + strlen(write_buffer), sizeof(write_buffer), __format, args); // 使用vsnprintf代替sprintf，因为它可以处理可变参数列表
    va_end(args); // 使args不再指向可变参数列表中的任何参数
    xSemaphoreGive(write_mutex);
}

/**
 * @brief 提取参数放入字符串数组
 * @param pcParameter 整个参数字符串
 * @param n 需要寻找的参数个数
 * @param parameterArr 字符串数组，存放提取出的参数
 * @return 找到的参数个数
 */
static uint8_t find_parameters(const char *pcParameter, const uint8_t n, char **parameterArr)
{
    uint8_t len = strlen(pcParameter);
    uint8_t found_num = 0;
    uint8_t *index = (uint8_t*) pvPortMalloc(sizeof(uint8_t) * (n + 1));  // 增加一个元素用于最后一个参数
    if (index == NULL)
    {
        return 0;
    }

    // 找到参数位置
    for (uint8_t i = 0; i < len - 1; i++)
    {
        if (pcParameter[i] == ' ' && pcParameter[i + 1] == '-')
        {
            index[found_num++] = i + 2;
            if (found_num == n)
                break;
        }
    }

    // 提取处理参数
    for (uint8_t i = 0; i < found_num; i++)
    {
        uint8_t start = index[i];
        uint8_t end = (i + 1 < found_num) ? index[i + 1] - 2: len;  // 确保最后一个参数处理正确
        uint8_t param_len = end - start;

        parameterArr[i] = (char *)pvPortMalloc(param_len + 1);  // 分配内存
        if (parameterArr[i] == NULL)//错误处理
        {
            for (uint8_t j = 0; j < i; j++)
            {
                vPortFree(parameterArr[j]);
            }
            vPortFree(index);
            return 0;
        }
        memcpy(parameterArr[i], pcParameter + start, param_len);
        parameterArr[i][param_len] = '\0';  // 添加字符串结束符 避免乱码
    }

    vPortFree(index);
    return found_num;
}
