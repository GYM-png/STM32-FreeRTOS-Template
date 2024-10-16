#ifndef __MYUSART_H
#define __MYUSART_H

#include "main.h"
#include "usart.h"
#include "dma.h"
#include "global.h"


#define UART_RX_LEN_MAX 30//最大接收长度
#define UART_TX_LEN_MAX 50//最大发送长度


typedef struct
{
    UART_HandleTypeDef * uart_t;    //串口号
    DMA_HandleTypeDef* dma_t;       //dma号
    uint8_t  rx_buffer[UART_RX_LEN_MAX];            //接收缓冲
    uint8_t tx_data[UART_TX_LEN_MAX];              //发送数组
    uint8_t rx_flag;                //接收完成标志
}uart_dma_t;


uint8_t uart_dma_init(uart_dma_t* uart_dma, UART_HandleTypeDef* huart, DMA_HandleTypeDef* dma);
void uart_idle_callback(uart_dma_t *uart_dma);


#endif
