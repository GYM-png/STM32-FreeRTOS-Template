/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-14 20:52:01
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-14 23:04:10
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Usart\usart_task.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "usart_task.h"
#include "myusart.h"
#include "dma.h"
#include "cmd.h"

#include "system.h"

#include <string.h>

/**
 * @def 串口接收任务参数
 */
#define USRAT_RX_TASK_PRIO 4  //任务优先级
#define USRAT_RX_TASK_SIZE 100 //任务堆栈大小
TaskHandle_t USRAT_RX_TASK_Handler;//任务句柄
void usart_rx_task(void *pvparameters);

/**
 * @def 串口发送任务参数
 */
#define USRAT_TX_TASK_PRIO 5  //任务优先级
#define USRAT_TX_TASK_SIZE 100 //任务堆栈大小
TaskHandle_t USRAT_TX_TASK_Handler;//任务句句柄
void usart_tx_task(void *pvparameters);


/**
 * @brief 串口相关任务初始化
 * @param  
 */
void usart_task_init(void)
{
    myTaskCreate((TaskFunction_t )usart_rx_task,    //任务函数
                (const char *   )"usart_rx_task",   //任务名称
                (uint16_t       )USRAT_RX_TASK_SIZE, //任务堆栈大小
                (void *         )NULL,                //任务参数
                (UBaseType_t    )USRAT_RX_TASK_PRIO, //任务优先级
                (TaskHandle_t * )&USRAT_RX_TASK_Handler); //任务句句柄

    myTaskCreate((TaskFunction_t )usart_tx_task,    //任务函数
                (const char *   )"usart_tx_task",   //任务名称
                (uint16_t       )USRAT_TX_TASK_SIZE, //任务堆栈大小
                (void *         )NULL,                //任务参数
                (UBaseType_t    )USRAT_TX_TASK_PRIO, //任务优先级
                (TaskHandle_t * )&USRAT_TX_TASK_Handler); //任务句句柄

}

extern DMA_HandleTypeDef hdma_usart1_rx;
uart_dma_t debug_uart;      //调试串口句柄
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
