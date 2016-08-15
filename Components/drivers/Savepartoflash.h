#ifndef __SAVEPARTOFLASH_H
#define __SAVEPARTOFLASH_H
#include "stm32f10x.h"

/*
���ýṹ���С������4�ֽ��������������벹��
�磺
*/
///////////////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
   uint16_t  Para1;   
   uint16_t  Para2; 
   uint16_t Para3; 
   uint16_t  backup;//ռλ,�ܴ�С����˫��         
}WORKPARA_STRUCT;

//xl 768kflash 0xc0000 
#define CONFIGDATAADDR   0x080BF000 //hd��Ʒflash  ���һҳ

//ʹ���ڲ�Flash ISP �迪���ڲ����پ���


 typedef struct  //ע��ṹ���ֽڶ��� 
{
	char	   	GPRSSeverAddr[32];
	uint8_t		SensorState[16];

	uint8_t 	ucSampSwitch;	
	uint8_t 	ucBLEmode;
	uint8_t    	ucGPRS_Switch;
	uint8_t   	ucDataUpmode;
	
	uint8_t 	ucSampletime; 
	uint8_t     backup;
	uint8_t   	DeviceID[2];//
	
	uint8_t 	VerPCB;
	uint8_t		VerSystem;
	uint8_t		backup1;
	uint8_t		backup2;
 
	//	backup;//ռλ,�ܴ�С����˫��         
}ParameterStruct ;

void CFG_SaveWorkPara( ParameterStruct *ptPar);
void CFG_LoadWorkPara(void); 
void ParameterDefault(void);
int GetSetedPar(ParameterStruct *ptPar);
#endif
