#include "flash.h"
#include "stm8s_flash.h"

void EEPROM_INIT(void)
{
    FLASH_DeInit();

  
/* Unlock program memory */
    FLASH->PUKR = FLASH_RASS_KEY1;
    FLASH->PUKR = FLASH_RASS_KEY2;
    
/* Unlock data memory */
    FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
    FLASH->DUKR = FLASH_RASS_KEY1;
    
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

void writeSomeByte(uint32_t Address, uint8_t *Data,uint8_t length)
{
  uint8_t i;
  for(i=0;i<length;i++)
  {
    FLASH_ProgramByte( Address , *Data++ );   
    Address = Address + 1;
  }
}

void readSomeByte(uint32_t Address, uint8_t Data[],uint8_t length)
{
  uint8_t i;
  //uint8_t ReadWord;
  for(i=0;i<length;i++)
  {
    Data[i] = FLASH_ReadByte( Address);
    Address = Address + 1;
  }
}
void Read_ChipID_Init(Typdef_ProgramPara *programpara)
{
  uint8_t i;
//  uint8_t privilegeID[8] = {0};
  
  uint8_t *pIDStart=(uint8_t *)(ID_BaseAddress + 4);//只读最后8个字节  
  for(i=0;i<10;i++)//防止漏读 所以多读几遍
  {
    readSomeByte(DEVID_BaseAddress,programpara->Dev_ID,10);
    if(programpara->Dev_ID[0] == 'G' && programpara->Dev_ID[1] == 'A')//如果EEPROM中前两字节是GA的话
    {
      break;
    }           
  }
  
  if(programpara->Dev_ID[0] != 'G' && programpara->Dev_ID[1] != 'A')//如果EEPROM中前两字节不是GA的话   
  {
    programpara->Dev_ID[0] = 'G';
    programpara->Dev_ID[1] = 'A';
    programpara->Dev_ID[2] = 'F';
    for(i = 0; i<7;i++)
    {
         programpara->Dev_ID[i+3] = *pIDStart++;//以独有字节中的后面7字节为ID
    }

  }   
  
  
  
}
void Read_SetPPM(Typdef_ProgramPara *programpara)
{
//    uint8_t i = 0;
    readSomeByte(PPM_BaseAddress,programpara->Alarm.Arr,4);
    readSomeByte(PPMoffset_BaseAddress,programpara->ppm_offset.Arr,4);
    readSomeByte(Offsetflag_BaseAddress,&programpara->flag_revise,1);
    if(programpara->Alarm.Num<PPM_MIN)
    {
      programpara->Alarm.Num = PPM_MIN;

      writeSomeByte(PPM_BaseAddress,programpara->Alarm.Arr,4);
    }
    else if(programpara->Alarm.Num> PPM_MAX)
    {
      programpara->Alarm.Num= PPM_MAX;
      writeSomeByte(PPM_BaseAddress,programpara->Alarm.Arr,4);
        
    }
}

