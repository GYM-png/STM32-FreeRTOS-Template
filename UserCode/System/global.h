/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-13 00:23:37
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-15 23:04:54
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\global\global.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "main.h"
#include "rtc.h"
#include "system.h"





#define ERROR   1
#define OK      0




void start_task_init(void);



extern uint32_t systern_run_time;
extern uint16_t rtc_ms;//ϵͳʱ��ms rtcֻ���ṩ��ȷ��s

extern RTC_TimeTypeDef rtc_time;
extern RTC_DateTypeDef rtc_date;


#endif
