#include "stm32f10x.h"
#include "led.h"
#include "user_exti.h"
#include "delay.h"

/*******************************************************************************
 * ��  �� : ��ʼ��PE5Ϊ�ⲿ�ж�5
 * ��  �� : ��
 * ����ֵ : ��
 ******************************************************************************/
void EXTI_PE5_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the EXTI_PE5 Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE , ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                 //��������   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);   //����PE5�ܽ�Ϊ�ⲿ�ж���·��
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;                    //����Ϊ�ⲿ�ж���5
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //����Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	                    //ʹ���ж�
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //NVIC_Group:��ռ���ȼ�2λ�������ȼ�2λ  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;            //����Ϊ�ⲿ�ж�5�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //��ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //�����ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 
}
void EXTI_PE2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the EXTI_PE2 Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE , ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                 //��������   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);   //����PE2�ܽ�Ϊ�ⲿ�ж���·��
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;                    //����Ϊ�ⲿ�ж���2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //����Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	                    //ʹ���ж�
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //NVIC_Group:��ռ���ȼ�2λ�������ȼ�2λ  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //����Ϊ�ⲿ�ж�2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //��ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //�����ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 
}

/**************************************************************************************
 * ��  �� : �ⲿ�жϷ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/



