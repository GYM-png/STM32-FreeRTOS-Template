/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-14 20:52:01
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-14 23:04:10
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
    myTaskCreate((TaskFunction_t )usart_rx_task,    //������
                (const char *   )"usart_rx_task",   //��������
                (uint16_t       )USRAT_RX_TASK_SIZE, //�����ջ��С
                (void *         )NULL,                //�������
                (UBaseType_t    )USRAT_RX_TASK_PRIO, //�������ȼ�
                (TaskHandle_t * )&USRAT_RX_TASK_Handler); //�������

    myTaskCreate((TaskFunction_t )usart_tx_task,    //������
                (const char *   )"usart_tx_task",   //��������
                (uint16_t       )USRAT_TX_TASK_SIZE, //�����ջ��С
                (void *         )NULL,                //�������
                (UBaseType_t    )USRAT_TX_TASK_PRIO, //�������ȼ�
                (TaskHandle_t * )&USRAT_TX_TASK_Handler); //�������

}

extern DMA_HandleTypeDef hdma_usart1_rx;
uart_dma_t debug_uart;      //���Դ��ھ��
extern QueueHandle_t cmd_receive_queue;

static void usart_rx_task(void *pvparameters)
{
    uart_dma_init(&debug_uart, &huart1, &hdma_usart1_rx);
    for(;;)
    {
        // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (debug_uart.rx_flag == 1)
        {
            xQueueSendToBack(cmd_receive_queue, (void *)debug_uart.rx_buffer, 0);
            debug_uart.rx_flag = 0;
        }
        else
        {
            vTaskDelay(20);
        }
        
    }
}


extern QueueHandle_t debug_print_queue;

static void usart_tx_task(void *pvparameters)
{
    for(;;)
    {
        if(xQueueReceive(debug_print_queue, debug_uart.tx_data, 100) == pdTRUE)
        {
            HAL_UART_Transmit(debug_uart.uart_t, debug_uart.tx_data, strlen((char*)debug_uart.tx_data), 100);
        }

    }
}
