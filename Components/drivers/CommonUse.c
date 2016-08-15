#include "stm32f10x.h"
#include "CommonUse.h"
/**********************************************************
* �������� ---> HEX תΪBCD��
* ��ڲ��� ---> BCD_Data��Ҫת����BCD����
* ������ֵ ---> HEX��
* ����˵�� ---> none
**********************************************************/	
uint8_t HEX_to_BCD(uint8_t HEX_Data)
{
	return((HEX_Data / 10) << 4 | (HEX_Data % 10));
}
/**********************************************************
* �������� ---> BCD��תΪHEX
* ��ڲ��� ---> HEX_Data��Ҫת����BCD����
* ������ֵ ---> BCD��
* ����˵�� ---> none
**********************************************************/	
uint8_t BCD_to_HEX(uint8_t BCD_Data)
{
	return((BCD_Data >> 4) * 10 + (BCD_Data & 0x0f));
}
/**********************************************************
* �������� ---> 10������תΪ16����
* ��ڲ��� ---> DX_Data��Ҫת����10��������
* ������ֵ ---> 16����
* ����˵�� ---> none
**********************************************************/
uint16_t DX_to_HX(uint16_t DX_Data)
{
	return(((DX_Data/1000)<<12) | ((DX_Data%1000/100)<<8) | ((DX_Data%100/10)<<4) | (DX_Data%10));
}
/**********************************************************
* �������� ---> 16������תΪ10����
* ��ڲ��� ---> HX_Data��Ҫת����16��������
* ������ֵ ---> 10����
* ����˵�� ---> none
**********************************************************/
uint16_t HX_to_DX(uint16_t HX_Data)
{
	return((HX_Data>>12)*1000+((HX_Data&0x0f00)>>8)*100+((HX_Data&0x00f0)>>4)*10+(HX_Data&0x000f));
}	

// src  Դ���� ,src_len Դ���鳤��,const uint8 * search ������  search_len �����鳤��
  int Search_SubArray(const uint8_t* src,uint16_t src_len,const uint8_t * search,uint16_t search_len)
{
	uint16_t i,j;
	if(src_len < search_len)
		return -1;
	if((src_len == 0)||(search_len == 0))
		return -1;
	
	for(i = 0; i< src_len;i++) 
	{
		if(src[i] == search[0])
		{
			for(j=0;j< search_len;j++)
			{
				if(search[j] == src[j+i])
					continue;
				else
					break;					
			}
			if(j == search_len)
			{
				return 0;
			 
			}
		}
	}
	return -1;
	
}
