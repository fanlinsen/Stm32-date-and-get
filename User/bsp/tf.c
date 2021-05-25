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
 * ��  �� : �������ݲ���
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void SD_EraseTest(void)
{
  if (Status == SD_OK)
  {    
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS)); //��һ������Ϊ������ʼ��ַ���ڶ�������Ϊ����������ַ
  }

  if (Status == SD_OK)
  {	
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);//��ȡ�ող���������
    Status = SD_WaitReadOperation(); //��ѯ�����Ƿ����    
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
  if (Status == SD_OK)
  {	
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);//�Ѳ�������������Ա�
  }
  
  if(EraseStatus == PASSED)
  	printf("\r\n �������Գɹ��� " );
 
  else	  
  	printf("\r\n ��������ʧ�ܣ� " );  
}

/**************************************************************************************
 * ��  �� : �������ݿ��д����
 * ��  �� : ��
 * ����ֵ : ��
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
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);//��ȡ����
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
  if (Status == SD_OK)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);	//�Ƚ�
  }
  
  if(TransferStatus1 == PASSED)
    printf("\r\n �������ݿ��д���Գɹ���" );
 
  else  
  	printf("\r\n �������ݿ��д����ʧ�ܣ� " );  
}

/**************************************************************************************
 * ��  �� : �����ݿ��д����
 * ��  �� : ��
 * ����ֵ : ��
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
  	printf("\r\n �����ݿ��д���Գɹ��� " );

  else 
  	printf("\r\n �����ݿ��д����ʧ�ܣ� " );  

}

/**************************************************************************************
 * ��  �� : �Ƚ������������е������Ƿ����
 * ��  �� : pBuffer1 Ҫ�ȽϵĻ�����1��  pBuffer2 Ҫ�ȽϵĻ�����2 ��  BufferLength ����������
 * ����ֵ : PASSED ��ȣ� FAILED �����  
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
 * ��  �� : �ڻ���������д����
 * ��  �� : pBuffer Ҫ���Ļ�������  BufferLength Ҫ���ĳ��� ��  Offset ���ڻ������ĵ�һ��ֵ
 * ����ֵ : ��  
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
 * ��  �� : ��黺�����������Ƿ�Ϊ0
 * ��  �� : pBuffer Ҫ�ȽϵĻ�������  BufferLength ���������� 
 * ����ֵ : PASSED ������������ȫΪ0�� FAILED ������������������һ����Ϊ0  
 **************************************************************************************/
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))//��������0xff��0x00
    {
      return FAILED;
    }
    pBuffer++;
  }
  return PASSED;
}



