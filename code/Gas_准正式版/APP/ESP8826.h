#ifndef __ESP8826_H__
#define __ESP8826_H__

#include <stdlib.h>
#include <string.h>
#include "stm8s.h"
#include "stm8s_flash.h"

#include "Text.h"
#include "flash.h"
#include "uart.h"
#include "Led.h"

//#define POST "POST /baas/tianhesm HTTP/1.1\r\nAccept: */*\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n%s"

uint8_t mystrlen(uint8_t * s);
void ESP8266_Reload(uint16_t delayTime);
//int mystrstr(unsigned char *source,unsigned char *target);
int mystrstr(char *s1,const char *s2);//²éÕÒ×Ó´®
void ESP8266_RST(void);

void ESP8266_Reload(uint16_t delayTime);
void ESP8266_GPIO_Init(void);
void ESP8266_Monitor(Typdef_ProgramPara *programpara);
void ESP8266Init(Typdef_ProgramPara *programpara);
void ESP8266_Smart(Typdef_ProgramPara *programpara);

void  ESP8288_WebConnect(Typdef_ProgramPara *programpara);
uint8_t  WebSocket_Data(void);

extern uint8_t DatePackege[DataMax];
void RC4(uint8_t* message,char* key,uint8_t msglen);
 
#endif