#include "ESP8826.h"
#include "beep.h"
#include "stm8s_gpio.h"

//typedef @far void (*)(void) TFunction;
typedef FAR void (*TFunction)(void);
//BLD start address: change it if your user app is not this one
#define BLD_USER_RESET_ADDR  0x8000ul
//Jump to bootloader(reset) api
const TFunction BldUserApplication = (TFunction)BLD_USER_RESET_ADDR;
const uint8_t hardversion = 0x01;
//extern volatile uint8_t ubBootFlag;
volatile int16_t waitTime = 0;
uint8_t open_key[]      = "Ftl_201207166688";//˽Կ�������ʼ��ע�ᡢ��¼
uint8_t primaryKey[16] = {0};//��ע��͵�¼�������Ķ���˽Կ
uint8_t cipmode[]       ="AT+CIPMODE=1\r\n";    //͸��ģʽ
uint8_t cipsend[]       ="AT+CIPSEND\r\n";      //����ģʽ
uint8_t endSend[]       ="+++";                 //��������
uint8_t Postdata[33] = "action=saveDevid&devid=";


uint8_t DatePackege[DataMax] = {0};

void GetIPv4(char *dst,Typdef_IPAddress *src);//ͨ���������������벢���ϵ�����������
uint8_t UDP_AddrPackege(char *iparr,uint8_t IPNum,uint8_t index);

int strstr_index(char *str,char *sub_str)  //�����Ӵ�λ���±�
{  
    for(int i = 0; str[i] != '\0'; i++)  
    {  
        int tem = i; //tem���������е���ʼ�ж��±�λ��   
        int j = 0;  
        while(str[i++] == sub_str[j++])  
        {  
            if(sub_str[j] == '\0')  
            {  
                return tem;
            }  
        }  
        i = tem;  
    }  
  
    return 0;  
}  
void RC4(uint8_t* message,char* key,uint8_t msglen)//
{
  
  uint8_t iS[256] ={0};   
  uint16_t i = 0;
//  uint8_t keylen = strlen(key);
  uint8_t keylen = LENGTH_KEY;//(sizeof(key)/sizeof(key[0]))
  uint8_t temp = 0;
  
  uint8_t j = 0;
  uint8_t x = 0;
  
  uint8_t *iInputChar = 0;
  
  uint8_t iOutputChar[DataMax] = {0};
  
  uint8_t t = 0 ;
  
  iInputChar = message;
  
  for ( i = 0; i < 256; i++)
  {
    iS[i] = i;	
  }
  for ( i = 0; i < 256; i++) 
  {		
    j = (j + iS[i] + key[i%keylen]) % 256;
    temp = iS[i];
    iS[i] = iS[j];
    iS[j] = temp;
  }
  for (x = 0,i=0,j=0; x < msglen; x++) 
  {
    i = (i + 1) % 256;
    j = (j + iS[i]) % 256;
    temp = iS[i];
    iS[i] = iS[j];
    iS[j] = temp;
    t = (iS[i] + (iS[j] % 256)) % 256;		
    iOutputChar[x] = (uint8_t) (iInputChar[x] ^ iS[t]);
  }
  
  memset( message,0,msglen);
  memcpy( message,iOutputChar,msglen);
}
uint8_t mystrlen(uint8_t * s){
   uint8_t i;
   for (i = 0; s[i]; i++) ;
   return i;
}
int mystrstr(char *s1,const char *s2){//�����Ӵ�

  int n;
  if(*s2){
    while(*s1){
      for( n=0; *(s1+n) == *(s2+n); n++){
        if( !*(s2 + n +1) )
          return 1;
      }
      s1++;
    }
    return 0;
  }
  else
    return 1;
}


void ESP8266_Reload(uint16_t delayTime){	
  ubIndex = 0;
  memset(ubUartRxBuf,0,RXMAX);
  waitTime = delayTime;
}

#ifdef VERSION_2
void ESP8266_GPIO_Init(void){
      //------WRST-------
//    ESP9266_RST_PORT->DDR |= 0X10;
//    ESP9266_RST_PORT->CR1 |= 0X10;
//    ESP9266_RST_PORT->CR2 |= 0X10;
    //-----------------
  GPIO_Init(ESP9266_RST_PORT,(GPIO_Pin_TypeDef)ESP9266_RST_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

#endif 

void ESP8266_RST(void){
    GPIO_WriteLow(ESP9266_RST_PORT, ESP9266_RST_PIN);//����
    DelayApi(25);//����50ms
    GPIO_WriteHigh(ESP9266_RST_PORT,ESP9266_RST_PIN);//����
}

void ESP8266_Smart(Typdef_ProgramPara *programpara){   
    uint8_t Position = 0;
    uint8_t i                    = 0;
    Typdef_IPAddress IP;
    Typdef_IPAddress NetMask;
    
    uint8_t cipsta[]="AT+CIPSTA?\r\n";
    uint8_t stopsmartstr[]      ="AT+CWSTOPSMART\r\n";
    uint8_t startsmartstr[]     ="AT+CWSTARTSMART\r\n";
    uint8_t SmartACK[18]        ="smartok,";    
    uint8_t close[]             ="AT+CIPCLOSE\r\n";
    uint8_t Connect_Broadcast[]="AT+CIPSTART=\"UDP\",\"255.255.255.255\",18267\r\n";
    switch(programpara->SubMode){
    case SMARTCONFIG_INIT:
                ESP8266_RST();
                DelayApi(500);//�ȴ�1���ʼ����Э�� 
                
                ESP8266_Reload(200);
                uartSendDataBuf(stopsmartstr,mystrlen(stopsmartstr)); //��ֹͣsmartģʽ  
                programpara->SubMode = SMARTCONFIG_START;//stopsmartģʽ 
                
                break;                                  
    case SMARTCONFIG_START:
                if(waitTime){//������ڵȴ�ʱ����
                  waitTime--;
                  if(mystrstr(ubUartRxBuf,"OK"))//ָ��ִ�гɹ�
                  {
                    ESP8266_Reload(200);
                    uartSendDataBuf(startsmartstr,mystrlen(startsmartstr));//����startsmartָ��
                    programpara->SubMode = SMARTCONFIG_WAIT;//����startsmartָ���ģʽ
                    break;
                  }
                }
                else
                {
                   programpara->SubMode = SMARTCONFIG_ERROR;//�����ʱ�ȴ�
                }   				
                break;
    case SMARTCONFIG_WAIT: 
                if(waitTime){
                  waitTime--;
                  if(mystrstr(ubUartRxBuf,"OK")){//���ָ��ִ�гɹ�
                    ESP8266_Reload(7000);//�ȴ�70s
                    programpara->SubMode = SMARTCONFIG_IP;//�����������ӵȴ�ģʽ
                    break;
                  }	
                }else{
                  programpara->SubMode = SMARTCONFIG_ERROR;//�����ʱ�ȴ�
                }
                break;
    case SMARTCONFIG_IP: 
                if(waitTime){
                  waitTime--;
                  if(mystrstr(ubUartRxBuf,"GOT IP")|| mystrstr(ubUartRxBuf,"connected wifi")){//����ɹ�����  
                    ESP8266_Reload(0);
                    uartSendDataBuf(cipsta,mystrlen(cipsta));//�������

                    DelayApi(500);//�ȴ�һ����
                    
                    GetIPv4("ip",&IP);//���IP
                    GetIPv4("netmask",&NetMask);//��ú�����
                    
                    for(i = 0;i<LENGTH_IPADDR;i++){
                        IP.Arr[i] |= (~NetMask.Arr[i]);//��������ȡ������IP���
                    }
                    
                    Position = 18;
                    for(i = 0;i<LENGTH_IPADDR;i++){
                        Position = UDP_AddrPackege(Connect_Broadcast,IP.Arr[i],Position);
                    }

                    Connect_Broadcast[Position] = '"';
                    Connect_Broadcast[++Position] = ',';
                    Connect_Broadcast[++Position] = '1';
                    Connect_Broadcast[++Position] = '8'; 
                    Connect_Broadcast[++Position] = '2';
                    Connect_Broadcast[++Position] = '6';
                    Connect_Broadcast[++Position] = '7';
                    Connect_Broadcast[++Position] = 0x0d;//  /r
                    Connect_Broadcast[++Position] = 0x0a;//  /n
                    Connect_Broadcast[++Position] = 0;
                    
                    ESP8266_Reload(200);
                    uartSendDataBuf(Connect_Broadcast,mystrlen(Connect_Broadcast));//��ʼ����㲥ģʽ  
                    
                    programpara->SubMode = SMARTCONFIG_BROAD;
                  }
                }
                else{
                  programpara->SubMode = SMARTCONFIG_ERROR;//�����ʱ�ȴ�
                }
                break;
    case SMARTCONFIG_BROAD:  
                if(waitTime){
                  waitTime--;
                  if(mystrstr(ubUartRxBuf,"OK")){//����㲥ģʽ�ɹ�//|| mystrstr(ubUartRxBuf,"ALREADY")
                  
                    ESP8266_Reload(100);
                    uartSendDataBuf(cipmode,mystrlen(cipmode));//����Ϊ͸��ģʽ
                    programpara->SubMode = SMARTCONFIG_TRAN;//����͸��ģʽCIPMODE=1
                    break;
                  }
                }
                else{
                  programpara->SubMode = SMARTCONFIG_ERROR;
                }
                break;
    case SMARTCONFIG_TRAN:  
                if(waitTime){
                  waitTime--;
                  if(mystrstr(ubUartRxBuf,"OK")){//����͸��ģʽ�ɹ�
                    uartSendDataBuf(cipsend,mystrlen(cipsend));//���뷢��ģʽ
                    programpara->SubMode = SMARTCONFIG_SEND;//��ʼ����
                  }
                }
                else{
                  programpara->SubMode = SMARTCONFIG_ERROR;
                }
                break;
    case SMARTCONFIG_SEND:
                for(i = 0;i<LENGTH_DEV_ID;i++){
                    SmartACK[i+8] = programpara->Dev_ID[i];
                }//װ������
                for(i = 0;i<RESENDCNT;i++){//�ط�10��  ��ֹû�յ�
                    uartSendDataBuf(SmartACK,18);//���ͳɹ�����ָ��
                    DelayApi(10);//�ȴ�20ms
                }
                DelayApi(10);//�ȴ�20ms���ͽ���
                uartSendDataBuf(endSend,mystrlen(endSend));//��������
                DelayApi(600);//�ȴ�1.2s���ͽ���
                ESP8266_Reload(100);
                uartSendDataBuf(close,mystrlen(close));//�ر�͸��
                programpara->SubMode = SMARTCONFIG_FINISH;
    case SMARTCONFIG_FINISH:
                if(waitTime){
                  waitTime--;
                  if(mystrstr(ubUartRxBuf,"OK")){//�ر�͸��ģʽ�ɹ�
                    Mode = Mode_Bias;//����У��ģʽ*MainMode
                    programpara->SubMode = 0x00;
                    SetBit(programpara->Status,BIT_ONLINE);//�ɹ���������
                    SetBit(programpara->Status,BIT_HASNET);//�Ѿ���������
                  }					
                }else{
                  programpara->SubMode = SMARTCONFIG_ERROR;
                }
                break;             
    default:      //û��������������
      
                ClrBit(programpara->Status,BIT_ONLINE);//��������ʧ�ܣ����ӱ�־λ����
                
                Mode = Mode_Bias;//����У��ģʽ
                programpara->SubMode = 0x00;//�´ν���ʱ�ӵ�һ����ʼ
//              LED_BLUE_Ctrl(OFF);//����ʧ�ܣ����Ƴ���
//                LED_GREEN_Ctrl(OFF);//����ʧ�ܣ��̵Ƴ���
                OFF_All();
                break;
    }
}
void ESP8266Init(Typdef_ProgramPara *programpara){//ESP8266ģ���ʼ����������λ�������ж�
  uint8_t cipstatus[]="AT+CIPSTATUS\r\n";
 
  switch(programpara->SubMode){ //(*Status)
        case 0x00:    
                    ESP8266_RST();
                    ESP8266_Reload(800);//��õȴ�8��
                    programpara->SubMode = 0x01;
        case 0x01:
                    if(waitTime){
                        waitTime--;
                        if(mystrstr(ubUartRxBuf,"GOT IP")){
                            programpara->SubMode  = 0x00;//*Status
                            Mode = Mode_Bias;//����У��ģʽ
                            SetBit(programpara->Status,BIT_ONLINE);
                            SetBit(programpara->Status,BIT_HASNET);//�ɹ��������磬�򿪿�������
                            break;
                        }
                    }else{
                        ESP8266_Reload(200);//��õȴ�2��
                        uartSendDataBuf(cipstatus,mystrlen(cipstatus));
                        programpara->SubMode = 0x02;
                    }
                    break;
        case 0x02:
                    if(waitTime){
                      waitTime--;
                      if(mystrstr(ubUartRxBuf,"STATUS:2") || mystrstr(ubUartRxBuf,"STATUS:4")){//˫�ر���
                        programpara->SubMode  = 0x00;//*Status
                        Mode = Mode_Bias;//����У��ģʽ
                        
			SetBit(programpara->Status,BIT_ONLINE);
			SetBit(programpara->Status,BIT_HASNET);//�ɹ��������磬�򿪿�������
                        break;
                      }
                    }
                    else{//�ȴ���ʱ
                      Mode = Mode_Config;//���û�����óɹ������������ģʽ
                      programpara->SubMode = 0x00;//*Status
                    }
                    break;
        default:
                    Mode = Mode_Init;//��ͷ��ʼ
                    programpara->SubMode = 0x00;//*Status
                    break;
  }
  
}

void  ESP8288_WebConnect(Typdef_ProgramPara *programpara)//����websocket����Э��
{
  uint8_t Web_HandSake[]="GET /SmartHomeServer1/gaws HTTP/1.1\r\nConnection:Upgrade\r\nHost:120.76.210.47:80\r\nOrigin: http://120.76.210.47\r\nSec-WebSocket-Key:Rmx6dHJvbmxpbmswMjcxNg==\r\nSec-WebSocket-Version:13\r\nUpgrade:websocket\r\n\r\n";
  uint8_t Connect_URL[] = "AT+CIPSTART=\"TCP\",\"flytronlink.net\",80\r\n";

  switch(programpara->SubMode){
        case 0x00:
                    ESP8266_Reload(200);
                    uartSendDataBuf(Connect_URL,mystrlen(Connect_URL));
                    programpara->SubMode = 0x01;
                    break;
        case 0x01:
                    if(waitTime){
                      waitTime--;
                      if(mystrstr(ubUartRxBuf,"OK")){//|| mystrstr(ubUartRxBuf,"ALREADY CONNECTED")
                        ESP8266_Reload(200);
                        uartSendDataBuf(cipmode,mystrlen(cipmode));//����Ϊ͸��ģʽ												
                        programpara->SubMode = 0x02;//*Status
                        break;
                      }
                    }
                    else{
                      programpara->SubMode  = 0xFF;
                    }
                    break;
        case 0x02:
                    if(waitTime){
                      waitTime--;
                      if(mystrstr(ubUartRxBuf,"OK")){
                        uartSendDataBuf(cipsend,mystrlen(cipsend));//����Ϊ����ģʽ
                        DelayApi(15);//�ȴ�30ms
                        ESP8266_Reload(200);
                        uartSendDataBuf(Web_HandSake,(sizeof(Web_HandSake))/(sizeof(char))-1);
                        programpara->SubMode  = 0x03;
                        break;
                      }
                    }else{
                      programpara->SubMode = 0xFF;
                    }
                    break;
        case 0x03:
                    if(waitTime){
                      waitTime--;
                      if(mystrstr(ubUartRxBuf,"HTTP/1.1 101")){
                        ESP8266_Reload(0);
                        programpara->SubMode = 0x03;
                        Mode = Mode_Nomal;//���ݿ�ʼ���� 		
                        WebSocketOK = 1;//���Խ���ͨ��
                        OFF_All();//�ص�����LED�ͷ�����
                        		


						
                        if(programpara->Dev_ID[0] == 'G' && programpara->Dev_ID[1] == 'A'){//���EEPROM��ǰ���ֽ���GA�Ļ� 
                          if(programpara->Dev_ID[2] == 'F'){
                            DataFormat.command = COMMAND_GETNEWID;//��δע�ᣬ��ע��
                            Receive = 0x01;
                          }
                          else{
                            DataFormat.command = COMMAND_LOGIN;//��ע�ᣬ���е�¼
                            Receive = 0x04;
                          }
                        }else{
                            DataFormat.command = COMMAND_GETNEWID;//��δע�ᣬ��ע��
                            Receive = 0x01;
                        }
                        
                      }                                             
                    }else{
                      programpara->SubMode = 0xFF;
                    }
                    break;

			
        default:
                    Mode = Mode_Nomal;//����ģʽ
                    programpara->SubMode = 0x00;
                    DataFormat.command=0x0000;
                    WebSocketOK = 0;//û�гɹ�����websocket�Ự         
                    break;  
  }
}

uint8_t WebSocket_Data(void){//websocket ���ݴ��
  uint8_t sendlen = 0;	
  uint8_t i;
  for(i=0;i<DataMax;i++){
    DatePackege[i] = 0;
  }
  //-----------�涨�����ݸ�ʽͷ----------
  DatePackege[0] = 0x82; 
  DatePackege[2] = 0x00;
  DatePackege[3] = 0x00;
  DatePackege[4] = 0x00;
  DatePackege[5] = 0x00;
  //--------------------Flag-------------------
  DatePackege[6] = 'F';
  sendlen = 7;
  //--------------------DST_ID-----------------
  memcpy(&DatePackege[sendlen],DataFormat.DST_ID,LENGTH_DST_ID);
  sendlen += LENGTH_DST_ID;//17
  //--------------------SRC_ID-----------------
  
  memcpy(&DatePackege[sendlen],DataFormat.SRC_ID,LENGTH_SRC_ID);
  sendlen += LENGTH_SRC_ID;//27
  //------------------��������---------------
  
  memcpy(&DatePackege[sendlen],(uint8_t*)&DataFormat.encrypt,LENGTH_ENCRYPT);
  sendlen += LENGTH_ENCRYPT;//28
  //--------------------����------------------
  
  DataFormat.idx +=1;
  //DatePackege[sendlen]    = DataFormat.idx>>8;
  //DatePackege[sendlen +1] = DataFormat.idx;
  memcpy(&DatePackege[sendlen],(uint16_t*)&DataFormat.idx,LENGTH_IDX);
  
  sendlen += LENGTH_IDX;//30	
  //-------------�������ݳ���--------------
  
  memcpy(&DatePackege[sendlen],(uint16_t*)&DataFormat.payloadlength,LENGTH_PAYLOADLENGTH);
  sendlen += LENGTH_PAYLOADLENGTH;//32
  
  //-------------------����-------------------
  
  memcpy(&DatePackege[sendlen],(uint16_t*)&DataFormat.command,LENGTH_COMMAND);
  sendlen += LENGTH_COMMAND;//34
  //-------------------����-------------------	
  memcpy(&DatePackege[sendlen],DataFormat.mems,(DataFormat.payloadlength - 2));
  if(DataFormat.encrypt == 1){
    RC4(&DatePackege[sendlen - LENGTH_COMMAND],open_key,DataFormat.payloadlength);
  }
  else if(DataFormat.encrypt == 2){
    RC4(&DatePackege[sendlen - LENGTH_COMMAND],primaryKey,DataFormat.payloadlength);
  }
  
  sendlen += DataFormat.payloadlength;
  
  DatePackege[1] = 0x80 | (sendlen - 6-2);
  
  return (sendlen - 2);
}

void ProcessPackege(uint8_t *dst,uint8_t *src,uint8_t encrypt,uint16_t length,uint16_t command){
  memcpy(DataFormat.DST_ID,dst,LENGTH_DST_ID);
  memcpy(DataFormat.SRC_ID,src,LENGTH_SRC_ID);
  DataFormat.encrypt = encrypt;
  DataFormat.payloadlength = length;
  DataFormat.command = command;
  
}
void Reflash(uint16_t command,uint8_t receive,uint16_t waittime){
    DataFormat.command = command;
    Receive = receive;
    ESP8266_Reload(waittime);
}
void ESP8266_Monitor(Typdef_ProgramPara *programpara){
  static uint16_t LastTime_Blue;
  static uint16_t LastTime_Red;
  static uint16_t LastTime_Green;//���������������˶�Ӧ������ʱ��
  static uint16_t LastTime_BEEP;//������������˵�����ʱ�� 
  static uint8_t  Cnt_Reboot;

  uint8_t length;//Ҫ���͵����ݰ�����
  uint32_t AddrBias = 0x00000000; //��ַƫ�Ƴ���
  uint8_t dst[] = "SSFFFFFFFF";//Ŀ��ID
  uint8_t i;
  
  switch(Receive){
    case 0x01:DataFormat.command = COMMAND_GETNEWID;       break;//����µ��豸ID
    case 0x02:DataFormat.command = COMMAND_NEWIDACK;       break;//����µ��豸ID ACK
    case 0x04:DataFormat.command = COMMAND_LOGIN;          break;//Login���ڳɹ�����websocket�Ự����λ
    case 0x08:DataFormat.command = COMMAND_LOGINACK;       break;//LoginACK
    case 0x10:DataFormat.command = COMMAND_READGAS;        break;//APP��Gas�� PPMֵ
    case 0x20:DataFormat.command = COMMAND_SETPPM;         break;//����PPM����ֵ
    case 0x30:DataFormat.command = COMMAND_LEDCOLOR;       break;//LED���Ե�
    case 0x40:DataFormat.command = COMMAND_SELFTEST;       break;//�Բ�
    case 0x50:DataFormat.command = COMMAND_SELFTESTACK;    break;//�Բ�ACK
    case 0xE0:DataFormat.command = COMMAND_UPDATEFIR;      break;//���¹̼�
    case 0xF0:DataFormat.command = COMMAND_UPDATEFIRACK;   break;//���¹̼�ACK
    default:break;
  }
    if((programpara->Flag_Test&BIT(BIT_RED))){
      if(abs((int16_t)(programpara->Cnt_Minite - LastTime_Red))>=450){//4.5s
            LED_RED_Ctrl(OFF);
            ClrBit(programpara->Flag_Test,BIT_RED);
        }
    }
    else if((programpara->Flag_Test&BIT(BIT_GREEN))){
        if(abs((int16_t)(programpara->Cnt_Minite - LastTime_Green))>=450){//4.5s
            LED_GREEN_Ctrl(OFF);
            ClrBit(programpara->Flag_Test,BIT_GREEN);
        }    
    }
    else if((programpara->Flag_Test&BIT(BIT_BLUE))){
        if(abs((int16_t)(programpara->Cnt_Minite - LastTime_Blue))>=450){//4.5s
            LED_BLUE_Ctrl(OFF);
            ClrBit(programpara->Flag_Test,BIT_BLUE);
        }
    }
    if((programpara->Flag_Test&BIT(BIT_BEEP))){
        if(abs((int16_t)(programpara->Cnt_Minite - LastTime_BEEP))>=60){//600ms
            GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
            ClrBit(programpara->Flag_Test,BIT_BEEP);
        } 
   }
  if(DataFormat.command != 0X00 && (DataFormat.command != COMMAND_SELFTESTACK && DataFormat.command != COMMAND_WARNING && DataFormat.command != COMMAND_LEDCOLOR)){
        OFF_All();
  }
  switch(DataFormat.command){    
    case COMMAND_READGAS:
                        memcpy(dst,DataFormat.SRC_ID,LENGTH_SRC_ID);
                        ProcessPackege(dst,programpara->Dev_ID,PRIMARYKEY,0x000a,COMMAND_READGASACK);
                        for(i = 0;i<LENGTH_PPM;i++)//װ�ص�ǰPPMֵ
                        {
                          DataFormat.mems[3-i] = programpara->PPM>>(8*i); 
                        }          
                        memcpy(&DataFormat.mems[LENGTH_PPM],programpara->Alarm.Arr,LENGTH_PPM);//װ��PPM�趨ֵ

                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        ClrBit(programpara->Status,BIT_SELFTEST);

                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_WARNING:
                        ProcessPackege(dst,programpara->Dev_ID,PRIMARYKEY,0x0006,COMMAND_WARNING);//Ŀ��ID,ԴID���������ͣ��������ݳ��ȣ�����
                        
                        programpara->SendMEM = 1;
                        
                        for(i = 0;i<LENGTH_PPM;i++){
                          DataFormat.mems[3-i] = programpara->PPM>>(8*i); 
                        }
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_HEARTRATE:
                        ProcessPackege(dst,programpara->Dev_ID,PRIMARYKEY,0x0002,COMMAND_HEARTRATE);
                                 
                        length = WebSocket_Data();
                        DataFormat.command = 0x0000;
                        
                        if(Cnt_Reboot > 2){//�������3��û���յ�ACK�����������
                            Mode = Mode_Init;
                            WebSocketOK = 0;
                            programpara->Status &= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
                            programpara->Status |= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
                            programpara->SubMode = 0x00;
                        }
                        Cnt_Reboot++;//��������
                        
                        ESP8266_Reload(0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_HEARTRATEACK:
                        
                        Cnt_Reboot = 0;
                        
                        Reflash(0x0000,0xFF,0);
                        
                        break;
    case COMMAND_UPDATEPPM:
                        ProcessPackege(dst,programpara->Dev_ID,PRIMARYKEY,0x0006,COMMAND_UPDATEPPM);
                        
                        for(i = 0;i<4;i++){
                          DataFormat.mems[3-i] = programpara->PPM>>(8*i); 
                        }
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_GETNEWID:          
                        ProcessPackege(dst,programpara->Dev_ID,OPENKEY,0x000c,COMMAND_GETNEWID);
                        
                        memcpy(DataFormat.mems,programpara->Dev_ID,LENGTH_DEV_ID);  
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_NEWIDACK:
                        for(i = 0;i<LENGTH_DEV_ID;i++){

                          programpara->Dev_ID[i] = DataFormat.mems[i];//������������������
                          FLASH_ProgramByte(DEVID_BaseAddress + i,programpara->Dev_ID[i]);
                        } 
                        Reflash(COMMAND_LOGIN,0x04,0);
                        
                        break;
    case COMMAND_LOGIN:
                        ProcessPackege(dst,programpara->Dev_ID,OPENKEY,0x0006,0x220b);
                        
                        programpara->HWVersion.Num = hardversion;
                        memcpy(DataFormat.mems,programpara->HWVersion.Arr,LENGTH_DEVVER); 
                                                
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_LOGINACK:      
                        memcpy(primaryKey,DataFormat.mems,LENGTH_KEY); 
                        SetBit(programpara->Status,BIT_LOGINOK);
                        Reflash(0x0000,0xE0,0);//һ��ʼ�ͼ��̼�
                        break;
    case COMMAND_SETPPM:  
                        memcpy(dst,DataFormat.SRC_ID,LENGTH_SRC_ID);
                        memcpy(programpara->Alarm.Arr,DataFormat.mems,4);
                        if(programpara->Alarm.Num > PPM_MAX){
                            programpara->Alarm.Num= PPM_MAX;
                        }
                       // else if(programpara->Alarm.Num< PPM_MIN){
                         //   programpara->Alarm.Num= PPM_MIN;
                        //}
                        writeSomeByte(PPM_BaseAddress,programpara->Alarm.Arr,LENGTH_PPM);
                        
                        ProcessPackege(dst,programpara->Dev_ID,PRIMARYKEY,0x0002,COMMAND_SETPPMACK);
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_LEDCOLOR:
                        memcpy(dst,DataFormat.SRC_ID,LENGTH_SRC_ID);
                        if(DataFormat.mems[2] == 0xff){//��
                          if(DataFormat.mems[1] == 0xff){
                            if(DataFormat.mems[0] == 0xff){//������
                                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);
                                LastTime_BEEP = programpara->Cnt_Minite;
                                SetBit(programpara->Flag_Test,BIT_BEEP);
                            }
                          }else{
                                LED_GREEN_Ctrl(OFF);
                                LED_RED_Ctrl(OFF);
                                LED_BLUE_Ctrl(ON);

                                LastTime_Blue = programpara->Cnt_Minite;
                                programpara->Flag_Test =0;
                                SetBit(programpara->Flag_Test,BIT_BLUE); 
                          }
                        }else{
                            if(DataFormat.mems[1] == 0xff){//��
                                LED_BLUE_Ctrl(OFF);
                                LED_GREEN_Ctrl(ON);
                                LED_RED_Ctrl(OFF);
                                
                                LastTime_Green = programpara->Cnt_Minite;
                                programpara->Flag_Test =0;
                                SetBit(programpara->Flag_Test,BIT_GREEN);                                  
                            }else{//��
                                LED_GREEN_Ctrl(OFF);
                                LED_RED_Ctrl(ON);
                                LED_BLUE_Ctrl(OFF);
                                
                                LastTime_Red = programpara->Cnt_Minite;
                                programpara->Flag_Test =0;
                                SetBit(programpara->Flag_Test,BIT_RED);                              
                            }
                        }
                        ProcessPackege(dst,programpara->Dev_ID,PRIMARYKEY,0x0002,COMMAND_LEDCOLORACK);
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        ClrBit(programpara->Status,BIT_SELFTEST);
                        programpara->SelfTest = 0;
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_UPDATEFIR: 
                        //��û��ʼ���£����ݰ���Ϊ0
                        DataFormat.mems[0] = 0;
                        DataFormat.mems[1] = 0;
                        
                        //��������Flash�еİ汾��
                        readSomeByte(VerBaseAddress,programpara->Version.Arr,LENGTH_DEVVER);
                        memcpy(&DataFormat.mems[LENGTH_SOFTNUM],programpara->Version.Arr,LENGTH_DEVVER); 
                        
                        //��������
                        ProcessPackege(dst,programpara->Dev_ID,OPENKEY,0x0008,COMMAND_UPDATEFIR);
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_UPDATEFIRACK:
//ÿ��ACK���ص���50Byte�Ķ��������ݣ���Gas�˷��͹�ȥ����UPDATEָ������Ӱ汾�ź����ݿ��
//                        Softpnum = (uint16_t)(DataFormat.mems[0]<<8 & 0xff00) | (DataFormat.mems[1] & 0xff);//��ȡ���ݿ����
                        programpara->Updating = 1;
                        AddrBias = ((uint32_t)Softpnum) * 50;//��ַƫ��      
                        for(i = 0;i<(DataFormat.payloadlength - 2);i++){//������д�뱸�ݵ�Flash��
                            FLASH_Firmware(BACKUP_START_ADDR + AddrBias + i,DataFormat.mems[i]);
                        }
                        
                        //------------------------------------------------------
                        if((DataFormat.payloadlength < (MESMAX + 2)) && Softpnum >100){//��������������Ѿ�����
                            programpara->Version.Num++;//�汾��������д��Flash�����´εĸ��²�ѯ
                            writeSomeByte(VerBaseAddress,programpara->Version.Arr,LENGTH_DEVVER);
                            
                            Softpnum = 0;
                            AddrBias = 0;
                            LED_BLUE_Ctrl(OFF);
                            
                            FLASH_ProgramByte(FLAG_BaseAddress,UPDATE_FLAG);//д����±�־λ
                            
                            //��������ת��Bootload
                            asm("LDW X,  SP ");
                            asm("LD  A,  $FF");
                            asm("LD  XL, A  ");
                            asm("LDW SP, X  ");
                            // software reset
                            BldUserApplication();
                        }
                        //------------------------------------------------------
                        
                        memset(DataFormat.mems,0,MESMAX);//���յ���������Է����ݴ���
                        
                        Softpnum++;//���ݿ�������������߷�����������һ�����ݿ�
                        DataFormat.mems[0] = ((uint8_t)(Softpnum>>8) & 0xFF);
                        DataFormat.mems[1] = (uint8_t)(Softpnum & 0xFF);
                        
                        readSomeByte(VerBaseAddress,programpara->Version.Arr,LENGTH_DEVVER);//���ڴ��п����汾�ŵ�����������
                        memcpy(&DataFormat.mems[LENGTH_SOFTNUM],programpara->Version.Arr,LENGTH_DEVVER); 
                        
                        ProcessPackege(dst,programpara->Dev_ID,OPENKEY,0x08,COMMAND_UPDATEFIR);
                        length = WebSocket_Data();
                        
                        Reflash(0x0000,0xFF,0);
                        
                        uartSendDataBuf(DatePackege,length);
                        break;
    case COMMAND_SELFTEST:
                        DataFormat.mems[0] = 0x0000;
                        DataFormat.mems[1] = programpara->SelfTest;
                        SetBit(programpara->Status,BIT_SELFTEST);
                        
                        ProcessPackege(dst,programpara->Dev_ID,OPENKEY,0x04,COMMAND_SELFTEST);
                        length = WebSocket_Data();

                        Receive = 0x50;
                        DataFormat.command = COMMAND_SELFTESTACK;
//                        ESP8266_Reload(0);
                        Reflash(COMMAND_SELFTESTACK,0x50,0);
                        
                        uartSendDataBuf(DatePackege,length);

                        break;
    case COMMAND_STOPALARM:
                        SetBit(programpara->Status,BIT_ALARMOFF);
                        ClrBit(ProgramPara.flag_Time,BIT_MINITE);
                        LED_RED_Ctrl(OFF);
                        GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
                        programpara->Cnt_Minite = 0;
                        Reflash(0x0000,0xFF,0);
                        break;
    default:             
                        break;
    }
}

uint8_t myatoi(uint8_t  *str){
        uint8_t value = 0;
        while(*str>='0' && *str<='9'){
                value *= 10;
                value += *str - '0';
                str++;
        }
        return value;
}
void GetIPv4(char *dst,Typdef_IPAddress *src){//ͨ������������IPv4�ĵ�ַ�����������DevAddress��
    uint8_t Arrindex = 0;
    uint8_t i = 0;
    uint8_t cnt = 0;
    uint8_t Dev_IP[4] = {0};
    for(i = strstr_index(ubUartRxBuf,dst);i< RXMAX;i++){//������������
    
        if(ubUartRxBuf[i] == 0x22 || ubUartRxBuf[i] == 0x2e){//0x22:"    0x2e: .
        
            cnt++;
            if(cnt > 1 && cnt < 6){
                src->Arr[cnt - 2] = myatoi(Dev_IP);//����ת����
                Arrindex = 0;
                memset(Dev_IP,0,4);
            }
        }
        if(cnt < 5 && cnt >= 1 ){//��һ���ǡ� ���м�����   .  ���һ��Ҳ�ǡ� һ��5����ʶ
        
            if(ubUartRxBuf[i] == 0x22 || ubUartRxBuf[i] == 0x2e)
              continue;
            Dev_IP[Arrindex] = ubUartRxBuf[i];
            Arrindex ++;
        }
        if(cnt >= 6){
            cnt = 0;
            break;
        }
            
    } 
}



uint8_t UDP_AddrPackege(char *iparr,uint8_t IPNum,uint8_t index){//��IP��ַ���������Ϊ�ַ�
    uint8_t bai = IPNum / 100;
    uint8_t shi = IPNum % 100 / 10;
    uint8_t ge  = IPNum % 100 % 10;
    
    if(bai !=0){
        iparr[++index] = bai + '0';
    }
    if(shi == 0){
        if(bai !=0){
            iparr[++index] = '0';
        }
    }else{
        iparr[++index] = shi + '0';
    }
    
    if(ge != 0){
        iparr[++index] = ge + '0';
    }
    else{
        iparr[++index] = '0';
    }
    iparr[++index] = '.';
    return index;
}



