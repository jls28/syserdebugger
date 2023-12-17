#ifndef _WISP_WND_H_
#define _WISP_WND_H_

#include "WispBaseWnd.h"

class CWispWnd : public CWispBaseWnd
{
	friend class CWispBase;
	friend class CWispDrawObj;
public:
	WISP_RECT	m_NormalRect;				//�������ʱ���洰������
	WISP_RECT	m_ScrCaptionRect;			//����������Ļ�е�λ��
	WISP_RECT	m_ScrClientRect;			//�ͻ�������Ļ�е�λ��
	WISP_RECT	m_CaptionRect;				//�������ڴ����е�λ��
	WISP_RECT	m_ClientRect;				//�ͻ����ڴ����е�λ��
	WISP_RECT*	m_pBarRect;					//Bar���ڴ����е�λ��
	int			m_BarCount;
	WISP_SIZE	m_MinWndSize;
	WISP_SIZE	m_MaxWndSize;
	bool		m_bInResizeArea;
public:
	int			m_BorderSize;
	WISP_WND_CARET	m_Caret;
	CWispDC		m_ClientDC;
	CWispDC		m_CaptionDC;
	CWispDIB*	m_pTitleDIB;
	CWispDIB*	m_pBKDIB;
	COLORREF	m_crBGColor;				//����ɫ
	COLORREF	m_crCaret;
	bool		m_bBGColor;					//�Ƿ���������ϵͳ��ͬ�ı���ɫ
private://���ڴ��ڴ�С�仯
	int			m_SignResizeX;
	int			m_SignResizeY;
	int			m_SignResizeDX;
	int			m_SignResizeDY;
	int			m_CloseBTState;
	int			m_MaxBTState;
	int			m_MinBTState;
	WISP_RECT	m_CloseBTRect;
	WISP_RECT	m_MaxBTRect;
	WISP_RECT	m_MinBTRect;
public://��Ϣӳ��
	DECLARE_WISP_MSG_MAP//��ͨ��Ϣ
	DECLARE_WISP_MSG(OnPrecreate)
	DECLARE_WISP_MSG(OnMouseMove)
	DECLARE_WISP_MSG(OnMouseLeave)
	DECLARE_WISP_MSG(OnKeyEvent)
	DECLARE_WISP_MSG(OnUpdate)
	DECLARE_WISP_MSG(OnUpdateClient)
	DECLARE_WISP_MSG(OnUpdateCaption)
	DECLARE_WISP_MSG(OnUpdateBorder)
	DECLARE_WISP_MSG(OnUpdateCaret)
	DECLARE_WISP_MSG(OnUpdateVertScrollBar)
	DECLARE_WISP_MSG(OnUpdateHorzScrollBar)
	DECLARE_WISP_MSG(OnScrollEvent)
	DECLARE_WISP_MSG(OnHorzScrollMove)
	DECLARE_WISP_MSG(OnVertScrollMove)
	DECLARE_WISP_MSG(OnMoving)
	DECLARE_WISP_MSG(OnSizing)
	DECLARE_WISP_MSG(OnMove)
	DECLARE_WISP_MSG(OnSize)
	DECLARE_WISP_MSG(OnHitTest)
	DECLARE_WISP_MSG(OnMouseWheel)
	DECLARE_WISP_MSG(OnTimer)
	DECLARE_WISP_MSG(OnGetFocus)
	DECLARE_WISP_MSG(OnLostFocus)
	DECLARE_WISP_MSG_CMD_MAP//Command��Ϣ
	DECLARE_WISP_MSG_CMD(OnCmdClose)
	DECLARE_WISP_MSG_CMD(OnCmdMax)
	DECLARE_WISP_MSG_CMD(OnCmdMin)
	DECLARE_WISP_MSG_CMD(OnCmdScrollPageUp)
	DECLARE_WISP_MSG_CMD(OnCmdScrollPageDown)
	DECLARE_WISP_MSG_CMD(OnCmdScrollSpinInc)
	DECLARE_WISP_MSG_CMD(OnCmdScrollSpinDec)
	DECLARE_WISP_MSG_CMD(OnCmdScrollSlide)
	DECLARE_WISP_MSG_EVENT_MAP//Event��Ϣ
public:
	CWispWnd();
	virtual ~CWispWnd();
	virtual bool	Create(	IN const WISP_CHAR*Name,IN int x,IN int y,IN int cx,IN int cy,IN CWispBaseWnd*pParentWnd = NULL,
							IN UINT CmdID = 0,IN UINT Style=WISP_WS_NORMAL,IN UINT ShowMode=WISP_SH_NORMAL);
	virtual bool	Destroy();
	virtual void	RecalcLayout();
	virtual bool	InitWnd();
	virtual bool	SetWindowText(const WISP_CHAR* pString);
	bool	Create( IN const WISP_CHAR*Name,IN const CWispRect&Rect,IN CWispBaseWnd*pParentWnd = NULL,IN UINT CmdID = 0,
					IN UINT Style=WISP_WS_NORMAL,IN UINT ShowMode=WISP_SH_NORMAL);
	void	ChangeColor(IN UINT ColorID,IN COLORREF Color);
	void	SetBGColor(IN COLORREF crBGColor) { m_crBGColor=crBGColor; m_bBGColor = true; }
	void	ClearBGColor() { m_bBGColor = false; }

	void	RecalcMinWndSize();
	void	ScreenToClient(INOUT WISP_POINT* pPoint) { PointToRect(pPoint, &m_ScrClientRect); }
	void	ClientToScreen(INOUT WISP_POINT* pPoint) { PointSpliceRect(pPoint, &m_ScrClientRect); }
	void	ScreenToWindow(INOUT WISP_POINT* pPoint) { PointToRect(pPoint, &m_ScrWindowRect); }
	void	WindowToScreen(INOUT WISP_POINT* pPoint) { PointSpliceRect(pPoint, &m_ScrWindowRect); }
	void	UpdateClient(){Update(&m_ClientRect);}
	bool	AttachBKDIB(CWispDIB*pDIB);
	void	DetachBKDIB();
	bool	LoadBKDIB(PCSTR FileName);

	bool	AttachTitleDIB(CWispDIB*pDIB);
	void	DetachTitleDIB();
	bool	LoadTitleDIB(PCSTR FileName,int Index = 0);
	CWispBaseWnd*GetChildWnd(UINT CmdID);
//////////////////////////////////////////////////////////////
//���
//////////////////////////////////////////////////////////////
	bool	CreateCaret(IN UINT ID);
	bool	SetCaretPos(IN int x,IN int y);
	bool	GetCaretPos(OUT WISP_POINT*pPT);
	void	ShowCaret(IN bool bShow);
	void	BlinkCaret(IN ULONG BlinkTime);//0 Not Blink
	void	DestroyCaret();
//////////////////////////////////////////////////////////////
//������
//////////////////////////////////////////////////////////////
protected:
	void	CalcVertScrollSlideWithPos();
	void	CalcHorzScrollSlideWithPos();
	//���ƹ���������
	void	LimitScrollInfo(WISP_SB_TYPE Type,bool bChgMsg = true);
	//���ݵ�ǰλ�ü��㻮���λ��
	void	CalcScrollSlideWithPos(WISP_SB_TYPE Type);
public:
	WISP_SCROLL_BAR*m_ScrollBar[WISP_SB_COUNT];
	WISP_SCROLL_LONG SendScrollEvent(WISP_SB_TYPE Type,WISP_SB_EVENT_TYPE EventType);
	void	UpdateScrollBarState(WISP_SB_TYPE Type);
	bool	EnableScrollBar(IN WISP_SB_TYPE Type,IN bool bEnable = true,IN UINT Style = WISP_SS_AUTOHIDE);

	void	SetScrollBarInfo(IN WISP_SB_TYPE Type,IN WISP_SCROLL_LONG CurPos,IN WISP_SCROLL_LONG MaxPos,IN WISP_SCROLL_LONG Page,IN WISP_SCROLL_LONG MinPos = 0,IN bool bChangeEvent = true);
	void	SetScrollBarCurPos(IN WISP_SB_TYPE Type,IN WISP_SCROLL_LONG CurPos,IN bool bChangeEvent = true);
	void	SetScrollBarRange(IN WISP_SB_TYPE Type,IN WISP_SCROLL_LONG MaxPos,IN WISP_SCROLL_LONG MinPos = 0);
	void	SetScrollBarPage(IN WISP_SB_TYPE Type,IN WISP_SCROLL_LONG Page);
	void	AdjustScrollBarMaxPos(IN WISP_SB_TYPE Type,IN WISP_SCROLL_LONG Delta);

	void	ProcessCaptionBTMouseMove(IN const WISP_POINT&ScrMousePT);
	void	ProcessCaptionBTMouseKeyUp(IN const WISP_POINT&ScrMousePT);
	UINT	IsPointInCaptionBT(IN const WISP_POINT&ScrMousePT);

	void	ProcessScrollBTSpinKey(IN WISP_SB_TYPE Type,IN WISP_MSG*pMsg);
	void	ProcessScrollMouseKey(IN WISP_SB_TYPE Type,IN WISP_MSG*pMsg);

	void	ProcessVertScrollMouseMove(IN WISP_MSG*pMsg);
	void	ProcessHorzScrollMouseMove(IN WISP_MSG*pMsg);
public:
	//��ק
	bool	BeginDrag();
	void	EndDrag();
};


class CWispDIBDCWnd : public CWispWnd
{
public:
	CWispDIBDCWnd();
	~CWispDIBDCWnd();
public://��Ϣӳ��
	DECLARE_WISP_MSG_MAP//��ͨ��Ϣ
	DECLARE_WISP_MSG(OnPrecreate)
	DECLARE_WISP_MSG(OnRecalcLayout)
	DECLARE_WISP_MSG(OnUpdateClient)
	DECLARE_WISP_MSG(OnDestroy)
public:
	CWispDIB	m_DIB;
};

#endif
