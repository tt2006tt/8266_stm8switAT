#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm8s.h"
#include "Text.h"


void EEPROM_INIT(void);
void writeSomeByte(uint32_t Address, uint8_t *Data,uint8_t length);
void readSomeByte(uint32_t Address, uint8_t Data[],uint8_t length);
void Read_ChipID_Init(Typdef_ProgramPara *programpara);
void Read_SetPPM(Typdef_ProgramPara *programpara);
#endif