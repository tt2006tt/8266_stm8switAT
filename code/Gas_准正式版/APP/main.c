/*----------------------------------------------------------------------------------------------------------------------------
�޶�ʱ��:2017��4��25��
����˵��:DataFormat:���ͨ��Э����صĲ���
         ProgramPara���ϵͳ���õ�������״̬��־������־λ ������,�������PPM���豸ID��
           
         Mode����������״̬ɢת
           
         ubUartRxBuf[RXMAX]:���ڽ�������
         Softpnum:��¼�̼����ƵĴ������
         WebSocketOK �ɹ���������Websocket��־��Ӧ�ÿ����Ż�ʡ�ԣ�
         primaryKey;˽Կ
         open_key:��Կ
								 
ͨ��Э��:--------------------------------------------------------------------------------------------------------------------
         ��ʾλ         Ŀ��ID              ԴID         ��������       ����      �������ݳ���        ����      ����
          F               DSTID           SRCID           entry           idx     payloadlenth      command      payload               
       1byte            10byte           10byte           1byte          2byte       2byte            2byte         n byte
        ---------------------------------------------------------------------------------------------------------------------
        Boot����ַΪ        0x8000---0x87FF   �ܴ�С��2K
        APP����ַΪ     0x8800---0xC3FF   �ܴ�С��15K
        Backup����ַΪ      0xC400---0xFFFF   �ܴ�С��15K

**********************************************�����Լ�*******************************************************
              ����������� �����졢�̡������� ������˸һ��ͬʱ����AD��ֵ���Ƿ�����
              ��������   ���Ƴ����� ���������⣨Ҳ�п����ǵ�Դ�ĵ��µģ�
                         ��Ƴ����� ESP8266û�гɹ�ͨ�ţ������ǲ������޸�ʧ�ܣ�Ҳ��������ȫ�޷�ͨ�ţ�
************************************************************************************************************   


********************************************��������********************************************************
                1���豸����wifi���̵Ƴ�����  ���ӳɹ� ���������ģʽ
                                            ����ʧ�� ����������ģʽ 
               2������ģʽ ���̵�����      ���ӳɹ��� �������ģʽ
               3������ģʽ �����ӳɹ����̵Ƴ��� 
                             ����ʧ�ܣ��̵���
                             ���֮�󣺷�������һ��������wifi����ģʽ
               4��wifi���ӣ����۳ɹ�ʧ�ܶ���������ģʽ
               5������ģʽ������������ADֵ��ת����PPM
                            �������ͨ��
                ע�⣺���� �Բ�� �����Լ��⣬���������ƶ����ж��и���

������ void OFF_ALL(void) :�ر����еĵ��Լ�������
       void Reflash(uint16_t command,uint8_t receive,uint16_t waittime): ���� ������룬���ձ�ʶ�����ȴ�ʱ��
       void ESP8266_Monitor(Typdef_ProgramPara *programpara) :�������ͨ�ŵĲ���

����  command��receiveͬʱ�����������ͨ�ŵ����� 
      waittime ��������ʱ����ʱ��ͨ�������ܵĴ���ʵ�ֵġ�
************************************************************************************************************

  2017_04_27�������Bootload����    
  2017_04_28���޸����ж����ȼ�����UART2���ж����ȼ�����TIM2֮ǰ
  2017_05_02: �� Typdef_ProgramPara �����ReootCnt ��������10���з�������ȴû���յ�ACK���������������
  2017_05_03: ������Զ����µ�Э�飬����δ����
  2017_05_09: �����IAP�ı�̣�ÿ�ε�½���Զ����������°�̼������У�����£������������С�����ʱGas���˽��չ̼����ݰ���û���������ܡ�
              ����ʱ���̵�
  2017_05_11:�Գ�������Ż�����ʡ��һЩ�ռ䣬����˲��Եƺͷ�������Э�飬�������������ֵ��Э�顣
             ��PPMֵ����΢����ʹ��ʼֵ���͡���PPM����.
             ����˰����жϾ������ܡ�
  2017_05_12:��һ�����Ż��˳��򣬶�PPMֵ���趨�ٴ�����΢����������ֵ����Ϊ0�ˣ�������һ���Ĳ�������
  2017_05_15:�������Բ⹦�ܣ���UDP�㲥��ԭ����8�ι㲥��Ϊ2�ι㲥
  2017_05_16:�������Զ������Ĺ��ܣ��ڷ������������ ������������ ���ѡ����Բ��Ϊ����������APP������ͬʱ����״ֵ̬
  2017_05_18:�����Ż��˴��룬���Բ⹦�ܶ������������������������������������¶�����ʵ���Բ⡣�Ż��˱������ܣ����������ȶ���
  2017_05_19:������Լ칦�ܣ����κ���������������ΰ����Ϳ��Խ����Լ죬�Լ���Ϸ���ModeInitģʽ��(���Լ칦��ֻ���ڿ���ʱʹ�ã��������ʹ�ö��ᵼ������)
  2017_05_23: �Ż��������������ܣ���Bootload�еĳ������������޸ģ�ԭ����ֱ�Ӹ��Ʊ������ĳ���app�����޸ĺ����Ȳ���APP���ĳ���
               �ٸ��Ʊ������ĳ�������ٲ����������ĳ���
  2017_05_24: �ı������Ʒ����� ����״̬����ɫ��Դ�Ƴ���                 
                               ����״̬�����κ�ɫ����˸����������Ƶ���˸����
                               �Լ�״̬�������̡��졢��ɫ����˸��������Ƶ���˸����
                               ����״̬��������ɫ����������һ�����������
                               ����״̬��������ɫ����˸
                               ����״̬��������ɫ����˸
  2017_05_31:�޸���һЩbug��
  2017_06_01:�޸��˳����Լ��ָʾ�����������������ֲ������޸�ʧ��ʱ ����ƣ������ִ���������ʱ�� ����
             ��ֹͣ������־λ�ӳ���10���� ͬ�������ֹͣ������APPָ��
  2017_06_07:���У׼���ܣ�����д�������һ�ο��������У׼����48Сʱ���У����Լ���ɺ�Ҳ�����У׼��Ϊ
  2017_06_09:���mode_connect���ܣ���������8266ģ���Ƿ�����������(���ӹ�˾wifi)
-------------------------------------------------------------------------------------------------------------------------*/
#include "allInclude.h"

//#pragma location = ".bootflag"
//__no_init volatile uint8_t ubBootFlag;


void InitCpuClock(void);
void InitTimer2(void);
void InitAll(void);

void NetWorking(void);
void Detect(void);
void SelfTEST(void);
void CheckSelf(void);
void Connect_Test_Wifi(void);


char ubUartRxBuf[RXMAX] = {0};//�������洮������
volatile uint16_t Softpnum = 0;
uint8_t Mode;             //ϵͳ״̬ɢת

Typdef_DataFormat DataFormat = {0};
Typdef_ProgramPara ProgramPara = {0};

volatile uint16_t uwTimerCount = 0;
volatile uint8_t ubStartTimerFlag = 0;
uint16_t connect_cnt = 0;

void DelayApi(uint16_t uwDelayCnt){    
    //Set the flag to make ISR start to count
    ubStartTimerFlag = 1;
    
    //Wait until expire 
    while(uwTimerCount <= uwDelayCnt);
    
    //Clear flag and timer cnt
    ubStartTimerFlag = 0;
    uwTimerCount = 0;
}
void led_revise_done(void)
{
    static uint8_t i = 0;
    if(i == 0){
        i = 1;
        LED_GREEN_Ctrl(OFF);
        DelayApi(500);
        LED_RED_Ctrl(OFF);
        DelayApi(500);
        LED_BLUE_Ctrl(ON);
        DelayApi(500);
    }
    else if(i == 1){
        i = 2;
        LED_GREEN_Ctrl(OFF);
        LED_BLUE_Ctrl(OFF);
        DelayApi(500);
        LED_RED_Ctrl(ON);
        DelayApi(500);
        
    }
    else if(i == 2){
        i = 0;
        LED_GREEN_Ctrl(OFF);
        LED_RED_Ctrl(OFF);
        DelayApi(500);
        LED_BLUE_Ctrl(OFF);
        DelayApi(500);
    }

}
int main(void){
    static uint8_t Cnt_Hour = 0;
    static uint8_t Offset_Hour = 0;
    static uint8_t Cnt_Day = 0;
    static uint8_t Cnt_2Day = 0;
    static uint8_t Cnt_AlarmOFF = 0;
    uint8_t i = 0;
    InitAll();//��������Ҫ��ʼ���Ķ�����ʼ��
    
    Read_ChipID_Init(&ProgramPara); //��ȡ������EEPROM�е��豸ID
    Read_SetPPM(&ProgramPara);//��ȡ�趨�õ�PPMֵ
    ProgramPara.Cnt_ERROR = 3000;//����ƣ��Ͽ����磩30s������
    //Mode = Mode_Nomal;
    Mode = Mode_Init;
//------------------��ʼ�����--------------------     
    while(1){
        while(!flag_10ms);//��������³���10msѭ��һ��
        flag_10ms = 0;
        mykey_Press(&ProgramPara);//����ɨ�裨����1.5�������������ģʽ��
//Debugģʽ��Ϊ       20s     /1����    /1Сʱ
//��Debugģʽ��Ϊ    2����    /1Сʱ    /һ��
        if(++ProgramPara.Cnt_Minite >= CNT_MINITE){

            ProgramPara.Cnt_Minite = 0;
            if((ProgramPara.Status & BIT(BIT_ALARMOFF))){
                Cnt_AlarmOFF++;
                if(Cnt_AlarmOFF > 5){//��ǿ��ֹͣ������ʮ���Ӻ��Զ��ָ�
                    Cnt_AlarmOFF = 0;
                    ClrBit(ProgramPara.Status,BIT_ALARMOFF);
                }
            }else{
                Cnt_AlarmOFF = 0;
            }
           
            SetBit(ProgramPara.flag_Time,BIT_MINITE);

            if(++Cnt_Hour>= CNT_HOUR){
                Cnt_Hour = 0;
                ProgramPara.SendMEM = 0;//1Сʱ������ط�����
                SetBit(ProgramPara.flag_Time,BIT_HOUR);              
                if(++Cnt_Day >= CNT_DAY){
                    Cnt_Day = 0;
                    SetBit(ProgramPara.flag_Time,BIT_DAY);
                }
            }
        }
        
        if(ProgramPara.flag_revise == 0){
            if(++ProgramPara.Offset_Minite >= CNT_MINITE){
                ProgramPara.Offset_Minite = 0;

                if(++Offset_Hour>= CNT_HOUR){
                    Offset_Hour = 0;
                    if(++Cnt_2Day >= CNT_2DAY){
                        Cnt_2Day = 0;

                        
                        ProgramPara.ppm_offset.Num = ProgramPara.PPM; //48hour ���¼��offset
                        if(ProgramPara.ppm_offset.Num > 350){
                            ProgramPara.flag_revise = 2;//У��ʧ��
                        }
                        else{
                            ProgramPara.flag_revise = 1;	//У׼���
                            writeSomeByte(PPMoffset_BaseAddress,ProgramPara.ppm_offset.Arr,4);
                            writeSomeByte(Offsetflag_BaseAddress,&ProgramPara.flag_revise,1);
                        }
                        if(ProgramPara.flag_revise == 1){
                            while(1){             
                                led_revise_done();
                            }
                        }
                        else if(ProgramPara.flag_revise == 2){
                          while(1){
                            LED_RED_Ctrl(ON);//У��ʧ����ʾ
                          }
                        }
                    }
                }
            }
        }
        
    	 switch(Mode){    
              case Mode_Init:           ESP8266Init(&ProgramPara);//ESP8266��ʼ��������������Ӳ���
              break;	               
              case Mode_Config:         ESP8266_Smart(&ProgramPara);//�������ã�ͨ���ֻ��㲥��ȡwifi�˺�����
              break; 
              case Mode_Bias:           InitOK(&ProgramPara);//�������������,���� 
              break;
              case Mode_CreateSession:  ESP8288_WebConnect(&ProgramPara);//��WebSocket�Ự
              break;
              case Mode_Nomal:          Detect();//ȼ����⡢������
                                        if(ProgramPara.Status & BIT(BIT_SELFTEST)){//�Բ��־λ��ÿ���ԲⶼҪ����������ؽ�����Ϣ
                                            SelfTEST();
                                        }
#ifdef VERSION_NET                                            
                                         NetWorking();
#endif                                            
                                        break;
              case Mode_Test:          CheckSelf();//�����Լ�
                                        break;
                                        
              case Mode_Connect:          Connect_Test_Wifi();//�����Լ�
                                        break;
                                        
              default:                 Mode = Mode_Init;
                                        break;
    	 }
    }
}
void Connect_Test_Wifi(void)
{
	uint8_t test[] = "AT+CWJAP=\"dlink_edio\",\"epform020716\"\r\n";
	
	ESP8266_Reload(200);
  uartSendDataBuf(test,mystrlen(test));
	if(waitTime){//������ڵȴ�ʱ����
            waitTime--;
            
            if(mystrstr(ubUartRxBuf,"OK")){
                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//��������
                DelayApi(250);
                
                GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//�ط�����
                DelayApi(250);
                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//��������
                DelayApi(250);
                
                GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//�ط�����
                DelayApi(250);
                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//��������
                DelayApi(250);
                
                GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//�ط�����
                Mode = Mode_Init;//Mode_Init;
                
            }
            else 
            {
            	if((ProgramPara.Cnt_Minite - connect_cnt) >= 500)
            	{
                    LED_BLUE_Ctrl(ON);
            	}
            }
            
        }
}
void CheckSelf(void){
    uint8_t ChangeBaud_4800[] = "AT+UART_DEF=4800,8,1,0,0\r\n";
    uint8_t ChangeMode[] = "AT+CWMODE=1\r\n";

    switch(ProgramPara.SubMode){
    case 0x00:
                ESP8266_RST();

                InitUart2(115200);
                DelayApi(500);//�ȴ�һ�������ȶ�
                uartSendDataBuf(ChangeBaud_4800,mystrlen(ChangeBaud_4800));//�޸Ĳ�����
                
                DelayApi(200);
                InitUart2(4800); 
                DelayApi(500);
                ESP8266_Reload(200);
                uartSendDataBuf(ChangeMode,mystrlen(ChangeMode));//�ı乤��ģʽ
                ProgramPara.SubMode = 0x01;
      break;
    case 0x01:
                if(waitTime){//������ڵȴ�ʱ����
                    waitTime--;
                    if(mystrstr(ubUartRxBuf,"OK")){
                      
                        OFF_All();
                        
                        GPIO_WriteHigh(LED_BLUE_PORT,LED_BLUE_PIN);//������
                        DelayApi(250);
                        
                        GPIO_WriteLow(LED_BLUE_PORT,LED_BLUE_PIN);//������
                        GPIO_WriteHigh(LED_RED_PORT, LED_RED_PIN);//�����
                        DelayApi(250);
                        
                        GPIO_WriteLow(LED_RED_PORT, LED_RED_PIN);//�غ��
                        GPIO_WriteHigh(LED_GREEN_PORT, LED_GREEN_PIN);//�̵���
                        DelayApi(250);
                        
                        GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);//���̵�
                        for(int i = 0 ; i < ADC_SAMPLE_CNT ; i++)
                        {
                            ProgramPara.Voltage = MQ4_Votage();
                        }
                        if(ProgramPara.Voltage < 20 || ProgramPara.Voltage > 300){//�жϵ�ѹ��С�����̫���̫С����������
                        
                            ProgramPara.SubMode = 0xF1;
                            break;
                        }
                        GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//��������
                        DelayApi(250);
                        
                        GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//�ط�����
                        Mode = Mode_Connect;//Mode_Init;
                        connect_cnt = ProgramPara.Cnt_Minite;
                        ProgramPara.SubMode = 0x00;
                    }
                }else{
                  ProgramPara.SubMode = 0xF0;
                }        
      break;
    case 0xF0://���
      LED_RED_Ctrl(ON);
      break;
    case 0xF1://����
      LED_BLUE_Ctrl(ON);
      break;
    default:
      LED_RED_Ctrl(ON);
      break;      
    }
}

void Detect(void){
     static uint8_t cnt_Alarm = 0;
     int int_ppm = 0;
     ProgramPara.Voltage = MQ4_Votage();
     //ProgramPara.flag_revise = 0;
     if(ProgramPara.flag_revise == 1)//�ѽ���У׼
     {
         if(ProgramPara.Voltage <120)                                              int_ppm =  (((ProgramPara.Voltage * 3) - ProgramPara.ppm_offset.Num));
         else if((ProgramPara.Voltage >=120) && (ProgramPara.Voltage < 200))      int_ppm =  ((ProgramPara.Voltage - 100) * 45)  + 360 - ProgramPara.ppm_offset.Num;
         else if((ProgramPara.Voltage >=200) && (ProgramPara.Voltage < 250))      int_ppm =  ((ProgramPara.Voltage - 200) * 35)  + 750 - ProgramPara.ppm_offset.Num;
         else if((ProgramPara.Voltage >=250) && (ProgramPara.Voltage < 280))      int_ppm =  ((ProgramPara.Voltage - 250) * 10)  + 2500 - ProgramPara.ppm_offset.Num;
         else if((ProgramPara.Voltage >=280) && (ProgramPara.Voltage < 320))      int_ppm =  ((ProgramPara.Voltage - 280) * 18)  + 2800 - ProgramPara.ppm_offset.Num;
         else if((ProgramPara.Voltage >=320) && (ProgramPara.Voltage < 330))      int_ppm =  ((ProgramPara.Voltage - 320) * 280) + 3200 - ProgramPara.ppm_offset.Num;
         if(int_ppm < 0){
            ProgramPara.PPM = 0;
         }
         else{
            ProgramPara.PPM  = int_ppm;
         }
     }
     else if(ProgramPara.flag_revise == 0)//�˴����������offset
     {
         if(ProgramPara.Voltage <120)                                              ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage * 3));
         else if((ProgramPara.Voltage >=120) && (ProgramPara.Voltage < 200))      ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage - 100) * 45)  + 360;
         else if((ProgramPara.Voltage >=200) && (ProgramPara.Voltage < 250))      ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage - 200) * 35)  + 750;
         else if((ProgramPara.Voltage >=250) && (ProgramPara.Voltage < 280))      ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage - 250) * 10)  + 2500;
         else if((ProgramPara.Voltage >=280) && (ProgramPara.Voltage < 320))      ProgramPara.PPM = (uint32_t)((ProgramPara.Voltage - 280) * 18)  + 2800;
         else if((ProgramPara.Voltage >=320) && (ProgramPara.Voltage < 330))      ProgramPara.PPM = (uint32_t)((ProgramPara.Voltage - 320) * 280) + 3200;

     }
      //�ڷ��Բ�״̬��
     if(((ProgramPara.Status & BIT(BIT_SELFTEST)) == 0)){
       //      û�����ֶ����Եƺͷ�����   &&  �Ѿ���ɳ�ʼ�� && ��ǰ��PPMֵ���趨ֵ��
         if(((ProgramPara.Status & BIT(BIT_ALARMOFF)) == 0) && ((ProgramPara.Status) & BIT(BIT_HASBIAS)) && ((ProgramPara.PPM) > ProgramPara.Alarm.Num)){
           //���ֳ���50�Σ��� 0.5s�����ж�Ϊȼ������
           if(cnt_Alarm < 50){
             cnt_Alarm ++;
           }
           if(cnt_Alarm >= 50){
             ProgramPara.flag_Alarm = 1;
           } 
        }else{   //���Բ⣬û�б��� || PPM��������ֵ���� && û�����ֶ����Ժ�ƺͷ����� &&û��������
            if(((ProgramPara.Status & BIT(BIT_ALARMOFF)))|| ((ProgramPara.PPM < (ProgramPara.Alarm.Num)) && ((ProgramPara.Flag_Test & 0x0F) == 0) && ((ProgramPara.Status & BIT(BIT_RECONNECT)) == 0))){
                 if(cnt_Alarm > 0){
                    cnt_Alarm--;
                 }
                 if(cnt_Alarm == 0){
                    OFF_All();
                    ProgramPara.flag_Alarm = 0;
                 }
            }
        }
        if(ProgramPara.flag_Alarm){
          //���û���͹�������Ϣ�Ҳ����ڸ���״̬
            if(ProgramPara.SendMEM == 0 && (!ProgramPara.Updating)){
                DataFormat.command = COMMAND_WARNING;
            }
        }
     }
}

void NetWorking(void){
    if(WebSocketOK){//����Ѿ�����Websocket�Ự
      if(mystrstr(ubUartRxBuf,"HTTP/1.1 400") || mystrstr(ubUartRxBuf,"web application is stopping")){//������������ж�
        Mode = Mode_Init;
        WebSocketOK = 0;
        ProgramPara.Status &= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
        ProgramPara.Status |= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
        ProgramPara.SubMode = 0x00;
        return;
      }
     //--------------------------------------------------------------
        ESP8266_Monitor(&ProgramPara);//����ESP8266���յ�����Ϣ
     //--------------------------------------------------------------
     
        if((!ProgramPara.Updating) && ((ProgramPara.Status & BIT(BIT_SELFTEST)) == 0)){//���û�д��ڸ��»��Բ�״̬
            if((ProgramPara.Status & BIT(BIT_LOGINOK))){//��¼�ɹ�if(programpara->Flag_LogInOK)
            
                if((ProgramPara.flag_Time & BIT(BIT_MINITE))){
                //  ÿ�����ӷ�һ��������Debug��ֻ��20s��
                        ClrBit(ProgramPara.flag_Time,BIT_MINITE);
                        DataFormat.command = COMMAND_HEARTRATE;
                }
                if((ProgramPara.flag_Time & BIT(BIT_HOUR))){
                //ÿСʱ�ϴ�һ��PPM��Debug��ֻ��60s��
                       ClrBit(ProgramPara.flag_Time,BIT_HOUR);
                       DataFormat.command = COMMAND_UPDATEPPM;
                }
                if((ProgramPara.flag_Time & BIT(BIT_DAY))){
                //������һ�ι̼�(Debug��ֻ��60����)
                       ClrBit(ProgramPara.flag_Time,BIT_DAY);
                       DataFormat.command = COMMAND_UPDATEFIR ;
                }
            }else{
                //��¼���ɹ�
                if(ProgramPara.Cnt_ERROR >0){
                    ProgramPara.Cnt_ERROR --;
                }
                else{
                    ProgramPara.Cnt_ERROR = 3000;
                    Mode = Mode_Init;
                }
            }
        }
    }else{//û�гɹ���������
        if((ProgramPara.Status & BIT(BIT_HASNET))){//����������Ϲ���������������˸������
            if(ProgramPara.Cnt_ERROR >0){
                ProgramPara.Cnt_ERROR --;
                SetBit(ProgramPara.Status,BIT_RECONNECT);//������־λ
            }else{
                Mode = Mode_Init;//����
                ProgramPara.Cnt_ERROR = 3000;
                LED_BLUE_Ctrl(OFF);
                ClrBit(ProgramPara.Status,BIT_RECONNECT);//��������־λ
            }
        }else{//��δ����������
        ClrBit(ProgramPara.Status,BIT_RECONNECT);//��������־λ
      }
    }
}

void SelfTEST(void){ 
    static uint16_t Cnt_SelfTest = 0;
    
    Cnt_SelfTest ++;
    if(Cnt_SelfTest >= 1000){//10s
        Cnt_SelfTest = 0;
        OFF_All();
      
        if(ProgramPara.SelfTest >= 0x04){
            ClrBit(ProgramPara.Status,BIT_SELFTEST);
            Receive = 0xFF;
            DataFormat.command = 0x0000;
            ProgramPara.SelfTest = 0;
            return;
        }

        Receive = 0x40;
        DataFormat.command = COMMAND_SELFTEST;
        ProgramPara.SelfTest ++;
    }
}
/*******************************************************************************
* Function Name  : InitCpuClock.
* Description    : Initial CPU clock, .
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void InitCpuClock(void){
#ifdef HSE_CLOCK   
        CLK_DeInit();
        CLK_HSECmd(ENABLE);//ʹ���ⲿʱ��
        while(SET != CLK_GetFlagStatus(CLK_FLAG_HSERDY));//�ȴ�ʱ���л����
        CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//CPUʱ�Ӳ���Ƶ
        CLK_ClockSwitchCmd(ENABLE); 
        CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO , CLK_SOURCE_HSE , DISABLE , CLK_CURRENTCLOCKSTATE_ENABLE);
#endif        
#ifdef HSI_CLOCK
        CLK_DeInit();
        CLK_HSECmd(DISABLE);//ʧ���ⲿʱ��
        CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);//�ڲ�ʱ�Ӷ���Ƶ
        CLK_HSICmd(ENABLE);
#endif    

      
}

/*******************************************************************************
* Function Name  : InitTimer2.
* Description    : Initial timer2 config, .
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void InitTimer2(void){
 #ifdef HSE_CLOCK        

  //�ⲿ����
    //1ms = 16/16000000 *1000
  /* Set the Prescaler value */
#ifdef OSC_16MHZ
    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_16);
#endif
#ifdef OSC_8MHZ
    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_8);
#endif    
    /* Set the Autoreload value */
    TIM2->ARRH = (uint8_t)(2000 >> 8);//2ms����һ���ж�
    TIM2->ARRL = (uint8_t)(2000);
#endif  
#ifdef HSI_CLOCK
  //�ڲ�����
//    /* Set the Prescaler value */
    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_8);
    /* Set the Autoreload value */
    TIM2->ARRH = (uint8_t)(2000 >> 8);//2ms����һ���ж�
    TIM2->ARRL = (uint8_t)(2000);
 #endif    

    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);  


}
void InitAll(void){
    InitCpuClock();//�ó���������Ϊ�ⲿʱ�� ����Ƶ
    
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//��������IO  
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);//��������IO  
    InitUart2(4800);
    
    InitTimer2();//����ϵͳ�ܶ�ʱ��1ms��һ���ж�
    ITC->ISPR4 &= 0xF3;//����ʱ��2�����ȼ����ͣ�����UART�ж�
    enableInterrupts();//�����ж�

    ADC_Init(Channel_12);//ADC��ʼ��

    ESP8266_GPIO_Init();
    Led_Init();//LED��ʼ��
    BeepInit();//��������ʼ��

    EEPROM_INIT();//FLASH��ʼ��
//    ubBootFlag = 0x00;
    FLASH_ProgramByte(FLAG_BaseAddress,0x00);
    
    WWDG->WR = WWDG_WR_RESET_VALUE;
    WWDG->CR = (uint8_t)((uint8_t)(WWDG_CR_WDGA | WWDG_CR_T6) | (uint8_t)WWDG_CNT);
    WWDG->WR = (uint8_t)((uint8_t)(~WWDG_CR_WDGA) & (uint8_t)(WWDG_CR_T6 | WWDG_WIN));
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
//uint32_t theline = 0;
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//theline = line;
  /* Infinite loop */
//  theline = line;
  while (1)
  {
    
  }
}
#endif




