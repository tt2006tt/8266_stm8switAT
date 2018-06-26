
/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 **************
 * 文件名  ：uart1.c
 * 描述    ：UART1设置文件   
 * 实验平台：风驰STM8开发板
 * 库版本  ：V2.1.0
 * 作者    ：风驰  QQ：779814207/2366260464
 * 博客    ：
 * 淘宝    ：http://shop71177993.taobao.com/
 *修改时间 ：2012-10-28

  风驰STM8开发板硬件连接
    |------------------------|
    |  TXD - PA4(UART1->RX)  |
    |  RXD - PA5(UART1->TX)  |
    |------------------------|

*******************************************************************************/

/* 包含系统头文件 */
#include <stdarg.h>

/* 包含自定义头文件 */
#include "stm8s.h"
#include "uart1.h"
#include "stm8s_gpio.h"

/* 自定义新类型 */

/* 自定义宏 */

/* 全局变量定义 */

/*******************************************************************************
 * 名称: Uart_Init
 * 功能: UART1初始化操作
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
//void Uart2_Init(void)
//{
//    UART2_DeInit();		/* 将寄存器的值复位 */
//	
//	/*
//	 * 将UART1配置为：
//	 * 波特率 = 115200
//	 * 数据位 = 8
//	 * 1位停止位
//	 * 无校验位
//	 * 使能接收和发送
//	 * 使能接收中断
//	 */
//    UART2_Init((u32)115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, \
//    	UART2_PARITY_NO , UART2_SYNCMODE_CLOCK_DISABLE , UART2_MODE_TXRX_ENABLE);
//    UART2_Cmd(ENABLE);
//}


/*******************************************************************************
 * 名称: UART1_SendByte
 * 功能: UART1发送一个字节
 * 形参: data -> 要发送的字节
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void UART2_SendByte(u8 data)
{
	UART2_SendData8((unsigned char)data);
	
	/* 等待传输结束 */
	while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
}

/*******************************************************************************
 * 名称: UART1_SendString
 * 功能: UART1发送len个字符
 * 形参: data -> 指向要发送的字符串
 *       len -> 要发送的字节数
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void UART2_SendString(u8* Data,u16 len)
{
	u16 i=0;
	for(; i < len; i++)
		UART2_SendByte(Data[i]);	/* 循环调用发送一个字符函数 */
}

/*******************************************************************************
 * 名称: UART1_ReceiveByte
 * 功能: UART1接收一个字符
 * 形参: 无
 * 返回: 接收到的字符
 * 说明: 无 
 ******************************************************************************/
u8 UART2_ReceiveByte(void)
{
	u8 USART2_RX_BUF; 
	
	/* 等待接收完成 */
	while (UART2_GetFlagStatus(UART2_FLAG_RXNE) == RESET)
	  	;
	
	USART2_RX_BUF = UART2_ReceiveData8();
	
	return  USART2_RX_BUF;
}
void SerialPutChar(uint8_t c)
{
    UART2_SendData8((unsigned char)c);
    /* 等待传输结束 */
    while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
}

/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
void Serial_PutString(uint8_t *s)
{
  while (*s != '\0')
  {
    SerialPutChar(*s);
    s++;
  }
}




/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/