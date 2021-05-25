/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SD卡文件系统例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-霸道 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
  *                              定义变量
  ******************************************************************************
  */
int a=0,b=0,c=0,jishi=0,weishu2,ll,xvhaoint=1,duowenjian=0,syssec,sysmin, filename_i=0,f=0,n=0;
unsigned char hex[2]={0};
int i=1,u,p=0,san=3;
unsigned char q,zhen[10]="";
uint32_t timecount;
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew,fnew1,fnew2,fnew3,fnew4,fnew5;													/* 文件对象 */
FRESULT res_sd,res_sd1,res_sd2,res_sd3,res_sd4,res_sd5;                   /* 文件操作结果 */
UINT fnum,fnum1,fnum2,fnum3,fnum4,fnum5;            	          /* 文件成功读写数量 */
BYTE ReadBuffer[50]={0};        /* 读缓冲区 */
CanRxMsg RxMessage_Tem;
BYTE        WriteBuffer[23] = " ",huanhang[]=" \n",kongge[]=" ",ms[10]=" ",cc[]=",",xvhao[10]=" ",len[1]="",
            tishi1[]="CAN数据",tishi2[]="时间/ms",tishi3[]="序号",tishi4[]="帧ID",
	          tishi5[]="帧格式",zhenstr[]="标准帧",zhenk[]="扩展帧",tishi6[]="帧的长度",tishi7[]="帧类型",
						yuan[]="远程帧",shujv[]="数据帧";
char File_name[50]="";	
char save_filename[100][12]={0};		//用来保存文件名

//定义can结构体
struct canmsg
{
  int bianhao;              //序号
	unsigned int zhenid;	    //帧id号
	unsigned char canlen;	    //帧长度
	unsigned int zhenstr1;    //标准和扩展
	unsigned char data[8];      //can数据
	int shijian;              //毫秒
	unsigned char yuanorshu;  //远程帧和数据帧
};
CanRxMsg msg1[500];
struct canmsg msg[6];
unsigned int front = 0;
unsigned int end = 0;

void InsertQueue(CanRxMsg *aa, CanRxMsg RxMessage_Tem)   //入队
{   aa[end] = RxMessage_Tem;
		msg1[end].bianhao=xvhaoint;
	  msg1[end].shijian=jishi;
	  xvhaoint++;
	  if(xvhaoint>18000) {xvhaoint=1;}
    /*memcpy(aa[end].data,RxMessage_Tem.Data,sizeof(RxMessage_Tem.Data));
	  aa[end].bianhao = xvhaoint;		         //序号写入结构体
		aa[end].canlen  = RxMessage_Tem.DLC;   //can长度写入结构体
		aa[end].shijian = jishi;							 //时间写入结构体
		aa[end].zhenstr1= RxMessage_Tem.IDE;	 //帧id标准帧和扩展帧判断
		if (RxMessage_Tem.IDE == CAN_Id_Standard) 	{aa[end].zhenid = RxMessage_Tem.StdId;} //帧id写入
		else {aa[end].zhenid = RxMessage_Tem.ExtId;}
		aa[end].yuanorshu  = RxMessage_Tem.RTR;//远程帧和数据帧判断*/
	  end=(end+1)%500;
		
}
void DeleteQueue(CanRxMsg *aa)         //出队
{
  
	   memset(&aa[front], 0, sizeof(struct canmsg));
     front=(front+1)%500;	
	
}

void biaoti(void)
{
    res_sd=f_write(&fnew,tishi3,sizeof(tishi3),&fnum);	   //序号
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi4,sizeof(tishi4),&fnum);	   //帧ID
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi5,sizeof(tishi5),&fnum);	   //帧格式
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi6,sizeof(tishi6),&fnum);	   //帧长度
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi7,sizeof(tishi7),&fnum);	   //帧类型
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi1,sizeof(tishi1),&fnum);     //数据
		res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);
		res_sd=f_write(&fnew,tishi2,sizeof(tishi2),&fnum);     //时间
		res_sd=f_write(&fnew,huanhang,sizeof(huanhang),&fnum);
		
}
void biaoti1(void)
{
    res_sd1=f_write(&fnew1,tishi3,sizeof(tishi3),&fnum1);	   //序号
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi4,sizeof(tishi4),&fnum1);	   //帧ID
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi5,sizeof(tishi5),&fnum1);	   //帧格式
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi6,sizeof(tishi6),&fnum1);	   //帧长度
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi7,sizeof(tishi7),&fnum1);	   //帧类型
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi1,sizeof(tishi1),&fnum1);     //数据
		res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);
		res_sd1=f_write(&fnew1,tishi2,sizeof(tishi2),&fnum1);     //时间
		res_sd1=f_write(&fnew1,huanhang,sizeof(huanhang),&fnum1);
		
}
void biaoti2(void)
{
    res_sd2=f_write(&fnew2,tishi3,sizeof(tishi3),&fnum2);	   //序号
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi4,sizeof(tishi4),&fnum2);	   //帧ID
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi5,sizeof(tishi5),&fnum2);	   //帧格式
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi6,sizeof(tishi6),&fnum2);	   //帧长度
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi7,sizeof(tishi7),&fnum2);	   //帧类型
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi1,sizeof(tishi1),&fnum2);     //数据
		res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);
		res_sd2=f_write(&fnew2,tishi2,sizeof(tishi2),&fnum2);     //时间
		res_sd2=f_write(&fnew2,huanhang,sizeof(huanhang),&fnum2);
		
}
void biaoti3(void)
{
    res_sd3=f_write(&fnew3,tishi3,sizeof(tishi3),&fnum3);	   //序号
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi4,sizeof(tishi4),&fnum3);	   //帧ID
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi5,sizeof(tishi5),&fnum3);	   //帧格式
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi6,sizeof(tishi6),&fnum3);	   //帧长度
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi7,sizeof(tishi7),&fnum3);	   //帧类型
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi1,sizeof(tishi1),&fnum3);     //数据
		res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);
		res_sd3=f_write(&fnew3,tishi2,sizeof(tishi2),&fnum3);     //时间
		res_sd3=f_write(&fnew3,huanhang,sizeof(huanhang),&fnum3);
		
}
void biaoti4(void)
{
    res_sd4=f_write(&fnew4,tishi3,sizeof(tishi3),&fnum4);	   //序号
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi4,sizeof(tishi4),&fnum4);	   //帧ID
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi5,sizeof(tishi5),&fnum4);	   //帧格式
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi6,sizeof(tishi6),&fnum4);	   //帧长度
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi7,sizeof(tishi7),&fnum4);	   //帧类型
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi1,sizeof(tishi1),&fnum4);     //数据
		res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);
		res_sd4=f_write(&fnew4,tishi2,sizeof(tishi2),&fnum4);     //时间
		res_sd4=f_write(&fnew4,huanhang,sizeof(huanhang),&fnum4);
		
}
void biaoti5(void)
{
    res_sd5=f_write(&fnew5,tishi3,sizeof(tishi3),&fnum5);	   //序号
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi4,sizeof(tishi4),&fnum5);	   //帧ID
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi5,sizeof(tishi5),&fnum5);	   //帧格式
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi6,sizeof(tishi6),&fnum5);	   //帧长度
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi7,sizeof(tishi7),&fnum5);	   //帧类型
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi1,sizeof(tishi1),&fnum5);     //数据
		res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);
		res_sd5=f_write(&fnew5,tishi2,sizeof(tishi2),&fnum5);     //时间
		res_sd5=f_write(&fnew5,huanhang,sizeof(huanhang),&fnum5);
		
}
BYTE Dchar(int x,BYTE *zz)                  //用于将十进制转为asc码     
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
void SysTick_Handler(void)                 //计时器
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
unsigned char CharToHex(unsigned char bHex,unsigned char *hex)//16进制转字符串
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

void USB_LP_CAN1_RX0_IRQHandler(void)                 //CAN1中断服务处理函数
{  	
  
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage_Tem);      //开发板读取CAN接口上接收到的数据		
	InsertQueue(msg1, RxMessage_Tem);	
	c=1;	
}
void EXTI9_5_IRQHandler(void)            //用于开启关闭中断(开关4)
{
  a+=1;
  if(a==1)  {printf("准备开始采集数据，请点击发送can数据\n");  led_on(LED_4);} //点亮D4
  if(a==2)  led_off(LED_4);
  if(a==3) {a=0;}
  sw_delay_ms(300);
  EXTI_ClearITPendingBit(EXTI_Line5);   //清除EXTI中断线路挂起位
}
void EXTI2_IRQHandler(void)             //中断，用于暂停
{
  b=!b; 
  if(b==0)led_off(LED_3);else {led_on(LED_3); printf("             暂停中 \n");}	
  sw_delay_ms(300);
	EXTI_ClearITPendingBit(EXTI_Line2);   //清除EXTI中断线路挂起位
}

void canchuli(void)                      //can处理
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
BYTE Dchar2(int x,int y,char *zz)                  //用于将十进制转为asc码     
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
memcpy(save_filename[filename_i],File_name,14);		//把文件名写进数组
filename_i++;
}

void delete1()      //数组名出队
{
   			
				
			  /*if(filename_i==12)	
				filename_i=0;*/
				if(filename_i==11)		//超过总数百分之九十内存
			  {
					f_unlink(save_filename[f]);	
					printf("删除文件成功\r\n");
					filename_i=10;
					f++;		//数组增加
					n--;		//删除完个数减一
					if(f==700)
					f=0;
		
		    }
}


int main(void)
{
  //初始化SysTick
  SysTick_Init();
	//初始化用于驱动指示灯D1、D2、D3、D4的引脚
  leds_init();	
  //初始化PE5为外部中断5
  EXTI_PE5_Init();
	EXTI_PE2_Init();
	/* 初始化LED */
	LED_GPIO_Config();	
  //初始化CAN接口用到的GPIO口  
  CAN_GPIO_Init();  
  //初始化CAN配置
  CAN1_Init();	
  /* 初始化调试串口，一般为串口1 */
	USART_Config();	
	printf("\r\n****** 这是一个SD卡 文件系统实验 ******\r\n");
	
	//在外部SD卡挂载文件系统，文件系统挂载时会对SDIO设备初始化
	res_sd = f_mount(&fs,"0:",1);
	LED1_ON;
	/*----------------------- 格式化测试 ---------------------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
		/* 格式化 */
		res_sd=f_mkfs("0:",0,0);							

		if(res_sd == FR_OK)
		{
			printf("》SD卡已成功格式化文件系统。\r\n");
			/* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL,"0:",1);			
			/* 重新挂载	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			LED_RED;
			printf("《《格式化失败。》》\r\n");
			while(1);
		}
	}
	else if(res_sd!=FR_OK)
	{
		printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
		printf("！！可能原因：SD卡未插入。\r\n");
		while(1);
	}
	else
	{
		printf("》文件系统挂载成功，可以进行读写测试\r\n");
	}

	/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
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
		     printf("》打开/创建FatFs读写测试文件.csv文件成功，按下按键4开始/关闭文件写入数据。\r\n");         /* 将指定存储区内容写入到文件内 */
		     biaoti();
		     while(1)
		     {   if(b==0)
			       if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd=f_write(&fnew,xvhao,sizeof(xvhao),&fnum);              //写入序号
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //换列
						      res_sd=f_write(&fnew,zhen,sizeof(zhen),&fnum);                //写入帧号
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //换列
						      if (msg1[front].IDE == CAN_Id_Standard) 									  //判断标准帧和扩展帧
						      {res_sd=f_write(&fnew,zhenstr,sizeof(zhenstr),&fnum);}
						      else{res_sd=f_write(&fnew,zhenk,sizeof(zhenk),&fnum);}
						      res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //换列
					     	  res_sd=f_write(&fnew,len,sizeof(len),&fnum);                  //写入can数据长度
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);                    //换列
					     	  if (msg1[front].RTR == 0) 									                 //判断数据帧和远程帧
						      {res_sd=f_write(&fnew,shujv,sizeof(shujv),&fnum);}
					     	  else{res_sd=f_write(&fnew,yuan,sizeof(yuan),&fnum);}
						      res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);  									 //换列
					      	 res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);  //写入can数据
		              res_sd=f_write(&fnew,cc,sizeof(cc),&fnum);                    //换列
						      res_sd=f_write(&fnew,ms,sizeof(ms),&fnum);                    //写入毫秒数
						      res_sd=f_write(&fnew,huanhang,sizeof(huanhang),&fnum);				 //换行
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
		/* 不再读写，关闭文件 */
		f_close(&fnew);
	  }
		
		
		
		case 1:
		{		 if(msg1[front].bianhao<3000){break;}
				 system();
				 File_name[5]='.';File_name[6]='c';File_name[7]='s';File_name[8]='v';
				 res_sd1 = f_open(&fnew1, File_name,FA_CREATE_ALWAYS | FA_WRITE );
				 rudui2();
				 delete1();
		     //printf("》打开/创建FatFs读写测试文件1.csv文件成功，按下按键4开始/关闭文件写入数据。\r\n");         /* 将指定存储区内容写入到文件内 */
		     biaoti1();
		     while(1)
		     {   if(b==0)
			       if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd1=f_write(&fnew1,xvhao,sizeof(xvhao),&fnum1);              //写入序号
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //换列
						      res_sd1=f_write(&fnew1,zhen,sizeof(zhen),&fnum1);                //写入帧号
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //换列
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //判断标准帧和扩展帧
						      {res_sd1=f_write(&fnew1,zhenstr,sizeof(zhenstr),&fnum1);}
						      else{res_sd1=f_write(&fnew1,zhenk,sizeof(zhenk),&fnum1);}
						      res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //换列
					     	  res_sd1=f_write(&fnew1,len,sizeof(len),&fnum1);                  //写入can数据长度
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);                    //换列
					     	  if ( msg1[front].RTR == 0) 									                 //判断标准帧和扩展帧
						      {res_sd1=f_write(&fnew1,shujv,sizeof(shujv),&fnum1);}
					     	  else{res_sd1=f_write(&fnew1,yuan,sizeof(yuan),&fnum1);}
						      res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);  									 //换列
					      	 res_sd1=f_write(&fnew1,WriteBuffer,sizeof(WriteBuffer),&fnum1);  //写入can数据
		              res_sd1=f_write(&fnew1,cc,sizeof(cc),&fnum1);                    //换列
						      res_sd1=f_write(&fnew1,ms,sizeof(ms),&fnum1);                    //写入毫秒数
						      res_sd1=f_write(&fnew1,huanhang,sizeof(huanhang),&fnum1);				 //换行
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
		/* 不再读写，关闭文件 */
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
		     //printf("》打开/创建FatFs读写测试文件1.csv文件成功，按下按键4开始/关闭文件写入数据。\r\n");         /* 将指定存储区内容写入到文件内 */
		     biaoti2();
		     while(1)
		     {   if(b==0)
			        if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd2=f_write(&fnew2,xvhao,sizeof(xvhao),&fnum2);              //写入序号
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //换列
						      res_sd2=f_write(&fnew2,zhen,sizeof(zhen),&fnum2);                //写入帧号
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //换列
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //判断标准帧和扩展帧
						      {res_sd2=f_write(&fnew2,zhenstr,sizeof(zhenstr),&fnum2);}
						      else{res_sd2=f_write(&fnew2,zhenk,sizeof(zhenk),&fnum2);}
						      res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //换列
					     	  res_sd2=f_write(&fnew2,len,sizeof(len),&fnum2);                  //写入can数据长度
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);                    //换列
					     	  if ( msg1[front].RTR == 0) 									                 //判断标准帧和扩展帧
						      {res_sd2=f_write(&fnew2,shujv,sizeof(shujv),&fnum2);}
					     	  else{res_sd2=f_write(&fnew2,yuan,sizeof(yuan),&fnum2);}
						      res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);  									 //换列
					      	 res_sd2=f_write(&fnew2,WriteBuffer,sizeof(WriteBuffer),&fnum2);  //写入can数据
		              res_sd2=f_write(&fnew2,cc,sizeof(cc),&fnum2);                    //换列
						      res_sd2=f_write(&fnew2,ms,sizeof(ms),&fnum2);                    //写入毫秒数
						      res_sd2=f_write(&fnew2,huanhang,sizeof(huanhang),&fnum2);				 //换行
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
		/* 不再读写，关闭文件 */
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
		     //printf("》打开/创建FatFs读写测试文件1.csv文件成功，按下按键4开始/关闭文件写入数据。\r\n");         /* 将指定存储区内容写入到文件内 */
		     biaoti3();
		     while(1)
		     {   if(b==0)
			        if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd3=f_write(&fnew3,xvhao,sizeof(xvhao),&fnum3);              //写入序号
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //换列
						      res_sd3=f_write(&fnew3,zhen,sizeof(zhen),&fnum3);                //写入帧号
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //换列
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //判断标准帧和扩展帧
						      {res_sd3=f_write(&fnew3,zhenstr,sizeof(zhenstr),&fnum3);}
						      else{res_sd3=f_write(&fnew3,zhenk,sizeof(zhenk),&fnum3);}
						      res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //换列
					     	  res_sd3=f_write(&fnew3,len,sizeof(len),&fnum3);                  //写入can数据长度
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);                    //换列
					     	  if ( msg1[front].RTR == 0) 									                 //判断标准帧和扩展帧
						      {res_sd3=f_write(&fnew3,shujv,sizeof(shujv),&fnum3);}
					     	  else{res_sd3=f_write(&fnew3,yuan,sizeof(yuan),&fnum3);}
						      res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);  									 //换列
					      	 res_sd3=f_write(&fnew3,WriteBuffer,sizeof(WriteBuffer),&fnum3);  //写入can数据
		              res_sd3=f_write(&fnew3,cc,sizeof(cc),&fnum3);                    //换列
						      res_sd3=f_write(&fnew3,ms,sizeof(ms),&fnum3);                    //写入毫秒数
						      res_sd3=f_write(&fnew3,huanhang,sizeof(huanhang),&fnum3);				 //换行
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
		/* 不再读写，关闭文件 */
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
		     //printf("》打开/创建FatFs读写测试文件1.csv文件成功，按下按键4开始/关闭文件写入数据。\r\n");         /* 将指定存储区内容写入到文件内 */
		     biaoti4();
		     while(1)
		     {   if(b==0)
			        if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd4=f_write(&fnew4,xvhao,sizeof(xvhao),&fnum4);              //写入序号
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //换列
						      res_sd4=f_write(&fnew4,zhen,sizeof(zhen),&fnum4);                //写入帧号
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //换列
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //判断标准帧和扩展帧
						      {res_sd4=f_write(&fnew4,zhenstr,sizeof(zhenstr),&fnum4);}
						      else{res_sd4=f_write(&fnew4,zhenk,sizeof(zhenk),&fnum4);}
						      res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //换列
					     	  res_sd4=f_write(&fnew4,len,sizeof(len),&fnum4);                  //写入can数据长度
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);                    //换列
					     	  if ( msg1[front].RTR == 0) 									                 //判断标准帧和扩展帧
						      {res_sd4=f_write(&fnew4,shujv,sizeof(shujv),&fnum4);}
					     	  else{res_sd4=f_write(&fnew4,yuan,sizeof(yuan),&fnum4);}
						      res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);  									 //换列
					      	 res_sd4=f_write(&fnew4,WriteBuffer,sizeof(WriteBuffer),&fnum4);  //写入can数据
		              res_sd4=f_write(&fnew4,cc,sizeof(cc),&fnum4);                    //换列
						      res_sd4=f_write(&fnew4,ms,sizeof(ms),&fnum4);                    //写入毫秒数
						      res_sd4=f_write(&fnew4,huanhang,sizeof(huanhang),&fnum4);				 //换行
					    
			           DeleteQueue(msg1); 
								//printf("  %d\n"  ,end);
									memset(xvhao,0,sizeof(xvhao));
						 }
						
						if(a==2||msg1[front].bianhao>15000)
             {
			          break;							  
			       }
		     }   
		/* 不再读写，关闭文件 */
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
		     //printf("》打开/创建FatFs读写测试文件1.csv文件成功，按下按键4开始/关闭文件写入数据。\r\n");         /* 将指定存储区内容写入到文件内 */
		     biaoti5();
		     while(1)
		     {   if(b==0)
			       if(a==1&&front!=end)
						 { 
     						  canchuli();			       
						      res_sd5=f_write(&fnew5,xvhao,sizeof(xvhao),&fnum5);              //写入序号
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //换列
						      res_sd5=f_write(&fnew5,zhen,sizeof(zhen),&fnum5);                //写入帧号
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //换列
						      if (msg1[front].IDE  == CAN_Id_Standard) 									  //判断标准帧和扩展帧
						      {res_sd5=f_write(&fnew5,zhenstr,sizeof(zhenstr),&fnum5);}
						      else{res_sd5=f_write(&fnew5,zhenk,sizeof(zhenk),&fnum5);}
						      res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //换列
					     	  res_sd5=f_write(&fnew5,len,sizeof(len),&fnum5);                  //写入can数据长度
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);                    //换列
					     	  if ( msg1[front].RTR == 0) 									                 //判断标准帧和扩展帧
						      {res_sd5=f_write(&fnew5,shujv,sizeof(shujv),&fnum5);}
					     	  else{res_sd5=f_write(&fnew5,yuan,sizeof(yuan),&fnum5);}
						      res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);  									 //换列
					      	 res_sd5=f_write(&fnew5,WriteBuffer,sizeof(WriteBuffer),&fnum5);  //写入can数据
		              res_sd5=f_write(&fnew5,cc,sizeof(cc),&fnum5);                    //换列
						      res_sd5=f_write(&fnew5,ms,sizeof(ms),&fnum5);                    //写入毫秒数
						      res_sd5=f_write(&fnew5,huanhang,sizeof(huanhang),&fnum5);				 //换行
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


	/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_sd = f_open(&fnew, "0:数据采集存储文件.csv", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
		LED_GREEN;
		printf("》打开文件成功。\r\n");
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
		if(res_sd==FR_OK)
		{
			printf("》文件读取成功,读到字节数据：\r\n");
		}
		else
		{
			printf("！！文件读取失败：(%d)\n",res_sd);
		}		
	}
	else
	{
		LED_GREEN;
		printf("！！打开文件成功\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	

	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"0:",1);

	/* 操作完成，停机 */
	while(1)
	{
	}
}
//memset(RxMessage_Tem.Data,0,sizeof(RxMessage_Tem.Data)); 
	  

