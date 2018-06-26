/**
  ******************************************************************************
  * @file     stm8s_it.c
  * @author   MCD Application Team
  * @version  V2.1.0
  * @date     18-November-2011
  * @brief    Main Interrupt Service Routines.
  *           This file provides template for all peripherals interrupt service 
  *           routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "stm8s_uart1.h"
#include "led.h"
#include "Text.h"
//#include "beep.h"
#include "key.h"
#include "ESP8826.h"
#include "beep.h"

extern volatile uint8_t ubStartTimerFlag;
extern volatile uint16_t uwTimerCount;

volatile uint8_t flag_10ms = 0;
volatile uint8_t Receive = 0;
uint8_t Cnt_10ms = 0;
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6) //PD外部中断
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */    
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S903
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
   
   
 }
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */


}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S903
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
    /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
    */
    WWDG_SetCounter(WWDG_CNT);//喂狗
    if(++Cnt_10ms >=5 && flag_10ms == 0)
    {		
        flag_10ms = 1;
        Cnt_10ms = 0;	         
    }
    if(ubStartTimerFlag == 1)
    {
        uwTimerCount++;
    }
    switch(Mode)
    {
    case Mode_Init:
        LED_GREEN_Ctrl(ON);
//      LED_Blink(LED_BLINK_FAST,LED_BLUE);
      break;
    case Mode_Config:
//      LED_Blink(LED_BLINK_SLOW,LED_BLUE);
      LED_Blink(LED_BLINK_SLOW,LED_GREEN);
      break;
    case Mode_Bias:
        if((ProgramPara.Status&BIT(BIT_ONLINE)) != 0)//if(Flag_ConNet != 0)
        {
          LED_GREEN_Ctrl(ON);
//            LED_Blink(LED_BLINK_FAST,LED_BLUE);
        }
      break;
    case Mode_Nomal:
        if((WebSocketOK == 0) && (ProgramPara.Status & BIT(BIT_HASNET)) && ProgramPara.flag_Alarm != 1)//如果曾经连上过网络则在蓝灯闪烁后重连
        {
//            LED_Blink(LED_BLINK_SLOW,LED_GREEN);LED_BLUE
                LED_Blink(LED_BLINK_SLOW,LED_BLUE);
        }
            
        if(ProgramPara.Status & BIT(BIT_SELFTEST))
        {
          switch(ProgramPara.SelfTest)
          {
              case 0x00://LED_Blink(LED_BLINK_FAST,LED_BLUE);
//                        LED_GREEN_Ctrl(OFF);
      
                        LED_GREEN_Ctrl(ON);
      
                        LED_RED_Ctrl(OFF);
                        LED_BLUE_Ctrl(OFF);
                        GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
                break;
              case 0x01://LED_Blink(LED_BLINK_SLOW,LED_BLUE);
        
                        LED_Blink(LED_BLINK_SLOW,LED_GREEN);
                        LED_BLUE_Ctrl(OFF);
    
//                        LED_GREEN_Ctrl(OFF);
                        LED_RED_Ctrl(OFF);
                        GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
                break;
              case 0x02: //LED_Blink(LED_BLINK_SLOW,LED_GREEN);
                         //LED_BLUE_Ctrl(OFF);
    
                         LED_Blink(LED_BLINK_SLOW,LED_BLUE);
                         LED_GREEN_Ctrl(OFF);
      
                         GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
                break;
              case 0x03: Warning();
                         GPIO_WriteLow(LED_BLUE_PORT,LED_BLUE_PIN);
                break;
              default:
                break;
          }
          
        }
        if(((ProgramPara.Status & BIT(BIT_SELFTEST)) == 0) && ((ProgramPara.Status & BIT(BIT_RECONNECT)) == 0) && (ProgramPara.Status & BIT(BIT_ALARMOFF)) ==0)
        {
            if(ProgramPara.flag_Alarm )
            {
                Warning();
            }
        }
      break;
    default:break;
    }
    
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);

 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
//      static uint8_t ubIndexTx = 0;
      
//      ubIndexTx++;
      if(UART1_GetITStatus(UART1_IT_TXE) != DISABLE)
      {
//          ubIndexTx++;
      }
      UART1_ClearITPendingBit(UART1_IT_TXE);
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 /*void forcpy(uint8_t dst[],uint8_t src[],uint8_t max,uint8_t index,uint8_t len)
 {
      index%=max; 
      if(len>(max-index))
      {
          memcpy(dst,&src[index],max-index);
          memcpy(&dst[max-index],src,len-(max-index)); 
      }else
      {
        memcpy(dst,&src[index],len);
      }  
         
 } 
*/ 
volatile uint8_t ubIndex = 0;

void WebRecvData_Proc(uint8_t * buf,uint8_t index)
{
	//uint8_t len = 0;
	uint8_t i = index;
        
	//if(buf[i] != 0x82) return;//添加这一句可以增强程序的安全性，但是会出现有些命令被抛弃的情况
        i++;
	//len = buf[i%RXMAX];//%max
	i++;
       // i%=max;
	//if(buf[i%RXMAX] != 'F')//同样的会出现有些命令被抛弃的情况
	//{
	//	return ;
	//}
	i++;
       // i%=max;
	//------------------获取DST_ID------------------
        //forcpy(DataFormat.DST_ID,buf,RXMAX,i,10);
	memcpy(DataFormat.DST_ID,&buf[i%RXMAX],LENGTH_DST_ID);
	i += 10;
	//i%=max;
	//------------------获取SRC_ID------------------
        //forcpy(DataFormat.SRC_ID,buf,RXMAX,i,10);
	memcpy(DataFormat.SRC_ID,&buf[i%RXMAX],LENGTH_SRC_ID);
	i += 10;
       // i%=max;
	//------------------获取加密类型--------------
	DataFormat.encrypt= buf[i%RXMAX];
	i++;
       // i%=max;
//----------------------获取包序-----------------
	DataFormat.idx = (uint16_t)((buf[i%RXMAX]<<8) | (buf[(i+1)%RXMAX]));
	i += 2;
        //i%=max;
//------------------获取数据部分长度----------
	DataFormat.payloadlength = (uint16_t)(buf[i%RXMAX]<<8) | (buf[(i+1)%RXMAX]);
	i += 2;
        //i%=max;
//----------------------获取命令-----------------
	DataFormat.command = (uint16_t)(buf[i%RXMAX]<<8) | (buf[(i+1)%RXMAX]);
	i = i + 2;
       // i%=max;
//----------------------数据部分-----------------
        if((DataFormat.payloadlength < (MESMAX + 2)) && (DataFormat.payloadlength >2))
        {
          //forcpy(DataFormat.mems,buf,RXMAX,i,DataFormat.payloadlength - 2);
          memcpy(DataFormat.mems,&buf[i%RXMAX],DataFormat.payloadlength - 2);
        }
        
        
        switch(DataFormat.command)
        {
            case COMMAND_READGAS:       Receive = 0x01;     break;
            case COMMAND_NEWIDACK:      Receive = 0x02;     break;
            case COMMAND_LOGINACK:      Receive = 0x08;     break;
            case COMMAND_SETPPM:        Receive = 0x20;     break;
            case COMMAND_UPDATEFIR:     Receive = 0xE0;     break;
            case COMMAND_UPDATEFIRACK:  Receive = 0xF0;     break;
            default:break;
        }
  
        ESP8266_Reload(0);
}
uint8_t i=0,cnt=0; 
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
  
{
    /* In order to detect unexpected events during development,
    it is recommended to set a breakpoint on the following instruction.
    */
#if 1
 //   static uint8_t ubIndex = 0;
    //ubUart1RxBuf[ubIndex++] = UART1_ReceiveData8();
   // ubUart1RxBuf[ubIndex++] = UART1_ReceiveData8();
   // ubUart1RxBuf[ubIndex++] = UART1_ReceiveData8();
    //GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);
    
   // if( UART1->SR&0X08){ 
   //      UART1->SR &= ~0X08;
   // } 
            
    if(UART1_GetITStatus(UART1_IT_RXNE) != DISABLE)//if(UART1_GetITStatus(UART1_IT_RXNE) != DISABLE)
    {
           ubUartRxBuf[ubIndex] = (uint8_t)UART1->DR;//UART1_ReceiveData8();
           ubIndex++;
           if(ubIndex >= RXMAX)
           {
              ubIndex = 0;
           }
           for(i=0;i<RXMAX;i++)
            {
               if(ubUartRxBuf[i]==0x82)
               {
                 cnt++;
                 if(ubUartRxBuf[(i+1)%RXMAX]<=(cnt-2))
                 {
                    WebRecvData_Proc(ubUartRxBuf,i);                 
                   cnt=0;
                 }
                 break;
               }
            }
           
           
          //ubIndex%=RXMAX;        
    }
#endif  
    
    //Clear pending bit
    UART1_ClearITPendingBit(UART1_IT_RXNE);

}
#endif /*STM8S208 or STM8S207 or STM8S103 or STM8S903 or STM8AF62Ax or STM8AF52Ax */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
      if(UART2_GetITStatus(UART2_IT_TXE) != DISABLE)
      {
//          ubIndexTx++;
      }
      UART2_ClearITPendingBit(UART2_IT_TXE);
 }
//void forcpy(uint8_t dst[],uint8_t src[],uint8_t max,uint8_t index,uint8_t len)
// {
//      index%=max; 
//      if(len>(max-index))
//      {
//          memcpy(dst,&src[index],max-index);
//          memcpy(&dst[max-index],src,len-(max-index)); 
//      }else
//      {
//        memcpy(dst,&src[index],len);
//      }  
//         
// } 

uint8_t ubIndex = 0;
volatile uint8_t WebSocketOK = 0;
void WebRecvData_Proc(char * buf,uint8_t index){
	//uint8_t len = 0;
	uint8_t i = index;
        
//	if(buf[i] != 0x82) //添加这一句可以增强程序的安全性，但是会出现有些命令被抛弃的情况
//          return;
        i++;
//	len = buf[i%RXMAX];//%max
	i++;
//	if(buf[i%RXMAX] != 'F')//同样的，会出现有些命令被抛弃的情况
//	{
//		return ;
//	}
	i++;
	//------------------获取DST_ID------------------
//        forcpy(DataFormat.DST_ID,buf,RXMAX,i,10);
	memcpy(DataFormat.DST_ID,&buf[i%RXMAX],10);
	i += 10;
	//------------------获取SRC_ID------------------
//        forcpy(DataFormat.SRC_ID,buf,RXMAX,i,10);
	memcpy(DataFormat.SRC_ID,&buf[i%RXMAX],10);
	i += 10;
	//------------------获取加密类型--------------
	DataFormat.encrypt= buf[i%RXMAX];
	i++;
//----------------------获取包序-----------------
	DataFormat.idx = (uint16_t)((buf[i%RXMAX]<<8) | (buf[(i+1)%RXMAX]));
	i += 2;
//------------------获取数据部分长度----------
	DataFormat.payloadlength = (uint16_t)(buf[i%RXMAX]<<8) | (buf[(i+1)%RXMAX]);
	i += 2;
        
        if(DataFormat.encrypt == 1)//公钥加密
        {
            RC4(&buf[i%RXMAX],open_key,DataFormat.payloadlength);
        }
        else if(DataFormat.encrypt == 3)//私钥加密
        {
            RC4(&buf[i%RXMAX],primaryKey,DataFormat.payloadlength);
        }
        
//----------------------获取命令-----------------
	DataFormat.command = (uint16_t)(buf[i%RXMAX]<<8) | (buf[(i+1)%RXMAX]);
	i = i + 2;
//----------------------数据部分-----------------
        if((DataFormat.payloadlength <= (MESMAX + 2)) && (DataFormat.payloadlength >2))
        {
//          forcpy(DataFormat.mems,buf,RXMAX,i,DataFormat.payloadlength - 2);
          memcpy(DataFormat.mems,&buf[i%RXMAX],DataFormat.payloadlength - 2);
        }
        ESP8266_Reload(0);
        
        switch(DataFormat.command)
        {
            case COMMAND_NEWIDACK:      Receive = 0x02;     break;
            case COMMAND_LOGINACK:      Receive = 0x08;     break;
            case COMMAND_READGAS:       Receive = 0x10;     break;
            case COMMAND_SETPPM:        Receive = 0x20;     break;
            case COMMAND_LEDCOLOR:      Receive = 0x30;     break;
            case COMMAND_SELFTEST:      Receive = 0x40;     break;
            case COMMAND_UPDATEFIR:     Receive = 0xE0;     break;
            case COMMAND_UPDATEFIRACK:  Receive = 0xF0;     break;
            default:break;
        }
        
}

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 uint8_t rxi=0,cnt=0; 
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   if(UART2_GetITStatus(UART2_IT_RXNE) != DISABLE)
    {
           ubUartRxBuf[ubIndex] = (uint8_t)UART2->DR;
           ubIndex++;
           if(ubIndex >= (RXMAX-1))
           {
              ubIndex = 0;
           }
           if(WebSocketOK)
           {
                for(rxi=0;rxi<(RXMAX-1);rxi++)
                {
                   if(ubUartRxBuf[rxi]==0x82)
                   {
                     cnt++;
                     if(ubUartRxBuf[(rxi+1)%RXMAX]<=(cnt-2))
                     {
                        WebRecvData_Proc(ubUartRxBuf,rxi);                 
                       cnt=0;
                     }
                     break;
                   }
                }
           }
           UART2_ClearITPendingBit(UART2_IT_RXNE);
    }
 }
#endif /* STM8S105 or STM8AF626x */




