/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-17 22:21:06
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-17 22:35:25
 * @FilePath: \MDK-ARMd:\warehouse\CmdDebug\CmdDebug\UserCode\System\config.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CONFIG_H
#define __CONFIG_H


#define OS_TASK_NUM_MAX 15//�����������

#define CMD_COUNT_MAX 30 //�����������

/*���ڵ�����ض���*/
#define DEBUG_UART (huart1)
#define DEBUG_UART_DMA_RX (hdma_usart1_rx)

/*���ڻ�����*/
#define UART_RX_LEN_MAX 30//�����ճ���
#define UART_TX_LEN_MAX 50//����ͳ���

#endif
