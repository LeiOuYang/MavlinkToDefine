
#ifndef CONTROL_HANDLE_H
#define CONTROL_HANDLE_H

/*** �����ƶ�������ֵ����Сֵ������ ***/
#define OBJ_MAX	0x32       
#define OBJ_MIN 0

typedef struct _control_handle
{
	int controlMid;    //ң���м�λ����
	int controlMax;		//�������
	int controlMin;    //��С����
	int currentVal;    //��ǰֵ
}controlHandle;

typedef enum _positon 
{
	LEFT = 0,
	RIGHT = 1	
}handlePosition;

static controlHandle controlHandles[2];   //��  ��

	/**************************************
	*   ����ң�еĳ�ʼ����
	*
	***************************************/
	void setControlHandleValue(controlHandle *phandle, int min, int mid, int max)
	{
		if(0==phandle||min<0||mid<=0||max<=0) return;
		phandle->controlMin = min;
		phandle->controlMid = mid;
		phandle->controlMax = max;
	}
	
	/**************************************
	*   ˮƽλ��ң��
	*
	***************************************/
	controlHandle* getControlHandle(unsigned char position)
	{
		return &controlHandles[position];
	}
	
	/**************************************
	*   ���ص�ǰֵ��Ӧ�Ŀ�����,
	*   ͨ����������ֵ���жϷ���
	***************************************/
	int getControlHandleVal(controlHandle *phandle)
	{
		float realVal = 0.0;
		
    realVal = (float)(phandle->currentVal-(phandle->controlMid));
		realVal = realVal*( (OBJ_MAX-OBJ_MIN)*1.0 / (phandle->controlMax-phandle->controlMid) );
		return (int)(realVal+0.5);	   //��������	round����
	}

#endif
