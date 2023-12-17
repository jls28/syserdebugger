#ifndef  _PROCESS_H_
#define  _PROCESS_H_

#include "Tlhelp32.h"

class CProcess
{
public:
	CProcess();
	~CProcess();
public:	
	static	bool	GetProcessUser(HANDLE hProcess, WCHAR*szUserName,int MaxLen);
	static	bool	GetPrivilege(PCSTR privName);
	bool	Start(IN PCSTR ProcessName,PCSTR szCmdLine,IN bool bSuspend = true,PCSTR szDir = NULL);	//�����½���
	bool	Terminate();											//������ǰ����
	bool	SuspendAllThread(IN bool bSuspend = true);				//TRUE :���������߳� FALSE:�ָ������߳�
	bool	ReadMemory(ULPOS Address,void*Buffer,int Size);
	bool	WriteMemory(ULPOS Address,void*Buffer,int Size);
public:
	CHAR	m_ProcessName[MAX_PATH];				//��ǰ������
	DWORD	m_ProcessID;							//��ǰ����ID��
	HANDLE	m_ProcessHandle;						//��ǰ���̾��
};

#endif
