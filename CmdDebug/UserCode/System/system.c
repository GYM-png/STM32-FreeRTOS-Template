/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-15 20:15:30
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-25 22:16:09
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\System\system.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "system.h"
#include "FreeRTOS_CLI.h"

extern void vRegisterSampleCLICommands( void );
extern void vUARTCommandConsoleStart( uint16_t usStackSize, UBaseType_t uxPriority );

/**
 * @brief 调试相关初始化 cmd和log
 * @param  
 */
void debug_init(void)
{
    vRegisterSampleCLICommands();
    cmd_init();
    vUARTCommandConsoleStart(200, 4);
    log_init();
}


