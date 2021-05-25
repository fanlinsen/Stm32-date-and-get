/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SD���ļ�ϵͳ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103-�Ե� ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./usart/bsp_usart.h"	
#include "./led/bsp_led.h"
#include "ff.h"
#include "user_can.h"
#include "delay.h"
#include "led.h"
#include "user_exti.h"
#include "string.h"
#include "huange.h"
#include "user_systick.h"
#include <time.h>
#include "user_rtc.h"

/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
int a=0,b=0,c=0,jishi=0,weishu2,ll,xvhaoint=1,duowenjian=0,syssec,sysmin, filename_i=0,f=0,n=0;
unsigned char hex[2]={0};
int i=1,u,p=0,san=3;
unsigned char q,zhen[10]="";
uint32_t timecount;
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew,fnew1,fnew2,fnew3,fnew4,fnew5;													/* �ļ����� */
FRESULT res_sd,res_sd1,res_sd2,res_sd3,res_sd4,res_sd5;                   /* �ļ�������� */
UINT fnum,fnum1,fnum2,fnum3,fnum4,fnum5;            	          /* �ļ��ɹ���д���� */
BYTE ReadBuffer[50]={0};        /* �������� */
CanRxMsg RxMessage_Tem;
BYTE        WriteBuffer[23] = " ",huanhang[]=" \n",kongge[]=" ",ms[10]=" ",cc[]=",",xvhao[10]=" ",len[1]="",
            tishi1[]="CAN����",tishi2[]="ʱ��/ms",tishi3[]="���",tishi4[]="֡ID",
	          tishi5[]="֡��ʽ",zhenstr[]="��׼֡",zhenk[]="��չ֡",tishi6[]="֡�ĳ���",tishi7[]="֡����",
						yuan[]="Զ��֡",shujv[]="����֡";
char File_name[50]="";	
char save_filename[100][12]={0};		//���������ļ���

//����can�ṹ��
struct canmsg
{
  int bianhao;              //���
	unsigned int zhenid;	    //֡id��
	unsigned char canlen;	    //֡����
	unsigned int zhenstr1;    //��׼����չ
	unsigned char data[8];      //can����
	int shijian;              //����
	unsigned char yuanorshu;  //Զ��֡������֡
};
CanRxMsg msg1[500];
struct canmsg msg[6];
unsigned int front = 0;
unsigned int end = 0;

void InsertQueue(CanRxMsg *aa, CanRxMsg RxMessage_Tem)   //���
{   aa[end] = RxMessage_Tem;
		msg1[end].bianhao=xvhaoint;
	  msg1[end].shijian=jishi;
	  xvhaoint++;
	  if(xvhaoint>18000) {xvhaoint=1;}
    /*memcpy(aa[end].data,RxMessage_Tem.Data,sizeof(RxMessage_Tem.Data));
	  aa[end].bianhao = xvhaoint;		         //���д��ṹ��
		aa[end].canlen  = RxMessage_Tem.DLC;   //can����д��ṹ��
		aa[end].shijian = jishi;							 //ʱ��д��ṹ��
		aa[end].zhenstr1= RxMessage_Tem.IDE;	 //֡id��׼֡����չ֡�ж�
		if (RxMessage_Tem.IDE == CAN_Id_Standard) 	{aa[end].zhenid = RxMessage_Tem.StdId;} //֡idд��
		else {aa[end].zhenid = RxMessage_Tem.ExtId;}
		aa[end].yuanorshu  = RxMessage_Tem.RTR;//Զ��֡������֡�ж�*/
	  end=(end+1)%500;
		
}
void DeleteQueue(CanRxMsg *aa)         //����
{
  
	   memset(&aa[front], 0, sizeof(struct canmsg));
     front=(front+1)%500;	
	
}

void biaoti(void)
{
    res_sd=f_write(&fnew,tishi3,sizeof(tishi3),&fnum);	   //���
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi4,sizeof(tishi4),&fnum);	   //֡ID
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi5,sizeof(tishi5),&fnum);	   //֡��ʽ
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi6,sizeof(tishi6),&fnum);	   //֡����
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi7,sizeof(tishi7),&fnum);	   //֡����
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi1,sizeof(tishi1),&fnum);     //����
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi2,sizeof(tishi2),&fnum);     //ʱ��
		res_sd=f_write(&fnew,huanhang,sizeof(huanhang),&fnum);
		
}
void biaoti1(void)
{
    res_sd1=f_write(&fnew1,tishi3,sizeof(tishi3),&fnum1);	   //���
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi4,sizeof(tishi4),&fnum1);	   //֡ID
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi5,sizeof(tishi5),&fnum1);	   //֡��ʽ
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi6,sizeof(tishi6),&fnum1);	   //֡����
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi7,sizeof(tishi7),&fnum1);	   //֡����
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi1,sizeof(tishi1),&fnum1);     //����
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi2,sizeof(tishi2),&fnum1);     //ʱ��
		res_sd1=f_write(&fnew1,huanhang,sizeof(huanhang),&fnum1);
		
}
void biaoti2(void)
{
    res_sd2=f_write(&fnew2,tishi3,sizeof(tishi3),&fnum2);	   //���
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi4,sizeof(tishi4),&fnum2);	   //֡ID
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi5,sizeof(tishi5),&fnum2);	   //֡��ʽ
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi6,sizeof(tishi6),&fnum2);	   //֡����
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi7,sizeof(tishi7),&fnum2);	   //֡����
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi1,sizeof(tishi1),&fnum2);     //����
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi2,sizeof(tishi2),&fnum2);     //ʱ��
		res_sd2=f_write(&fnew2,huanhang,sizeof(huanhang),&fnum2);
		
}
void biaoti3(void)
{
    res_sd3=f_write(&fnew3,tishi3,sizeof(tishi3),&fnum3);	   //���
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi4,sizeof(tishi4),&fnum3);	   //֡ID
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi5,sizeof(tishi5),&fnum3);	   //֡��ʽ
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi6,sizeof(tishi6),&fnum3);	   //֡����
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi7,sizeof(tishi7),&fnum3);	   //֡����
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi1,sizeof(tishi1),&fnum3);     //����
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi2,sizeof(tishi2),&fnum3);     //ʱ��
		res_sd3=f_write(&fnew3,huanhang,sizeof(huanhang),&fnum3);
		
}
void biaoti4(void)
{
    res_sd4=f_write(&fnew4,tishi3,sizeof(tishi3),&fnum4);	   //���
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi4,sizeof(tishi4),&fnum4);	   //֡ID
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi5,sizeof(tishi5),&fnum4);	   //֡��ʽ
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi6,sizeof(tishi6),&fnum4);	   //֡����
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi7,sizeof(tishi7),&fnum4);	   //֡����
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi1,sizeof(tishi1),&fnum4);     //����
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi2,sizeof(tishi2),&fnum4);     //ʱ��
		res_sd4=f_write(&fnew4,huanhang,sizeof(huanhang),&fnum4);
		
}
void biaoti5(void)
{
    res_sd5=f_write(&fnew5,tishi3,sizeof(tishi3),&fnum5);	   //���
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi4,sizeof(tishi4),&fnum5);	   //֡ID
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi5,sizeof(tishi5),&fnum5);	   //֡��ʽ
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi6,sizeof(tishi6),&fnum5);	   //֡����
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi7,sizeof(tishi7),&fnum5);	   //֡����
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi1,sizeof(tishi1),&fnum5);     //����
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi2,sizeof(tishi2),&fnum5);     //ʱ��
		res_sd5=f_write(&fnew5,huanhang,sizeof(huanhang),&fnum5);
		
}
BYTE Dchar(int x,BYTE *zz)                  //���ڽ�ʮ����תΪasc��     
{ int x1,n=0,i;
	x1=x;
  do{
  n++;
  x1/=10;
  }while(x1);
  for(i=n-1;i>=0;i--)
	{
    zz[i]=x%10+48;
    x=x/10;  	
	}
	return *zz;
}
void SysTick_Handler(void)                 //��ʱ��
{ 
	jishi=(jishi+1)%100;

}
void zhenhex(unsigned int i)
{int zhenint;
zhen[0] = 0x30;
zhen[1]	= 0x58;
zhen[2] =( i & 0xf0000000 )>>28;
zhen[3] =( i & 0x0f000000 )>>24;
zhen[4] =( i & 0x00f00000 )>>20;
zhen[5] =( i & 0x000f0000 )>>16;
zhen[6] =( i & 0x0000f000 )>>12;
zhen[7] =( i & 0x00000f00 )>>8;
zhen[8] =( i & 0x000000f0 )>>4;
zhen[9]=i & 0xf;
for(zhenint=2;zhenint<10;zhenint++)
	{
    if((zhen[zhenint]>=0x0)&&(zhen[zhenint]<=0x9)){zhen[zhenint]+=0x30;}
		if((zhen[zhenint]>=0xa)&&(zhen[zhenint]<=0xf)){zhen[zhenint]+=0x37;}	
	}

}
unsigned char CharToHex(unsigned char bHex,unsigned char *hex)//16����ת�ַ���
{
	hex[0] =( bHex & 0xf0 )>>4;
  hex[1]=bHex & 0xf;
	if((hex[0]>=0x00)&&(hex[0]<=0x09))
	{
	  if((hex[1]>=0x00)&&(hex[1]<=0x09))
		{hex[0] +=0x30; hex[1] +=0x30;}
		else if((hex[1]>=0x0a)&&(hex[1]<=0x0f))
		{hex[0] +=0x30; hex[1] +=0x37;}
	}
	if((hex[0]>=0x0a)&&(hex[0]<=0x0f))
	{
	  if((hex[1]>=0x00)&&(hex[1]<=0x09))
		{hex[0] +=0x37; hex[1] +=0x30;}
		else if((hex[1]>=0x0a)&&(hex[1]<=0x0f))
		{hex[0] +=0x37; hex[1] +=0x37;}
	}
	return *hex;
}

void USB_LP_CAN1_RX0_IRQHandler(void)                 //CAN1�жϷ�������
{  	
  
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage_Tem);      //�������ȡCAN�ӿ��Ͻ��յ�������		
	InsertQueue(msg1, RxMessage_Tem);	
	c=1;	
}
void EXTI9_5_IRQHandler(void)            //���ڿ����ر��ж�(����4)
{
  a+=1;
  if(a==1)  {printf("׼����ʼ�ɼ����ݣ���������can����\n");  led_on(LED_4);} //����D4
  if(a==2)  led_off(LED_4);
  if(a==3) {a=0;}
  sw_delay_ms(300);
  EXTI_ClearITPendingBit(EXTI_Line5);   //���EXTI�ж���·����λ
}
void EXTI2_IRQHandler(void)             //�жϣ�������ͣ
{
  b=!b; 
  if(b==0)led_off(LED_3);else {led_on(LED_3); printf("             ��ͣ�� \n");}	
  sw_delay_ms(300);
	EXTI_ClearITPendingBit(EXTI_Line2);   //���EXTI�ж���·����λ
}

void canchuli(void)                      //can����
{  			
	      len[0]= msg1[front].DLC +0x30;
			  memset(ms,0,sizeof(ms));
				Dchar(msg1[front].shijian,ms);
		    if (msg1[front].IDE == CAN_Id_Standard) 	{zhenhex(RxMessage_Tem.StdId);}
			  else {zhenhex(RxMessage_Tem.ExtId);}
			  Dchar(msg1[front].bianhao ,xvhao);
				for(u=-1;u<20;)
			    {
					 if(p==8) {p=0;}
					 *hex=CharToHex(msg1[front].Data[p] ,hex); 
				   if(u!=-1)WriteBuffer[++u]=kongge[0];
					 WriteBuffer[++u] = hex[0] ;
					 WriteBuffer[++u] = hex[1] ;
					 //printf("%c",hex[0]);
					 //printf("%c  ",hex[1]);
					 p++;
					}
			       i++;	
					   //printf("%d",msg[front].bianhao);
             for(ll=0;ll<weishu2;ll++){printf("%c",ms[ll]);}	
				     //printf(" %d"  ,front);
						 
											   						
       			
}
BYTE Dchar2(int x,int y,char *zz)                  //���ڽ�ʮ����תΪasc��     
{ int x1,n=0,i,a,b;
	x1=x;
  do{
  n++;
  x1/=10;
  }while(x1); 
	for(i=n-1;i>=0;i--)
	{
    zz[i]=x%10+48;
    x=x/10;  	
	}
	a=y%10; b=y/10;
	zz[2]='f';zz[3]=b+48;zz[4]=a+48;
	
	return *zz;
}
void system()
{
RTC_ReadClock();
syssec=g_tRTC.Sec;
sysmin = g_tRTC.Min;
Dchar2(sysmin,syssec,File_name);
}
void rudui2()
{
memcpy(save_filename[filename_i],File_name,14);		//���ļ���д������
filename_i++;
}

void delete1()      //����������
{
   			
				
			  /*if(filename_i==12)	
				filename_i=0;*/
				if(filename_i==11)		//���������ٷ�֮��ʮ�ڴ�
			  {
					f_unlink(save_filename[f]);	
					printf("ɾ���ļ��ɹ�\r\n");
					filename_i=10;
					f++;		//��������
					n--;		//ɾ���������һ
					if(f==700)
					f=0;
		
		    }
}


int main(void)
{
  //��ʼ��SysTick
  SysTick_Init();
	//��ʼ����������ָʾ��D1��D2��D3��D4������
  leds_init();	
  //��ʼ��PE5Ϊ�ⲿ�ж�5
  EXTI_PE5_Init();
	EXTI_PE2_Init();
	/* ��ʼ��LED */
	LED_GPIO_Config();	
  //��ʼ��CAN�ӿ��õ���GPIO��  
  CAN_GPIO_Init();  
  //��ʼ��CAN����
  CAN1_Init();	
  /* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	USART_Config();	
	printf("\r\n****** ����һ��SD�� �ļ�ϵͳʵ�� ******\r\n");
	
	//���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SDIO�豸��ʼ��
	res_sd = f_mount(&fs,"0:",1);
	LED1_ON;
	/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
		/* ��ʽ�� */
		res_sd=f_mkfs("0:",0,0);							

		if(res_sd == FR_OK)
		{
			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
			/* ��ʽ������ȡ������ */
			res_sd = f_mount(NULL,"0:",1);			
			/* ���¹���	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			LED_RED;
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
	else if(res_sd!=FR_OK)
	{
		printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
		printf("��������ԭ��SD��δ���롣\r\n");
		while(1);
	}
	else
	{
		printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
	}

	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
while(1)	
{	
	switch(duowenjian)
	{
		case 0:
		{   system();
				File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
		   	res_sd = f_open(&fnew,File_name,FA_CREATE_ALWAYS | FA_WRITE );	
				rudui2();
				delete1();
   			if ( res_sd == FR_OK )
	       {
		     printf("����/����FatFs��д�����ļ�.csv�ļ��ɹ������°���4��ʼ/�ر��ļ�д�����ݡ�\r\n");         /* ��ָ���洢������д�뵽�ļ��� */
		     biaoti();
		     while(1)
		     {   if(b==0)
			       if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd=f_write(&fnew,xvhao,sizeof(xvhao),&fnum);              //д�����
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //����
						      res_sd=f_write(&fnew,zhen,sizeof(zhen),&fnum);                //д��֡��
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //����
						      if (msg1[front].IDE == CAN_Id_Standard) 									  //�жϱ�׼֡����չ֡
						      {res_sd=f_write(&fnew,zhenstr,sizeof(zhenstr),&fnum);}
						      else{res_sd=f_write(&fnew,zhenk,sizeof(zhenk),&fnum);}
						      res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //����
					     	  res_sd=f_write(&fnew,len,sizeof(len),&fnum);                  //д��can���ݳ���
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);                    //����
					     	  if (msg1[front].RTR == 0) 									                 //�ж�����֡��Զ��֡
						      {res_sd=f_write(&fnew,shujv,sizeof(shujv),&fnum);}
					     	  else{res_sd=f_write(&fnew,yuan,sizeof(yuan),&fnum);}
						      res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //����
					      	 res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);  //д��can����
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);                    //����
						      res_sd=f_write(&fnew,ms,sizeof(ms),&fnum);                    //д�������
						      res_sd=f_write(&fnew,huanhang,sizeof(huanhang),&fnum);				 //����
					       c--;
                 DeleteQueue(msg1); 
								// printf("  %d\n"  ,end);
								 memset(xvhao,0,sizeof(xvhao));
			          }
						
						if(a==2||msg1[front].bianhao>3000)
             {
			         duowenjian++; 
							 break;							  
			       }
		     }
			 }  
		/* ���ٶ�д���ر��ļ� */
		f_close(&fnew);
	  }
		
		
		
		case 1:
		{		 if(msg1[front].bianhao<3000){break;}
				 system();
				 File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
				 res_sd1 = f_open(&fnew1, File_name,FA_CREATE_ALWAYS | FA_WRITE );
				 rudui2();
				 delete1();
		     //printf("����/����FatFs��д�����ļ�1.csv�ļ��ɹ������°���4��ʼ/�ر��ļ�д�����ݡ�\r\n");         /* ��ָ���洢������д�뵽�ļ��� */
		     biaoti1();
		     while(1)
		     {   if(b==0)
			       if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd1=f_write(&fnew1,xvhao,sizeof(xvhao),&fnum1);              //д�����
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //����
						      res_sd1=f_write(&fnew1,zhen,sizeof(zhen),&fnum1);                //д��֡��
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //����
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //�жϱ�׼֡����չ֡
						      {res_sd1=f_write(&fnew1,zhenstr,sizeof(zhenstr),&fnum1);}
						      else{res_sd1=f_write(&fnew1,zhenk,sizeof(zhenk),&fnum1);}
						      res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //����
					     	  res_sd1=f_write(&fnew1,len,sizeof(len),&fnum1);                  //д��can���ݳ���
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);                    //����
					     	  if ( msg1[front].RTR == 0) 									                 //�жϱ�׼֡����չ֡
						      {res_sd1=f_write(&fnew1,shujv,sizeof(shujv),&fnum1);}
					     	  else{res_sd1=f_write(&fnew1,yuan,sizeof(yuan),&fnum1);}
						      res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //����
					      	 res_sd1=f_write(&fnew1,WriteBuffer,sizeof(WriteBuffer),&fnum1);  //д��can����
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);                    //����
						      res_sd1=f_write(&fnew1,ms,sizeof(ms),&fnum1);                    //д�������
						      res_sd1=f_write(&fnew1,huanhang,sizeof(huanhang),&fnum1);				 //����
					       c--;
			           DeleteQueue(msg1); 
								 //printf("  %d\n"  ,end);
									memset(xvhao,0,sizeof(xvhao));
						 }
						
						if(a==2||msg1[front].bianhao>6000)
             {
			          break;
			       }
		     }   
		/* ���ٶ�д���ر��ļ� */
		f_close(&fnew1);
		if(a==2)  break;
	}	

	case 2:
		{		 if(msg1[front].bianhao<6000){break;}
				 system();
				 File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
			   res_sd2 = f_open(&fnew2, File_name,FA_CREATE_ALWAYS | FA_WRITE );	
				 rudui2();
				 delete1();
		     //printf("����/����FatFs��д�����ļ�1.csv�ļ��ɹ������°���4��ʼ/�ر��ļ�д�����ݡ�\r\n");         /* ��ָ���洢������д�뵽�ļ��� */
		     biaoti2();
		     while(1)
		     {   if(b==0)
			        if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd2=f_write(&fnew2,xvhao,sizeof(xvhao),&fnum2);              //д�����
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //����
						      res_sd2=f_write(&fnew2,zhen,sizeof(zhen),&fnum2);                //д��֡��
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //����
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //�жϱ�׼֡����չ֡
						      {res_sd2=f_write(&fnew2,zhenstr,sizeof(zhenstr),&fnum2);}
						      else{res_sd2=f_write(&fnew2,zhenk,sizeof(zhenk),&fnum2);}
						      res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //����
					     	  res_sd2=f_write(&fnew2,len,sizeof(len),&fnum2);                  //д��can���ݳ���
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);                    //����
					     	  if ( msg1[front].RTR == 0) 									                 //�жϱ�׼֡����չ֡
						      {res_sd2=f_write(&fnew2,shujv,sizeof(shujv),&fnum2);}
					     	  else{res_sd2=f_write(&fnew2,yuan,sizeof(yuan),&fnum2);}
						      res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //����
					      	 res_sd2=f_write(&fnew2,WriteBuffer,sizeof(WriteBuffer),&fnum2);  //д��can����
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);                    //����
						      res_sd2=f_write(&fnew2,ms,sizeof(ms),&fnum2);                    //д�������
						      res_sd2=f_write(&fnew2,huanhang,sizeof(huanhang),&fnum2);				 //����
					        c--;
			            DeleteQueue(msg1); 
									//printf("  %d\n"  ,end);
									memset(xvhao,0,sizeof(xvhao));
						 }
						
						if(a==2||msg1[front].bianhao>9000)
             {
			          break;							  
			       }
		     }   
		/* ���ٶ�д���ر��ļ� */
		f_close(&fnew2);
		if(a==2)  break;
	}		

	
		case 3:
		{		 if(msg1[front].bianhao<9000){break;}
					system();
					File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
				 res_sd3 = f_open(&fnew3, File_name,FA_CREATE_ALWAYS | FA_WRITE );
				 rudui2();
				 delete1();
		     //printf("����/����FatFs��д�����ļ�1.csv�ļ��ɹ������°���4��ʼ/�ر��ļ�д�����ݡ�\r\n");         /* ��ָ���洢������д�뵽�ļ��� */
		     biaoti3();
		     while(1)
		     {   if(b==0)
			        if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd3=f_write(&fnew3,xvhao,sizeof(xvhao),&fnum3);              //д�����
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //����
						      res_sd3=f_write(&fnew3,zhen,sizeof(zhen),&fnum3);                //д��֡��
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //����
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //�жϱ�׼֡����չ֡
						      {res_sd3=f_write(&fnew3,zhenstr,sizeof(zhenstr),&fnum3);}
						      else{res_sd3=f_write(&fnew3,zhenk,sizeof(zhenk),&fnum3);}
						      res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //����
					     	  res_sd3=f_write(&fnew3,len,sizeof(len),&fnum3);                  //д��can���ݳ���
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);                    //����
					     	  if ( msg1[front].RTR == 0) 									                 //�жϱ�׼֡����չ֡
						      {res_sd3=f_write(&fnew3,shujv,sizeof(shujv),&fnum3);}
					     	  else{res_sd3=f_write(&fnew3,yuan,sizeof(yuan),&fnum3);}
						      res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //����
					      	 res_sd3=f_write(&fnew3,WriteBuffer,sizeof(WriteBuffer),&fnum3);  //д��can����
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);                    //����
						      res_sd3=f_write(&fnew3,ms,sizeof(ms),&fnum3);                    //д�������
						      res_sd3=f_write(&fnew3,huanhang,sizeof(huanhang),&fnum3);				 //����
					       c--;
			           DeleteQueue(msg1); 
						     //printf("  %d\n"  ,end);
									memset(xvhao,0,sizeof(xvhao));
						 }
						
						if(a==2||msg1[front].bianhao>12000)
             {
			          break;
			       }
		     }   
		/* ���ٶ�д���ر��ļ� */
		f_close(&fnew3);
		if(a==2)  break;
	}		

			case 4:
		{		 if(msg1[front].bianhao<12000){break;}
				 system();
				 File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
			   res_sd4 = f_open(&fnew4, File_name,FA_CREATE_ALWAYS | FA_WRITE );	
				 rudui2();
				 delete1();
		     //printf("����/����FatFs��д�����ļ�1.csv�ļ��ɹ������°���4��ʼ/�ر��ļ�д�����ݡ�\r\n");         /* ��ָ���洢������д�뵽�ļ��� */
		     biaoti4();
		     while(1)
		     {   if(b==0)
			        if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd4=f_write(&fnew4,xvhao,sizeof(xvhao),&fnum4);              //д�����
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //����
						      res_sd4=f_write(&fnew4,zhen,sizeof(zhen),&fnum4);                //д��֡��
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //����
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //�жϱ�׼֡����չ֡
						      {res_sd4=f_write(&fnew4,zhenstr,sizeof(zhenstr),&fnum4);}
						      else{res_sd4=f_write(&fnew4,zhenk,sizeof(zhenk),&fnum4);}
						      res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //����
					     	  res_sd4=f_write(&fnew4,len,sizeof(len),&fnum4);                  //д��can���ݳ���
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);                    //����
					     	  if ( msg1[front].RTR == 0) 									                 //�жϱ�׼֡����չ֡
						      {res_sd4=f_write(&fnew4,shujv,sizeof(shujv),&fnum4);}
					     	  else{res_sd4=f_write(&fnew4,yuan,sizeof(yuan),&fnum4);}
						      res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //����
					      	 res_sd4=f_write(&fnew4,WriteBuffer,sizeof(WriteBuffer),&fnum4);  //д��can����
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);                    //����
						      res_sd4=f_write(&fnew4,ms,sizeof(ms),&fnum4);                    //д�������
						      res_sd4=f_write(&fnew4,huanhang,sizeof(huanhang),&fnum4);				 //����
					    
			           DeleteQueue(msg1); 
								//printf("  %d\n"  ,end);
									memset(xvhao,0,sizeof(xvhao));
						 }
						
						if(a==2||msg1[front].bianhao>15000)
             {
			          break;							  
			       }
		     }   
		/* ���ٶ�д���ر��ļ� */
		f_close(&fnew4);
		if(a==2)  break;
	}		

   	case 5:
		{		 if(msg1[front].bianhao<15000){break;}
				 system();
				 File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
			   res_sd5 = f_open(&fnew5,File_name,FA_CREATE_ALWAYS | FA_WRITE );	 
				 rudui2();
				 delete1();
		     //printf("����/����FatFs��д�����ļ�1.csv�ļ��ɹ������°���4��ʼ/�ر��ļ�д�����ݡ�\r\n");         /* ��ָ���洢������д�뵽�ļ��� */
		     biaoti5();
		     while(1)
		     {   if(b==0)
			       if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd5=f_write(&fnew5,xvhao,sizeof(xvhao),&fnum5);              //д�����
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //����
						      res_sd5=f_write(&fnew5,zhen,sizeof(zhen),&fnum5);                //д��֡��
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //����
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //�жϱ�׼֡����չ֡
						      {res_sd5=f_write(&fnew5,zhenstr,sizeof(zhenstr),&fnum5);}
						      else{res_sd5=f_write(&fnew5,zhenk,sizeof(zhenk),&fnum5);}
						      res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //����
					     	  res_sd5=f_write(&fnew5,len,sizeof(len),&fnum5);                  //д��can���ݳ���
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);                    //����
					     	  if ( msg1[front].RTR == 0) 									                 //�жϱ�׼֡����չ֡
						      {res_sd5=f_write(&fnew5,shujv,sizeof(shujv),&fnum5);}
					     	  else{res_sd5=f_write(&fnew5,yuan,sizeof(yuan),&fnum5);}
						      res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //����
					      	 res_sd5=f_write(&fnew5,WriteBuffer,sizeof(WriteBuffer),&fnum5);  //д��can����
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);                    //����
						      res_sd5=f_write(&fnew5,ms,sizeof(ms),&fnum5);                    //д�������
						      res_sd5=f_write(&fnew5,huanhang,sizeof(huanhang),&fnum5);				 //����
					       c--;
									if(a==2||msg1[front].bianhao==18000)
                 {
			                printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
							        duowenjian=0;
							        DeleteQueue(msg1); 
							        memset(xvhao,0,sizeof(xvhao)); 		                  
		                  f_close(&fnew5);																									 
									    break;							 
			           }
			            DeleteQueue(msg1); 								
									memset(xvhao,0,sizeof(xvhao));
									}
		     }  
		if(a==2)  break;
	}		
	}
if(a==2)  break;
}


	/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_sd = f_open(&fnew, "0:���ݲɼ��洢�ļ�.csv", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
		LED_GREEN;
		printf("�����ļ��ɹ���\r\n");
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
		if(res_sd==FR_OK)
		{
			printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�\r\n");
		}
		else
		{
			printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
		}		
	}
	else
	{
		LED_GREEN;
		printf("�������ļ��ɹ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	

	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"0:",1);

	/* ������ɣ�ͣ�� */
	while(1)
	{
	}
}
//memset(RxMessage_Tem.Data,0,sizeof(RxMessage_Tem.Data)); 
	  

