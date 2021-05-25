#include "stm32f10x.h"
#include "user_can.h"
#include "delay.h"
#include "stm32f10x_can.h"
/**************************************************************************************
 * ��  �� : ��ʼ��CAN1��GPIO��
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void CAN_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  //��CAN1����GPIO��
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
  
  //���õ�IO��PA12��CAN��TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //���õ�IO��PA11��CAN��RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**************************************************************************************
 * ��  �� : ��ʼ��CAN1������CAN1�ж����ȼ�
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void CAN1_Init(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the CAN1 Clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1 , ENABLE);  
	
  CAN_DeInit(CAN1);                                              //������CAN�����мĴ�������Ϊȱʡֵ
  CAN_StructInit(&CAN_InitStructure);

  CAN_InitStructure.CAN_TTCM=DISABLE;                           //��ֹʱ�䴥��ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM=DISABLE;                           //�����CAN_MCR�Ĵ�����INRQλ������1�����0��һ��Ӳ����⵽
	                                                              //128��11λ����������λ�����˳�����״̬
  CAN_InitStructure.CAN_AWUM=DISABLE;                           //˯��ģʽͨ�����CAN_MCR�Ĵ�����SLEEPλ�����������
  CAN_InitStructure.CAN_NART=ENABLE;                            //CAN����ֻ������һ�Σ����ܷ��͵Ľ�����(�ɹ���������ٲö�ʧ)
  CAN_InitStructure.CAN_RFLM=DISABLE;                           //�ڽ������ʱFIFOδ��������������FIFO�ı���δ��������
	                                                              //��һ���յ��ı��ĻḲ��ԭ���ı���
  CAN_InitStructure.CAN_TXFP=DISABLE;                           //����FIFO���ȼ��ɱ��ĵı�ʶ��������
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;                   //CANӲ������������ģʽ 
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;                        //����ͬ����Ծ���Ϊ1��ʱ�䵥λ,��Χ[CAN_SJW_1tq,CAN_SJW_4tq]
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;                        //ʱ���1Ϊ2��ʱ�䵥λ,��Χ[CAN_BS1_1tq,CAN_BS1_16tq]
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;                        //ʱ���2Ϊ3��ʱ�䵥λ,��Χ[CAN_BS2_1tq,CAN_BS2_8tq]
  CAN_InitStructure.CAN_Prescaler=12;                           //�趨��һ��ʱ�䵥λ�ĳ���Ϊ12,��Χ[1,1024]
  CAN_Init(CAN1,&CAN_InitStructure); 

	// CAN filter init CAN��������ʼ��     
  CAN_FilterInitStructure.CAN_FilterNumber=0;                      //ָ���˴���ʼ���Ĺ�����,��Χ[0,13]
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;    //ָ���˹�����������ʼ������ģʽΪ��ʶ������λģʽ
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   //�����˹�����λ��1��32λ������
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                 //�����趨��������ʶ��(32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��)
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                  //�����趨��������ʶ��(32λλ��ʱΪ��߶�λ��16λλ��ʱΪ�ڶ���)
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;             //�����趨���������α�ʶ��(32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��)
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;              //�����趨���������α�ʶ��(32λλ��ʱΪ��߶�λ��16λλ��ʱΪ�ڶ���)
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;      //�趨��ָ���������FIFO0  
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;             //ʹ�ܹ�����   
  CAN_FilterInit(&CAN_FilterInitStructure);                

  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);                          //ʹ��CAN1�ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                  //NVIC_Group:��ռ���ȼ�2λ�������ȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 	   //����ΪCAN1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;          //��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		           //�����ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 
/**************************************************************************************
 * ��  �� : ͨ��CAN���߷��ͽ��յ�������
 * ��  �� : CanRxMsg
 * ����ֵ : ��
 **************************************************************************************/
void ReponseCMD(CanRxMsg* RxMessage)
{	
	CanTxMsg TxMessage1;
		
	TxMessage1.StdId=RxMessage->StdId;       //�����趨��׼��ʶ��,��Χ[0,0x7FF] 
  TxMessage1.ExtId=RxMessage->ExtId;       //�����趨��չ��ʶ��,��Χ[0,0x3FFFF]
  TxMessage1.IDE=RxMessage->IDE;           //�����趨��Ϣ��ʶ��������,CAN_ID_STD/CAN_ID_EXT
  TxMessage1.RTR=RxMessage->RTR;           //�����趨��������Ϣ��֡����(CAN_RTR_DATA����֡/CAN_RTR_REMOTEԶ��֡)
  TxMessage1.DLC=RxMessage->DLC;           //�����趨��������Ϣ��֡����,��Χ[0,8] 		
  TxMessage1.Data[0]=RxMessage->Data[0];   //����������Data[0]    
  TxMessage1.Data[1]=RxMessage->Data[1];   //����������Data[1]  
	TxMessage1.Data[2]=RxMessage->Data[2];   //����������Data[2]  
	TxMessage1.Data[3]=RxMessage->Data[3];   //����������Data[3]  
	TxMessage1.Data[4]=RxMessage->Data[4];   //����������Data[4]  
	TxMessage1.Data[5]=RxMessage->Data[5];   //����������Data[5]  
	TxMessage1.Data[6]=RxMessage->Data[6];   //����������Data[6]  
	TxMessage1.Data[7]=RxMessage->Data[7];   //����������Data[7]  
	
  CAN_Transmit(CAN1,&TxMessage1);	
}



