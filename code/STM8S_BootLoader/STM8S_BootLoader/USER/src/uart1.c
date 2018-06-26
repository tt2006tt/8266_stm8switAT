
/******************** (C) COPYRIGHT  ���iCreateǶ��ʽ���������� **************
 * �ļ���  ��uart1.c
 * ����    ��UART1�����ļ�   
 * ʵ��ƽ̨�����STM8������
 * ��汾  ��V2.1.0
 * ����    �����  QQ��779814207/2366260464
 * ����    ��
 * �Ա�    ��http://shop71177993.taobao.com/
 *�޸�ʱ�� ��2012-10-28

  ���STM8������Ӳ������
    |------------------------|
    |  TXD - PA4(UART1->RX)  |
    |  RXD - PA5(UART1->TX)  |
    |------------------------|

*******************************************************************************/

/* ����ϵͳͷ�ļ� */
#include <stdarg.h>

/* �����Զ���ͷ�ļ� */
#include "stm8s.h"
#include "uart1.h"
#include "stm8s_gpio.h"

/* �Զ��������� */

/* �Զ���� */

/* ȫ�ֱ������� */

/*******************************************************************************
 * ����: Uart_Init
 * ����: UART1��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
//void Uart2_Init(void)
//{
//    UART2_DeInit();		/* ���Ĵ�����ֵ��λ */
//	
//	/*
//	 * ��UART1����Ϊ��
//	 * ������ = 115200
//	 * ����λ = 8
//	 * 1λֹͣλ
//	 * ��У��λ
//	 * ʹ�ܽ��պͷ���
//	 * ʹ�ܽ����ж�
//	 */
//    UART2_Init((u32)115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, \
//    	UART2_PARITY_NO , UART2_SYNCMODE_CLOCK_DISABLE , UART2_MODE_TXRX_ENABLE);
//    UART2_Cmd(ENABLE);
//}


/*******************************************************************************
 * ����: UART1_SendByte
 * ����: UART1����һ���ֽ�
 * �β�: data -> Ҫ���͵��ֽ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void UART2_SendByte(u8 data)
{
	UART2_SendData8((unsigned char)data);
	
	/* �ȴ�������� */
	while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
}

/*******************************************************************************
 * ����: UART1_SendString
 * ����: UART1����len���ַ�
 * �β�: data -> ָ��Ҫ���͵��ַ���
 *       len -> Ҫ���͵��ֽ���
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void UART2_SendString(u8* Data,u16 len)
{
	u16 i=0;
	for(; i < len; i++)
		UART2_SendByte(Data[i]);	/* ѭ�����÷���һ���ַ����� */
}

/*******************************************************************************
 * ����: UART1_ReceiveByte
 * ����: UART1����һ���ַ�
 * �β�: ��
 * ����: ���յ����ַ�
 * ˵��: �� 
 ******************************************************************************/
u8 UART2_ReceiveByte(void)
{
	u8 USART2_RX_BUF; 
	
	/* �ȴ�������� */
	while (UART2_GetFlagStatus(UART2_FLAG_RXNE) == RESET)
	  	;
	
	USART2_RX_BUF = UART2_ReceiveData8();
	
	return  USART2_RX_BUF;
}
void SerialPutChar(uint8_t c)
{
    UART2_SendData8((unsigned char)c);
    /* �ȴ�������� */
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




/******************* (C) COPYRIGHT ���iCreateǶ��ʽ���������� *****END OF FILE****/