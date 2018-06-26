#ifndef __LED_H__
#define __LED_H__
#include "stm8s.h"
#include "stm8s_gpio.h"


typedef void (*arrp[])(uint8_t);

#define KEY_LONG_PRESS          0x01 //长按
#define  ALREADY_CONF           0x02  //已配置
#define  GAS_ALARM              0x04   //燃气报警

#define LED_BLINK_FAST          0x01
#define LED_BLINK_SLOW          0x02

#define LED_RED         0x01
#define LED_BLUE        0x02
#define LED_GREEN       0x04
//#define LED_YELLOW      0x08

void Led_Init(void);
void LED_Blink(uint8_t LED_BlinkMode,uint8_t Color);

void Led_Proc(void);
void LED_GREEN_Ctrl(u8 mode);
void LED_RED_Ctrl(u8 mode);
void LED_BLUE_Ctrl(u8 mode);
//void LED_YELLOW_Ctrl(uint8_t mode);
//void YelloLED_Blink(void);
void workLedInit(void);
void OFF_All(void);
//extern arrp LED_Controler;
#endif