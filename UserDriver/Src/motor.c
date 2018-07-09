
#include "motor.h"

/*---------------------------------------------------------------------
 * ����LED�õ���I/O��
 */
void Motor_IO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;  	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   

	/* GPIOB8-9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          
	GPIO_Init(GPIOB, &GPIO_InitStructure);     

	GPIO_ResetBits(GPIOB,GPIO_Pin_6);     //�����Ϊ�͵�ƽ   оƬ���Ϊ����״̬
	GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
}

/*---------------------------------------------------------------------
 * �շ�����ܿ��ƣ�������ε���ȡ�þ���ֵ����ʱ��Ϊ7.1s�պò���ʹ�����ס
 * �½�ʱ��Ϊ6.6s�պò���ʹ�����ס
 */
	void Undercarriage_MotorControl(volatile u16 *ticks, u16 n)
	//void Undercarriage_MotorControl(void)
{
    static u8 s = 0;

    //if(c++ > 0)	//��һ�ν��뺯����Ч
		//{
			if(s++%2)	//�����ν��뺯�������������½�
			{  
				RIGHT_LEG_UP;
				LEFT_LEG_UP;
//				LED_OFF;
				ticks[2] = 8500;	//7.1s �½�ʱ��
				//delay(t_1s);
			}
			else     //ż���ν��뺯����������������
			{				
				RIGHT_LEG_DOWN;
				LEFT_LEG_DOWN;
				ticks[1] = 8500;	//6.6s	//�¼�ʱ��Ҫ��������ʱ��
//				LED_ON;
			//	delay(t_1s);
			} 
			//c = 1;
   // }
        
    /* �����ִ���շ�ʱ����Ӧ��ָ��*/
    while(ticks[1]||ticks[2])
		{  
			/*led on*/
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);  
    }
    
    RIGHT_LEG_STOP;
    LEFT_LEG_STOP;
}


///*---------------------------------------------------------------------
// * �շ�����ܿ���
// */
//void Undercarriage_MotorStart(volatile u16 *ticks, u16 n, u8 direction)
//{  
//    if(direction == LEG_UP){
//        //RIGHT_LEG_UP;
//        //LEFT_LEG_UP;
//    }else if(direction == LEG_DOWN){
//        //RIGHT_LEG_DOWN;
//        //LEFT_LEG_DOWN;     
//    } 
//    /*led */
//    GPIO_ResetBits(GPIOB,GPIO_Pin_0);  
//    
//    ticks[1] = n;
//}


///*---------------------------------------------------------------------
// * �շ�����ܿ���
// */
//void Undercarriage_MotorStop(volatile u16 *ticks)
//{   
//    if(!ticks[1]){
//        RIGHT_LEG_STOP;
//        LEFT_LEG_STOP;
//        GPIO_SetBits(GPIOB,GPIO_Pin_0);
//    }  
//}



/*********************************************************************************/
