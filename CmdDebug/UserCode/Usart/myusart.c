/*
 * @Author: GYM-png 480609450@qq.com
 * @Date: 2024-10-13 11:04:59
 * @LastEditors: GYM-png 480609450@qq.com
 * @LastEditTime: 2024-10-14 20:51:54
 * @FilePath: \EIDEd:\warehouse\CmdDebug\CmdDebug\UserCode\Usart\myusart.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "myusart.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
extern TaskHandle_t USRAT_RX_TASK_Handler;//任务句柄


/**
 * @brief 初始化对应串口号的MDA并开始接收
 * @param uart_dma 串口DMA结构体指针
 * @param huart 串口号
 * @param dma dma号
 * @return 
 */
uint8_t uart_dma_init(uart_dma_t* uart_dma, UART_HandleTypeDef* huart, DMA_HandleTypeDef* dma)
{
    uart_dma->uart_t = huart;
    uart_dma->dma_t = dma;
    uart_dma->rx_flag = 0;

    /*为缓冲区、数据区、 发送区申请内存空间*/
//    uart_dma->rx_buffer = (uint8_t *)pvPortMalloc(sizeof(uint8_t) * UART_RX_LEN_MAX);
//    if (uart_dma->rx_buffer)
//    {
//        return ERROR;
//    }
//    memset(uart_dma->rx_buffer, 0, UART_RX_LEN_MAX);
//    uart_dma->tx_data = (uint8_t*)pvPortMalloc(sizeof(uint8_t) * UART_TX_LEN_MAX);
//    if(uart_dma->tx_data == NULL)
//    {
//        return ERROR;
//    }
//	memset(uart_dma->tx_data, 0, UART_RX_LEN_MAX);

    // uart_dma->rx_buffer = (uint8_t*)malloc(sizeof(uint8_t) * UART_RX_LEN_MAX);
    // if (uart_dma->rx_buffer == NULL)
    // {
    //     return ERROR;
    // }
	// memset(uart_dma->rx_buffer, 0, UART_RX_LEN_MAX);
    // // uart_dma->rx_data = (uint8_t*)malloc(sizeof(uint8_t) * UART_RX_LEN_MAX);
    // // if (uart_dma->rx_data == NULL)
    // // {
    // //     return ERROR;
    // // }
    // uart_dma->tx_data = (uint8_t*)malloc(sizeof(uint8_t) * UART_TX_LEN_MAX);
    // if(uart_dma->tx_data == NULL)
    // {
    //     return ERROR;
    // }
	// memset(uart_dma->tx_data, 0, UART_RX_LEN_MAX);

    /*开启空闲中断并开启DMA接收*/
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, uart_dma->rx_buffer, UART_RX_LEN_MAX);
    return OK;
}



/**
 * @brief 串口空闲中断
 *        直接在串口IRQ中调用
 * @param uart_dma 串口DMA结构体指针
 */
void uart_idle_callback(uart_dma_t *uart_dma)
{
    if(__HAL_UART_GET_FLAG(uart_dma->uart_t, UART_FLAG_IDLE) != RESET)
    {
        /*DMA接收处理*/
        __HAL_UART_CLEAR_IDLEFLAG(uart_dma->uart_t);
        HAL_UART_DMAStop(uart_dma->uart_t);
        uint8_t uart_rx_len = 0;
        uart_rx_len = UART_RX_LEN_MAX - __HAL_DMA_GET_COUNTER(uart_dma->dma_t);
        memset(uart_dma->rx_buffer + uart_rx_len, 0, UART_RX_LEN_MAX - uart_rx_len);
        HAL_UART_Receive_DMA(uart_dma->uart_t, uart_dma->rx_buffer, UART_RX_LEN_MAX);

        /*任务通知给usart_task*/
        uart_dma->rx_flag = 1;
        // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // vTaskNotifyGiveFromISR(USRAT_RX_TASK_Handler, &xHigherPriorityTaskWoken);
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

