
#ifndef TIMER_H
#define TIMER_H

	#include "stm32f10x.h"
	#include "data_type.h"
	
  extern __IO unsigned int  timer2Delay;
	
	void timer2Config(void);			//��ʱ�����ú������жϡ�IO��������
	void TIM1_PWMConfig(void);		//�߼���ʱ��TIM1����:IO�ڡ���ʱ��
	
#endif
