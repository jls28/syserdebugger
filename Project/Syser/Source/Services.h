#ifndef _SERVICES_H_
#define _SERVICES_H_

#ifdef __cplusplus
extern "C" {
#endif

#define			FPU_REGISTER_IMAGE_SIZE	108
extern X86_GATE_DECR	gOldInterruptDescropterTable[0x100];
extern DWORD			gOldInterruptProcAddress[0x100];
extern bool				bGoToSoftice;
extern bool				gbSystemCrash;
extern bool				gbActive;
extern DWORD			dwTestSampleEntryPoint;
extern DWORD			gSofticeDriverEntry;
extern DWORD			gSofticeDriverEntryReturnAddress;
extern DWORD			OldESP;
extern DWORD			CurrentFpuImageIndex;
extern DWORD			gGenInterruptNumber;
extern DWORD			gGenInterruptOffset;

extern DWORD			SyserDR[8];
extern bool				gbTemplateTraceFlag;//��ʱ����һ�������ϵ�
extern BYTE*			gpStackBufferBottom;
extern BYTE*			gpStackBufferTop;
extern BYTE*			gpStackBuffer;
extern BYTE				FpuRegisterImage[2][FPU_REGISTER_IMAGE_SIZE];
extern BYTE				FxsaveBuffer[512+15];
extern WORD				gSystemSSSegment;
extern WORD				gGenInterruptSelector;
extern DWORD			gPrevStepEIP;//ǰһ�ε���ִ�е� EIP ��ַ������Ҫ����һЩ��������������ǰһ��ִ�еĵ��������� pushfd ��ô��Ҫ�����ջ�е� tf ��־
										//�����һ���˳���ʱ���ǵ����˳��ġ���ñ���Ϊ 0 
extern bool				gbBugCheckExExit;
extern bool				gbBugCheckExFlags;
extern DWORD			gpBugCheckExAddress;
extern bool				gbBugCheckExFirstByteIsCC;
extern LONG				gbLoadImage;

VOID SyserLoadReg(VOID);
VOID SyserSaveReg(VOID);
//��ʧ���쳣
VOID SyserInt0bProc(VOID);
//
VOID SyserInt06Proc(VOID);
//
VOID SyserInt0cProc(VOID);
//�����ж�
VOID SyserInt01Proc(VOID);
//�����ж�
VOID SyserInt2dProc(VOID);

//���̴�������
void CreateProcessHookProc(IN HANDLE  ParentId,IN HANDLE  ProcessId,IN BOOLEAN  Create);
//�̴߳�������
void CreateThreadHookProc(IN HANDLE  ProcessId,IN HANDLE  ThreadId,IN BOOLEAN  Create);
//ģ��װ�ع���
void LoadImageHookProc(IN PUNICODE_STRING  FullImageName,IN HANDLE  ProcessId,IN PIMAGE_INFO  ImageInfo);

void DebugOut(void);
void DebugOut1(DWORD dwHeight);
void WispTimeInterrupeService();

bool InstallSwapContextHook();
void RemoveSwapContextHook();
void SaveFpuRegister();
void RestorFpuRegister();

VOID SyserDisableDebugRegisterMontior(VOID);
VOID SyserEnableDebugRegisterMontior(VOID);
void EnableHardwaveBreakpoint();
VOID SyserDisableHardwaveBreakpoint(VOID);

VOID SyserSofticeDriverEntryExitCallBack(VOID);
VOID SyserSaveExceptionRegister(VOID);
void ClearExceptionRegister();
void OnPatchBugCheckEx();
void OnPatchMmUnmapViewOfSection();
void OnDebugSoftice();
void OnPatchTranslateUsageAndPagesToI8042ScanCodes();
int SyserCloseDebugRegisterMontior(VOID);
VOID SyserOpenDebugRegisterMontior(VOID);
//void PatchKeBugCheckEx();

typedef struct _KD_SYMBOLS_INFO 
{
	IN PVOID BaseOfDll;
	IN ULONG_PTR ProcessId;
	IN ULONG CheckSum;
	IN ULONG SizeOfImage;
} KD_SYMBOLS_INFO, *PKD_SYMBOLS_INFO;

void UnloadSystemModuleSymbols(PSTRING pModuleName,PKD_SYMBOLS_INFO pSymInfo);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif


