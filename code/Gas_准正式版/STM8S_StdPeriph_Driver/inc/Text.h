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

#define BIT_RECONNECT   7  //处于重连等待状态
#define BIT_ALARMOFF    6  //强制结束报警
#define BIT_SELFTEST    5  //自测
//#define BIT_KEYOK       4
#define BIT_LOGINOK     3  //完成登录
#define BIT_HASBIAS     2  //已校正（下次就不用再校正了）
#define BIT_HASNET      1  //曾经连接过网络(触发断网重连的条件)
#define BIT_ONLINE      0  //当前已连接wifi

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
    uint32_t    Num;         //版本号
    uint8_t     Arr[LENGTH_VERSION];      //设备版本号 
}Typdef_Version;
typedef union
{
    uint32_t    Num;   //警报值
    uint8_t     Arr[LENGTH_ALARMVAL];
}Typdef_AlarmValue;

typedef union
{
    uint16_t    Num;
    uint8_t     Arr[LENGTH_NETMASK];
}Typdef_NetMask;

typedef union
{
    uint32_t    Num;   //偏移值
    uint8_t     Arr[LENGTH_ALARMVAL];
}Typdef_PpmOffset;



typedef union
{
    uint32_t    Num;
    uint8_t     Arr[LENGTH_IPADDR];
}Typdef_IPAddress;

typedef struct
{
    uint8_t DST_ID[LENGTH_DST_ID];         //目标ID
    uint8_t SRC_ID[LENGTH_SRC_ID];         //源ID
    uint8_t mems[MESMAX];           //数据长度
    uint8_t encrypt;            //加密类型
    uint16_t idx;               //包序
    uint16_t payloadlength;     //命令数据长度
    uint16_t command;           //命令
}Typdef_DataFormat;

typedef struct
{
    uint16_t  Voltage;          //传感器电压
    uint16_t  Cnt_ERROR;        //计时器，错误状态倒计时  
    uint16_t  Cnt_Minite;       //计时器，分钟
    uint8_t   flag_revise;			 //校准标志位  0 位校准   1为校准完成
    uint8_t   SelfTest;
    uint8_t   SubMode;          //子模式散转
    uint8_t   flag_Time;	//时间 bit0 分钟,bit1 小时, bit2 天。
    
    uint8_t   Status;           //bit 0 网络连接，    bit 1网络已连接   ，bit 2 完成校正 ,   ，bit 3 成功登陆 ,
                                //bit 4 成功获得私钥 ,bit 5 自测        , bit 6 手动停止报警 , bit 7 重连中   （bit4已经不用了）
    uint8_t SendMEM;            //发送短信触发标志位 每次发送完  1小时后清空
    uint8_t Updating;           //升级标志位，升级时 屏蔽所有的心跳等请求
    uint8_t Flag_Test;          //测试灯两灭   bit0 红灯 bit1 绿灯 bit2 蓝灯 bit3 蜂鸣器
    uint8_t flag_Alarm;         //报警标志位，当此位为 1 时 报警
    uint8_t Dev_ID[LENGTH_DEV_ID];       //由服务器分配的设备ID
    uint32_t PPM;               //读到的PPM的值
    Typdef_AlarmValue Alarm;    //报警值
    Typdef_Version   Version;   //设备版本号
    Typdef_Version   HWVersion; //设备硬件版本号
    Typdef_PpmOffset ppm_offset;				 //校准偏移量
    uint16_t  Offset_Minite;       //校准计时器
}Typdef_ProgramPara;


extern char ubUartRxBuf[RXMAX];//串口接收数组
extern uint8_t ubIndex;//串口数组的填充位置

extern volatile uint8_t flag_10ms;//10ms定时标志位  在Time2中断中置位 ，在main函数中清0
extern volatile uint8_t WebSocketOK ;//WebSocket标志位，与服务器成功连接后此位置位，当断开连接时 此位清0
extern volatile uint8_t Receive;//接收标志，与Command共同决定要发送或者接收到的值（以防Command被意外修改，如收到command的时候正好遇到心跳触发指令）
extern volatile uint16_t Softpnum;//接收的数据包序
extern volatile int16_t waitTime;//延时时间 ，主要用在config中，已经用的比较少了

extern Typdef_ProgramPara ProgramPara;
extern Typdef_DataFormat DataFormat;//发送数据包时所需的数据
extern uint8_t  Mode;//系统状态散转
extern uint8_t open_key[];//公钥
extern uint8_t primaryKey[];//私钥
//extern Typdef_ProgramPara ProgramPara;
void clearArr(uint8_t* originalArr);//清空接收数组
void DelayApi(uint16_t uwDelayCnt);//延时函数，以定时器2为延时基准，最少延时2ms
#endif

