

/*** STM32��ص��ļ� ***/

#include "stm32f10x.h"


/*** �û���ص��ļ� ***/
#define	DELAY_10MS	1	  //ϵͳʱ��Ϊ9MHZ���ж�ʱ��Ϊ10ms

#include "Data_Type.h"
#include "stm32f10x_driver.h"
#include "user_tasks.h"


//static __IO uint32_t TimingDelay;
//void TimingDelay_Decrement(void);
//void Delay(__IO uint32_t nTime);

///*** ϵͳʱ���ӳٺ��� ***/
//void TimingDelay_Decrement(void)
//{
//  if (TimingDelay != 0x00)
//  { 
//    TimingDelay--;
//  }
//}

///*** ����ϵͳʱ���ж�ʵ���ӳٺ��� ***/
//void Delay(__IO uint32_t nTime)
//{ 
//  TimingDelay = nTime;

//  while(TimingDelay != 0);
//}


