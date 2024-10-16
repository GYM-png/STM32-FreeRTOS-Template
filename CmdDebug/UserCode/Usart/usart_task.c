/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-14 20:52:01
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-16 20:19:11
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Usart\usart_task.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "usart_task.h"
#include "myusart.h"
#include "dma.h"
#include "cmd.h"

#include "system.h"

#include <string.h>

/**
 * @def ���ڽ����������
 */
#define USRAT_RX_TASK_PRIO 4  //�������ȼ�
#define USRAT_RX_TASK_SIZE 100 //�����ջ��С
TaskHandle_t USRAT_RX_TASK_Handler;//������
void usart_rx_task(void *pvparameters);

/**
 * @def ���ڷ����������
 */
#define USRAT_TX_TASK_PRIO 5  //�������ȼ�
#define USRAT_TX_TASK_SIZE 100 //�����ջ��С
TaskHandle_t USRAT_TX_TASK_Handler;//�������
void usart_tx_task(void *pvparameters);


/**
 * @brief ������������ʼ��
 * @param  
 */
void usart_task_init(void)
{
//    myTaskCreate((TaskFunction_t )usart_rx_task,    //������
//                (const char *   )"usart_rx_task",   //��������
//                (uint16_t       )USRAT_RX_TASK_SIZE, //�����ջ��С
//                (void *         )NULL,                //�������
//                (UBaseType_t    )USRAT_RX_TASK_PRIO, //�������ȼ�
//                (TaskHandle_t * )&USRAT_RX_TASK_Handler); //�������

//    myTaskCreate((TaskFunction_t )usart_tx_task,    //������
//                (const char *   )"usart_tx_task",   //��������
//                (uint16_t       )USRAT_TX_TASK_SIZE, //�����ջ��С
//                (void *         )NULL,                //�������
//                (UBaseType_t    )USRAT_TX_TASK_PRIO, //�������ȼ�
//                (TaskHandle_t * )&USRAT_TX_TASK_Handler); //�������

}



static void usart_rx_task(void *pvparameters)
{
    mylog("uart_rx_task work\r\n");
    for(;;)
    {

		vTaskDelay(20);
		
    }
}



static void usart_tx_task(void *pvparameters)
{
    mylog("uart_tx_task work\r\n");
    for(;;)
    {

		vTaskDelay(100);
    }
}
