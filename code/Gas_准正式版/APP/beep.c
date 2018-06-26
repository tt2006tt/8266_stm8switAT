#include "beep.h"
#include "stm8s.h"
#include "Text.h"
#include "Led.h"
void BeepInit(void){
  /* Initialize I/Os in Output Mode for Beep */
  GPIO_Init(BEEP_PORT, (GPIO_Pin_TypeDef)BEEP_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
}
void Warning(void){	
    static uint8_t Cnt_1s = 0;	
    if(++Cnt_1s >100){
        Cnt_1s = 0;
        GPIO_WriteReverse(BEEP_PORT, BEEP_PIN);
        GPIO_WriteReverse(LED_RED_PORT, LED_RED_PIN);
    }
}
