#include "led.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "Text.h"

void Led_Init(void){
    GPIO_Init(LED_GREEN_PORT, (GPIO_Pin_TypeDef)LED_GREEN_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);	
    GPIO_Init(LED_RED_PORT, (GPIO_Pin_TypeDef)LED_RED_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(LED_BLUE_PORT, (GPIO_Pin_TypeDef)LED_BLUE_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(LED_WORK_PORT, (GPIO_Pin_TypeDef)LED_WORK_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    
    GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);
    GPIO_WriteLow(LED_RED_PORT, LED_RED_PIN);
    GPIO_WriteLow(LED_BLUE_PORT, LED_BLUE_PIN);
    GPIO_WriteLow(LED_WORK_PORT, LED_WORK_PIN);
}

void LED_RED_Ctrl(uint8_t mode){
    switch(mode){
      case ON:	    GPIO_WriteHigh(LED_RED_PORT, LED_RED_PIN);
      break;
      case OFF:     GPIO_WriteLow(LED_RED_PORT, LED_RED_PIN);
      break;
      case REVERSE: GPIO_WriteReverse(LED_RED_PORT, LED_RED_PIN);
      break;
    }
}

void LED_GREEN_Ctrl(uint8_t mode ){
  switch(mode){
      case ON:        GPIO_WriteHigh(LED_GREEN_PORT, LED_GREEN_PIN);
      break;
      case OFF:	      GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);
      break;
      case REVERSE:   GPIO_WriteReverse(LED_GREEN_PORT, LED_GREEN_PIN);
      break;

  } 
}
//void LED_YELLOW_Ctrl(uint8_t mode )
//{
//  switch(mode)
//  {
//      case ON:        GPIO_WriteHigh(LED_GREEN_PORT, LED_GREEN_PIN);
//                      GPIO_WriteHigh(LED_RED_PORT, LED_RED_PIN);
//      break;
//      case OFF:	      GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);
//                      GPIO_WriteLow(LED_RED_PORT, LED_RED_PIN);
//      break;
//      case REVERSE:   GPIO_WriteReverse(LED_GREEN_PORT, LED_GREEN_PIN);
//                      GPIO_WriteReverse(LED_RED_PORT, LED_RED_PIN);
//      break;
//  } 
//}
void LED_BLUE_Ctrl(uint8_t mode){
    switch(mode){
    case ON:        GPIO_WriteHigh(LED_BLUE_PORT, LED_BLUE_PIN);
    break;
    case OFF:       GPIO_WriteLow(LED_BLUE_PORT, LED_BLUE_PIN);
    break;
    case REVERSE:   GPIO_WriteReverse(LED_BLUE_PORT, LED_BLUE_PIN);
    break;
    }
}

void LED_Blink(uint8_t LED_BlinkMode,uint8_t Color){	
    static uint16_t Cnt_1ms = 0;
    if(LED_BlinkMode == LED_BLINK_FAST){
      if(++Cnt_1ms >LED_Delay_FAST){//这个值的大小决定了灯的两灭频率
        Cnt_1ms = 0;
        switch(Color){
            case LED_RED:LED_RED_Ctrl(REVERSE);break;
            case LED_BLUE:LED_BLUE_Ctrl(REVERSE);break;  
            case LED_GREEN:LED_GREEN_Ctrl(REVERSE);break;
//            case LED_YELLOW:LED_YELLOW_Ctrl(REVERSE);break;
        }
      }
    }else if(LED_BlinkMode == LED_BLINK_SLOW){
      if(++Cnt_1ms >LED_Delay_SLOW){
        Cnt_1ms = 0;
        switch(Color){
            case LED_RED:LED_RED_Ctrl(REVERSE);break;
            case LED_BLUE:LED_BLUE_Ctrl(REVERSE);break;
            case LED_GREEN:LED_GREEN_Ctrl(REVERSE);break;
//            case LED_YELLOW:LED_YELLOW_Ctrl(REVERSE);break;
        }
      }
    }
}
void OFF_All(void){
    LED_GREEN_Ctrl(OFF);
    LED_BLUE_Ctrl(OFF);
    LED_RED_Ctrl(OFF);
    GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
}
//void YelloLED_Blink(void)
//{	
//    static uint8_t Cnt_1s = 0;
//    Cnt_1s ++;
//      if( Cnt_1s < 100)
//      {
//        LED_YELLOW_Ctrl(ON);
//      }
//      else if(Cnt_1s>=100 && Cnt_1s <=200)
//      {
//        LED_YELLOW_Ctrl(OFF);
//      }
//      else
//      {
//        Cnt_1s = 0;
//      }
//      
//}




