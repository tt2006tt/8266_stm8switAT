/*----------------------------------------------------------------------------------------------------------------------------
修订时间:2017年4月25日
参数说明:DataFormat:存放通信协议相关的参数
         ProgramPara存放系统会用到的所有状态标志符、标志位 计数器,还存放了PPM和设备ID。
           
         Mode：主程序工作状态散转
           
         ubUartRxBuf[RXMAX]:串口接收数组
         Softpnum:记录固件复制的代码包序
         WebSocketOK 成功连接网络Websocket标志（应该可以优化省略）
         primaryKey;私钥
         open_key:公钥
								 
通信协议:--------------------------------------------------------------------------------------------------------------------
         标示位         目标ID              源ID         加密类型       包序      命令数据长度        命令      数据
          F               DSTID           SRCID           entry           idx     payloadlenth      command      payload               
       1byte            10byte           10byte           1byte          2byte       2byte            2byte         n byte
        ---------------------------------------------------------------------------------------------------------------------
        Boot区地址为        0x8000---0x87FF   总大小：2K
        APP区地址为     0x8800---0xC3FF   总大小：15K
        Backup区地址为      0xC400---0xFFFF   总大小：15K

**********************************************出厂自检*******************************************************
              正常：两秒后 蓝、红、绿、蜂鸣器 交替闪烁一遍同时测量AD的值看是否正常
              不正常：   蓝灯常亮： 传感器问题（也有肯能是电源的导致的）
                         红灯常亮： ESP8266没有成功通信（可能是波特率修改失败，也可能是完全无法通信）
************************************************************************************************************   


********************************************程序流程********************************************************
                1、设备连接wifi，绿灯常亮，  连接成功 ：进入加热模式
                                            连接失败 ：进入配置模式 
               2、配置模式 ：绿灯慢闪      连接成功： 进入加热模式
               3、加热模式 ：连接成功：绿灯常亮 
                             连接失败：绿灯灭
                             完成之后：蜂鸣器响一声，进入wifi连接模式
               4、wifi连接，不论成功失败都进入正常模式
               5、正常模式：读传感器的AD值，转化成PPM
                            与服务器通信
                注意：除了 自测和 开机自检外，其他的亮灯都在中断中负责

函数： void OFF_ALL(void) :关闭所有的灯以及蜂鸣器
       void Reflash(uint16_t command,uint8_t receive,uint16_t waittime): 重置 命令代码，接收标识符，等待时间
       void ESP8266_Monitor(Typdef_ProgramPara *programpara) :与服务器通信的部分

其中  command和receive同时控制与服务器通信的命令 
      waittime 做并行延时，延时是通过程序跑的次数实现的。
************************************************************************************************************

  2017_04_27：添加了Bootload分区    
  2017_04_28：修改了中断优先级，将UART2的中断优先级置于TIM2之前
  2017_05_02: 在 Typdef_ProgramPara 添加了ReootCnt ，在连续10次有发出心跳却没有收到ACK的情况下重启机制
  2017_05_03: 完成了自动更新的协议，但还未测试
  2017_05_09: 完成了IAP的编程，每次登陆后自动搜索有无新版固件，若有，则更新，否则正常运行。更新时Gas除了接收固件数据包外没有其他功能。
              更新时闪绿灯
  2017_05_11:对程序进行优化，节省了一些空间，添加了测试灯和蜂鸣器的协议，添加了设置上限值的协议。
             对PPM值做了微调，使起始值降低。对PPM降低.
             添加了按键中断警报功能。
  2017_05_12:进一步地优化了程序，对PPM值得设定再次做了微调（不将初值限制为0了，让其有一定的波动。）
  2017_05_15:增加了自测功能，将UDP广播由原来的8次广播改为2次广播
  2017_05_16:完善了自动升级的功能，在服务器端添加了 “正在升级” 这个选项。将自测分为按键触发和APP触发，同时发出状态值
  2017_05_18:继续优化了代码，将自测功能独立开来，让其可以在无网络和有网络的情况下都可以实现自测。优化了报警功能，令其连续稳定。
  2017_05_19:添加了自检功能，在任何情况下连续按三次按键就可以进入自检，自检完毕返回ModeInit模式。(该自检功能只能在开机时使用，其他情况使用都会导致重启)
  2017_05_23: 优化了在线升级功能，将Bootload中的程序作了如下修改：原本是直接复制备份区的程序到app区，修改后变成先擦除APP区的程序，
               再复制备份区的程序，最后再擦除备份区的程序
  2017_05_24: 改变了亮灯方案： 正常状态：绿色电源灯常亮                 
                               报警状态：环形红色灯闪烁，蜂鸣器随灯的闪烁鸣叫
                               自检状态：环形绿、红、蓝色灯闪烁蜂鸣器随灯的闪烁鸣叫
                               启动状态：环形绿色灯亮，鸣叫一声后完成启动
                               配置状态：环形绿色灯闪烁
                               故障状态：环形蓝色灯闪烁
  2017_05_31:修复了一些bug。
  2017_06_01:修改了出厂自检的指示灯亮灯条件：当出现波特率修改失败时 亮红灯，当出现传感器错误时亮 蓝灯
             将停止报警标志位延长到10分钟 同事添加了停止报警的APP指令
  2017_06_07:添加校准功能，刚烧写完程序后第一次开机会进行校准（需48小时进行），自检完成后也会进行校准行为
  2017_06_09:添加mode_connect功能，用来测试8266模块是否能正常工作(连接公司wifi)
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


char ubUartRxBuf[RXMAX] = {0};//用来储存串口数据
volatile uint16_t Softpnum = 0;
uint8_t Mode;             //系统状态散转

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
    InitAll();//将所有需要初始化的东西初始化
    
    Read_ChipID_Init(&ProgramPara); //读取保存在EEPROM中的设备ID
    Read_SetPPM(&ProgramPara);//读取设定好的PPM值
    ProgramPara.Cnt_ERROR = 3000;//错误灯（断开网络）30s后重启
    //Mode = Mode_Nomal;
    Mode = Mode_Init;
//------------------初始化完成--------------------     
    while(1){
        while(!flag_10ms);//正常情况下程序10ms循环一次
        flag_10ms = 0;
        mykey_Press(&ProgramPara);//按键扫描（长按1.5秒进入智能配置模式）
//Debug模式下为       20s     /1分钟    /1小时
//非Debug模式下为    2分钟    /1小时    /一天
        if(++ProgramPara.Cnt_Minite >= CNT_MINITE){

            ProgramPara.Cnt_Minite = 0;
            if((ProgramPara.Status & BIT(BIT_ALARMOFF))){
                Cnt_AlarmOFF++;
                if(Cnt_AlarmOFF > 5){//当强制停止报警后，十分钟后自动恢复
                    Cnt_AlarmOFF = 0;
                    ClrBit(ProgramPara.Status,BIT_ALARMOFF);
                }
            }else{
                Cnt_AlarmOFF = 0;
            }
           
            SetBit(ProgramPara.flag_Time,BIT_MINITE);

            if(++Cnt_Hour>= CNT_HOUR){
                Cnt_Hour = 0;
                ProgramPara.SendMEM = 0;//1小时后可以重发警报
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

                        
                        ProgramPara.ppm_offset.Num = ProgramPara.PPM; //48hour 后记录下offset
                        if(ProgramPara.ppm_offset.Num > 350){
                            ProgramPara.flag_revise = 2;//校零失败
                        }
                        else{
                            ProgramPara.flag_revise = 1;	//校准完成
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
                            LED_RED_Ctrl(ON);//校零失败提示
                          }
                        }
                    }
                }
            }
        }
        
    	 switch(Mode){    
              case Mode_Init:           ESP8266Init(&ProgramPara);//ESP8266初始化，完成网络连接操作
              break;	               
              case Mode_Config:         ESP8266_Smart(&ProgramPara);//智能配置，通过手机广播获取wifi账号密码
              break; 
              case Mode_Bias:           InitOK(&ProgramPara);//传感器加热完成,响铃 
              break;
              case Mode_CreateSession:  ESP8288_WebConnect(&ProgramPara);//打开WebSocket会话
              break;
              case Mode_Nomal:          Detect();//燃气检测、警报等
                                        if(ProgramPara.Status & BIT(BIT_SELFTEST)){//自测标志位，每次自测都要像服务器返回进度信息
                                            SelfTEST();
                                        }
#ifdef VERSION_NET                                            
                                         NetWorking();
#endif                                            
                                        break;
              case Mode_Test:          CheckSelf();//出厂自检
                                        break;
                                        
              case Mode_Connect:          Connect_Test_Wifi();//出厂自检
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
	if(waitTime){//如果还在等待时间内
            waitTime--;
            
            if(mystrstr(ubUartRxBuf,"OK")){
                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//开蜂鸣器
                DelayApi(250);
                
                GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//关蜂鸣器
                DelayApi(250);
                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//开蜂鸣器
                DelayApi(250);
                
                GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//关蜂鸣器
                DelayApi(250);
                GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//开蜂鸣器
                DelayApi(250);
                
                GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//关蜂鸣器
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
                DelayApi(500);//等待一秒令其稳定
                uartSendDataBuf(ChangeBaud_4800,mystrlen(ChangeBaud_4800));//修改波特率
                
                DelayApi(200);
                InitUart2(4800); 
                DelayApi(500);
                ESP8266_Reload(200);
                uartSendDataBuf(ChangeMode,mystrlen(ChangeMode));//改变工作模式
                ProgramPara.SubMode = 0x01;
      break;
    case 0x01:
                if(waitTime){//如果还在等待时间内
                    waitTime--;
                    if(mystrstr(ubUartRxBuf,"OK")){
                      
                        OFF_All();
                        
                        GPIO_WriteHigh(LED_BLUE_PORT,LED_BLUE_PIN);//蓝灯亮
                        DelayApi(250);
                        
                        GPIO_WriteLow(LED_BLUE_PORT,LED_BLUE_PIN);//关蓝灯
                        GPIO_WriteHigh(LED_RED_PORT, LED_RED_PIN);//红灯亮
                        DelayApi(250);
                        
                        GPIO_WriteLow(LED_RED_PORT, LED_RED_PIN);//关红灯
                        GPIO_WriteHigh(LED_GREEN_PORT, LED_GREEN_PIN);//绿灯亮
                        DelayApi(250);
                        
                        GPIO_WriteLow(LED_GREEN_PORT, LED_GREEN_PIN);//关绿灯
                        for(int i = 0 ; i < ADC_SAMPLE_CNT ; i++)
                        {
                            ProgramPara.Voltage = MQ4_Votage();
                        }
                        if(ProgramPara.Voltage < 20 || ProgramPara.Voltage > 300){//判断电压大小，如果太大或太小都报错重启
                        
                            ProgramPara.SubMode = 0xF1;
                            break;
                        }
                        GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);//开蜂鸣器
                        DelayApi(250);
                        
                        GPIO_WriteLow(BEEP_PORT, BEEP_PIN);//关蜂鸣器
                        Mode = Mode_Connect;//Mode_Init;
                        connect_cnt = ProgramPara.Cnt_Minite;
                        ProgramPara.SubMode = 0x00;
                    }
                }else{
                  ProgramPara.SubMode = 0xF0;
                }        
      break;
    case 0xF0://红灯
      LED_RED_Ctrl(ON);
      break;
    case 0xF1://蓝灯
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
     if(ProgramPara.flag_revise == 1)//已进行校准
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
     else if(ProgramPara.flag_revise == 0)//此处是用来获得offset
     {
         if(ProgramPara.Voltage <120)                                              ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage * 3));
         else if((ProgramPara.Voltage >=120) && (ProgramPara.Voltage < 200))      ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage - 100) * 45)  + 360;
         else if((ProgramPara.Voltage >=200) && (ProgramPara.Voltage < 250))      ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage - 200) * 35)  + 750;
         else if((ProgramPara.Voltage >=250) && (ProgramPara.Voltage < 280))      ProgramPara.PPM = ((uint32_t)(ProgramPara.Voltage - 250) * 10)  + 2500;
         else if((ProgramPara.Voltage >=280) && (ProgramPara.Voltage < 320))      ProgramPara.PPM = (uint32_t)((ProgramPara.Voltage - 280) * 18)  + 2800;
         else if((ProgramPara.Voltage >=320) && (ProgramPara.Voltage < 330))      ProgramPara.PPM = (uint32_t)((ProgramPara.Voltage - 320) * 280) + 3200;

     }
      //在非自测状态下
     if(((ProgramPara.Status & BIT(BIT_SELFTEST)) == 0)){
       //      没有在手动测试灯和蜂鸣器   &&  已经完成初始化 && 当前的PPM值比设定值高
         if(((ProgramPara.Status & BIT(BIT_ALARMOFF)) == 0) && ((ProgramPara.Status) & BIT(BIT_HASBIAS)) && ((ProgramPara.PPM) > ProgramPara.Alarm.Num)){
           //保持超标50次（即 0.5s）则判定为燃气超标
           if(cnt_Alarm < 50){
             cnt_Alarm ++;
           }
           if(cnt_Alarm >= 50){
             ProgramPara.flag_Alarm = 1;
           } 
        }else{   //非自测，没有报警 || PPM降到警戒值以下 && 没有在手动测试红灯和蜂鸣器 &&没有在重启
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
          //如果没发送过报警信息且不处于更新状态
            if(ProgramPara.SendMEM == 0 && (!ProgramPara.Updating)){
                DataFormat.command = COMMAND_WARNING;
            }
        }
     }
}

void NetWorking(void){
    if(WebSocketOK){//如果已经开启Websocket会话
      if(mystrstr(ubUartRxBuf,"HTTP/1.1 400") || mystrstr(ubUartRxBuf,"web application is stopping")){//如果连接意外中断
        Mode = Mode_Init;
        WebSocketOK = 0;
        ProgramPara.Status &= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
        ProgramPara.Status |= ((1<<BIT_HASBIAS) | (1<<BIT_HASNET));
        ProgramPara.SubMode = 0x00;
        return;
      }
     //--------------------------------------------------------------
        ESP8266_Monitor(&ProgramPara);//处理ESP8266接收到的消息
     //--------------------------------------------------------------
     
        if((!ProgramPara.Updating) && ((ProgramPara.Status & BIT(BIT_SELFTEST)) == 0)){//如果没有处于更新或自测状态
            if((ProgramPara.Status & BIT(BIT_LOGINOK))){//登录成功if(programpara->Flag_LogInOK)
            
                if((ProgramPara.flag_Time & BIT(BIT_MINITE))){
                //  每两分钟发一次心跳（Debug中只用20s）
                        ClrBit(ProgramPara.flag_Time,BIT_MINITE);
                        DataFormat.command = COMMAND_HEARTRATE;
                }
                if((ProgramPara.flag_Time & BIT(BIT_HOUR))){
                //每小时上传一次PPM（Debug中只用60s）
                       ClrBit(ProgramPara.flag_Time,BIT_HOUR);
                       DataFormat.command = COMMAND_UPDATEPPM;
                }
                if((ProgramPara.flag_Time & BIT(BIT_DAY))){
                //两天检查一次固件(Debug中只用60分钟)
                       ClrBit(ProgramPara.flag_Time,BIT_DAY);
                       DataFormat.command = COMMAND_UPDATEFIR ;
                }
            }else{
                //登录不成功
                if(ProgramPara.Cnt_ERROR >0){
                    ProgramPara.Cnt_ERROR --;
                }
                else{
                    ProgramPara.Cnt_ERROR = 3000;
                    Mode = Mode_Init;
                }
            }
        }
    }else{//没有成功连上网络
        if((ProgramPara.Status & BIT(BIT_HASNET))){//如果曾经连上过网络则在蓝灯闪烁后重连
            if(ProgramPara.Cnt_ERROR >0){
                ProgramPara.Cnt_ERROR --;
                SetBit(ProgramPara.Status,BIT_RECONNECT);//重连标志位
            }else{
                Mode = Mode_Init;//重连
                ProgramPara.Cnt_ERROR = 3000;
                LED_BLUE_Ctrl(OFF);
                ClrBit(ProgramPara.Status,BIT_RECONNECT);//清重连标志位
            }
        }else{//从未连接上网络
        ClrBit(ProgramPara.Status,BIT_RECONNECT);//清重连标志位
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
        CLK_HSECmd(ENABLE);//使能外部时钟
        while(SET != CLK_GetFlagStatus(CLK_FLAG_HSERDY));//等待时钟切换完成
        CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//CPU时钟不分频
        CLK_ClockSwitchCmd(ENABLE); 
        CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO , CLK_SOURCE_HSE , DISABLE , CLK_CURRENTCLOCKSTATE_ENABLE);
#endif        
#ifdef HSI_CLOCK
        CLK_DeInit();
        CLK_HSECmd(DISABLE);//失能外部时钟
        CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);//内部时钟二分频
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

  //外部晶振
    //1ms = 16/16000000 *1000
  /* Set the Prescaler value */
#ifdef OSC_16MHZ
    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_16);
#endif
#ifdef OSC_8MHZ
    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_8);
#endif    
    /* Set the Autoreload value */
    TIM2->ARRH = (uint8_t)(2000 >> 8);//2ms进入一次中断
    TIM2->ARRL = (uint8_t)(2000);
#endif  
#ifdef HSI_CLOCK
  //内部晶振
//    /* Set the Prescaler value */
    TIM2->PSCR = (uint8_t)(TIM2_PRESCALER_8);
    /* Set the Autoreload value */
    TIM2->ARRH = (uint8_t)(2000 >> 8);//2ms进入一次中断
    TIM2->ARRL = (uint8_t)(2000);
 #endif    

    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);  


}
void InitAll(void){
    InitCpuClock();//该程序中配置为外部时钟 不分频
    
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//发送数据IO  
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);//接受数据IO  
    InitUart2(4800);
    
    InitTimer2();//用于系统总定时，1ms进一次中断
    ITC->ISPR4 &= 0xF3;//将定时器2的优先级调低，低于UART中断
    enableInterrupts();//开总中断

    ADC_Init(Channel_12);//ADC初始化

    ESP8266_GPIO_Init();
    Led_Init();//LED初始化
    BeepInit();//蜂鸣器初始化

    EEPROM_INIT();//FLASH初始化
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




