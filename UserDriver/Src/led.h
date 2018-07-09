
#ifndef LED_H
#define LED_H

#include "stm32f10x.h"
#include "Data_Type.h"

#define ON 0     //LED�͵�ƽ����
#define OFF 1

#define LED PBout(0)   //λ����  PB0 �˿�
#define LED_Action(x)  {LED=x;}   //����LED
#define LED_CLK_SOURCE   RCC_APB2Periph_GPIOB   //ʱ������

/* LED  IO���� */
#define LED_IO 		GPIO_Pin_0
#define LED_IO_CLASS	GPIOB

void LED_IO_Config(void);
unsigned int getFlashingTime(void);
void setFlashingTime(unsigned int timeMs);


#endif

