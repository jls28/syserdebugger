#ifndef _WISP_HEX_WND_H_
#define _WISP_HEX_WND_H_

#include "WispWnd.h"
#define MAX_POS_LIST_COUNT 100
enum ENUM_ADDR_SHOW_MOD
{
	ADDRESS_MODE_HEX,
	OFFSET_MODE_HEX,
	ADDRESS_MODE_MAX,
};

#define	MAX_LINES				0x100


#define WISP_HEX_WS_RELATIVE_SCROLL		0x00010000

enum
{
	HEX_START_INDEX = 0,
	HEX_POS_INDEX,		//�ļ�λ�ÿո�
	HEX_HEX_INDEX,		//HEX�ֽڿո�
	HEX_ASCII_INDEX,	//ASCII�ո�
	HEX_ITEM_COUNT
};

enum
{
	HEX_BYTE_NORMAL = 0,
	HEX_BYTE_SEL,
	HEX_BYTE_MASK
};

class CWispHexWnd : public CWispWnd
{
	typedef struct _HISTORY_ADDRESS
	{
		DWORD dwAddress;
		WORD	wOffset;
		WORD	wFlags;
	}HISTORY_ADDRESS;
	typedef TList<HISTORY_ADDRESS> ADDR_LIST;
public:
	CWispHexWnd();
	~CWispHexWnd();
public:
	DECLARE_WISP_MSG_MAP
	DECLARE_WISP_MSG(OnChar)
	DECLARE_WISP_MSG(OnCreate)
	DECLARE_WISP_MSG(OnKeyEvent)
	DECLARE_WISP_MSG(OnMouseMove)
	DECLARE_WISP_MSG(OnUpdateClient)
	DECLARE_WISP_MSG(OnRecalcLayout)
	DECLARE_WISP_MSG(OnScrollEvent)
	DECLARE_WISP_MSG(OnSize)
	DECLARE_WISP_MSG(OnMouseDBClick);
	DECLARE_WISP_MSG_CMD_MAP
	DECLARE_WISP_MSG(OnCmdVSlide)
	DECLARE_WISP_MSG (OnPrevPos)
	DECLARE_WISP_MSG(OnNextPos)
	DECLARE_WISP_MSG(OnSavePos)
public:
	virtual ULSIZE	WriteData(ULPOS Address,BYTE*Buffer,ULSIZE Size);
	virtual ULSIZE	ReadData(ULPOS Address,BYTE*Buffer,ULSIZE Size);
	virtual	UINT	GetHexByteAttribute(IN DWORD Address);
	virtual ULPOS	GetFloatAddress(CStrW& CmdStr){return m_CurAddr;};
	virtual	void	OnAddrChange(){};
	virtual	void	OnDataChange(){};
	virtual	int	GetStartLine(){return m_bShowOffset?1:0;};
	UINT		CorrectHitPoint(IN WISP_POINT*pPoint);
	bool		CalcPonitPos(IN WISP_POINT*pPoint,IN ULPOS*pPos,INOUT ULPOS* pOffset=NULL);
	bool		ViewAddress(IN ULPOS Address,bool bSave=true);
	bool		ViewAddressMsg(IN ULPOS Address,bool bSave=false);
	bool		SetDataWidth(int width);
	int			GetDataWidth()const;
	void		SetAddressMode(int Mod,ULPOS BaseAddr=0);
	int			GetAddressMode();
	void		ToggleShowOffset();
	void		SetFloatCmdString(WCHAR* pCmdString);
	bool		ToggleFloatMode();
	bool		IsFloatMode();
	void		SaveContext();//�������浱ǰ��ʾ���������ݣ������´θ�����ʾ�ı�
public:
	int			m_OffsetMode;//0: +offset; 1:   //�����ʾ��һ�е�ƫ����Ϣ,ƫ��ģʽ 0 ģʽ +1 +2 +3  ƫ��ģʽ 1 ģʽ  1 2 3
	bool		m_bShowOffset;					//�Ƿ���ʾ��һ�е�ƫ��
	ULPOS		m_ImageBase;
	ULSIZE		m_ImageSize;
	ULPOS		m_ImageHighBase;
	ULPOS		m_CurAddr;						//��ǰ��ַ
	COLORREF	m_BSelColor;					//ѡ��ʱ������ɫ
	COLORREF	m_TextColor;					//������ɫ
	COLORREF	m_TSelColor;					//ѡ��ʱ������ɫ
	COLORREF	m_BBPXColor;					//DebugʱBPX������ɫ 
	//��Ļ��Ϣ
	int			m_ScrollSpace;
	int			m_ShowLength;					//������ʾ���
	int			m_nLinePerPage;					//��ǰÿҳ����
	int			m_nChars_Line;					//ÿ������
	int			m_LineHeight;					//�и�
	int			m_CharWidth;					//�ֿ�
	int			m_nByte_Line;					//һ���ֽ���
	int			m_CharXPos;						//�ַ���λ��
	int			m_CharMax;						//һ������ַ���
	int			m_CaretIndex;					//��ǰ����������ַ���������
	int			m_CaretnLine;					//��ǰ���������
	WISP_POINT	m_CaretPos;						//����λ��
	//���ڼ���
	DWORD		m_SelStart;						//���п�ʼ
	DWORD		m_SelEnd;						//���н���
	bool		m_bSelect;						//�Ƿ��ڼ���ģʽ
	bool		m_bSelText;						//�Ƿ�����ı�
	DWORD		m_CurArea;						//��ǰ����
	//����Search Next
	DWORD		m_SrhAddr;
	BYTE		m_SrhData[256];
	DWORD		m_SrhSize;
	DWORD		m_SrhLeftSize;
	bool		m_SrhbCase;
	int			m_LenArray[HEX_ITEM_COUNT][MAX_LINES];//�������飬�����������λ��
	COLORREF	m_ItemColor[HEX_ITEM_COUNT];	//��ɫ����
	int			m_ItemSpace[HEX_ITEM_COUNT];	//�ո�����
	static const WISP_CHAR*	m_UnReadable;
	static const WISP_CHAR*	m_UnReadableASCII;
	static const WISP_CHAR*	m_Spaces;
	ENUM_ADDR_SHOW_MOD	m_AddrShowMode;
	ULPOS	m_BaseActiveAddress;
	CStrW	m_FloatString;
	bool	m_FloatMode;
	//������ʾ�ı����ʹ��
	BYTE	m_PrevData[0x1000];
	DWORD	m_PrevDataAddress;
	DWORD	m_CurrentDataAddress;
	DWORD	m_PrevDataLen;
	BYTE	m_CurrentData[0x1000];
	DWORD	m_CurrentDataLen;
	bool IsModifyValue(ULPOS Address,BYTE* ValueBuffer);//�ж������Ƿ�ı߹�
	ADDR_LIST m_AddrList;
	ADDR_LIST::IT	m_AddrIter;
	void EmptyPosList();
	void	RecordCurAddr(ULPOS Address,ULPOS uPrevAddress);

};

#endif
