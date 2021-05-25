#include "stm32f10x.h"
#include "led.h"
#include "user_exti.h"
#include "delay.h"

/*******************************************************************************
 * 描  述 : 初始化PE5为外部中断5
 * 参  数 : 无
 * 返回值 : 无
 ******************************************************************************/
void EXTI_PE5_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the EXTI_PE5 Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOE , ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                 //上拉输入   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);   //配置PE5管脚为外部中断线路用
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;                    //配置为外部中断线5
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //配置为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	                    //使能中断
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //NVIC_Group:先占优先级2位，从优先级2位  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;            //配置为外部中断5中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //先占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //从优先级为2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断通道
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                 //上拉输入   
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);   //配置PE2管脚为外部中断线路用
	  
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;                    //配置为外部中断线2
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //配置为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	                    //使能中断
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //NVIC_Group:先占优先级2位，从优先级2位  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //配置为外部中断2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //先占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //从优先级为2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断通道
  NVIC_Init(&NVIC_InitStructure); 
}

/**************************************************************************************
 * 描  述 : 外部中断服务程序
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/



