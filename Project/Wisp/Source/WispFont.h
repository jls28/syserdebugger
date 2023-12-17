#ifndef _WISP_FONT_DATA_H_
#define _WISP_FONT_DATA_H_


enum WISP_FONT_ID
{
	WISP_FONT_8X16=0,
	WISP_FONT_6X12,
	WISP_FONT_ASCII_12X16,
	WISP_FONT_EBCDIC_12X16,
	WISP_FONT_ANSI_12X16,
	WISP_FONT_COUNT
};

struct WISP_PHYS_FONT_INFO
{
	int		Width;
	int		Height;
	PCSTR	FontFN;
	PCSTR	WideFontFN;
};

class CWispFont
{
public:
	CWispFont();
public:
	bool	LoadChar(WISP_PHYS_FONT_INFO*pFontInfo);
	bool	LoadWideChar(WISP_PHYS_FONT_INFO*pFontInfo);
	void	Destroy();
	int		m_CharLenAry[256];
	int		m_Height;										//����ĸ߶�
	int		m_Width;										//����Ŀ��
	BYTE*	m_Buffer;										//��ģָ��
	int		m_BufSize;										//��ģ�����С

	int		m_WideHeight;									//����ĸ߶�
	int		m_WideWidth;									//����Ŀ��
	BYTE*	m_WideBuffer;									//��ģָ��
	int		m_WideBufSize;									//��ģ�����С
	int		m_BytesPerChar;									//ÿ���ַ���ռ�ֽ�

	BYTE*	GetCharPixelBuffer(WISP_CHAR Char);
	int		GetCharWidth(WISP_CHAR Char);
	int		GetCharHeight(WISP_CHAR Char);
};

#endif
