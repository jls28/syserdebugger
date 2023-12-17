#ifndef _HANDLE_H_
#define _HANDLE_H_

NAME_SPACE_BEGIN
///////////////////////////////////���λ�ֲ�////////////////////////////////////////////////////
//[                  �������                   ] [�������ҳĿ¼ ] [        ������ڲ�         ]
//1F 1E 1D 1C 1B 1A 19 18 17 16 15 14 13 12 11 10 0F 0E 0D 0C 0B 0A 09 08 07 06 05 04 03 02 01 00
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef DWORD	HANDLE_TYPE;

#define	INVALID_HANDLE			0x00			//��Ч���
#define	HANDLE_DIR_COUNT		0x40			//������ҳ��
#define	HANDLE_SLOT_COUNT		0x400			//ÿҳ���������
#define	HANDLE_TYPE_COUNT		0x40			//���ǼǾ����
#define HANDLE_EXIST			0x00000001		//���æ��־
#define HANDLE_BUSY				0x00000002		//���æ��־
#define HANDLE_DUP				0x00000004		//����Ƿ�ɸ���
#define HANDLE_INHERIT			0x00000008		//����Ƿ�ɼ̳�
#define MAX_HANDLE_TYPE			0x100			//��������͸���
#define INVALID_HANDLE_TYPE     0

struct HANDLE_INFO
{
	void*	HandleInfo;
	DWORD   Attribute;
};

typedef	bool(*REL_HI_FUNC)(void* Param,void* pInfo);

struct HANDLE_TYPE_INFO
{
	REL_HI_FUNC RelHIFunc;
	void*	Param;
	bool	bExist;
};

#define CHandle			CCodeHandle

class CHandle
{
	HANDLE_INFO*		m_HandleInfoDir[HANDLE_DIR_COUNT];
	HANDLE_TYPE_INFO	m_RegisterType[MAX_HANDLE_TYPE];
public:
	CHandle();
	HANDLE_TYPE	CreateHandle(void* HandleInfo,DWORD Type);
	bool	CloseHandle(HANDLE_TYPE Handle);
	void	CloseAllHandle();
	void	Reset();
	HANDLE_TYPE	GetFirstHandle(DWORD Type);
	DWORD	GetHandleType(HANDLE_TYPE Handle);
	bool	IsHandleExist(HANDLE_TYPE Handle);
	void*	GetHandleInfo(HANDLE_TYPE Handle);
	HANDLE_TYPE  GetNextHandle(HANDLE_TYPE Handle);
	DWORD	Register(REL_HI_FUNC RelHIFunc = NULL,void* Param = NULL);
	bool	Unregister(DWORD Type);
};

NAME_SPACE_END

#endif

