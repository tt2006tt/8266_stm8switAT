/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 **************
 * 文件名  ：main.c
 * 描述    ：STM8S BooterLoader  
 * 实验平台：风驰STM8S NOTE开发板
 * 库版本  ：V3.0.0
 * 作者    ：风驰  QQ：779814207/2366260464
 * 博客    ：
 * 淘宝    ：http://shop71177993.taobao.com/
 *修改时间 ：2015-10-15



*******************************************************************************/
#include "flash_if.h"
#include "menu.h"
//#include "uart1.h"
#include "stm8s_gpio.h"
/* Includes ------------------------------------------------------------------*/
/** @addtogroup STM8Sxx_IAP
  * @{
  */
//__root const long reintvec[]@".intvec"=
//
//{   
//
//    0x82008080,0x8200A004,0x8200A008,0x8200A00c,
//    
//    0x8200A010,0x8200A014,0x8200A018,0x8200A01c,
//
//    0x8200A020,0x8200A024,0x8200A028,0x8200A82c,
//
//    0x8200A030,0x8200A034,0x8200A038,0x8200A03c,
//
//    0x8200A040,0x8200A044,0x8200A048,0x8200A04c,
//
//    0x8200A050,0x8200A054,0x8200A058,0x8200A05c,
//
//    0x8200A060,0x8200A064,0x8200A068,0x8200A06c,
//
//    0x8200A070,0x8200A074,0x8200A078,0x8200A07c,
//
//};     
__root const long reintvec[]@".intvec"=

{   

    0x82008080,0x82008804,0x82008808,0x8200880c,
    
    0x82008810,0x82008814,0x82008818,0x8200881c,

    0x82008820,0x82008824,0x82008828,0x8200882c,

    0x82008830,0x82008834,0x82008838,0x8200883c,

    0x82008840,0x82008844,0x82008848,0x8200884c,

    0x82008850,0x82008854,0x82008858,0x8200885c,

    0x82008860,0x82008864,0x82008868,0x8200886c,

    0x82008870,0x82008874,0x82008878,0x8200887c,

};     
//#pragma location = ".bootflag"
//__no_init volatile uint8_t ubBootFlag;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void IAP_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
    
void LEDInit(void);


int main(void)
{
 
  IAP_Init();
  /* Infinite loop */
  while (1)
  {
    Main_Menu();
  }
}

/**
  * @brief  Initialize the IAP.
  * @param  None
  * @retval None
  */
static void IAP_Init(void)
{
  /* Unlock the Flash Program Erase controller */
  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
  FLASH_If_Init();
  
  LEDInit();

//  FLASH_If_Erase(APPLICATION_ADDRESS);
  
}

#define LED_RED_PORT (GPIOC)      
#define LED_RED_PIN  (GPIO_PIN_5)

#define LED_GREEN_PORT (GPIOC) 
#define LED_GREEN_PIN  (GPIO_PIN_6)

void LEDInit(void)
{
    GPIO_Init(LED_GREEN_PORT, (GPIO_Pin_TypeDef)LED_GREEN_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);
    
    GPIO_Init(LED_RED_PORT, (GPIO_Pin_TypeDef)LED_RED_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_WriteLow(LED_RED_PORT, LED_RED_PIN);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
//uint32_t theLine = 0;
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//theLine = line;
  /* Infinite loop */
       GPIO_WriteHigh(LED_RED_PORT,LED_RED_PIN);
       GPIO_WriteHigh(LED_GREEN_PORT,LED_GREEN_PIN);
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
