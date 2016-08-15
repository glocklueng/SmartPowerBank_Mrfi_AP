/**************************************************************************************************
  Revised:        $Date: 2007-07-06 11:19:00 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Copyright 2007 Texas Instruments Incorporated.  All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS�
  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *   BSP (Board Support Package)
 *   Top-level BSP code file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "bsp.h"
#include "bsp_driver_defs.h"
#include "stm32f10x_tim.h"
uint32_t SysTicksVal32Bit;
void TIM2_IRQHandler(void)
{
	((uint16_t*)&SysTicksVal32Bit)[1] += 1;
	((uint16_t*)&SysTicksVal32Bit)[0] = (uint32_t)TIM2 -> CNT;
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//����жϱ�־
}
//�ú����������жϺ����ڵ���
uint32_t PDInit_GetSysTick(void)
{
	 ((uint16_t*)&SysTicksVal32Bit)[0] = (uint32_t)TIM2 -> CNT;
	 return(SysTicksVal32Bit);	
}

void RCC_Configuration(void)
{
	//�ο���ַ��http://blog.csdn.net/iamlvshijie/article/details/9249545
	ErrorStatus HSEStartUpStatus; 
	RCC_DeInit();     //����RCC�Ĵ�����������ΪĬ��ֵ
	RCC_HSEConfig (RCC_HSE_ON);//���ⲿ����ʱ�Ӿ���HSE��
	HSEStartUpStatus = RCC_WaitForHSEStartUp();//�ȴ��ⲿʱ�Ӿ����ȶ�����
	if(HSEStartUpStatus == SUCCESS)//SUCCESS��HSE�����ȶ��Ҿ���
	{
		//FLASH_PrefetchBufferCmd(ENABLE);          //����FLASH��Ԥȡ����
		//FLASH_SetLatency(FLASH_Latency_2);      //FLASH�ӳ���������
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//����AHBʱ�Ӳ���Ƶ
		RCC_PCLK2Config(RCC_HCLK_Div1); //����APB2ʱ�Ӳ���Ƶ
		RCC_PCLK1Config(RCC_HCLK_Div2); //����APB1ʱ�Ӷ���Ƶ ��Ӧ�Ķ�ʱ����ʱ��Ϊ2��Ƶ
//	FLASH_SetLatency(FLASH_Latency_2);//����FLASH�洢����ʱ��������FLASH_Latency_2 2��ʱ����
//	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//ѡ��FLASHԤȡָ�����ģʽ��Ԥȡָ����ʹ��
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//����PLL  11.0592�屶Ƶ=55.296��
		RCC_PLLCmd(ENABLE); //PLLʱ��ʹ��
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource()!=0x08);//�ȴ�������
	}
	
	 RCC_LSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
 
	//��������˿�ʱ��
	//ʹ��PA~PE�˿�ʱ��IN1~IN32
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 
//	//��������1��2��3ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2��USART3ʱ��
//	
	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	//������ʱ��
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//
}
// USE TIMER2
void BSP_INIT_SYS_TICKS(void)  
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	  NVIC_InitTypeDef NVIC_InitStructure;
	  /* Enable the TIM2 gloabal Interrupt */
	  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);	
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_DeInit(TIM2);                                           //���½�Timer����Ϊȱʡֵ
       
    TIM_InternalClockConfig(TIM2);                              //�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ      
    TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;               //Ԥ��Ƶϵ��Ϊ36000-1������������ʱ��Ϊ72MHz/36000 = 2kHz       
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�      
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ü�����ģʽΪ���ϼ���ģʽ       
    TIM_TimeBaseStructure.TIM_Period = 0xffff;                  //���ü��������С�� �Ͳ���һ�������¼�
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);              //������Ӧ�õ�TIM2��

	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//����жϱ�־
	  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//���ж�

	  TIM_Cmd(TIM2, ENABLE);	
}
/**************************************************************************************************
 * @fn          BSP_Init
 *
 * @brief       Initialize the board and drivers.
 *
 * @param       none
 *
 * @return      none
 **************************************************************************************************
 */
void BSP_Init(void)
{
	RCC_Configuration();
	BSP_INIT_BOARD();
	BSP_INIT_DRIVERS();
	Sys_delay_init();
	BSP_INIT_SYS_TICKS()  ;
  /*-------------------------------------------------------------
   *  Run time integrity checks.  Perform only if asserts
   *  are enabled.
   */
#ifdef BSP_ASSERTS_ARE_ON
	/* verify endianess is correctly specified */
	{
		uint16_t test = 0x00AA; /* first storage byte of 'test' is non-zero for little endian */
		BSP_ASSERT(!(*((uint8_t *)&test)) == !BSP_LITTLE_ENDIAN); /* endianess mismatch */
	}
#endif
}


/* ================================================================================================
 *                                        C Code Includes
 * ================================================================================================
 */
#ifdef BSP_BOARD_C
#include BSP_BOARD_C
#endif

#ifdef BSP_DRIVERS_C
#include BSP_DRIVERS_C
#endif


/* ************************************************************************************************
 *                                   Compile Time Integrity Checks
 * ************************************************************************************************
 */
BSP_STATIC_ASSERT( sizeof(  uint8_t ) == 1 );
BSP_STATIC_ASSERT( sizeof(   int8_t ) == 1 );
BSP_STATIC_ASSERT( sizeof( uint16_t ) == 2 );
BSP_STATIC_ASSERT( sizeof(  int16_t ) == 2 );
BSP_STATIC_ASSERT( sizeof( uint32_t ) == 4 );
BSP_STATIC_ASSERT( sizeof(  int32_t ) == 4 );


/**************************************************************************************************
 */
