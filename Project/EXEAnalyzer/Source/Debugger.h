#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

enum BP_STATE
{
	BP_STATE_DISABLE=0,				//��Ч
	BP_STATE_ENABLE,				//��Ч
	BP_STATE_RECOV,					//Ҫ�ָ��Ķϵ�
	BP_STATE_RECOV_COPY_ON_WRITE,	//�������ַ�ı�Ҫ�ָ��Ķϵ�
	BP_REV_BPR_TYPE = 0x40000000,	//Ҫ�ָ��� BPR ���͵Ķϵ㣬��Ҫ������ҳ�� P ����
	BP_SET_BPR_TYPE = 0x80000000    //�Ѿ������� BPR ���͵Ķϵ� 
};

typedef struct _BP_RANGE_INFO
{
	DWORD	RangeStartAddress;
	DWORD	RangeLength;
	DWORD	State;	
	DWORD	Type; //1 read ,2 write, 4 execute  read and write	
	DWORD	PTEAddress;
	WORD   PTESize;
	WORD   PTECount;
	DWORD	PageSize;
	CStrA	Condition;					//�������ڶϵ�ʱ�Ƚ�
	CStrA	DoCmd;						//����ʱִ�е�����
}BP_RANGE_INFO;


struct MODULE_BP
{
	CIStrA		ModuleFileName;
	CStrA		Condition;
	CStrA		Name;
	CStrA		DoCmd;						//����ʱִ�е�����
	BP_STATE	State;
};

typedef struct _BREAK_POINT
{
	CStrA		Name;						//����������ʾ
	CStrA		Description;				//����,���ݶϵ������������RW,W,X
	CStrA		Condition;					//�������ڶϵ�ʱ�Ƚ�
	CStrA		OwnerName;					//��������������ʾ
	CStrA		DoCmd;						//����ʱִ�е�����
	UINT		BPCount;					//�ϵ㱻ִ�еĴ�����ֻҪ�ϵ㱻ִ�е�������������������������������Ƿ�Ϊ��
	ULPOS		Address;					//�ϵ������ַ
	UINT		PID;						//����ID
	BP_STATE	State;						//�ϵ�״̬
	UINT		Type;						//�ϵ�����
	UINT		Length;						//�ϵ㳤��,�������ݶϵ�
	NUM_PTR		UserData;					//�Զ�������,Patch�ϵ��������Patch����ָ��
	MODULE_BP*	pModuleBP;
	BYTE		CodeBackup;
}BREAK_POINT,*PBREAK_POINT;

typedef TMap<ULONGLONG,BREAK_POINT>	CCodeBPMap;
typedef TMap<ULPOS,MODULE_BP>		CModuleBPMap;
typedef TList<BREAK_POINT>			CDataBPList;

#define BP_TYPE_HARDWARE			0x00000001
#define BP_TYPE_EXECUTE				0x00000002			//Ӳ��ִ�жϵ�
#define BP_TYPE_WRITE				0x00000004
#define BP_TYPE_READ				0x00000008
#define BP_TYPE_READWRITE			(BP_TYPE_WRITE|BP_TYPE_READ)
#define BP_TYPE_IO_WRITE			0x00000010
#define BP_TYPE_IO_READ				0x00000020
#define BP_TYPE_IO_READWRITE		(BP_TYPE_IO_READ|BP_TYPE_IO_WRITE)

#define BP_TYPE_PATCH				0x00000100
#define BP_TYPE_DEBUG				0x00000200
#define BP_TYPE_ONCE				0x00000400

#define BP_TYPE_FULL_CMP			0x80000000
#define BP_TYPE_INC_ALL				0x00000000
#define BP_TYPE_EXC_NON				0x00000000

enum
{
	DEBUG_PLUNGE_OTHER,
	DEBUG_PLUNGE_BREAK_POINT,
	DEBUG_PLUNGE_HARD_BREAK_POINT,
	DEBUG_PLUNGE_STEP,
	DEBUG_PLUNGE_EXCEPTION,
	DEBUG_PLUNGE_USER_BREAK,
};

enum TRACE_TYPE
{
	TRACE_NULL,
	TRACE_STEP_RANGE_IN,
	TRACE_STEP_RANGE_OUT,
	TRACE_PROCEED_STEP_RANGE_IN,
	TRACE_PROCEED_STEP_RANGE_OUT,
	TRACE_STEP_COUNT,
	TRACE_PROCEED_STEP_COUNT,
	TRACE_CONDITION,
	TRACE_PROCEED_CONDITION,
	TRACE_X86_RET_BREAK,
	TRACE_PAGE_RANGE_BREAK,
	TRACE_RECOVER_BREAK_POINT, //�����ָ� int3 ����ϵ�
	TRACE_MAX_COUNT
};

class CDebugger;
class CCodeDoc;
class CSymbolContainer;
class CCommentContainer;

bool IsTypeMatched(UINT TypeInc,UINT TypeExc,const BREAK_POINT&BP);

class CDebugInterface//�����ṩ��CDebugger�Ļص��ӿں���
{
	friend class CDebugger;
public:
	CCodeDoc*			m_pCodeDoc;
	CSymbolContainer*	m_pSymbolContainer;
	CCommentContainer*	m_pCommentContainer;
public:
	CDebugInterface();
	virtual void	OnLoadModule(CDebugger*pDbgProc,PCSTR szFileName,ULPOS ModuleBase,ULSIZE ModuleSize,MODULE_ID ModuleID)=0;
	virtual void	OnUnloadModule(CDebugger*pDbgProc,ULPOS ModuleBase)=0;
	//////////////////////////////////////////////////////////
	//�������
	//////////////////////////////////////////////////////////
	virtual void	UpdateTraceConditionVal(){};
	virtual bool	OnDebugTerminate(bool bCloseDoc)=0;
	virtual bool	OnDebugPlunge(UINT PlungeType)=0;
	virtual bool	OnDebugTerminateMultiThread(bool bCloseDoc)=0;	//�������е����߳�
	virtual bool	OnDebugPlungeMultiThread(UINT PlungeType)=0;	//�������е����߳�
	virtual void	DisplayMsg(PCSTR szMessage){}
	virtual void	UpdateContext(){}
	virtual	bool	TestCondition(PCSTR szCondition,PBREAK_POINT pBP=NULL){return true;}
	virtual bool	RunCmd(PCSTR szCmd){return false;}
	virtual void	RunCmdList(PCSTR szCmd){}
public://Trace
	virtual	bool	IsHandledTraceEvent();
	virtual void	SetTraceRange(ULPOS RangeStart,ULPOS RangeEnd,bool bRangeIn = false);
	virtual void	SetProceedRange(ULPOS RangeStart,ULPOS RangeEnd,bool bRangeIn = false);
	virtual	void	SetTraceCondition(PCSTR szTraceCondition);
	virtual	void	SetTraceProceedCondition(PCSTR szTraceCondition);
	virtual void	SetTracePageRangeCondition(DWORD dwPageCR2,DWORD dwPageEIP);
	virtual	void	SetTraceCount(int StepCount);
	virtual	void	SetProceedCount(int StepCount);
	virtual	void	SetX86RetBreak(CDebugger*pDbg);
	virtual	void	ResetTrace();
	virtual bool	CalcPageRangeCondition();
	virtual	CDebugger*GetCurrentDebugger()=0;
	CStrA			m_strTraceCondition;
	CDebugger*		m_TraceDebugger;
	int				m_TraceCount;
	TRACE_TYPE		m_TraceType;
	ULPOS			m_RangeStart;
	ULPOS			m_RangeEnd;
	ULPOS			m_PageCR2;//��Ҫ���� BPR �ڴ淶Χ�ϵ�ʹ�á����������ȱҳ�жϷ���ʱ��ȱҳ��ַ
	ULPOS			m_PageEIP;//��Ҫ���� BPR �ڴ淶Χ�ϵ�ʹ�á����������ȱҳ�жϷ���ʱ��EIPֵ
};

enum
{
	BP_ERROR_SUCCESS = 0,
	BP_ERROR_SWITCH_PROCESS,
	BP_ERROR_MAP_TO_PHYS,
	BP_ERROR_BP_EXIST,
	BP_ERROR_BP_NOT_EXIST,
	BP_ERROR_FAIL_TO_SET,
	BP_ERROR_FAIL_TO_CLEAR,
};


class CDebugger
{
public:
	CDebugger();
	virtual ~CDebugger();
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool		Init(CDebugInterface*pDebugInterface);//��ʼ��Debug����
	virtual void		Release();//�ͷ�Debug����
	virtual bool		Open(PCSTR ModuleName);
	virtual bool		Close();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ڴ����
	virtual ULSIZE		WriteMemory(ULPOS Address,const void*Buffer,ULSIZE Size);	//д�ڴ�
	virtual ULSIZE		ReadMemory(ULPOS Address,void*Buffer,ULSIZE Size);	//���ڴ�
	virtual ULSIZE		MoveMem(ULPOS DesAddr,ULPOS SrcAddr,ULSIZE Size);	//�ڽ��̿ռ����ƶ�
	virtual ULSIZE		CmpMem(ULPOS DesAddr,ULPOS SrcAddr,ULSIZE Size);	//�ڿռ��ڱȽ��ڴ�
	virtual ULPOS		SearchMem(ULPOS Address,ULSIZE Length,BYTE*ByteCode,ULSIZE Size,bool bCase = true);
	static	ULSIZE		ReadMemory(ULPOS Address,void*Buffer,ULSIZE Size,void*Param);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�Ĵ���
	virtual bool		UpdateRegister();									//���¼Ĵ���
	virtual bool		SaveRegister();										//�ı�Ĵ���
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ϵ�
	virtual bool		SetCodeBreakPoint(BREAK_POINT&BP);					//���ô���ϵ�
	virtual bool		ClearCodeBreakPoint(BREAK_POINT&BP);				//�������ϵ�
	virtual bool		SetDataBreakPoint(BREAK_POINT&BP);					//�������ݶϵ�
	virtual bool		ClearDataBreakPoint(BREAK_POINT&BP);				//������ݶϵ�
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����
	virtual bool		MapToPhysAddr(ULPOS Address,ULONGLONG&PhysAddr);
	virtual	bool		SwitchProcessSpace(UINT PID);						//�л���ָ�����̿ռ�
	virtual	void		RestoreProcessSpace();								//�ָ����̿ռ�
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���Կ���
	virtual bool		SetSingleStep();									//���õ����ϵ�
	virtual bool		RemoveSingleStep();									//ȥ�������ϵ�
	virtual bool		ContinueDebug(bool bRestoreScreen = true);			//��������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Code BP ����
	virtual	BREAK_POINT*GetCodeBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	BREAK_POINT*InsertCodeBP(ULPOS Address,UINT Type,BP_STATE State = BP_STATE_ENABLE);
	virtual	bool		RemoveCodeBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	bool		RemoveCodeBP(BREAK_POINT*pBP);
	virtual	int			RemoveAllCodeBP(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	int			EnableAllCodeBP(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	int			DisableAllCodeBP(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	bool		EnableCodeBP(BREAK_POINT*pBP);
	virtual	bool		EnableCodeBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	bool		DisableCodeBP(BREAK_POINT*pBP);
	virtual	bool		DisableCodeBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Data BP ����
	virtual	int			GetDataBPList(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON,TList<BREAK_POINT*>*pList = NULL);
	virtual	BREAK_POINT*InsertDataBP(ULPOS Address,UINT Type,BP_STATE State,UINT Length);
	virtual	int			RemoveDataBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	bool		RemoveDataBP(BREAK_POINT*pBP);
	virtual	int			RemoveAllDataBP(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	int			EnableAllDataBP(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	int			DisableAllDataBP(UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	bool		EnableDataBP(BREAK_POINT*pBP);
	virtual	int			EnableDataBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	virtual	bool		DisableDataBP(BREAK_POINT*pBP);
	virtual	int			DisableDataBP(ULPOS Address,UINT TypeInc = BP_TYPE_INC_ALL,UINT TypeExc = BP_TYPE_EXC_NON);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	CCodeBPMap			m_CodeBPMap;
	CDataBPList			m_DataBPList;
public:
	UINT				m_PID;
	CStrA				m_ProcName;
	ULPOS*				m_pExecutePoint;
	CDebugInterface*	m_pDebugInterface;
	CDbgModuleList		m_ModuleList;
	CCodeBPMap*			m_pCodeBPMap;
	CDataBPList*		m_pDataBPList;
	UINT				m_LastError;
};

#endif
