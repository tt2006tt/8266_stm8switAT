#ifndef __KEY_H__
#define __KEY_H__


#include "stm8s.h"
#include "Text.h"

#define KEY_PORT  (GPIOD)     //
#define KEY_PIN   (GPIO_PIN_3)
#define KEY_INT_SRC  (EXTI_PORT_GPIOD)

void Key_init(void);
void mykey_Press(Typdef_ProgramPara *programpara);

#endif

