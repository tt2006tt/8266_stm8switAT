/******************** (C) COPYRIGHT  ���iCreateǶ��ʽ���������� **************
 * �ļ���  ��menu.c
 * ����    ��STM8S BooterLoader
 * ʵ��ƽ̨�����STM8S NOTE������
 * ��汾  ��V3.0.0
 * ����    �����  QQ��779814207/2366260464
 * ����    ��
 * �Ա�    ��http://shop71177993.taobao.com/
 *�޸�ʱ�� ��2015-10-15



*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "menu.h"
#include "stm8s_gpio.h"
#include "uart1.h"
#define LED_GREEN_PORT (GPIOC)  //2016/12/9 21:30
#define LED_GREEN_PIN  (GPIO_PIN_6)

#define LED_RED_PORT (GPIOC)      
#define LED_RED_PIN  (GPIO_PIN_5)

#define SerialPutString(x)   Serial_PutString((uint8_t*)(x))
/** @addtogroup STM32F0xx_IAP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern uint32_t UserMemoryMask;


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
#define FLAG_BaseAddress        ((uint32_t)(0x4045))
#define UPDATE_FLAG             ((uint8_t)0x88) 
//extern volatile uint8_t ubBootFlag;

void  Main_Menu(void)
{
  uint8_t cnt = 0;
  uint32_t i;
  while (1)
  {

      /* Download user application in the Flash */
      if(FLASH_ReadByte(FLAG_BaseAddress) == UPDATE_FLAG)
      {
        //---------�������(APP����)-----------
          GPIO_WriteHigh(LED_RED_PORT,LED_RED_PIN);
          FLASH_If_Erase(FLASH_APP_START_PHYSICAL_ADDRESS,FLASH_APP_BLOCKS_NUMBER);
          GPIO_WriteLow(LED_RED_PORT,LED_RED_PIN);
        
        
        
        //---------���򿽱�������->APP��----------
        for(i = 0;i<FLASH_BACKU_END_PHYSICAL_ADDRESS - FLASH_BACKUP_START_PHYSICAL_ADDRESS;i++)
        {
          uint8_t data = FLASH_ReadByte(FLASH_BACKUP_START_PHYSICAL_ADDRESS + i);
          FLASH_ProgramByte(FLASH_APP_START_PHYSICAL_ADDRESS + i,data);
          if(++cnt >= 30)
          {
            cnt = 0;
            GPIO_WriteReverse(LED_GREEN_PORT, LED_GREEN_PIN);
          }
        }
        GPIO_WriteLow(LED_GREEN_PORT,LED_GREEN_PIN);
        
        FLASH_ProgramByte(FLAG_BaseAddress,0x00);//���³ɹ���д���־λ
        
        //---------�������(��������----------
          GPIO_WriteHigh(LED_RED_PORT,LED_RED_PIN);
          FLASH_If_Erase(FLASH_BACKUP_START_PHYSICAL_ADDRESS,FLASH_BACKUP_BLOCKS_NUMBER);
          GPIO_WriteLow(LED_RED_PORT,LED_RED_PIN);
        //----------------------------
        
      }
//      ubBootFlag = 0x00;
      
      asm("LDW X,  SP ");
      asm("LD  A,  $FF");
      asm("LD  XL, A  ");
      asm("LDW SP, X  ");
      asm("JPF $8800");
  }
}


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
