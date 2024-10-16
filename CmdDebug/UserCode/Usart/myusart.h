#ifndef __MYUSART_H
#define __MYUSART_H

#include "main.h"
#include "usart.h"
#include "dma.h"
#include "global.h"


#define UART_RX_LEN_MAX 30//�����ճ���
#define UART_TX_LEN_MAX 50//����ͳ���


typedef struct
{
    UART_HandleTypeDef * uart_t;    //���ں�
    DMA_HandleTypeDef* dma_t;       //dma��
    uint8_t  rx_buffer[UART_RX_LEN_MAX];            //���ջ���
    uint8_t tx_data[UART_TX_LEN_MAX];              //��������
    uint8_t rx_flag;                //������ɱ�־
}uart_dma_t;


uint8_t uart_dma_init(uart_dma_t* uart_dma, UART_HandleTypeDef* huart, DMA_HandleTypeDef* dma);
void uart_idle_callback(uart_dma_t *uart_dma);


#endif
