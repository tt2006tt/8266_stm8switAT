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
�������ܣ�ADC��ͨ��ת����ʼ��
���������Channel��Channel_0����Channel_15
���������AD��ֵ
��    ע��ʹ�øú���ǰ�����޸���ADC.h�е�IO�ں궨��
*********************************************/
void ADC_Init(u8 Channel){  
    ADC_IOConfig();

    ADC1->CSR |= Channel;//ѡ��ͨ��
    ADC1->CR1 &=~BIT(1);//����ת��ģʽ
    
    ADC1->CR2 |= BIT(3);//�����Ҷ���
    
    ADC1->CR1 |= BIT(0);//��AD����
}

/*********************************************
�������ܣ���ȡADCת������
�����������
���������AD��ֵ
��    ע����
*********************************************/
u16 ADC_Data(void){
    u16 data=0;
    
    ADC1->CR1 |= BIT(0);//ADCʹ��
    
    while((ADC1->CSR&0X80)==0);//�ȴ�ת������
    ADC1->CSR &=~BIT(7);//���ת��������־λ

    data =  ADC1->DRL;
    data |= ((ADC1->DRH << 8) & 0x0300);
 
    return data;
}

/*********************************************
�������ܣ�ADC IO��ʼ�� 
�����������
�����������
��    ע��ͨ���޸ĺ궨��ı���Ҫ��ʼ����IO
*********************************************/
void ADC_IOConfig(void){
    ADC_GPIO->DDR &=~ADC_PIN;//����ģʽ 
    ADC_GPIO->CR1 &=~ADC_PIN;//��������ģʽ    
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
	return (adc_total*33/102);//Ĩ����β�� 4 ��Ϊ�˽���β������ֹ�������
#endif /* VDD_3_3V */

#ifdef VDD_5_V
	return (adc_total*500/1024/ADC_SAMPLE_CNT);
#endif /* VDD_5_V */
}



void InitOK(Typdef_ProgramPara *programpara){
    if((programpara->Status&BIT(BIT_HASBIAS)) == 0){//�����δУ��
        if(programpara->flag_Time & BIT(BIT_MINITE)){//�������ʱ���ѳ���2���� Flag_2_M
            GPIO_WriteHigh(BEEP_PORT, BEEP_PIN);
            DelayApi(300);//�ȴ�600ms
            GPIO_WriteLow(BEEP_PORT, BEEP_PIN);
            
            Mode = Mode_CreateSession; //*MainMode				
            programpara->SubMode = 0;//*Status
            SetBit(programpara->Status,BIT_HASBIAS);
//            LED_BLUE_Ctrl(OFF);//�ر�����
            OFF_All();
        }
    }else{//����Ѿ�У����ֻ���ڰ�������Ż�������״̬��
            Mode = Mode_CreateSession;//���ӷ�����*MainMode
//            LED_BLUE_Ctrl(OFF);//�ر�����
//            LED_GREEN_Ctrl(OFF);
    }
}

