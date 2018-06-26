#include "key.h"
#include "led.h"
#include "Text.h"

/*void Key_init(void)
{
EXTI_DeInit();
GPIO_Init(KEY_PORT, KEY_PIN, GPIO_MODE_IN_FL_IT);
// Initialize the Interrupt sensitivity 
EXTI_SetExtIntSensitivity(KEY_INT_SRC, EXTI_SENSITIVITY_FALL_ONLY);
EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}
*/

void mykey_Press(Typdef_ProgramPara *programpara){//
  static uint16_t Cnt_Kye = 0;
  static uint16_t Cnt_KeyUP = 0;
  static uint8_t flag_keyup = 0;
  if ((((uint8_t)KEY_PORT->IDR) & KEY_PIN) == 0x00){// 按键按下(GPIO_ReadInputData(KEY_PORT)
    Cnt_Kye++;
    if( Cnt_Kye > 150){//长按 1.5s
        WebSocketOK = 0;
        OFF_All();
        Mode = Mode_Config; 
        programpara->Status &= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
        programpara->SubMode = 0x00;
    }
  }else{
    if(Cnt_Kye > 4 && Cnt_Kye < 50){//按下时间大于40ms小于500ms
        flag_keyup ++;
    } 

    if(flag_keyup){//如果发生单击
        Cnt_KeyUP++;
    }
    if(flag_keyup == 3 && Cnt_KeyUP > 50){//如果连按三次
        Cnt_KeyUP = 0;
        flag_keyup = 0;
        programpara->SubMode = 0x00;
        OFF_All();
        Mode = Mode_Test;

    }
    if(flag_keyup == 1 && Cnt_KeyUP > 50){//如果只按一次
        Cnt_KeyUP = 0;
        flag_keyup = 0;
        if(((programpara->PPM) > (programpara->Alarm.Num))){
            SetBit(programpara->Status,BIT_ALARMOFF);
            ClrBit(ProgramPara.flag_Time,BIT_MINITE);
            LED_RED_Ctrl(OFF);
            GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
            programpara->Cnt_Minite = 0;
            
        }else{
            if(Mode == Mode_Nomal){
                SetBit(programpara->Status,BIT_SELFTEST);
                Receive = 0x40;
                DataFormat.command = COMMAND_SELFTEST;
            }
        }
    }
    if(flag_keyup != 1 && flag_keyup != 3 && Cnt_KeyUP > 50){
        flag_keyup = 0;
        Cnt_KeyUP = 0;
    }
    Cnt_Kye = 0;
  }
}

