#ifndef __UART_H__
#define __UART_H__
#include "stm8s.h"



void InitUart2(uint32_t baudrate);

void uartSendDataBuf(uint8_t *tempbuf,uint8_t buflen);

#endif