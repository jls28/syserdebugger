#ifndef _VMWARE_SUPPORT_H_
#define _VMWARE_SUPPORT_H_

#ifdef __cplusplus
extern "C" {
#endif


//     Vmware ������ vmware �� vmware tools ͨ�ŵ�һ���ӿڡ�
//     ������ſ��� IO �˿� 0x5658�������������ʱ�����裺
//     EAX = 0x564D5868 ( ��VMXh�� )
//     ECX �� 16 λΪ���ܺš���ʵ��һ�����������������
//     ECX �ĸ� 16 λΪ���ܲ�����
//     EDX = 0x5658 ( ��VX�� )��Ϊ IO �˿ںš�
//
//     0xa   �õ� vmware �汾
//
//     ��л��̱��ṩ����ϸ����
typedef struct _VMVWARE_CALL{
	unsigned long     Return_Status;    //vm_eax  
	unsigned long     Exist_flag;       //vm_ebx
	unsigned long     Func_num;         //vm_ecx
	unsigned long     Port;				//vm_edx
	unsigned long     Trans_Src;		//vm_esi
	unsigned long     Trans_Dest;		//vm_edi
}VMVWARE_CALL,*PVMVWARE_CALL;
void FlushVedioBuffer(int x,int y,int width,int heigh,int command = 0);
extern bool gInVMWARE;
extern bool gIsSupportVMMouse;
bool VMWareTest();
DWORD __stdcall SyserVMWareCall(PVMVWARE_CALL pVMCParam);
bool IsSupportVMMouse();
typedef struct _TRANS_MOUSEDATA{
	USHORT    Priority_flag;
	USHORT    ButtonStatus;
	DWORD   DeltaX;
	DWORD   DeltaY;
	DWORD   DeltaWheel;
}TRANS_MOUSEDATA,*PTRANS_MOUSEDATA;
bool GetVMMouseData(PTRANS_MOUSEDATA pTranMouseData);
#define VMWARESET_CURSOR_POINT_FLAG			0x18
#define VMWARESET_CURSOR_POINT_X			0x19
#define VMWARESET_CURSOR_POINT_Y			0x1a
#define VMWARESET_ACCESS_FLAG               0x1B
void SetVMWareMousePos(DWORD CommandPort,DWORD DataPort,DWORD mousex,DWORD mousey);
void GetVMWareMousePos(DWORD* mousex,DWORD* mousey);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif