#ifndef _NTSTATSTR_H_
#define _NTSTATSTR_H_
typedef struct _NTSTATUS_STR
{
	const char* Description;
	const char* Name;
	int Code;
}NTSTATUS_STR,*P_NTSTATUS_STR;
extern NTSTATUS_STR gNtstatusTable[];
#endif //_NTSTATSTR_H_