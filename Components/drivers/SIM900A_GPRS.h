// #include "AllStruct.h" 
#ifndef __SIM900A_GPRS_H
#define __SIM900A_GPRS_H
#include "stm32f10x.h"
 



#define GPRS_SER_PORT  serCOM3
#define CENTERPHONEON   "18351871133\0"//�����ֻ�����
#define SQRSETSMSTEXT   "SMS Format of Server Addr:\"CMD<NEWADDR>\",\"len(IP+Port+5)<22>\",\"IP<192.168.1.1>\",\"Port<9000>\"\0"//�����������

#define GPRSRECBUFDEEP  128 //�������ݻ�����
#define GPRSTRABUFDEEP	256//����֡�Ļ�����

#define GPRSDATABUFTIME 30000   //��������
#define HEATBEATPERIOD  60000   //��������
#define AUTOHEARTPACTET 1        //�Զ�����������
#define HEARTPACKETSTRING  "\0\1"//���������ݣ�Ĭ�Ϸ�4���ֽڳ���

 
 
 
 #define GPRS_PORT  GPIOD
 #define GPRS_PWR_PIN		GPIO_Pin_14
 #define GPRS_RESET_PIN     GPIO_Pin_13
 #define GPRS_STATUS_PIN	GPIO_Pin_15
 #define GPRS_RI_PIN		GPIO_Pin_12
 //GPRSģ���õĿ���IO
#define GPRS_POWERKEY_H   GPIO_SetBits(GPRS_PORT, GPRS_PWR_PIN)		  //ģ�鿪�أ�SIM900A���ػ���Ƽ�ʱ��  PWRKEY �����ڲ����������裬������IO��λ���͵�ƽ����·���缫Ϊ�ߣ����ϵ� ��
#define GPRS_POWERKEY_L	  GPIO_ResetBits(GPRS_PORT, GPRS_PWR_PIN)
#define GPRS_RESET_H	  GPIO_SetBits(GPRS_PORT, GPRS_RESET_PIN)		  //��λ
#define GPRS_RESET_L	  GPIO_ResetBits(GPRS_PORT, GPRS_RESET_PIN)
#define GSM_RI_VAL	      GPIO_ReadInputDataBit(GPRS_PORT,GPRS_RI_PIN)  //�жϽ�������
#define GPRS_STATUS_VAL   GPIO_ReadInputDataBit(GPRS_PORT,GPRS_STATUS_PIN)  //ģ��״̬

#define GPRSMDBYTEGAP      100
//#define HEATBEATPERIOD     60

#define GPRSCONFIGTRYTIMES 3

#define GRPS_ModuleError   0x0001
#define GRPS_SIMError      0x0002
#define GRPS_GPRSNETWORK   0x0004
#define GRPS_CONNECTION    0x0008
/***********
**AT+CPIN?   AT+CSQ?   AT+CGATT?
** AT+CSTT  AT+CIICR  AT+CIFSR 
**AT+CIPSTART="TCP","IP",PORT
**
**
**
**
**
**
*****/
//#define HEATBEATPERIOD     60
//  typedef enum 
//  {
//     INIT0,
//     SIM900_POWERON,
// 	AT_TEST,
// 	GPRS_OPEN_FINISH,       /// GPRS �򿪳ɹ���
//     SIMCARD,	// ����Ƿ����sim��
//     INIT_GPRS,
// 	GPRS_CONFIG0,	// CGCLASS �����ƶ�̨���
// 	GPRS_CONFIG1, 	//CGDCONT
// 	GPRS_CONFIG2, 	//CGATT
// 	GPRS_CONFIG3, 	//CGACT
// 	 TCPIP_SENDAUTO,	//�Զ���������
// 	TCPIP_CGREG,	// ���gprs����״̬ CGREG
//     TCPIP_HEAD_CONF,// ���������Ƿ���ʾIP��ַͷ
// 	TCPIP_TOCONNECTING,	// ����tcp����
//     TCP_IP_OK,//		tcp���ӳɹ�
//     TCP_IP_NO,//		tcp���Ӳ��ɹ�
//     TCPIP_CONNECTING,
//     SENDDATA_START,		// ��������
//     SENDDATA_ING,		// ���ݷ��͹�����
//     TCPIP_CLOSE_START,	// �رյ�ǰtcp����
//     TCP_IP_CLOSE_OK,
//     TCP_IP_CLOSE_NO,
// 	SIM900_POWRDOWN		//�ػ�
// }SIM900A_STATUS ;

struct GPRS_DEV
{
//	SemaphoreHandle_t pSem;
	u8 ucUartPort;
	u8 ucRecDataFlag;
	u8 buf[GPRSRECBUFDEEP];
	
	u16 wLen;
	u16 wReadPos;
	u32 dwStatus;
	u32 dwCmdAck;

	int (*Init)(struct GPRS_DEV *dev); 
	int (*Open)(struct GPRS_DEV *dev);
	int (*Close)(struct GPRS_DEV *dev,uint8_t bCloseMode);
	int (*SendCmd)(struct GPRS_DEV *dev, u8 *pCmd);
	int (*Read)(struct GPRS_DEV *dev, u8 *pBuf, u16 *pwLen);
	int (*Write)(struct GPRS_DEV *dev, u8 *pBuf, u16 wLen);
	int (*GetConnetStatus)(struct GPRS_DEV *dev);
	int (*GetSignal)(struct GPRS_DEV *dev,u8* dwVal);
	int (*HandleRecv)(struct GPRS_DEV *dev, u8 *pBuf, u16 wLen);
};

#define GPRS_INIT0						0x0000
#define GPRS_POWERON  					0x0001
#define GPRS_SIM_CARD  					0x0002
#define GPRS_TCP_CONFIG	  				0x0004
#define GPRS_TCP_CONNECTING  			0x0010
#define GPRS_TCP_OK						0x0020
#define GPRS_TCP_CLOSED  				0x0040
#define GPRS_POWERDOWN 					0x0080
struct GPRS_DEV *GetGprsDev(void);
int Gprs_Init(void);
int Gprs_Open(void);
int Gprs_SimCardCheck(void);
int Gprs_TcpConfig(void);
int Gprs_TcpConnect(void);
int Gprs_TcpClose(void);
int Gprs_Close(void);
int Gprs_SendCmd(u8 *pCmd, u16 wCmdLen);
int Gprs_Write(u8* pBuf, u16 wLen);
int Gprs_Read(u8* pBuf, u16 *pwLen);
int Gprs_GetConnectStatus(u32 *pdwStatus);

int Gprs_GetRecStatus(u8 *pucStatus);

void StartGprsRecvTasks(void);
  void  vGprsReceiveTask( void  );

#endif
