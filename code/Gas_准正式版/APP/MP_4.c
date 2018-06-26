#include "MP_4.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_adc1.h"
#include "Text.h"
#include "beep.h"
#include "led.h"
volatile u8 g_adc_index = 0;
 

#ifdef STM8S105

/*********************************************
函数功能：ADC单通道转换初始化
输入参数：Channel：Channel_0——Channel_15
输出参数：AD数值
备    注：使用该函数前，需修改在ADC.h中的IO口宏定义
*********************************************/
void ADC_Init(u8 Channel){  
    ADC_IOConfig();

    ADC1->CSR |= Channel;//选择通道
    ADC1->CR1 &=~BIT(1);//单次转换模式
    
    ADC1->CR2 |= BIT(3);//数据右对齐
    
    ADC1->CR1 |= BIT(0);//把AD唤醒
}

/*********************************************
函数功能：获取ADC转换数据
输入参数：无
输出参数：AD数值
备    注：无
*********************************************/
u16 ADC_Data(void){
    u16 data=0;
    
    ADC1->CR1 |= BIT(0);//ADC使能
    
    while((ADC1->CSR&0X80)==0);//等待转换结束
    ADC1->CSR &=~BIT(7);//清除转换结束标志位

    data =  ADC1->DRL;
    data |= ((ADC1->DRH << 8) & 0x0300);
 
    return data;
}

/*********************************************
函数功能：ADC IO初始化 
输入参数：无
输出参数：无
备    注：通过修改宏定义改变所要初始化的IO
*********************************************/
void ADC_IOConfig(void){
    ADC_GPIO->DDR &=~ADC_PIN;//输入模式 
    ADC_GPIO->CR1 &=~ADC_PIN;//浮空输入模式    
}

#endif



uint16_t MQ4_Votage(void){
    static uint8_t adc_index = 0;
    static uint16_t Current_ADC_Value[ADC_SAMPLE_CNT] ={0};
    uint16_t adc_total = 0;
	adc_index = 0;
 
    for(uint8_t i = 0;i < ADC_SAMPLE_CNT;i++){
		Current_ADC_Value[(adc_index++)] = ADC_Data();
        adc_total += Current_ADC_Value[i];
    }
    adc_total /= ADC_SAMPLE_CNT;
#ifdef VDD_3_3V
	return (adc_total*33/102);//抹掉了尾数 4 是为了降低尾数，防止数据溢出
#endif /* VDD_3_3V */

#ifdef VDD_5_V
	return (adc_total*500/1024/ADC_SAMPLE_CNT);
#endif /* VDD_5_V */
}



void InitOK(Typdef_ProgramPara *programpara){
    if((programpara->Status&BIT(BIT_HASBIAS)) == 0){//如果尚未校正
        if(programpara->flag_Time & BIT(BIT_MINITE)){//如果运行时间已超过2分钟 Flag_2_M
            GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);
            DelayApi(300);//等待600ms
            GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
            
            Mode = Mode_CreateSession; //*MainMode				
            programpara->SubMode = 0;//*Status
            SetBit(programpara->Status,BIT_HASBIAS);
//            LED_BLUE_Ctrl(OFF);//关闭蓝灯
            OFF_All();
        }
    }else{//如果已经校正（只有在按按键后才会进入这个状态）
            Mode = Mode_CreateSession;//连接服务器*MainMode
//            LED_BLUE_Ctrl(OFF);//关闭蓝灯
//            LED_GREEN_Ctrl(OFF);
    }
}

