#include "stm32f10x.h"
#include "tf.h"
#include "user_usart.h"
#include "delay.h"


uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t readbuff[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;

 /**************************************************************************************
 * 描  述 : 擦除数据测试
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SD_EraseTest(void)
{
  if (Status == SD_OK)
  {    
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS)); //第一个参数为擦除起始地址，第二个参数为擦除结束地址
  }

  if (Status == SD_OK)
  {	
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);//读取刚刚擦除的区域
    Status = SD_WaitReadOperation(); //查询传输是否结束    
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
  if (Status == SD_OK)
  {	
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);//把擦除区域读出来对比
  }
  
  if(EraseStatus == PASSED)
  	printf("\r\n 擦除测试成功！ " );
 
  else	  
  	printf("\r\n 擦除测试失败！ " );  
}

/**************************************************************************************
 * 描  述 : 单个数据块读写测试
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SD_SingleBlockTest(void)
{  
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

  if (Status == SD_OK)
  {
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
    Status = SD_WaitWriteOperation();	  
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }

  if (Status == SD_OK)
  {
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);//读取数据
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
  if (Status == SD_OK)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);	//比较
  }
  
  if(TransferStatus1 == PASSED)
    printf("\r\n 单个数据块读写测试成功！" );
 
  else  
  	printf("\r\n 单个数据块读写测试失败！ " );  
}

/**************************************************************************************
 * 描  述 : 多数据块读写测试
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void SD_MultiBlockTest(void)
{
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_OK)
  {
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == PASSED)	  
  	printf("\r\n 多数据块读写测试成功！ " );

  else 
  	printf("\r\n 多数据块读写测试失败！ " );  

}

/**************************************************************************************
 * 描  述 : 比较两个缓冲区中的数据是否相等
 * 入  参 : pBuffer1 要比较的缓冲区1，  pBuffer2 要比较的缓冲区2 ，  BufferLength 缓冲区长度
 * 返回值 : PASSED 相等； FAILED 不相等  
 **************************************************************************************/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

/**************************************************************************************
 * 描  述 : 在缓冲区中填写数据
 * 入  参 : pBuffer 要填充的缓冲区，  BufferLength 要填充的长度 ，  Offset 填在缓冲区的第一个值
 * 返回值 : 无  
 **************************************************************************************/
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/**************************************************************************************
 * 描  述 : 检查缓冲区的数据是否为0
 * 入  参 : pBuffer 要比较的缓冲区，  BufferLength 缓冲区长度 
 * 返回值 : PASSED 缓冲区的数据全为0； FAILED 缓冲区的数据至少有一个不为0  
 **************************************************************************************/
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))//擦除后是0xff或0x00
    {
      return FAILED;
    }
    pBuffer++;
  }
  return PASSED;
}



