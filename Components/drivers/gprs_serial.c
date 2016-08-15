#include "stm32f10x.h"
#include "gprs_serial.h"  
#include "sim900a_gprs.h" 
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
 
 
 uint16_t gGprsRecDmaFlag = 0;
 void TIM4_Set(u8 sta)
{
	if(sta)
	{       
		TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	
		TIM_SetCounter(TIM4,0);//���������
	}else TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��4	   
}
//ͨ�ö�ʱ���жϳ�ʼ��
 
void TIM4_Init(u16 time_ms)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��//TIM4ʱ��ʹ��    
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x05 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
        TIM_DeInit(TIM4);
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = (time_ms*10)-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 72M/7200 =10kHZ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	 	  

	
}
//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	struct GPRS_DEV *pDev = GetGprsDev();
	 
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	
		//pDev->wReadPos = 0;
//		xSemaphoreGiveFromISR( pDev->pSem , &xHigherPriorityTaskWoken );  
		 	//��ǽ������
		//vGprsReceiveTask();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־    
		TIM4_Set(0);			//�ر�TIM4  
	}
	 
	// portEND_SWITCHING_ISR(xHigherPriorityTaskWoken );
}
void GPRS_DMARecvHandler(void)
{
	vGprsReceiveTask();
}
void GPRSREC_HANDLER( void )
//static int Gprs_RecvHandler( unsigned char *data, u16 len)
{
	//static u16 wPos;
	struct GPRS_DEV *pDev = GetGprsDev();
	//uint8_t ucdata ,len;
	uint8_t temp;
 
	 
//	T_MSG tMsg;
//	static u16 wDataLen = 0 ;
//	T_MSG_RECV *ptRecvMsg = (T_MSG_RECV *)&tMsg.Data[0];
	 
	if( USART_GetITStatus( GPRSREC_USART, USART_IT_RXNE ) == SET )
	{
		USART_ClearITPendingBit(GPRSREC_USART,USART_IT_RXNE);
		//wPos = pDev->wReadPos;
		if(pDev->wReadPos >= GPRSRECBUFDEEP)
			pDev->wReadPos = 0;
	//	wPos = pDev->wReadPos;
		pDev->buf[pDev->wReadPos] = USART_ReceiveData(GPRSREC_USART); 
		pDev->wReadPos++;
		TIM4_Set(1); 
	}
	if( USART_GetITStatus( GPRSREC_USART, USART_IT_ORE ) == SET )
	{
		USART_ReceiveData(GPRSREC_USART);
		USART_ClearITPendingBit(GPRSREC_USART,USART_IT_ORE);
		
	}
	if(USART_GetITStatus(GPRSREC_USART, USART_IT_IDLE) != RESET)  
    { 
		temp = GPRSREC_USART->SR;  
		temp = GPRSREC_USART->DR; //?USART_IT_IDLE?? 
		USART_ClearITPendingBit(GPRSREC_USART, USART_IT_IDLE);		
		DMA_Cmd(GPRS_DMA_CHANNEL,DISABLE);  
		pDev->wLen = GPRSRECBUFDEEP - DMA_GetCurrDataCounter(GPRS_DMA_CHANNEL);
			
		if(pDev->wLen > 0)
		{
			pDev->wReadPos = pDev->wLen;
			pDev->ucRecDataFlag  = 1;
			GPRS_DMARecvHandler(); 
		}
		pDev->wLen		 =  0;
		DMA_SetCurrDataCounter(GPRS_DMA_CHANNEL,GPRSRECBUFDEEP);  
		DMA_Cmd(GPRS_DMA_CHANNEL,ENABLE);  
		
	}
	 
}

void  Uart_PutChar(u32 dwPort,u8 data)
{
	switch(dwPort)
	{
		case serCOM1:
			USART_SendData(USART1, data);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);	//USART_FLAG_TC
			break;
		case serCOM2:
			USART_SendData(USART2, data);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);	
			break;
		case serCOM3:
			USART_SendData(USART3, data);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);	
			break;
		default:
			break;
	}
	 
}
int Serial_PutData(u32 dwPort, unsigned char *pcData, u16 wLen)
{
	u16 i;
	 
	for(i=0; i<wLen; i++)
	{
		//Uart_PutChar( dwPort, pcData[i]  );
		USART_SendData(USART3,  pcData[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);	
	}
	return 0;
} 
 
 void Serial_PutString(u32 dwPort,unsigned char *pcStr)
{
	unsigned char *pxNext;
	pxNext = ( unsigned char *)pcStr;
	while(*pxNext)
	{
		//Uart_PutChar(dev, *pxNext, serNO_BLOCK );
	 //	Uart_PutChar(dwPort,*pxNext);
		USART_SendData(USART3, *pxNext);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);	
		pxNext++;
	}
	 
	

}
void Gprs_SerInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = GPRSREC_VEC;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	//USART3 ��ʼ��=========================================================	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		        // USART3 TXD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			    // USART3 RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	
	USART_DeInit(GPRSREC_USART);  //��λ���� 
	USART_InitStructure.USART_BaudRate            = 9600  ;    
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(GPRSREC_USART, &USART_InitStructure);
// 	USART_ITConfig(GPRSREC_USART, USART_IT_RXNE, ENABLE);  //���жϽ���
// 	USART_ClearFlag(GPRSREC_USART,USART_FLAG_TC);
////	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
//	USART_Cmd(GPRSREC_USART, ENABLE);
	USART_ITConfig(GPRSREC_USART, USART_IT_TC, DISABLE);
	USART_ITConfig(GPRSREC_USART, USART_IT_RXNE, DISABLE);
	USART_ITConfig(GPRSREC_USART, USART_IT_IDLE, ENABLE);
	USART_Cmd(GPRSREC_USART, DISABLE); 
			
}
void ConfigGPSDMARec(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	struct GPRS_DEV *ptDev = GetGprsDev();
	//NVIC_InitTypeDef NVIC_InitStructure;
 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	 
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	 
	DMA_DeInit(GPRS_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&GPRSREC_USART->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ptDev->buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = GPRSRECBUFDEEP;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(GPRS_DMA_CHANNEL,&DMA_InitStructure);

	DMA_Cmd(GPRS_DMA_CHANNEL,ENABLE);
	//USART_DMACmd(GPS_USART,USART_DMAReq_Tx,ENABLE);
	USART_DMACmd(GPRSREC_USART,USART_DMAReq_Rx,ENABLE);
    USART_Cmd(GPRSREC_USART, ENABLE); 
	
}