#include "timer.h"
#include "sim800c.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

extern vu16 USART3_RX_STA;

//��ʱ��7����Ԥװ������ֵ
void TIM7_SetARR(u16 period)
{
	  TIM_SetCounter(TIM7,0);//���������
	  TIM7->ARR&=0x00;       //����Ԥװ������ֵΪ0
	  TIM7->ARR|= period;    //����Ԥװ������ֵ 
}


//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	static u8 i=0;
	
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //�Ǹ����ж�
	{	 			   
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		if(Scan_Wtime!=0)//����ɨ��ģʽ
		{
			i++;
			if(i==Scan_Wtime)
			{
				i=0;
				Scan_Wtime = 0;
				USART3_RX_STA|=1<<15;	 //��ǽ������
				TIM_Cmd(TIM7, DISABLE);  //�رն�ʱ��7 
				TIM7_SetARR(100-1);      //��������Ϊ10ms�ж�
			}
		}
		else//������ɨ��ģʽ
        {
             USART3_RX_STA|=1<<15;	     //��ǽ������
			 TIM_Cmd(TIM7, DISABLE);     //�ر�TIM7 
			  
		}
	}	    
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;                    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);            //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );                  //ʹ��ָ����TIM7�ж�,���������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;   //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		   //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}

extern vu16 USART4_RX_STA;

////��ʱ��7����Ԥװ������ֵ
void TIM2_SetARR(u16 period)
{
	  TIM_SetCounter(TIM2,0);//���������
	  TIM2->ARR&=0x00;       //����Ԥװ������ֵΪ0
	  TIM2->ARR|= period;    //����Ԥװ������ֵ 
}
//��ʱ��7�жϷ������		    
void TIM2_IRQHandler(void)
{ 	
	static u8 i=0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //�Ǹ����ж�
	{	 			   
		USART4_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM2, DISABLE);  //�ر�TIM7 
	}	    
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM2_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,���������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}
	 