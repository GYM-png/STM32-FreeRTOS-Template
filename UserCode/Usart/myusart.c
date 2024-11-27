
#include "myusart.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
/**
 * @brief 初始化对应串口号的MDA并开始接收
 * @param uart_dma 串口DMA结构体指针
 * @param huart 串口号
 * @param dma dma号
 * @return 
 */
uint8_t uart_dma_init(uart_t* uart, UART_HandleTypeDef* huart, DMA_HandleTypeDef* dma)
{
    uart->huart = huart;
    uart->dma_t = dma;
    uart->rx_flag = 0;
    uart->rx_len = 0;
    uart->mutex = NULL;
    uart->mutex = xSemaphoreCreateMutex();

    uart->rx_buffer = (uint8_t *)pvPortMalloc(UART_RX_LEN_MAX);
    uart->tx_data = (uint8_t *)pvPortMalloc(UART_TX_LEN_MAX);

    /*开启空闲中断并开启DMA接收*/
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, uart->rx_buffer, UART_RX_LEN_MAX);
    return OK;
}



/**
 * @brief 串口空闲中断
 *        直接在串口IRQ中调用
 * @param uart_dma 串口DMA结构体指针
 */
void uart_idle_callback(uart_t *uart)
{
    if(__HAL_UART_GET_FLAG(uart->huart, UART_FLAG_IDLE) != RESET)
    {
        /*DMA接收处理*/
        __HAL_UART_CLEAR_IDLEFLAG(uart->huart);
        HAL_UART_DMAStop(uart->huart);
        uart->rx_len = UART_RX_LEN_MAX - __HAL_DMA_GET_COUNTER(uart->dma_t);
        memset(uart->rx_buffer + uart->rx_len, 0, UART_RX_LEN_MAX - uart->rx_len);
        HAL_UART_Receive_DMA(uart->huart, uart->rx_buffer, UART_RX_LEN_MAX);

        /*标志位置1*/
        uart->rx_flag = 1;
    }
}

/**
 * @brief 串口发送 应用层调用
 * @param uart 串口句柄
 * @param data 发送数据
 * @param len 数据长度
 * @return 1-失败 0成功
 */
uint8_t uart_transmit(uart_t *uart, uint8_t* data, uint16_t len)
{
    if(uart->mutex != NULL)
    {
        xSemaphoreTake(uart->mutex, 200);
        HAL_UART_Transmit(uart->huart, data, len, 100);
        xSemaphoreGive(uart->mutex);
    }
    else
    {
        HAL_UART_Transmit(uart->huart, data, len, 100);
    }
    return 0;
}


static uint32_t print_length = 0;
/**
 * @brief 串口打印数据(直接输出)
 * @param uart 串口句柄
 * @param format 
 * @return 
 */
int uart_print_fast(uart_t *uart, const char* format, ...)
{
    memset(uart->tx_data, 0, print_length);
    va_list args;
    va_start(args, format);
    print_length = vsnprintf(uart->tx_data, UART_TX_LEN_MAX, format, args);
    va_end(args);
    return HAL_UART_Transmit(uart->huart, uart->tx_data, print_length, 10);
}


/**
 * @brief 串口打印数据追加换行(直接输出)
 * @param uart 串口句柄
 * @param format 
 * @return 
 */
int uart_println_fast(uart_t *uart, const char* format, ...)
{
    memset(uart->tx_data, 0, print_length);
    va_list args;
    va_start(args, format);
    print_length = vsnprintf(uart->tx_data, UART_TX_LEN_MAX, format, args);
    print_length += sprintf(uart->tx_data + print_length, "\r\n");
    va_end(args);
    return HAL_UART_Transmit(uart->huart, uart->tx_data, print_length, 10);
}

static char debug_buffer[100] = {0};
static uint32_t debug_length = 0;
int uart_debug_print(const char* format, ...)
{
    memset(debug_buffer, 0, debug_length);
    va_list args;
    va_start(args, format);
    debug_length = vsnprintf(debug_buffer, 100, format, args);
    debug_length += sprintf(debug_buffer + debug_length, "\r\n");
    va_end(args);
    return HAL_UART_Transmit(&DEBUG_UART, debug_buffer, debug_length, 10);
}