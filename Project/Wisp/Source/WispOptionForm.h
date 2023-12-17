#ifndef _WISP_OPTION_FORM_H_
#define _WISP_OPTION_FORM_H_

#include "WispForm.h"
#include "WispList.h"
#include "WispButton.h"
#include "WispEdit.h"
#include "WispMisc.h"

typedef bool (*FN_ValueTest)(void*pValue);

enum
{
	CMD_ID_OPTION_LIST = WISP_ID_DEVELOP_START,
	CMD_ID_OPTION_APPLY,
	CMD_ID_OPTION_RESTORE,
	CMD_ID_OPTION_DEFAULT,
};

enum WISP_OPTION_TYPE
{
	OPTION_TITLE,
	OPTION_INT,
	OPTION_SELECT_INT,
	OPTION_STRING,
	OPTION_DIR_NAME,
	OPTION_RGB_COLORREF,
};


#define WISP_OFS_HIDE_DEFAULT_BT	0x00010000

#define WISP_CMD_OPTION_APPLY		WISP_CMD_DEVELOP_START

struct WISP_OPTION_ITEM
{
	WISP_PCSTR	szText;				//����
	WISP_OPTION_TYPE Type;			//��������
	void*		pValue;				//������ַ
	int			Length;				//��������
	WISP_PCSTR*	SelectData;			//ѡ���б��ַ������飬���� "Yes","NO",NULL
	FN_ValueTest pfnValueTest;		//�����Ϸ���麯��������Ϊ�գ�Ҳ����ģ��TValueTest<int,0,10>
	int			*pRef;				//�����ı�֪ͨ����������������޸ľͻ�Ѵ�ֵ+1
	void*		pNewValue;			//��ű�������ֵ���ڲ�ʹ��
};

template<class TYPE,TYPE Min,TYPE Max>
bool TValueTest(void*pValue)
{
	return *(TYPE*)pValue>=Min && *(TYPE*)pValue<=Max;
}

class CWispOptionList : public CWispList
{
public:
	void	DrawCellText(int Col,CWispListItemCell*pCell,WISP_RECT*pRect);
};


class CWispOptionForm : public CWispForm
{
public:
	CWispOptionForm();
	~CWispOptionForm();
public:
	static  WISP_PCSTR m_NoYesList[];
	static	WISP_FORM_RES_ITEM	m_FormRes[];
	WISP_OPTION_ITEM*m_pOptionItem;
public:
	DECLARE_WISP_MSG_MAP
	DECLARE_WISP_MSG(OnFormCreate)
	DECLARE_WISP_MSG(OnResize)
	DECLARE_WISP_MSG(OnKeyEvent)
	DECLARE_WISP_MSG(OnDestroy)
	DECLARE_WISP_MSG_CMD_MAP
	DECLARE_WISP_MSG(OnListCommand)
	DECLARE_WISP_MSG_EVENT_MAP
	DECLARE_WISP_MSG(OnEventApply)
	DECLARE_WISP_MSG(OnEventRestore)
	DECLARE_WISP_MSG(OnEventDefault)
	virtual	bool OptionBeforeApplyNotify(){return true;}
	virtual	void OptionApplyNotify(){}
	virtual	bool OptionBeforeRestoreNotify(){return true;}
	virtual	void OptionRestoreNotify(){}
	virtual	bool OptionBeforeDefaultNotify(){return true;}
	virtual	void OptionDefaultNotify(){}
	virtual	void	OnModify(HANDLE hItem);
	virtual	void	OnModifyRestore(HANDLE hItem);
	void	InsertOptionTree(WISP_OPTION_ITEM*pItem,HANDLE hParent);
	void	RemoveOptionTree(HANDLE hParent);
	void	RestoreOptionTree(HANDLE hParent);
	void	ReloadOptionTree(HANDLE hParent);
	void	ApplyOptionTree(HANDLE hParent);
	bool	ProcessOptionBeginModify(int x,int y,HANDLE hItem,WISP_OPTION_ITEM*pItem);
	bool	ProcessOptionEndModify(HANDLE hItem,WISP_OPTION_ITEM*pItem);
	void	FillOptionValue(WISP_OPTION_ITEM*pItem,HANDLE hItem);
	void	FillOptionTreeValue(HANDLE hParent = NULL);
	int		m_nModifyItem;
	CWispOptionList	m_OptionList;
	CWispButton*m_pApplyBT;
	CWispButton*m_pRestoreBT;
	CWispButton*m_pDefaultBT;
};

#endif
