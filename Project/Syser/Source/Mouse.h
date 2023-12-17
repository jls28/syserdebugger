#ifndef _MOUSE_H_
#define _MOUSE_H_
typedef union PS2MousePackage{//PS2������ݵĸ�ʽ
	struct {
		CHAR cFirstData;
		CHAR cXOffset;
		CHAR cYOffset;
	}D2;
	struct {
		CHAR cFirstData;
		CHAR cXOffset;
		CHAR cYOffset;
		CHAR cZOffset;
	}D3;
	struct{
		union
		{
			BYTE Left:1;
			BYTE Right:1;
			BYTE Gesture:1;
			BYTE ReservedBit0:1;
			BYTE Reserved:1;
			BYTE Finger:1;
			BYTE ReservedBit1:2;
			BYTE Byte0;
		};
		union 
		{
			BYTE XMiddle4:4;
			BYTE YMiddle4:4;
			BYTE Byte1;
		};
		union{
			BYTE ZPressure;
			BYTE Byte2;
		};

		union{
			BYTE Left1:1;
			BYTE Right1:1;
			BYTE Gesture1:1;
			BYTE Reserved:1;
			BYTE XHighBit:1;
			BYTE YHighBit:1;
			BYTE ReservedBit3:2;
			BYTE Byte3;
		};
		union{
			BYTE XLowBit;
			BYTE Byte4;
		};
		union
		{
			BYTE YLowBit;
			BYTE Byte5;
		};
	}D6;
	struct{
		union{
			BYTE Left:1;
			BYTE Right:1;
			BYTE W1Bit:1;
			BYTE ReservedBit0:1;
			BYTE W23Bit:2;
			BYTE ReservedBit1:2;
			BYTE Byte0;
		};
		union 
		{
			BYTE XMiddle4:4;
			BYTE YMiddle4:4;
			BYTE Byte1;
		};
		union{
			BYTE ZPressure;
			BYTE Byte2;
		};
		
		union{
			BYTE LU:1;
			BYTE RD:1;
			BYTE W0Bit:1;
			BYTE ReservedBit2:1;
			BYTE XHighBit:1;
			BYTE YHighBit:1;
			BYTE ReservedBit3:2;
			BYTE Byte3;
		};
		union{
			BYTE XLowBit;
			BYTE Byte4;
		};
		union
		{
			BYTE YLowBit;
			BYTE Byte5;
		};
		
	}WModeD6;
	DWORD dwData;
	BYTE chArray[4];
}MouseDataPackage;//PS2������ݵĸ�ʽ

#ifdef __cplusplus
extern "C" {
#endif

extern MouseDataPackage CurrentMouseData;  //��ǰ����ƶ���������
extern MouseDataPackage PrevMouseData;		//��һ������ƶ���������
extern MouseDataPackage MouseBuffer[];
extern BYTE MouseBeginBuffer[];
extern DWORD dwMouseBufferIndex;
extern MouseDataPackage *pMouseDataPackage;
extern DWORD dwMousePrePackageSize;				//����ƶ����ݰ����ֽ���  3  �� 4��
extern DWORD dwMousePackageBeginOffset;			//����������������Windows ��������ݰ����Ҷ̵�ƫ�ƣ�����Ƿ� 0 ����
												//�������˳���Ҫģ���������� Windows���������������Ļ���һΡ�
extern DWORD dwMouseBeginBufferIndex;
extern ULONGLONG gPrevMouseTickCount;
extern ULONGLONG gCurrMouseTickCount;
extern ULONGLONG gPrevLButtonDownTickCount;
extern ULONGLONG gPrevRButtonDownTickCount;
extern ULONGLONG gPrevMButtonDownTickCount;
extern bool gLButtonDblClkSecond;
extern bool gRButtonDblClkSecond;
extern bool gMButtonDblClkSecond;
extern ULONGLONG gMaxDblClkInterval;
extern ULONGLONG gTSCTickCount;

#ifdef __cplusplus
}; // extern "C"
#endif

void TranslateUSBMouse(char*pData,char*pBtStatus);

#endif //_MOUSE_H_