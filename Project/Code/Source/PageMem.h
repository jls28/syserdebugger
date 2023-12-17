#ifndef _PAGE_MEMORY_H_
#define _PAGE_MEMORY_H_

#include "ImageFile.h"

NAME_SPACE_BEGIN

#define	PMS_NO_PAGE_OUT		0x00000000
#define	PMS_PAGE_OUT		0x00000001
#define PMS_DYN_PAGE		0x00000002

struct MEMORY_PAGE
{
	BYTE*	PhysAddr;
	DWORD	Attribute;
	DWORD	AccessReadCount;		//�����ʼ���
	DWORD	AccessWriteCount;		//д���ʼ���
	DWORD	AccessExecuteCount;		//ִ�з��ʼ���
	DWORD	AccessLoaderWriteCount; //װ����д���ʼ���
	DWORD	PageData;
};

#define		PAGE_DIR_CONTENT	0x400
#define		PAGE_DIR_COUNT		0x400
#define		PHYS_PAGE_EXIST		1
#define		PHYS_PAGE_NULL		0

struct MEMORY_PAGE_CACHE
{
	MEMORY_PAGE*MemoryPage;
	DWORD		BaseAddr;
	DWORD		AccessCount;
	bool		bExist;
};

typedef TRangeMap<ULPOS,CPageImage*>			CImgRangeMap;
////////////////////////////////////32�����ַλ�ֲ�/////////////////////////////////////////////
//[     ҳĿ¼����(10BIT)     ] [        ҳ����(10BIT)      ] [       ��ַ����ҳƫ��(12BIT)     ]
//1F 1E 1D 1C 1B 1A 19 18 17 16 15 14 13 12 11 10 0F 0E 0D 0C 0B 0A 09 08 07 06 05 04 03 02 01 00
/////////////////////////////////////////////////////////////////////////////////////////////////

class CPageMemory
{
public:
	CPageMemory();
	virtual ~CPageMemory();
	MEMORY_PAGE_CACHE m_PageDataCache1;			//���ݶ�д����1
	MEMORY_PAGE_CACHE m_PageDataCache2;			//���ݶ�д����2
	int			m_PageUsedCount;				//ҳ����
	int			m_PageCount;					//ҳ����
	BYTE*		m_PhysMemory;					//�����ڴ�
	BYTE*		m_PageExistMap;					//ҳĿ¼
	MEMORY_PAGE*m_PageDir[PAGE_DIR_COUNT];		//ҳĿ¼
	int			m_WriteAccessPageCount;			//д�����ڴ�ҳ����
	int			m_FreePhysIndex;
	UINT		m_Style;
	CImgRangeMap	m_ImageRangeMap;
public:
	virtual	bool	Init(int PageCount,UINT Style);
	virtual	void	Release();
	virtual	void	RefreshCache(MEMORY_PAGE*pPage);
	virtual	bool	LoadPage(ULPOS Address,MEMORY_PAGE*pPage);		//��ַ������ҳ��С�ı���
	virtual	bool	SavePage(ULPOS Address,MEMORY_PAGE*pPage);
	virtual	bool	ExchangePage();
	virtual	int		FlushAllPage();
	virtual	bool	MapImage(CPageImage*pImageFile);
	virtual	bool	UnmapImage(CPageImage*pImageFile);
	MEMORY_PAGE*GetDataPage(ULPOS Address,bool bWrite = false);		//���ݵ�ַ�õ�����ҳ
	CPageImage*	GetImage(PCSTR ModuleName);
	CPageImage*	GetImage(ULPOS ModuleBase);
	bool		AllocatePage(ULPOS Address,UINT Attribute=MP_READ|MP_WRITE,void*Buffer=NULL,ULSIZE Size=MP_SIZE);
	bool		AllocatePage(ULPOS Address,UINT Attribute,BYTE PageData);
	bool		ReleasePage(ULPOS Address);
	DWORD		ReleaseClearPage();
	bool		IsPageExist(ULPOS Address);
	bool		IsPageDirExist(ULPOS Address);
public:
	static ULSIZE  ReadMemory(ULPOS Address,void* Buffer,ULSIZE Size,CPageMemory*pPageMemory);
	static ULSIZE  WriteMemory(ULPOS Address,void* Buffer,ULSIZE Size,CPageMemory*pPageMemory);
	ULSIZE		ReadString(ULPOS Address,PSTR Buffer,ULSIZE MaxLen);
	ULSIZE		ReadString(ULPOS Address,PWORD Buffer,ULSIZE MaxLen);
	ULSIZE		ReadString(ULPOS Address,PDSTR Buffer,ULSIZE MaxLen);
	ULSIZE		WriteString(ULPOS Address,PSTR Buffer,ULSIZE MaxLen);
	ULSIZE		WriteString(ULPOS Address,PWORD Buffer,ULSIZE MaxLen);
	ULSIZE		WriteString(ULPOS Address,PDSTR Buffer,ULSIZE MaxLen);
	ULSIZE		ReadMemX(ULPOS Address,void* Buffer,ULSIZE Size);
	ULSIZE		WriteMemX(ULPOS Address,void* Buffer,ULSIZE Size);
	ULSIZE		ZeroMemX(ULPOS Address,ULSIZE Size);
	bool		ReadMemB(ULPOS Address,void* Buffer);
	bool		ReadMemW(ULPOS Address,void* Buffer);
	bool		ReadMemD(ULPOS Address,void* Buffer);
	bool		WriteMemB(ULPOS Address,void* Buffer);
	bool		WriteMemW(ULPOS Address,void* Buffer);
	bool		WriteMemD(ULPOS Address,void* Buffer);
protected:
	bool		NewPage(MEMORY_PAGE*pPage);
	bool		DeletePage(MEMORY_PAGE*pPage);
};

NAME_SPACE_END

#endif 

