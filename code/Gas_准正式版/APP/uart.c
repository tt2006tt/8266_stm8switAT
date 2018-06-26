#include "uart.h"
#include "stm8s.h"
#include "stm8s_uart2.h"
#include "key.h"



void InitUart2(uint32_t baudrate){
    
////-------UART2_RX-------   
//    GPIOD->DDR |= 0X40;
//    GPIOD->CR1 |= 0X00;
//    GPIOD->CR2 |= 0X40;
//// ------UART2_TX-------
//    GPIOD->DDR |= 0X20;
//    GPIOD->CR1 |= 0X20;
//    GPIOD->CR2 |= 0X20;
////----------------------    

    
    UART2_DeInit();
    
    //4800, 8bit, 1 stopbit, both rx and tx
    if(baudrate == 4800){
        UART2_Init((u32)baudrate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
                    UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    }else if(baudrate == 115200){
        UART2_Init((u32)baudrate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
                    UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
   }
    UART2_ClearFlag(UART2_FLAG_RXNE);
    
    //Enable Rx interrupt
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
    
    UART2_Cmd(ENABLE);  
}

//串口发送字符串函数
void uartSendDataBuf(uint8_t *tempbuf,uint8_t buflen){
      uint8_t i;
      for(i = 0; i < buflen; i++) {             
              while(!(SET==UART2_GetFlagStatus(UART2_FLAG_TC)));//发送完成
              UART2_SendData8(tempbuf[i]);
      }  
}
