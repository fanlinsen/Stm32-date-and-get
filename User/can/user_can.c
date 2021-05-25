#include "stm32f10x.h"
#include "user_can.h"
#include "delay.h"
#include "stm32f10x_can.h"
/**************************************************************************************
 * 描  述 : 初始化CAN1用GPIO口
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void CAN_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  //打开CAN1所用GPIO口
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
  
  //配置的IO是PA12，CAN的TXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  //配置的IO是PA11，CAN的RXD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**************************************************************************************
 * 描  述 : 初始化CAN1并配置CAN1中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void CAN1_Init(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the CAN1 Clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1 , ENABLE);  
	
  CAN_DeInit(CAN1);                                              //将外设CAN的所有寄存器重设为缺省值
  CAN_StructInit(&CAN_InitStructure);

  CAN_InitStructure.CAN_TTCM=DISABLE;                           //禁止时间触发通信模式
  CAN_InitStructure.CAN_ABOM=DISABLE;                           //软件对CAN_MCR寄存器的INRQ位进行置1随后清0后，一旦硬件检测到
	                                                              //128次11位连续的隐性位，就退出离线状态
  CAN_InitStructure.CAN_AWUM=DISABLE;                           //睡眠模式通过清除CAN_MCR寄存器的SLEEP位，由软件唤醒
  CAN_InitStructure.CAN_NART=ENABLE;                            //CAN报文只被发送一次，不管发送的结果如何(成功、出错或仲裁丢失)
  CAN_InitStructure.CAN_RFLM=DISABLE;                           //在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，
	                                                              //下一个收到的报文会覆盖原来的报文
  CAN_InitStructure.CAN_TXFP=DISABLE;                           //发送FIFO优先级由报文的标识符来决定
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;                   //CAN硬件工作在正常模式 
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;                        //重新同步跳跃宽度为1个时间单位,范围[CAN_SJW_1tq,CAN_SJW_4tq]
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;                        //时间段1为2个时间单位,范围[CAN_BS1_1tq,CAN_BS1_16tq]
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;                        //时间段2为3个时间单位,范围[CAN_BS2_1tq,CAN_BS2_8tq]
  CAN_InitStructure.CAN_Prescaler=12;                           //设定了一个时间单位的长度为12,范围[1,1024]
  CAN_Init(CAN1,&CAN_InitStructure); 

	// CAN filter init CAN过滤器初始化     
  CAN_FilterInitStructure.CAN_FilterNumber=0;                      //指定了待初始化的过滤器,范围[0,13]
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;    //指定了过滤器将被初始化到的模式为标识符屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   //给出了过滤器位宽1个32位过滤器
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;                 //用来设定过滤器标识符(32位位宽时为其高段位，16位位宽时为第一个)
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                  //用来设定过滤器标识符(32位位宽时为其高段位，16位位宽时为第二个)
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;             //用来设定过滤器屏蔽标识符(32位位宽时为其高段位，16位位宽时为第一个)
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;              //用来设定过滤器屏蔽标识符(32位位宽时为其高段位，16位位宽时为第二个)
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;      //设定了指向过滤器的FIFO0  
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;             //使能过滤器   
  CAN_FilterInit(&CAN_FilterInitStructure);                

  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);                          //使能CAN1中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                  //NVIC_Group:先占优先级2位，从优先级2位
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 	   //配置为CAN1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;          //先占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		           //从优先级为3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 
/**************************************************************************************
 * 描  述 : 通过CAN总线发送接收到的内容
 * 入  参 : CanRxMsg
 * 返回值 : 无
 **************************************************************************************/
void ReponseCMD(CanRxMsg* RxMessage)
{	
	CanTxMsg TxMessage1;
		
	TxMessage1.StdId=RxMessage->StdId;       //用来设定标准标识符,范围[0,0x7FF] 
  TxMessage1.ExtId=RxMessage->ExtId;       //用来设定扩展标识符,范围[0,0x3FFFF]
  TxMessage1.IDE=RxMessage->IDE;           //用来设定消息标识符的类型,CAN_ID_STD/CAN_ID_EXT
  TxMessage1.RTR=RxMessage->RTR;           //用来设定待传输消息的帧类型(CAN_RTR_DATA数据帧/CAN_RTR_REMOTE远程帧)
  TxMessage1.DLC=RxMessage->DLC;           //用来设定待传输消息的帧长度,范围[0,8] 		
  TxMessage1.Data[0]=RxMessage->Data[0];   //带传输数据Data[0]    
  TxMessage1.Data[1]=RxMessage->Data[1];   //带传输数据Data[1]  
	TxMessage1.Data[2]=RxMessage->Data[2];   //带传输数据Data[2]  
	TxMessage1.Data[3]=RxMessage->Data[3];   //带传输数据Data[3]  
	TxMessage1.Data[4]=RxMessage->Data[4];   //带传输数据Data[4]  
	TxMessage1.Data[5]=RxMessage->Data[5];   //带传输数据Data[5]  
	TxMessage1.Data[6]=RxMessage->Data[6];   //带传输数据Data[6]  
	TxMessage1.Data[7]=RxMessage->Data[7];   //带传输数据Data[7]  
	
  CAN_Transmit(CAN1,&TxMessage1);	
}



