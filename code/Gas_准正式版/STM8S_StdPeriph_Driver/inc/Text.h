#ifndef __TEXT_H
#define __TEXT_H




//-----------------------
#define BACKUP_START_ADDR      ((uint32_t)0x00C400)
#define DEBUG
#ifdef DEBUG
    #define CNT_MINITE 2000//20s
    #define CNT_HOUR   3 //1min
    #define CNT_DAY    60
    #define CNT_2DAY	  120
#else        
    #define CNT_MINITE 12000//2min
    #define CNT_HOUR   30//1hour
    #define CNT_DAY    24//1day  
    #define CNT_2DAY		48	//48hour
#endif 
#define VERSION_NET
//#define OSC_8MHZ
#define OSC_16MHZ
#define HSE_CLOCK
//#define HSI_CLOCK
#define PAYLOADKEY
#define VERSION_2
//#define VDD_5_V
//#define BIAS
#define VDD_3_3V

#define NONEKEY         0
#define OPENKEY         1
#define PRIMARYKEY      2

//--------------------------
#define LENGTH_KEY              16
#define LENGTH_DEV_ID           10
#define LENGTH_DST_ID           10
#define LENGTH_SRC_ID           10
#define LENGTH_ENCRYPT          1
#define LENGTH_IDX               2
#define LENGTH_PAYLOADLENGTH   2
#define LENGTH_COMMAND          2
#define LENGTH_PPM              4
#define LENGTH_KEY              16
#define LENGTH_SOFTNUM          2
#define LENGTH_DEVVER           4
#define LENGTH_FIRDATA          50
#define LENGTH_UPDATE LENGTH_COMMAND + LENGTH_SOFTNUM + LENGTH_DEVVER
#define LENGTH_VERSION  4
#define LENGTH_ALARMVAL 4
#define LENGTH_NETMASK  4
#define LENGTH_IPADDR   4


#define COMMAND_READGAS         0x2200
#define COMMAND_READGASACK      0x2201
#define COMMAND_WARNING         0x2202
#define COMMAND_HEARTRATE       0x2204
#define COMMAND_HEARTRATEACK    0x2205
#define COMMAND_UPDATEPPM       0x2206
#define COMMAND_GETNEWID        0x2209
#define COMMAND_NEWIDACK        0x220a
#define COMMAND_LOGIN           0x220b
#define COMMAND_LOGINACK        0x220c
#define COMMAND_UPDATEFIR       0x220f
#define COMMAND_UPDATEFIRACK    0x2210
#define COMMAND_SETPPM          0x2211
#define COMMAND_SETPPMACK       0x2212
#define COMMAND_LEDCOLOR        0x2213
#define COMMAND_LEDCOLORACK     0x2214
#define COMMAND_SELFTEST        0x2215
#define COMMAND_SELFTESTACK     0x2216
#define COMMAND_STOPALARM       0x2219
#define COMMAND_STOPALARMACK    0x221a
//-------------------------------
#define Mode_Init    		0x01
#define Mode_Bias               0x02
#define Mode_Config  		0x04
#define Mode_Nomal		0x08
#define Mode_Detect		0x10
#define Mode_Warning		0x20
#define Mode_CreateSession      0x40
#define Mode_Test               0x80
#define Mode_Connect               0xA0


#define BIT_IS  0
#define BIT_HAS 1

#define BIT_MINITE      0
#define BIT_HOUR 	1
#define BIT_DAY 	2

#define BIT_RECONNECT   7  //���������ȴ�״̬
#define BIT_ALARMOFF    6  //ǿ�ƽ�������
#define BIT_SELFTEST    5  //�Բ�
//#define BIT_KEYOK       4
#define BIT_LOGINOK     3  //��ɵ�¼
#define BIT_HASBIAS     2  //��У�����´ξͲ�����У���ˣ�
#define BIT_HASNET      1  //�������ӹ�����(������������������)
#define BIT_ONLINE      0  //��ǰ������wifi

#define BIT_RED      0
#define BIT_GREEN    1
#define BIT_BLUE     2
#define BIT_BEEP     3

#define SMARTCONFIG_INIT        0x00
#define SMARTCONFIG_START       0x01
#define SMARTCONFIG_WAIT        0x02
#define SMARTCONFIG_IP          0x03
#define SMARTCONFIG_BROAD       0x04
#define SMARTCONFIG_TRAN        0x05
#define SMARTCONFIG_SEND        0x06
#define SMARTCONFIG_FINISH      0x07
#define SMARTCONFIG_ERROR       0xFF

#define ON  0x01
#define OFF 0x00
#define REVERSE 0x02

#define WARNING_FAST 0x00
#define WARNING_SLOW 0x01
#define LED_Delay_FAST 150
#define LED_Delay_SLOW 500
//-------------------------------
//#define Bias_ADC_CNT            20
#define ADC_SAMPLE_CNT          16
//#define MP_4_ADC_CHECK_CNT      25
#define RESENDCNT               10
#define WebMaxSize      128
#define PATTERN_SIZE	128
#define RXMAX           128
//#define RXMAX 1024
#define DataMax         64
#define MESMAX          50

#define WWDG_CNT        0x7E
#define WWDG_WIN        0x7F


#ifdef STM8S103
     #define     ID_BaseAddress         (0x4865)
#else//  defined(STM8S105) 
     #define     ID_BaseAddress         (0x48CD)
#endif

#define UPDATE_FLAG             ((uint8_t)0x88) 
#define FLAG_BaseAddress                (0x4045)
#define DEVID_BaseAddress               (0x4055)
#define VerBaseAddress                  (0x4075)
#define PPM_BaseAddress                 (0x4095)
#define PPMoffset_BaseAddress           (0x4115)
#define Offsetflag_BaseAddress           (0x4135)


#ifndef BIT
#define BIT(x)	(1 << (x))
#endif



#define PPM_MAX 4000
#define PPM_MIN 3000    

#define  BEEP_PORT    (GPIOC)              
#define  BEEP_PIN    (GPIO_PIN_7) 

#define LED_GREEN_PORT (GPIOC)  
#define LED_GREEN_PIN  (GPIO_PIN_6)
 
#define LED_RED_PORT (GPIOC)      
#define LED_RED_PIN  (GPIO_PIN_5)

#define LED_BLUE_PORT (GPIOC)        
#define LED_BLUE_PIN  (GPIO_PIN_4)

#define LED_WORK_PORT (GPIOC)        
#define LED_WORK_PIN  (GPIO_PIN_3)

#define ESP9266_RST_PORT (GPIOD)  
#define ESP9266_RST_PIN  (GPIO_PIN_4)

typedef union
{
    uint32_t    Num;         //�汾��
    uint8_t     Arr[LENGTH_VERSION];      //�豸�汾�� 
}Typdef_Version;
typedef union
{
    uint32_t    Num;   //����ֵ
    uint8_t     Arr[LENGTH_ALARMVAL];
}Typdef_AlarmValue;

typedef union
{
    uint16_t    Num;
    uint8_t     Arr[LENGTH_NETMASK];
}Typdef_NetMask;

typedef union
{
    uint32_t    Num;   //ƫ��ֵ
    uint8_t     Arr[LENGTH_ALARMVAL];
}Typdef_PpmOffset;



typedef union
{
    uint32_t    Num;
    uint8_t     Arr[LENGTH_IPADDR];
}Typdef_IPAddress;

typedef struct
{
    uint8_t DST_ID[LENGTH_DST_ID];         //Ŀ��ID
    uint8_t SRC_ID[LENGTH_SRC_ID];         //ԴID
    uint8_t mems[MESMAX];           //���ݳ���
    uint8_t encrypt;            //��������
    uint16_t idx;               //����
    uint16_t payloadlength;     //�������ݳ���
    uint16_t command;           //����
}Typdef_DataFormat;

typedef struct
{
    uint16_t  Voltage;          //��������ѹ
    uint16_t  Cnt_ERROR;        //��ʱ��������״̬����ʱ  
    uint16_t  Cnt_Minite;       //��ʱ��������
    uint8_t   flag_revise;			 //У׼��־λ  0 λУ׼   1ΪУ׼���
    uint8_t   SelfTest;
    uint8_t   SubMode;          //��ģʽɢת
    uint8_t   flag_Time;	//ʱ�� bit0 ����,bit1 Сʱ, bit2 �졣
    
    uint8_t   Status;           //bit 0 �������ӣ�    bit 1����������   ��bit 2 ���У�� ,   ��bit 3 �ɹ���½ ,
                                //bit 4 �ɹ����˽Կ ,bit 5 �Բ�        , bit 6 �ֶ�ֹͣ���� , bit 7 ������   ��bit4�Ѿ������ˣ�
    uint8_t SendMEM;            //���Ͷ��Ŵ�����־λ ÿ�η�����  1Сʱ�����
    uint8_t Updating;           //������־λ������ʱ �������е�����������
    uint8_t Flag_Test;          //���Ե�����   bit0 ��� bit1 �̵� bit2 ���� bit3 ������
    uint8_t flag_Alarm;         //������־λ������λΪ 1 ʱ ����
    uint8_t Dev_ID[LENGTH_DEV_ID];       //�ɷ�����������豸ID
    uint32_t PPM;               //������PPM��ֵ
    Typdef_AlarmValue Alarm;    //����ֵ
    Typdef_Version   Version;   //�豸�汾��
    Typdef_Version   HWVersion; //�豸Ӳ���汾��
    Typdef_PpmOffset ppm_offset;				 //У׼ƫ����
    uint16_t  Offset_Minite;       //У׼��ʱ��
}Typdef_ProgramPara;


extern char ubUartRxBuf[RXMAX];//���ڽ�������
extern uint8_t ubIndex;//������������λ��

extern volatile uint8_t flag_10ms;//10ms��ʱ��־λ  ��Time2�ж�����λ ����main��������0
extern volatile uint8_t WebSocketOK ;//WebSocket��־λ����������ɹ����Ӻ��λ��λ�����Ͽ�����ʱ ��λ��0
extern volatile uint8_t Receive;//���ձ�־����Command��ͬ����Ҫ���ͻ��߽��յ���ֵ���Է�Command�������޸ģ����յ�command��ʱ������������������ָ�
extern volatile uint16_t Softpnum;//���յ����ݰ���
extern volatile int16_t waitTime;//��ʱʱ�� ����Ҫ����config�У��Ѿ��õıȽ�����

extern Typdef_ProgramPara ProgramPara;
extern Typdef_DataFormat DataFormat;//�������ݰ�ʱ���������
extern uint8_t  Mode;//ϵͳ״̬ɢת
extern uint8_t open_key[];//��Կ
extern uint8_t primaryKey[];//˽Կ
//extern Typdef_ProgramPara ProgramPara;
void clearArr(uint8_t* originalArr);//��ս�������
void DelayApi(uint16_t uwDelayCnt);//��ʱ�������Զ�ʱ��2Ϊ��ʱ��׼��������ʱ2ms
#endif

