#ifndef __MP_4_H__
#define __MP_4_H__
#include "stm8s.h"
#include "Text.h"

#ifdef BIAS   
extern volatile u16 Bias_Votage;
#endif

#ifdef STM8S103
void ADC_init(void);
u16 ADC1_GetValue(void);
#endif

#ifdef STM8S105

#define Channel_0       0x00
#define Channel_1       0x01
#define Channel_2       0x02
#define Channel_3       0x03
#define Channel_4       0x04
#define Channel_5       0x05
#define Channel_6       0x06
#define Channel_7       0x07
#define Channel_8       0x08
#define Channel_9       0x09
#define Channel_10      0x0A
#define Channel_11      0x0B
#define Channel_12      0x0C
#define Channel_13      0x0D
#define Channel_14      0x0E
#define Channel_15      0x0F

/***************************
AD IO¿Úºê¶¨Òå
***************************/
#define ADC_GPIO        GPIOF
#define ADC_PIN         (1<<4)


void ADC_Init(u8 Channel);
u16 ADC_Data(void);
void ADC_IOConfig(void);

#endif

#ifdef BIAS   
uint8_t Bias_MP_4_Votage(void);
void Bias_MP_4(Typdef_ProgramPara *programpara);//uint8_t *MainMode,uint8_t *Status,uint8_t Flag_2_M
#else
void InitOK(Typdef_ProgramPara *programpara);
#endif



uint16_t MQ4_Votage(void);




#endif