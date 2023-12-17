#ifndef _TIMERINTERRUPT_H_
#define _TIMERINTERRUPT_H_
//////////////////////////////////////////////////////////////////////////
///     ������������ 8253 ʱ���ж��� ioapic �е��жϺ��� 2
///     ��finaldoom �ҵĻ����� 8253 ʱ���ж��� ioapic �е��жϺ��� 0
//////////////////////////////////////////////////////////////////////////
#define IOAPIC_TIMER_INTERRUPT_NUM			0
										
void Interrupt_0xFA_Timer_Service();
bool UninstallTimeInterrupt();
bool InstallTimeInterrupt();
void TimeInterrupeService();
ULONG  CalibrateByTimer();

#ifdef __cplusplus
extern "C" {
#endif
extern ULONG gSystemTimerFrequency;
extern ULONGLONG gSystemTickCount;
extern BYTE byteOldRTC_Register_A;
extern BYTE byteOldRTC_Register_B;
extern BYTE TimeSecond;
extern BYTE TimeHour;
extern BYTE TimeMinute;
extern bool bIsExecuteTimerInterrupt;
extern DWORD gdwOldTimerInterrupt;
extern DWORD gdwTimerInterruptNum;
ULONGLONG InitSTCTickCount();
extern ULONGLONG gTSCTickCount;
extern ULONGLONG gPrevTSC;
#ifdef __cplusplus
}; // extern "C"
#endif

#endif //_TIMERINTERRUPT_H_