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
  
  uint8_t *pIDStart=(uint8_t *)(ID_BaseAddress + 4);//ֻ�����8���ֽ�  
  for(i=0;i<10;i++)//��ֹ©�� ���Զ������
  {
    readSomeByte(DEVID_BaseAddress,programpara->Dev_ID,10);
    if(programpara->Dev_ID[0] == 'G' && programpara->Dev_ID[1] == 'A')//���EEPROM��ǰ���ֽ���GA�Ļ�
    {
      break;
    }           
  }
  
  if(programpara->Dev_ID[0] != 'G' && programpara->Dev_ID[1] != 'A')//���EEPROM��ǰ���ֽڲ���GA�Ļ�   
  {
    programpara->Dev_ID[0] = 'G';
    programpara->Dev_ID[1] = 'A';
    programpara->Dev_ID[2] = 'F';
    for(i = 0; i<7;i++)
    {
         programpara->Dev_ID[i+3] = *pIDStart++;//�Զ����ֽ��еĺ���7�ֽ�ΪID
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

