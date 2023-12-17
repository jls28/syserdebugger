#ifndef	_WISP_EDIT_H_
#define _WISP_EDIT_H_

#include "WispWnd.h"
//////////////////////////////////////////////
//�༭����
//////////////////////////////////////////////
#define WISP_ES_EDIT_NORMAL			(WISP_ES_LEFT | WISP_WS_THIN_BORDER | WISP_ES_AUTOHSCROLL)
#define WISP_ES_AUTOHSCROLL			0x01000000
#define WISP_ES_AUTOVSCROLL   		0x02000000
#define WISP_ES_CENTER   			0x04000000
#define WISP_ES_LEFT				0x08000000
#define WISP_ES_LOWERCASE			0x10000000
#define WISP_ES_MULTILINE			0x20000000
#define WISP_ES_NOHIDESEL			0x40000000
#define WISP_ES_PASSWORD			0x80000000
#define WISP_ES_RIGHT				0x00010000
#define WISP_ES_UPPERCASE			0x00020000
#define WISP_ES_READONLY			0x00040000
#define WISP_ES_WANTRETURN			0x00080000
#define WISP_ES_HSCROLL				0x00100000
#define WISP_ES_VSCROLL				0x00200000

#define WISP_ES_EX_STATIC_EDGE			0x00000001
#define WISP_EDIT_TEXT_LEN				0x100
//#define WISP_EDIT_BORDER_COLOR		RGB(127,157,185)

#define WISP_EDIT_MAX_TEXT_LEN			30000
#define WISP_EDIT_MAX_LINETEXT_LEN		20

typedef struct _SELECTPOS{
	int begin;
	int end;
}SELECTPOS;


class CWispEdit : public CWispWnd
{
	typedef enum _INPUTSTAT{
		EDIT_STATE=0,
		SELECT_STATE,		
	}INPUTSTATE;
	//����ÿ�� ASCII �ַ��Ŀ��
	INT		m_CharLenArray[0x140];	
	//�����ַ����ڵ��ַ����е�λ�á�
	INT		m_CurrentCaretHot;
	//�����ַ��ڵ��С�
	INT		m_CurrentCaretLine;
	//��ǰ�༭�ؼ����ַ��ĸ���
	INT		m_CurrentStringLen;
	// ����༭�ؼ��Ƕ�����ñ���Ϊ�档
	bool m_bIsMultiLine;
	//�༭�ؼ������������ַ������ȡ��� Create �����ã������Create ��û�����ø�ֵ��ʹ��Ĭ��ֵ WISP_EDIT_MAX_TEXT_LEN
	//����� Style ��û�� WISP_ES_AUTOHSCROLL  WISP_ES_AUTOVSCROLL WISP_ES_VSCROLL WISP_ES_VSCROLL ����ݿͻ����Ĵ�С�����㳤��
	INT		m_StringMaxLen;
	//���Ƕ��еı༭�ؼ�ʱ���п���ʾ������ַ���
	INT		m_LineMaxLen;
	// �༭�ؼ������ɵ��ı�������
	UINT m_TextRowNum;
	// �༭�ؼ���ÿ�п����ɵ��ַ��ĸ���
	UINT m_TextColNum;
	// ��ǰ�еĿ�ʼλ��,��ǰ��ָ�������ַ����ڵ���	
	INT m_CurrentLineCharIndex;
	// ��ǰ�к�,��ǰ��ָ�������ַ����ڵ���
	INT m_CurrentLineNum;

	WISP_RECT m_CurrentShowRect;
	INT m_DebugValue;
	bool m_bSelectFlag;
	int m_SelectBegin;
	int m_SelectEnd;
	INPUTSTATE m_CurrentInputStat; 
public:
	CWispEdit(void);
	~CWispEdit(void);
public:

	bool	Create( IN const WISP_CHAR*Name,IN const CWispRect&Rect,IN CWispBaseWnd*pParentWnd = NULL,IN UINT CmdID = 0,		IN UINT Style=WISP_WS_NORMAL,IN UINT ShowMode=WISP_SH_NORMAL);

	bool	Create(IN WISP_RECT&RectAtWnd,IN UINT Style,IN CWispBaseWnd*pParentWnd,IN UINT CmdID);
	bool	Create(IN const WISP_CHAR* Name,IN INT x,IN INT y,IN INT cx,IN INT cy,IN CWispBaseWnd*pParentWnd=NULL,IN UINT CmdID=0,IN UINT Style=WISP_ES_EDIT_NORMAL,IN UINT ShowMode=WISP_SH_NORMAL);
public://��Ϣӳ��
	DECLARE_WISP_MSG_MAP
	DECLARE_WISP_MSG(OnGetFocus)
	DECLARE_WISP_MSG(OnLostFocus)
	DECLARE_WISP_MSG(OnChar)
	DECLARE_WISP_MSG(OnUpdateClient)
	DECLARE_WISP_MSG(OnUpdateBorder)	
	DECLARE_WISP_MSG(OnCreate)
	DECLARE_WISP_MSG(OnMouseLeave)
	DECLARE_WISP_MSG(OnKeyEvent)
	DECLARE_WISP_MSG(OnMouseEvent)
	DECLARE_WISP_MSG(OnMouseLeftButtonUp)
	DECLARE_WISP_MSG(OnMouseLeftButtonDown)
	DECLARE_WISP_MSG(OnMouseMove)
	DECLARE_WISP_MSG(OnRecalcLayout)
	DECLARE_WISP_MSG_EVENT_MAP
	DECLARE_WISP_MSG(OnEventPaste);
	DECLARE_WISP_MSG(OnEventCopy);
public:
	void OnKeyBack(void);	
	void OnLeftKey(IN WISP_MSG *pMsg);
	void OnRightKey(IN WISP_MSG *pMsg);
	void OnReturnKey(IN WISP_MSG *pMsg);
	void OnBackKey(IN WISP_MSG *pMsg);
	void OnHomeKey(IN WISP_MSG *pMsg);
	void OnEndKey(IN WISP_MSG *pMsg);
	void OnDeleteKey(IN WISP_MSG *pMsg);
	
	//�����ڸ����Ŀ�����������ɵ�����ַ�����
	//������  nLength �����ɵ��ַ��ĳ���
	int GetMaxShowCharNum(WISP_CHAR* lpszString, INT nWidth,OUT INT *nLength = NULL, bool isMultiLine=FALSE);
	//�����жϱ༭�ؼ��Ƿ���Բ�������ַ�
	bool IsInsertChar(IN WISP_CHAR wch);
	INT InsertChar(IN WISP_CHAR *pChar,IN INT nPosition=-1,IN int Counter=1);
	INT DeleteChar(IN INT nPosition=-1,IN INT nCount = 1,IN WISP_MSG *pMsg=NULL);
	//�������ַ����ڵ�λ�á�����ڵ�ǰ�еġ�
	//int GetCurrentLinePosition();
	INT GetCurrentLinePosition(INT nCharNum = -1);
	INT GetPrevLineBeginPosition(IN INT nEndPostion,OUT INT *nStringLen = NULL);
	//�������ַ����ڵ��еĳ��ȡ�
	INT GetCaretLineCharWidth();
	INT GetLineWidth(IN const WISP_CHAR *pSrc);
	//�������� nPerLineMaxCharNum ��һ�п������ɵ�����ַ���
	INT CalcLineNum(IN const WISP_CHAR *pSrc,IN INT nCount = -1,IN INT nPerLineMaxCharNum = 0);
	//�������� nLineWidth ���е����ؿ��
	INT CalcLineNumForWidth(IN const WISP_CHAR *pSrc,IN INT nCount = -1,IN INT nLineWidth = -1);
	INT CaretLeftMove();
	INT CaretRightMove();
	INT GetCurrentPosition(OUT INT nIndex,INT *nLineLength=NULL);
	bool SetWindowText(const WISP_CHAR* pString);
	int GetCharIndexByPoint(WISP_POINT point,WISP_POINT&retpoint);
	WISP_POINT GetPositionByIndex(IN int index);
	WISP_RECT m_SelectRect;
	COLORREF m_BSelColor;
	int SelectText(int SelectBegin,int SelectEnd);
	bool CharIsVisible(int index);
	void EnableVisible(int index);
};

#endif


